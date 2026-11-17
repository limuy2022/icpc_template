# ICPC 赛场模板

手册覆盖 PBDS、STL、字符串、数论、计算几何、图论、组合计数、博弈论、高斯消元、FFT 和异或线性基。代码按 GNU C++17 整理。图的点号和并查集元素从 1 开始，字符串下标从 0 开始。

## 生成 PDF

在仓库根目录运行：

```sh
python3 -m venv .venv
.venv/bin/python -m pip install -r requirements.txt
.venv/bin/python build.py
```

脚本按文件名顺序读取 `chapters/NN-*.md`，直接生成 `output/pdf/ICPC-Template.pdf`。每个一级标题对应一页 A4，并生成 PDF 书签。公式在本地转成矢量图形，打印时无需联网。

构建需要可嵌入的中文 TrueType 字体。Linux 可安装文泉驿正黑（Debian/Ubuntu 的 `fonts-wqy-zenhei`），Windows 可用系统的微软雅黑。也可用 `python build.py --font /path/to/font.ttf` 指定 TTF 或 TrueType TTC 文件。

生成的 PDF 已加入 Git 忽略列表。

## 编辑模板

代码在 `templates/` 下，按章节分目录。修改板子时直接编辑对应的 `.cpp`，说明和公式在 `chapters/` 下。Markdown 中单独一行的源码链接会在生成 PDF 时展开成 C++ 代码块：

```markdown
[C++ 源码](../templates/07-graphs/dinic.cpp)
```

路径相对于当前章节文件。每个 `.cpp` 的前两行统一写 `#include <bits/stdc++.h>` 和 `using namespace std;`，生成 PDF 时会省略这两行。在 GitHub 的 Markdown 预览里可以点击链接查看源码。部分板子依赖同章示例或共用类型和函数，按手册说明搭配使用。

可执行的用法示例用 `int main() { // PDF: body` 开头，正文统一缩进四个空格，最后一行用 `}` 结束。把示例依赖的 `#include` 放在入口前。生成 PDF 时只展开 main 正文，并还原它的缩进。

使用全局数组的板子每次只维护一份状态。多测按章节说明重新初始化，或重新调用构建函数。

所有 `.cpp` 都可以单独编译，例如 `g++ -std=gnu++17 templates/07-graphs/two-sat.cpp -o two-sat`。纯模板的依赖和空入口放在 `#if __INCLUDE_LEVEL__ == 0 // PDF: hide` 与 `#endif // PDF: endhide` 之间。这样单独编译时会带上依赖和入口，作为片段被另一个示例包含时会跳过这些内容，PDF 也会省略这些行。

## 格式化 C++ 源码

仓库根目录的 `.clang-format` 统一使用 4 空格缩进和 88 列。格式化全部板子：

```sh
find templates -name '*.cpp' -exec clang-format -i {} +
```

只检查格式时，把 `-i` 换成 `--dry-run --Werror`。


## 本地对拍

先编译待测程序和暴力程序，直接修改 `stress.py` 开头的 `candidate`、`brute`，再运行：

```sh
python3 templates/13-stress/stress.py
```

赛时填写 `gen.py` 里的 `make_case()`，例如改成 `return tree_case()`。运行 `python3 templates/13-stress/gen.py` 可单独生成一组数据。对拍用 `diff` 逐行比较，失败时保留 `fail.in`。
