# ICPC 模板 · 链式前向星、位运算、并查集

## 01 · 共用链式前向星

[C++ 源码](../templates/02-common/forward-star.cpp)

- head=0 表示链尾，cnt=0 使第一条弧为 1。遍历用 i=head[u]，下一条取 line[i].next。
- 从空图开始只用 add_undirected 时，两条弧的编号为 2k+1、2k+2，无向边号为 k。重建图先 init。
- 按题目调整 GN、GA。点号须小于 GN，单张图最多 GA-1 条弧。大对象放全局或声明为 static。

## 02 · 64 位掩码（GNU C++）

[C++ 源码](../templates/02-common/bit-mask.cpp)

- `popcountll(0)` 返回 0。`ctzll` 的参数必须非零。对 64 位掩码，位号范围为 0 到 63。

## 03 · 并查集（0-based）

[C++ 源码](../templates/02-common/dsu.cpp)

- 先调用 `dsu_init(n)`。`dsu_find(a) == dsu_find(b)` 判连通，`dsu_size[dsu_find(a)]` 取块大小。`unite` 返回是否真的合并。
- 路径压缩配合按大小合并，单次操作均摊$\mathcal{O}(\alpha (n))$。

- 割点、桥与点双连通分量见图论章节，其中的桥模板支持平行边。
