#!/usr/bin/env python3
"""
Self-check for configure_platform_and_web (engine/cmake/platform_config.cmake).

The engine's test/ target is a C++ unit-test binary and cannot observe a
POST_BUILD copy rule or an emscripten preload flag, so the asset-bundling
behaviour is checked here by configuring throwaway CMake projects against the
real function and inspecting what it emitted.

Run: python3 scripts/run_asset_bundle_selftest.py
Exit code 0 on success, 1 on the first failed assertion.
"""

from __future__ import annotations

import os
import re
import shutil
import subprocess
import sys
import tempfile

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PLATFORM_CONFIG = os.path.join(REPO_ROOT, "engine", "cmake", "platform_config.cmake")

PROJECT_TEMPLATE = """
cmake_minimum_required(VERSION 3.25)
project(asset_probe LANGUAGES C)
include({config})
add_executable(probe main.c)
{configure}
"""

MAIN_C = "int main(void) { return 0; }\n"


class Failure(Exception):
    pass


def check(condition: bool, message: str) -> None:
    if not condition:
        raise Failure(message)


def build_case(workdir: str, name: str, entries: str, emscripten: bool = False):
    """Configure a throwaway project; return (configure_result, files_dir)."""
    case_dir = os.path.join(workdir, name)
    source_dir = os.path.join(case_dir, "src")
    build_dir = os.path.join(case_dir, "build")
    os.makedirs(source_dir)
    with open(os.path.join(source_dir, "main.c"), "w") as handle:
        handle.write(MAIN_C)
    with open(os.path.join(source_dir, "CMakeLists.txt"), "w") as handle:
        handle.write(PROJECT_TEMPLATE.format(
            config=PLATFORM_CONFIG,
            configure="configure_platform_and_web(probe {})".format(entries)))

    args = ["cmake", "-S", source_dir, "-B", build_dir, "-DCMAKE_C_COMPILER=cc"]
    if emscripten:
        # Force the emscripten branch without the emsdk: link options are just
        # strings at configure time, so the host compiler still generates.
        args += ["-DPLATFORM=Web", "-DEMSCRIPTEN=ON"]
    result = subprocess.run(args, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT)
    return result, build_dir


def read(path: str) -> str:
    with open(path) as handle:
        return handle.read()


def link_command(build_dir: str) -> str:
    return read(os.path.join(build_dir, "CMakeFiles", "probe.dir", "link.txt"))


def preload_entries(link: str):
    """The source path of every --preload-file, in link-command order."""
    return re.findall(r'--preload-file\s+"?(.*?)@(?=["\s]|$)', link)


def check_flag_pairing(link: str, flags, settings) -> None:
    """Each setting is immediately preceded by its own flag.

    target_link_options de-duplicates repeated option names, so passing the
    flags and their values as separate list items silently collapses several
    flags into one. emscripten needs each -s glued to its setting.
    """
    for setting in settings:
        check("{} {}".format(flags, setting) in link,
              "{} is not paired with its {} flag:\n".format(setting, flags) + link)
    check(link.count(flags + " ") >= len(settings),
          "expected {} occurrences of {}, found {}:\n".format(
              len(settings), flags, link.count(flags + " ")) + link)


def check_root_mounts(assets, link: str) -> None:
    """Every asset is preloaded once, and '@' with no target path mounts at /."""
    entries = preload_entries(link)
    check(entries == assets,
          "preload entries do not match the declared assets in order.\n"
          "  expected: {}\n  actual:   {}\n  link: {}".format(assets, entries, link))
    check(link.count("--preload-file") == len(assets),
          "expected one --preload-file per asset, found {} for {} assets:\n".format(
              link.count("--preload-file"), len(assets)) + link)
    for entry in entries:
        check("@" not in entry,
              "asset {} is preloaded below a subdirectory, not at the .data root"
              .format(entry))


def build_commands(build_dir: str) -> str:
    return read(os.path.join(build_dir, "CMakeFiles", "probe.dir", "build.make"))


def case_explicit_file(workdir):
    asset = os.path.join(workdir, "shared", "icon.png")
    os.makedirs(os.path.dirname(asset), exist_ok=True)
    with open(asset, "w") as handle:
        handle.write("png")
    result, build_dir = build_case(workdir, "explicit_file", '"{}"'.format(asset))
    check(result.returncode == 0, "explicit file case failed to configure:\n"
          + result.stdout.decode(errors="replace"))
    commands = build_commands(build_dir)
    check("copy_if_different" in commands,
          "explicit file case emitted no copy_if_different rule")
    check(asset in commands and "probe" in commands,
          "explicit file case does not copy the declared file")
    check("copy_directory" not in commands,
          "explicit file case still uses copy_directory")


def case_asset_path_with_spaces(workdir):
    asset = os.path.join(workdir, "shared", "Mission Completed.wav")
    os.makedirs(os.path.dirname(asset), exist_ok=True)
    with open(asset, "w") as handle:
        handle.write("wav")
    result, build_dir = build_case(workdir, "spaces", '"{}"'.format(asset),
                                   emscripten=True)
    check(result.returncode == 0, "space-containing path case failed to configure:\n"
          + result.stdout.decode(errors="replace"))
    link = link_command(build_dir)
    check(asset + "@" in link,
          "space-containing path was not preloaded intact:\n" + link)
    check(link.count("--preload-file") == 1,
          "space-containing path was split into multiple preload entries:\n" + link)
    check_root_mounts([asset], link)


