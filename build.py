#!/usr/bin/env python3
from pathlib import Path


def main():
    root = Path(__file__).resolve().parent
    chapters = sorted((root / "chapters").glob("[0-9][0-9]-*.md"))
    if not chapters:
        raise SystemExit("找不到章节文件：chapters/NN-*.md")
    output = root / "PBDS-ICPC.md"
    parts = [chapter.read_bytes().rstrip(b"\r\n") for chapter in chapters]
    output.write_bytes(b"\n\n".join(parts) + b"\n")
    print(output)


if __name__ == "__main__":
    main()
