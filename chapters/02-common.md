# ICPC 模板 · 链式前向星、位运算、并查集

## 01 · 共用链式前向星

```cpp
constexpr int GN = 2005, GA = 10010; // vertices, arc slots
template<class W = int>
struct ForwardStar {
    struct Edge { int v, next; W val; } line[GA]{};
    int n, cnt = 0, head[GN]{};
    ForwardStar(int n) : n(n) {}
    void init(int vertices) {
        n = vertices; cnt = 0;
        fill_n(head,n,0);
    }
    int add(int u,int v,W w = W{}) {
        ++cnt;
        line[cnt] = {v,head[u],w};
        head[u] = cnt;
        return cnt;
    }
    int add_undirected(int u,int v,W w = W{}) {
        int id = add(u,v,w);
        add(v,u,w);
        return id;
    }
};
using Graph = ForwardStar<>;
```

- head=0 表示链尾，cnt=0 使第一条弧为 1。遍历用 i=head[u]，下一条取 line[i].next。
- 从空图开始只用 add_undirected 时，两条弧的编号为 2k+1、2k+2，无向边号为 k。重建图先 init。
- 按题目调整 GN、GA。点号须小于 GN，单张图最多 GA-1 条弧。大对象放全局或声明为 static。

## 02 · 64 位掩码（GNU C++）

```cpp
unsigned long long mask = (1ULL << 2) | (1ULL << 5);
int count = __builtin_popcountll(mask);       // 2
for (auto m = mask; m; m &= m - 1) {
    int bit = __builtin_ctzll(m);             // indices: 2, 5
    (void)bit;
}
```

- `popcountll(0)` 返回 0。`ctzll` 的参数必须非零。对 64 位掩码，位号范围为 0 到 63。

## 03 · 并查集（0-based）

```cpp
struct DSU {
    int parent[GN], size[GN];
    DSU(int n) {
        iota(parent,parent+n,0);
        fill_n(size,n,1);
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        return true;
    }
};
```

- `find(a) == find(b)` 判连通，`size[find(a)]` 取连通块大小。`unite` 返回是否真的合并。
- 路径压缩配合按大小合并，单次操作均摊$\mathcal{O}(\alpha (n))$。

- 割点、桥与点双连通分量见图论章节，其中的桥模板支持平行边。
