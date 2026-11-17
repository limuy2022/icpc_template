# ICPC 赛场模板

手册覆盖 PBDS、STL、字符串算法、数论、计算几何、图论、组合计数、博弈论、消元、FFT 和异或线性基。代码按 GNU C++17 整理。

## 生成 Markdown

在仓库根目录运行：

```sh
python3 build.py
```

脚本按文件名顺序合并 `chapters/NN-*.md`，生成根目录的 `PBDS-ICPC.md`。只需要 Python 3。生成文件已被 Git 忽略，修改手册请直接编辑 `chapters/` 里的源文件。
