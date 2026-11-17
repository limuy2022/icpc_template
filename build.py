#!/usr/bin/env python3
import argparse
from functools import lru_cache
from io import BytesIO
from pathlib import Path
import re
import shutil
import subprocess

from matplotlib import rcParams
from matplotlib.font_manager import FontProperties
from matplotlib.mathtext import math_to_image
from reportlab.graphics import renderPDF
from reportlab.lib.pagesizes import A4
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont, TTFError
from reportlab.pdfgen import canvas
from svglib.svglib import svg2rlg

ROOT = Path(__file__).resolve().parent
WIDTH, HEIGHT = A4
MARGIN = 36
TEXT_WIDTH = WIDTH - 2 * MARGIN
BODY = "Chinese"
MONO = "Courier"
FENCE = chr(96) * 3
INLINE = re.compile(r"(\$[^$]+\$|\x60[^\x60]+\x60)")
CPP_INCLUDE = re.compile(r"^\[C\+\+ 源码\]\(([^)]+\.cpp)\)$")
CPP_PREAMBLE = ["#include <bits/stdc++.h>", "using namespace std;"]


def register_font(path):
    if path is None:
        candidates = [
            Path("/usr/share/fonts/truetype/wqy/wqy-zenhei.ttc"),
            Path("/usr/share/fonts/windows/msyh.ttc"),
            Path("C:/Windows/Fonts/msyh.ttc"),
        ]
        if shutil.which("fc-match"):
            for family in ("Microsoft YaHei", "WenQuanYi Zen Hei", "Noto Sans SC"):
                match = subprocess.run(
                    ["fc-match", "-f", "%{file}", family],
                    capture_output=True, text=True, check=True,
                ).stdout
                if match:
                    candidates.append(Path(match))
    else:
        candidates = [path]

    for candidate in candidates:
        if candidate.is_file():
            try:
                font = TTFont(BODY, str(candidate))
            except TTFError:
                continue
            if all(ord(ch) in font.face.charWidths for ch in "算法图论"):
                pdfmetrics.registerFont(font)
                return
    raise SystemExit("找不到可嵌入的中文 TrueType 字体，请用 --font 指定字体文件。")


def read_pages():
    chapters = sorted((ROOT / "chapters").glob("[0-9][0-9]-*.md"))
    if not chapters:
        raise SystemExit("找不到章节文件：chapters/NN-*.md")
    pages = []
    for chapter in chapters:
        code = False
        for number, line in enumerate(chapter.read_text(encoding="utf-8").splitlines(), 1):
            include = CPP_INCLUDE.fullmatch(line) if not code else None
            if include:
                source = (chapter.parent / include[1]).resolve()
                if not source.is_relative_to(ROOT / "templates") or not source.is_file():
                    raise SystemExit(f"{chapter.name}:{number} 找不到模板 {include[1]}")
                body = pages[-1][1]
                body.append(("space", ""))
                lines = source.read_text(encoding="utf-8").splitlines()
                if lines[:2] == CPP_PREAMBLE:
                    lines = lines[2:]
                for cpp_line in lines:
                    body.append(("code", cpp_line.expandtabs(4)))
                body.append(("space", ""))
            elif line.startswith(FENCE):
                code = not code
                pages[-1][1].append(("space", ""))
            elif not code and line.startswith("# "):
                pages.append((line[2:], []))
            elif code:
                pages[-1][1].append(("code", line.expandtabs(4)))
            elif line.startswith("## "):
                pages[-1][1].append(("heading", line[3:]))
            else:
                pages[-1][1].append(("text" if line else "space", line))
        if code:
            raise SystemExit(f"{chapter.name} 的代码块没有结束标记。")
    return pages


@lru_cache(maxsize=2048)
def formula(tex, size):
    tex = re.sub(r"\\mathcal\s+([A-Za-z])", r"\\mathcal{\1}", tex)
    svg = BytesIO()
    depth = math_to_image(f"${tex}$", svg, prop=FontProperties(size=size),
                          dpi=72, format="svg")
    svg.seek(0)
    return svg2rlg(svg), depth


