# ICPC 模板 · 链式前向星、位运算、并查集

## 01 · 共用链式前向星

[C++ 源码](../templates/02-common/forward-star.cpp)

- head=0 表示链尾，cnt=0 使第一条弧为 1。遍历用 i=head[u]，下一条取 line[i].next。
- 从空图开始只用 add_undirected 时，两条弧的编号为 2k+1、2k+2，割点与桥模板中的无向边号为 k+1。重建图先 init。
- 按题目调整 GN、GA。点号为 1..n，且 n < GN，单张图最多 GA-1 条弧。大对象放全局或声明为 static。

## 02 · 64 位掩码（GNU C++）

[C++ 源码](../templates/02-common/bit-mask.cpp)

- `popcountll(0)` 返回 0。`ctzll` 的参数必须非零。对 64 位掩码，位号范围为 0 到 63。

## 03 · 并查集（1-based）

[C++ 源码](../templates/02-common/dsu.cpp)

- 元素编号 1..n，先调用 `dsu_init(n)`。`dsu_find(a) == dsu_find(b)` 判连通，`dsu_size[dsu_find(a)]` 取块大小。`unite` 返回是否真的合并。
- 路径压缩配合按大小合并，单次操作均摊$\mathcal{O}(\alpha (n))$。

- 割点、桥与点双连通分量见图论章节，其中的桥模板支持平行边。

# ICPC 模板 · 浮点精度

## 01 · long double

[C++ 源码](../templates/02-common/long-double.cpp)

- 浮点常量加 `L`，例如 `0.1L`。先按 double 舍入的常量再转成 long double，精度已经丢失。
- C 风格输入用 `scanf("%Lf", &x)`，输出用 `printf("%.18Lf\n", x)`。用 `fixed` 时，`setprecision` 指定小数位数。
- x86-64 Linux 上用默认 GCC 选项，double 和 long double 通常分别有 53 和 64 个二进制有效位。用 `numeric_limits<T>::digits` 确认。

## 02 · __float128（GNU C++ 与 glibc）

[C++ 源码](../templates/02-common/float128.cpp)

- GNU C++ 用 `Q` 后缀写常量。x86-64 Linux 上有 113 个二进制有效位。四则运算和比较可直接写。
- 支持 Float128 的 glibc 可用 `strtof128` 读入、`strfromf128` 输出、`sqrtf128` 开方，无需 `-lquadmath`。`%.36g` 保留最多 36 位有效数字，可用于往返转换。
- `sqrtq`、`strtoflt128`、`quadmath_snprintf` 需要链接 libquadmath。赛前按评测命令试编译。

## 03 · 常用数学函数对照

```text
long double              __float128 (glibc)
sqrtl, fabsl             sqrtf128, fabsf128
sinl, cosl, atan2l       sinf128, cosf128, atan2f128
expl, logl, powl         expf128, logf128, powf128
hypotl, fmal             hypotf128, fmaf128
```

- `std::sqrt(x)`、`std::sin(x)`、`std::log(x)` 接受 long double 参数，也返回 long double。
- GNU C++17/20 下，`std::sqrt`、`std::sin`、`std::log` 等可能没有匹配的 __float128 重载。用 glibc 的 `f128` 函数。
- 对 `__float128 x` 调用 `sqrtl(x)` 会先把参数转成 long double。用 `sqrtf128(x)` 保留精度。
