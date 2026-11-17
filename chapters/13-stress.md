# Python · 本地对拍

编译好两份程序后，直接改脚本开头的 candidate 和 brute。将 stress.py、gen.py 放在同一目录。

```text
python3 templates/13-stress/stress.py
```

[Python 源码](../templates/13-stress/stress.py)

- 每轮直接随机生成数据，一直跑到程序退出非零或答案不同。diff 逐行比较，空格和换行不同也会停拍。
- 失败时直接用当前目录的 fail.in 复现。两份程序的标准输出分别写入 candidate.out 和 brute.out，错误输出留在终端。
- 运行前按题意填写 make_case。需要手动停拍时按 Ctrl+C。

# Python · 常用小数据生成

按题目填写 make_case。例如需要随机树时，写 return tree_case()。

```text
python3 templates/13-stress/gen.py > test.in
```

[Python 源码](../templates/13-stress/gen.py)

- array、perm 和 string 都先输出长度。tree 输出点数及 n-1 条边。graph 先输出 n m，再输出 m 条无向边。
- 图和树的点号为 1..n。graph 无重边和自环，可能不连通。枚举所有点对适合对拍小数据。
- tree 保证连通，但边由每个点向较小编号选父亲。可手动加链、星形等极端用例补足覆盖。
