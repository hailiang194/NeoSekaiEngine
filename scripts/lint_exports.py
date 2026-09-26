#!/usr/bin/env python3
"""
Lint the engine's public headers for consumer-facing declarations that lack EXTENDAPI.

A declaration is consumer-facing when a consumer can name it and the compiler must
emit a symbol for it, so a consumer linking a Windows shared build resolves it from
the import library instead of failing with LNK2019. That means a non-inline,
non-pure, out-of-line function: a free function, or a public member of a class.

Skipped, because they emit no symbol a consumer can reference: templates, pure
virtual functions, deleted and defaulted functions, inline definitions, data
members, enums, typedefs, using-aliases, and friends.

Anything the heuristics cannot classify is reported as UNCLASSIFIED rather than
passed silently, so an unparsable header fails the check instead of hiding a gap.

Usage: lint_exports.py <header> [<header> ...]
Exits non-zero when findings or unclassified declarations exist.
"""

import re
import sys
from pathlib import Path

EXCLUSIONS = {
    "include/wrapper/": "internal raylib backend behind USE_RAYLIB, not consumer API",
}

SYMBOL_EXCLUSIONS = {
    "CreateApplication": "library-declared hook fulfilled by each consumer, not a library symbol",
}

SCOPE_RE = re.compile(
    r"(class|struct|interface|namespace|union|enum)\s+(EXTENDAPI\s+)?([A-Za-z_]\w*)"
    r"\s*(?:<[^;{}]*>)?\s*(?::[^{};]*)?$"
)
FUNCTION_NAME_RE = re.compile(r"([A-Za-z_~][\w]*)\s*\(")
TEMPLATE_RE = re.compile(r"\btemplate\s*<")
MACRO_CALL_RE = re.compile(
    r"\b[A-Z_][A-Z0-9_]*[ \t]*\((?:[^(){}]|\([^(){}]*\))*\)[ \t]*;?"
)


def blank(match):
    return "".join(c if c == "\n" else " " for c in match.group(0))


def strip_macro_invocations(text):
    return MACRO_CALL_RE.sub(blank, text)
SPECIFIER_RE = re.compile(r"^\s*(public|private|protected)\s*:")

SKIP_TOKENS = (
    ("= delete", "deleted"),
    ("=default", "defaulted"),
    ("= default", "defaulted"),
    ("= 0", "pure virtual"),
    ("typedef ", "typedef"),
    ("using ", "alias"),
    ("friend ", "friend"),
)


def strip_comments(text):
    out = []
    i = 0
    n = len(text)
    while i < n:
        two = text[i : i + 2]
        if two == "/*":
            end = text.find("*/", i + 2)
            end = n if end < 0 else end + 2
            out.append("".join(c if c == "\n" else " " for c in text[i:end]))
            i = end
        elif two == "//":
            end = text.find("\n", i)
            end = n if end < 0 else end
            out.append(" " * (end - i))
            i = end
        else:
            out.append(text[i])
            i += 1
    return "".join(out)


def strip_preprocessor(text):
    return "\n".join("" if ln.lstrip().startswith("#") else ln for ln in text.split("\n"))


def line_of(text, index):
    return text.count("\n", 0, index) + 1


def first_token_index(statement, offset):
    return offset + (len(statement) - len(statement.lstrip()))


def scan(text):
    """Walk the file once, yielding (scope, start, statement, has_body) per declaration."""
    stack = []
    buf = []
    buf_start = 0
    has_body = False
    i = 0
    n = len(text)
    results = []
    inline_defs = set()
    while i < n:
        c = text[i]
        if c == "{":
            head = "".join(buf).strip()
            matched = SCOPE_RE.search(head)
            if TEMPLATE_RE.search(head):
                stack.append({"kind": "template", "name": "", "access": "public"})
            elif matched:
                kind = matched.group(1)
                stack.append(
                    {
                        "kind": kind,
                        "name": matched.group(3),
                        "marked": bool(matched.group(2)),
                        "access": "private" if kind in ("class",) else "public",
                    }
                )
            elif ")" in head or TEMPLATE_RE.search(head):
                has_body = True
                if re.search(r"\binline\b", head):
                    defined = FUNCTION_NAME_RE.search(head)
                    if defined:
                        inline_defs.add(defined.group(1))
                stack.append({"kind": "block", "name": "", "access": "public"})
            else:
                stack.append({"kind": "block", "name": "", "access": "public"})
            buf.clear()
            i += 1
            continue
        if c == "}":
            if stack:
                stack.pop()
            buf.clear()
            i += 1
            continue
        if c == ";":
            stmt = "".join(buf)
            scope = stack[-1] if stack else {"kind": "file", "name": "", "access": "public"}
            if stmt.strip() and scope["kind"] not in ("block", "template"):
                results.append((scope, first_token_index(stmt, buf_start), stmt, has_body))
            buf.clear()
            has_body = False
            i += 1
            continue
        if not buf:
            buf_start = i
        buf.append(c)
        i += 1

    if buf and "".join(buf).strip():
        scope = stack[-1] if stack else {"kind": "file", "name": "", "access": "public"}
        if scope["kind"] not in ("block", "template"):
            tail = "".join(buf)
            results.append((scope, first_token_index(tail, buf_start), tail, has_body))

    return results, inline_defs


