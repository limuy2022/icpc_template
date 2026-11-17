# ICPC 赛场模板

手册覆盖 PBDS、STL、字符串、数论、计算几何、图论、组合计数、博弈论、高斯消元、FFT 和异或线性基。代码按 GNU C++17 整理。

## 生成 PDF

在仓库根目录运行：

```sh
python3 -m venv .venv
.venv/bin/python -m pip install -r requirements.txt
.venv/bin/python build.py
```

脚本按文件名顺序读取 `chapters/NN-*.md`，直接生成 `output/pdf/ICPC-Template.pdf`。每个一级标题对应一页 A4，并生成 PDF 书签。公式在本地转成矢量图形，打印时无需联网。

构建需要可嵌入的中文 TrueType 字体。Linux 可安装文泉驿正黑（Debian/Ubuntu 的 `fonts-wqy-zenhei`），Windows 可用系统的微软雅黑。也可用 `python build.py --font /path/to/font.ttf` 指定 TTF 或 TrueType TTC 文件。

修改手册请直接编辑 `chapters/` 的源文件。生成的 PDF 已加入 Git 忽略列表。