def inline_items(line, size):
    for token in INLINE.split(line):
        if token.startswith("$") and token.endswith("$"):
            drawing, depth = formula(token[1:-1], size * 1.12)
            factor = min(1, TEXT_WIDTH / drawing.width)
            yield drawing.width * factor, (drawing, depth, factor)
        else:
            code = token.startswith(chr(96)) and token.endswith(chr(96))
            font = MONO if code else BODY
            font_size = size * 0.94 if code else size
            words = [token[1:-1]] if code else re.findall(
                r"[A-Za-z0-9_]+|\s+|.", token
            )
            for word in words:
                yield pdfmetrics.stringWidth(word, font, font_size), (word, font, font_size)


def paragraph(pdf, text, y, size, leading):
    rows, row, width = [], [], 0
    for w, item in inline_items(text, size):
        if width + w > TEXT_WIDTH and row:
            rows.append(row)
            row, width = [], 0
        if not row and isinstance(item[0], str) and item[0].isspace():
            continue
        row.append((w, item))
        width += w
    if row:
        rows.append(row)
    for row in rows:
        upper, lower = size, 2
        for _, (value, depth, factor) in row:
            if not isinstance(value, str):
                upper = max(upper, (value.height - depth) * factor)
                lower = max(lower, depth * factor)
        y -= upper
        x = MARGIN
        if pdf is not None:
            for w, (value, font, zoom) in row:
                if isinstance(value, str):
                    pdf.setFont(font, zoom)
                    pdf.drawString(x, y, value)
                else:
                    pdf.saveState()
                    pdf.translate(x, y - font * zoom)
                    pdf.scale(zoom, zoom)
                    renderPDF.draw(value, pdf, 0, 0)
                    pdf.restoreState()
                x += w
        y -= max(leading - upper, lower + 1)
    return y


def layout(pdf, title, body, scale):
    y = paragraph(pdf, title, HEIGHT - 31, 21, 27) - 5
    max_code = max((len(line) for kind, line in body if kind == "code"), default=1)
    code_size = min(9.25 * scale, (TEXT_WIDTH - 10) / max(1, max_code) / 0.6)
    if code_size < 7.5:
        raise ValueError("代码行过长，请在 Markdown 源文件中断行。")
    for kind, line in body:
        if kind == "space":
            y -= 3 * scale
        elif kind == "code":
            y -= 10.2 * scale
            if pdf is not None:
                pdf.setFont(MONO, code_size)
                pdf.drawString(MARGIN + 10, y + 2, line)
        elif kind == "heading":
            y = paragraph(pdf, line, y, 13 * scale, 17 * scale) - 2
        else:
            y = paragraph(pdf, line, y, 9.6 * scale, 13 * scale)
    return y


def build_pdf(pages):
    rcParams["mathtext.fontset"] = "stix"
    rcParams["svg.fonttype"] = "path"
    stream = BytesIO()
    pdf = canvas.Canvas(stream, pagesize=A4, invariant=True)
    pdf.setTitle("ICPC Template")
    for number, (title, body) in enumerate(pages, 1):
        try:
            for scale in (1, 0.97, 0.94, 0.91, 0.88):
                if layout(None, title, body, scale) >= 43:
                    break
            else:
                raise ValueError("内容放不进一页，请压缩内容或拆分标题。")
            layout(pdf, title, body, scale)
        except ValueError as exc:
            raise SystemExit(f"第 {number} 页《{title}》：{exc}") from exc
        key = f"page-{number}"
        pdf.bookmarkPage(key)
        pdf.addOutlineEntry(title, key)
        pdf.setLineWidth(0.5)
        pdf.line(MARGIN, 32, WIDTH - MARGIN, 32)
        pdf.setFont(MONO, 8)
        pdf.drawString(MARGIN, 20, "ICPC / GNU C++17")
        pdf.setFont("Helvetica", 8)
        pdf.drawRightString(WIDTH - MARGIN, 20, f"{number} / {len(pages)}")
        pdf.showPage()
    pdf.save()
    return stream.getvalue()


def main():
    parser = argparse.ArgumentParser(description="从章节 Markdown 生成 ICPC 赛场模板 PDF")
    parser.add_argument("--font", type=Path, help="中文 TrueType 字体文件（TTF 或 TTC）")
    args = parser.parse_args()
    register_font(args.font)
    content = build_pdf(read_pages())
    output = ROOT / "output" / "pdf" / "ICPC-Template.pdf"
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(content)
    print(output)


if __name__ == "__main__":
    main()