def classify(stmt, has_body, in_class):
    flat = " ".join(stmt.split())
    if not flat:
        return None, ""
    if has_body or "{" in flat:
        return None, "inline definition"
    if flat.startswith("EXTENDAPI") or re.search(r"\bEXTENDAPI\b", flat):
        return None, "annotated"
    for token, reason in SKIP_TOKENS:
        if token in flat:
            return None, reason
    if TEMPLATE_RE.search(flat):
        return None, "template"
    if re.match(r"^(extern|static)\b", flat):
        return None, "not an exported free function"
    if "(" not in flat:
        return None, "data member or type"
    if re.search(r"\binline\b", flat):
        return None, "inline"
    if flat.count("(") != flat.count(")"):
        return "UNCLASSIFIED", "unbalanced parentheses"
    return "MISSING", ""


def function_name(flat):
    m = FUNCTION_NAME_RE.search(flat)
    return m.group(1) if m else flat[:48]


def scan_header(text):
    text = strip_preprocessor(strip_comments(text))
    text = strip_macro_invocations(text)
    findings = []
    unmarked = {}
    excluded = []
    results, inline_defs = scan(text)
    for scope, start, stmt, has_body in results:
        kind = scope["kind"]
        if kind in ("namespace", "block", "file", "enum", "union"):
            in_class = False
        elif kind in ("class", "struct", "interface"):
            in_class = True
        else:
            in_class = False

        if in_class:
            flat = " ".join(stmt.split())
            spec = SPECIFIER_RE.match(flat)
            declaration = stmt
            decl_index = start
            if spec:
                scope["access"] = spec.group(1)
                flat = flat[spec.end() :].strip()
                colon = stmt.index(":")
                decl_index = first_token_index(stmt[colon + 1 :], start + colon + 1)
                declaration = SPECIFIER_RE.sub("", stmt, count=1)
                if not flat:
                    continue
            if scope["marked"]:
                continue
            if scope["access"] != "public":
                continue
            if re.match(r"^(class|struct|interface|enum|union|typedef|using|static_assert)\b", flat):
                continue
            if TEMPLATE_RE.search(flat):
                continue
            verdict, _ = classify(declaration, has_body, True)
            if verdict == "MISSING":
                signature = " ".join(flat.split())
                if len(signature) > 70:
                    signature = signature[:67] + "..."
                unmarked.setdefault(scope["name"], (scope, []))[1].append(
                    (line_of(text, decl_index), signature)
                )
            elif verdict:
                findings.append((line_of(text, decl_index), f"{scope['name']}::{flat[:48]}", verdict))
        else:
            flat = " ".join(stmt.split())
            if re.match(r"^(class|struct|interface|enum|union|static_assert)\b", flat):
                continue
            verdict, _ = classify(stmt, has_body, False)
            if verdict == "MISSING":
                name = FUNCTION_NAME_RE.search(flat)
                if name and name.group(1) != "EXTENDAPI":
                    symbol = name.group(1)
                    if symbol in inline_defs:
                        excluded.append(
                            (
                                line_of(text, start),
                                symbol,
                                f"SKIPPED: inline definition in this header emits no symbol"
                                f" -> {flat[:60]}",
                            )
                        )
                    elif symbol in SYMBOL_EXCLUSIONS:
                        excluded.append(
                            (
                                line_of(text, start),
                                symbol,
                                f"SKIPPED: {SYMBOL_EXCLUSIONS[symbol]} -> {flat[:60]}",
                            )
                        )
                    else:
                        findings.append(
                            (line_of(text, start), symbol, "MISSING: unannotated free function")
                        )
            elif verdict:
                findings.append((line_of(text, start), flat[:48], verdict))

    for name, (scope, members) in sorted(unmarked.items()):
        listing = "; ".join(f"line {ln}: {sig}" for ln, sig in members)
        findings.append(
            (
                min(ln for ln, _ in members),
                name,
                f"MISSING: class and public members have no EXTENDAPI -> {listing}",
            )
        )
    return findings, excluded


def excluded(relpath):
    posix = relpath.as_posix()
    for fragment, reason in EXCLUSIONS.items():
        if fragment in posix:
            return reason
    return None


def main(argv):
    if not argv:
        print("usage: lint_exports.py <header> [<header> ...]", file=sys.stderr)
        return 2

    root = Path.cwd()
    total = 0
    unclassified = 0
    for arg in argv:
        path = Path(arg)
        try:
            relpath = path.relative_to(root)
        except ValueError:
            relpath = path
        reason = excluded(relpath)
        if reason:
            print(f"-- skipped {relpath}: {reason}")
            continue
        if not path.is_file():
            print(f"error: header not found: {path}", file=sys.stderr)
            return 2
        findings, skipped = scan_header(path.read_text(errors="replace"))
        if not findings and not skipped:
            continue
        print(f"{relpath}")
        for line, symbol, note in sorted(skipped):
            print(f"    {relpath}:{line}: {symbol}: {note}")
        for line, symbol, verdict in sorted(findings):
            print(f"    {relpath}:{line}: {symbol}: {verdict}")
            if verdict.startswith("UNCLASSIFIED"):
                unclassified += 1
        total += len(findings)

    if total:
        print(f"\n{total} finding(s): {total - unclassified} missing EXTENDAPI, {unclassified} unclassified.")
        return 1
    print("public headers: every consumer-facing declaration carries EXTENDAPI")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
