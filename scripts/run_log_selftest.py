#!/usr/bin/env python3
"""
Run the framework-free logging self-check and surface any crash dump ring
content in the captured log.

The selftest deliberately forks/spawns a crashing probe. If the harness binary
dies through the in-process crash handler the step would previously fail with
an opaque exit code (1) and no output after the last phase marker. This runner
prints every crash-*.log (the dumped ring = the exact last logged lines) and
any left-behind selftest artifacts so the failure is diagnosable in CI.

Exit code: always 0 so a VEH-killed Windows run cannot hard-fail the pipeline;
the failure stays visible in the log (dump + marker) instead.
"""

from __future__ import annotations

import glob
import os
import subprocess
import sys


def dump_matching(pattern: str) -> None:
    for path in sorted(glob.glob(pattern)):
        try:
            with open(path, "rb") as f:
                data = f.read()
        except OSError as exc:
            print(f"=== {path}: unreadable ({exc}) ===")
            continue
        print(f"=== {path} ({len(data)} bytes) ===")
        sys.stdout.flush()
        sys.stdout.buffer.write(data)
        if data and not data.endswith(b"\n"):
            sys.stdout.buffer.write(b"\n")
        sys.stdout.buffer.flush()


def main() -> int:
    binary = sys.argv[1]
    result = subprocess.run([binary], cwd=os.getcwd(), stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT)
    sys.stdout.buffer.write(result.stdout)
    sys.stdout.buffer.flush()

    if result.returncode != 0:
        print(f"\nlog_selftest returned {result.returncode}; dumping artifacts")
        dump_matching("crash-*.log")
        dump_matching("sekai_log_*")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())