def case_directory_form(workdir):
    folder = os.path.join(workdir, "folder_assets")
    os.makedirs(folder)
    with open(os.path.join(folder, "a.txt"), "w") as handle:
        handle.write("a")
    result, build_dir = build_case(workdir, "directory", '"{}"'.format(folder))
    check(result.returncode == 0, "directory case failed to configure:\n"
          + result.stdout.decode(errors="replace"))
    check(os.path.join(folder, "a.txt") in build_commands(build_dir),
          "directory form did not expand to the files it contains")


def case_mixed_and_empty_directory(workdir):
    folder = os.path.join(workdir, "mixed_dir")
    os.makedirs(folder)
    with open(os.path.join(folder, "b.txt"), "w") as handle:
        handle.write("b")
    lone = os.path.join(workdir, "lone.txt")
    with open(lone, "w") as handle:
        handle.write("lone")
    result, build_dir = build_case(
        workdir, "mixed", '"{}" "{}"'.format(folder, lone))
    check(result.returncode == 0, "mixed case failed to configure:\n"
          + result.stdout.decode(errors="replace"))
    commands = build_commands(build_dir)
    check(os.path.join(folder, "b.txt") in commands and lone in commands,
          "mixed case did not ship both the directory contents and the file")

    empty = os.path.join(workdir, "empty_dir")
    os.makedirs(empty)
    result, build_dir = build_case(workdir, "empty_dir", '"{}"'.format(empty))
    check(result.returncode == 0, "empty directory case failed to configure:\n"
          + result.stdout.decode(errors="replace"))

    result, build_dir = build_case(workdir, "no_assets", "")
    check(result.returncode == 0, "empty declaration failed to configure:\n"
          + result.stdout.decode(errors="replace"))
    check("copy_if_different" not in build_commands(build_dir),
          "empty declaration still emits copy rules")


def case_duplicate_name_warns(workdir):
    first = os.path.join(workdir, "one", "dup.txt")
    second = os.path.join(workdir, "two", "dup.txt")
    for path in (first, second):
        os.makedirs(os.path.dirname(path))
        with open(path, "w") as handle:
            handle.write(path)
    result, _ = build_case(workdir, "duplicate", '"{}" "{}"'.format(first, second))
    output = result.stdout.decode(errors="replace")
    check(result.returncode == 0, "duplicate-name case failed to configure:\n"
          + output)
    check("overwrite each other" in output and first in output and second in output,
          "duplicate-name case did not warn naming both entries:\n" + output)


def case_nonexistent_entry_fails(workdir):
    missing = os.path.join(workdir, "does_not_exist.txt")
    result, _ = build_case(workdir, "missing", '"{}"'.format(missing))
    output = result.stdout.decode(errors="replace")
    check(result.returncode != 0, "nonexistent entry did not fail configure")
    check(missing in output,
          "nonexistent entry error does not name the entry:\n" + output)


def case_html_suffix_and_link_settings(workdir):
    folder = os.path.join(workdir, "web_assets")
    os.makedirs(folder)
    with open(os.path.join(folder, "c.txt"), "w") as handle:
        handle.write("c")
    with open(os.path.join(folder, "d.txt"), "w") as handle:
        handle.write("d")
    result, build_dir = build_case(workdir, "web", '"{}"'.format(folder),
                                   emscripten=True)
    check(result.returncode == 0, "web case failed to configure:\n"
          + result.stdout.decode(errors="replace"))
    link = link_command(build_dir)
    check_flag_pairing(link, "-s",
                       ("USE_GLFW=3", "ASSERTIONS=1", "WASM=1", "ASYNCIFY"))
    check(os.path.join(folder, "c.txt") + "@" in link,
          "directory form does not preload its files at the .data root:\n" + link)
    check_root_mounts(sorted([os.path.join(folder, "c.txt"),
                              os.path.join(folder, "d.txt")]), link)
    check(".html" in link, "web link does not target an .html output:\n" + link)

    # The -s settings and --preload-file must not leak into compile flags.
    flags = read(os.path.join(build_dir, "CMakeFiles", "probe.dir", "flags.make"))
    check("--preload-file" not in flags and "USE_GLFW=3" not in flags,
          "link settings still leak into compile flags:\n" + flags)


CASES = (
    case_explicit_file,
    case_asset_path_with_spaces,
    case_directory_form,
    case_mixed_and_empty_directory,
    case_duplicate_name_warns,
    case_nonexistent_entry_fails,
    case_html_suffix_and_link_settings,
)


def main() -> int:
    workdir = tempfile.mkdtemp(prefix="asset_bundle_selftest_")
    try:
        os.makedirs(os.path.join(workdir, "shared"))
        for case in CASES:
            try:
                case(workdir)
            except Failure as failure:
                print("FAIL {}: {}".format(case.__name__, failure))
                return 1
            print("ok   {}".format(case.__name__))
        print("\nall asset-bundling self-checks passed")
        return 0
    finally:
        shutil.rmtree(workdir, ignore_errors=True)


if __name__ == "__main__":
    raise SystemExit(main())
