# Python · 本地对拍

先编译待测程序和暴力程序，两者都用标准输入输出。把 stress.py 和 gen.py 放在同一目录。

```text
python3 templates/13-stress/stress.py ./main ./brute --seed 0 --rounds 200
```

[Python 源码](../templates/13-stress/stress.py)

- 默认按空白分词后比较答案。输出有浮点误差或允许多解时，按题目规则修改比较逻辑。
- 失败时在当前目录保存 fail.in、两份标准输出和错误输出。记录 seed 与 kind，复现时可以直接喂 fail.in。
- 修改 gen.py 让生成数据符合题目输入格式。暴力程序应能在小数据上稳定给出正确结果。

# Python · 常用小数据生成

每次以同一个种子初始化随机数生成器。用 --kind 选数据类型，也可以把同一参数传给对拍脚本。

```text
python3 templates/13-stress/gen.py 42 --kind tree > test.in
```

[Python 源码](../templates/13-stress/gen.py)

- array、perm 和 string 都先输出长度。tree 输出点数及 n-1 条边。graph 先输出 n m，再输出 m 条无向边。
- 图和树的点号为 1..n。graph 无重边和自环，可能不连通。枚举所有点对适合对拍小数据。
- tree 保证连通，但边由每个点向较小编号选父亲。可手动加链、星形等极端用例补足覆盖。
