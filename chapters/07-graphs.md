# 图论 · 范围、建图与树的重心

按 OI Wiki 图论目录整理本次指定专题。GNU C++17，点编号从 0 开始，区间统一为左闭右开。容量、权值用 long long，累加超界时使用 i128。

图论页码：重心 48、HLD 49-50、树上启发式合并 51、树哈希 52、SCC 53、割点桥与双连通 54-55、2-SAT 56、二分图 57-60、最大流与最小割 61-62、费用流与上下界流 63-64。

## 树的重心

- 删除点 u 后的每个连通块大小都不超过原树的一半，则 u 是重心。令子树大小为 size，最大块大小为 $\max(n-\mathrm{size}[u],\max_{v\in\operatorname{child}(u)}\mathrm{size}[v])$。
- 树有一个或两个重心，两个重心必相邻。重心使到全部点的距离之和最小，删除重心适合做点分治。
- 树的直径中心最小化最远距离，树的重心最小化距离总和，两种目标应按题意选择。

```cpp
vector<int> tree_centroids(const Graph& g) {
    int n = g.n;
    if (n == 0) return {};
    static int parent[GN], size[GN], order[GN];
    fill_n(size,n,1);
    vector<int> answer;
    int used = 1; order[0] = 0;
    parent[0] = 0;
    for (int i = 0; i < used; ++i) {
        int u = order[i];
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (v == parent[u]) continue;
            parent[v] = u; order[used++] = v;
        }
    }
    for (int i = n-1; i >= 0; --i) {
        int u = order[i], largest = 0;
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (parent[v] != u) continue;
            size[u] += size[v]; largest = max(largest,size[v]);
        }
        largest = max(largest,n-size[u]);
        if (largest <= n/2) answer.push_back(u);
    }
    return answer;
}
```

- 输入为无自环、无重边的连通无向树，每条边用 add_undirected 加入。模板迭代求解，时间与空间均为 $\mathcal{O}(n)$。
- 带非负点权时，将大小改为权值和，条件改为每块权值不超过总权一半。负点权不满足这些结论。
- 换根求距离和，根答案为深度总和，从 u 到子节点 v 有 $F(v)=F(u)+n-2\mathrm{size}[v]$。

## 图论模板的边界约定

- 复用第 6 页的链式前向星。SCC 加有向边。割点与桥的模板成对加边，用 (arc-1)/2 判断父边。
- 部分 DFS、匹配和流模板含递归。链形图的递归深度可达点数，栈空间不足时需改显式栈或调整可用栈空间。
- 本部分共享 ll、i128、u64 类型别名，见第 13 页。来源范围按 OI Wiki Team 图论目录核对，整理日期 2026-11-05。

# 图论 · 树链剖分 HLD

## 迭代重链剖分

重儿子取子树最大的儿子。重链连续，子树也连续，子树 u 对应 $[\mathrm{pos}[u],\mathrm{pos}[u]+\mathrm{size}[u])$。

```cpp
struct HLD {
    int n;
    int parent[GN], depth[GN], size[GN], heavy[GN];
    int head[GN], pos[GN], rev[GN];
    HLD(const Graph& g, int root = 0)
        : n(g.n) {
        fill_n(size,n,1); fill_n(heavy,n,-1);
        if (n == 0) return;
        static int order[GN];
        int used = 1; order[0] = root;
        parent[root] = root; depth[root] = 0;
        for (int i = 0; i < n; ++i) {
            int u = order[i];
            for (int id = g.head[u]; id; id = g.line[id].next) {
                int v = g.line[id].v;
                if (v == parent[u]) continue;
                parent[v] = u; depth[v] = depth[u]+1;
                order[used++] = v;
            }
        }
        for (int i = n-1; i >= 0; --i) {
            int u = order[i];
            for (int id = g.head[u]; id; id = g.line[id].next) {
                int v = g.line[id].v;
                if (parent[v] != u) continue;
                size[u] += size[v];
                if (heavy[u] == -1 || size[v] > size[heavy[u]])
                    heavy[u] = v;
            }
        }
        int timer = 0;
        vector<pair<int,int>> tasks{{root,root}};
        while (!tasks.empty()) {
            auto [u,top] = tasks.back(); tasks.pop_back();
            for (; u != -1; u = heavy[u]) {
                head[u] = top; pos[u] = timer; rev[timer++] = u;
                for (int id = g.head[u]; id; id = g.line[id].next) {
                    int v = g.line[id].v;
                    if (parent[v] == u && v != heavy[u])
                        tasks.push_back({v,v});
                }
            }
        }
    }
    int lca(int u,int v) const {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u,v);
            u = parent[head[u]];
        }
        return depth[u] < depth[v] ? u : v;
    }
    template<class Visit>
    void path(int u,int v,Visit visit,bool edge_values=false) const {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u,v);
            visit(pos[head[u]],pos[u]+1);
            u = parent[head[u]];
        }
        if (depth[u] > depth[v]) swap(u,v);
        int l = pos[u]+(int)edge_values, r = pos[v]+1;
        if (l < r) visit(l,r);
    }
};
```

- 输入为连通树。预处理 $\mathcal{O}(n)$，一次路径拆成 $\mathcal{O}(\log n)$ 段，LCA 为 $\mathcal{O}(\log n)$。
- path 的段次序和方向适用于和、最大值等交换运算。矩阵积、字符串等需分 u/v 两侧收集，再按方向合并。
- 边权放在较深端点的位置，root 位置放单位元，调用时 edge_values=true，最后一段排除 LCA。

# 图论 · HLD 区间维护与换根

## 区间加、区间和线段树

```cpp
struct RangeAddSum {
    int n;
    ll sum[4*GN+4]{}, lazy[4*GN+4]{};
    RangeAddSum(int n) : n(n) {}
    void apply(int o,int l,int r,ll v) {
        sum[o] += v*(r-l); lazy[o] += v;
    }
    void push(int o,int l,int r) {
        if (r-l <= 1 || lazy[o] == 0) return;
        int mid = (l+r)/2;
        apply(o*2,l,mid,lazy[o]); apply(o*2+1,mid,r,lazy[o]);
        lazy[o] = 0;
    }
    void update(int o,int l,int r,int ql,int qr,ll v) {
        if (ql <= l && r <= qr) { apply(o,l,r,v); return; }
        push(o,l,r);
        int mid = (l+r)/2;
        if (ql < mid) update(o*2,l,mid,ql,qr,v);
        if (qr > mid) update(o*2+1,mid,r,ql,qr,v);
        sum[o] = sum[o*2]+sum[o*2+1];
    }
    ll query(int o,int l,int r,int ql,int qr) {
        if (ql <= l && r <= qr) return sum[o];
        push(o,l,r);
        int mid = (l+r)/2;
        ll ans = 0;
        if (ql < mid) ans += query(o*2,l,mid,ql,qr);
        if (qr > mid) ans += query(o*2+1,mid,r,ql,qr);
        return ans;
    }
    void add(int l,int r,ll v) { if (l < r) update(1,0,n,l,r,v); }
    ll get(int l,int r) { return l < r ? query(1,0,n,l,r) : 0; }
};
```

- 先建 HLD，再按原点权 w[u] 对 pos[u] 单点加入。输入满足 $0\leq l\leq r\leq n$，所有和、懒标记及乘积须在 ll 范围内。
- 路径加用 `h.path(u,v,[&](int l,int r){seg.add(l,r,delta);});`。
- 路径和用 `h.path(u,v,[&](int l,int r){ans+=seg.get(l,r);});`。点权子树和用 `seg.get(h.pos[u],h.pos[u]+h.size[u])`。
- 本实现路径操作 $\mathcal{O}(\log^2 n)$，固定根的子树操作 $\mathcal{O}(\log n)$。初始逐点加入成本为 $\mathcal{O}(n\log n)$。

## 换根后的子树

- 设预处理根不动、逻辑根改为 r。u=r 时，新子树是整棵树。
- 若 u 不是 r 的祖先，新子树仍是原来的子树区间。祖先关系可用 pos、size 判断。
- 若 u 是 r 的严格祖先，找到 u 到 r 路径上紧邻 u 的孩子 c，新子树是整棵树去掉 c 的原子树，即至多两个区间。
- 寻找 c 可沿链顶向上跳，或用倍增把 r 提升到 depth[u]+1。处理边权时另核对边界边是否计入。
- 长链剖分按子树最大深度选重儿子，常用于按深度 DP 与 k 级祖先；此处模板按子树大小剖分。

# 图论 · 树上启发式合并

## DSU on Tree，统计每个子树的不同颜色数

复用 HLD 的 size、heavy、pos、rev。颜色需预先离散化到 $[0,C)$，col.size() 与点数相同，C 不超过 GN。

```cpp
vector<int> subtree_distinct_colors(const Graph& g,
                                  const vector<int>& col,int C) {
    HLD h(g);
    int n = g.n;
    static int count[GN];
    fill_n(count,C,0);
    vector<int> answer(n);
    int distinct = 0;
    auto add_vertex = [&](int u,int delta) {
        int c = col[u];
        if (count[c] == 0) ++distinct;
        count[c] += delta;
        if (count[c] == 0) --distinct;
    };
    auto add_subtree = [&](int u,int delta) {
        for (int i = h.pos[u]; i < h.pos[u]+h.size[u]; ++i)
            add_vertex(h.rev[i],delta);
    };
    auto dfs = [&](auto&& self,int u,bool keep) -> void {
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (h.parent[v] == u && v != h.heavy[u]) self(self,v,false);
        }
        if (h.heavy[u] != -1) self(self,h.heavy[u],true);
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (h.parent[v] == u && v != h.heavy[u]) add_subtree(v,1);
        }
        add_vertex(u,1);
        answer[u] = distinct;
        if (!keep) add_subtree(u,-1);
    };
    if (n) dfs(dfs,0,false);
    return answer;
}
```

- 顺序为轻子树算完清空、重子树算完保留、加入轻子树与当前点、读取答案、按 keep 决定是否清空。
- 每个点只会经轻边被重复加入 $\mathcal{O}(\log n)$ 次，单次计数增删 $\mathcal{O}(1)$，总时间 $\mathcal{O}(n\log n)$。
- count 记录当前保留集合中每种颜色的频次。若要维护“频次最大颜色之和”，还要同步维护频次桶或可恢复的最大值。
- 清空时需要同步清空全部辅助信息；只把颜色 count 减回 0，却保留旧最大值，会污染后续子树答案。

## 容器小并大

- 另一种实现为每个子树维护 set/map，合并时把小容器插入大容器，再释放小容器。
- 用有序容器单次插入 $\mathcal{O}(\log n)$，常见总成本 $\mathcal{O}(n\log^2 n)$。必须按容器实际大小决定合并方向。
- 多重计数用 map 记录次数；set 仅能表示是否出现。动态删除、路径查询需另选数据结构，离线子树合并的假设是树结构和询问已知。

# 图论 · 树哈希与同构

## 无序有根树，双 64 位哈希

对子节点哈希做可交换聚合，避免邻接表次序影响结果。下面返回根处哈希，输入为连通树。

```cpp
u64 tree_mix(u64 x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x^(x>>30))*0xbf58476d1ce4e5b9ULL;
    x = (x^(x>>27))*0x94d049bb133111ebULL;
    return x^(x>>31);
}
u64 rooted_tree_hash(const Graph& g,int root,u64 salt) {
    int n = g.n;
    static int parent[GN], order[GN];
    int used = 1; order[0] = root;
    parent[root] = root;
    for (int i = 0; i < n; ++i) {
        int u = order[i];
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (v == parent[u]) continue;
            parent[v] = u; order[used++] = v;
        }
    }
    static u64 h[GN];
    for (int i = n-1; i >= 0; --i) {
        int u = order[i];
        h[u] = tree_mix(salt);
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (parent[v] == u) h[u] += tree_mix(h[v]^salt);
        }
    }
    return h[root];
}
```

- 比较两棵树必须共用同一组 salt，分别用两个独立 salt 得到一对哈希。先比较点数，再比较哈希。
- 无根树求一个或两个重心，分别以重心为根计算哈希，将结果排序后比较。空树须先单独处理。
- 无符号 64 位加乘按 $2^{64}$ 回绕是刻意的模运算。它有碰撞可能，双哈希不能提供确定性同构证明。
- 带点标号时，将标号的混合值作为节点初值；带边标号时，将边标号与对应子树哈希一起混合。
- 有序树的子节点顺序有意义，应按顺序滚动合并；交换求和会丢失次序信息。

## 确定性 AHU 编码

- 自底向上，将一个点的全部子节点类型编号排序，用该编号序列作为 key，分配唯一的新类型编号。
- 两棵树必须共用“序列到编号”的字典。分别建立字典后恰好得到相同整数编号，不能说明同构。
- 无根树仍先取重心。双重心时也可拆掉重心间的边，为两个半树编码后排序成一对。
- 排序版常用 $\mathcal{O}(n\log n)$ 级别时间与 $\mathcal{O}(n)$ 结构空间；字典比较长序列的具体成本要计入。

# 图论 · 强连通分量 SCC

## Kosaraju，迭代实现

强连通分量中的任意两点互相可达。将各分量缩成点，得到有向无环图，适合继续做拓扑 DP。

```cpp
struct SCC {
    int n, components = 0;
    Graph g, rg;
    int comp[GN];
    SCC(int n) : n(n), g(n), rg(n) {}
    void add_edge(int u,int v) { g.add(u,v); rg.add(v,u); }
    void build() {
        static char seen[GN];
        fill_n(seen,n,0);
        vector<int> order;
        for (int root = 0; root < n; ++root) if (!seen[root]) {
            vector<pair<int,int>> st{{root,g.head[root]}}; seen[root] = true;
            while (!st.empty()) {
                int u = st.back().first;
                int& i = st.back().second;
                if (i) {
                    int v = g.line[i].v; i = g.line[i].next;
                    if (!seen[v]) { seen[v] = true; st.push_back({v,g.head[v]}); }
                } else { order.push_back(u); st.pop_back(); }
            }
        }
        fill_n(comp,n,-1); components = 0;
        for (auto it = order.rbegin(); it != order.rend(); ++it) {
            int root = *it;
            if (comp[root] != -1) continue;
            vector<int> st{root}; comp[root] = components;
            while (!st.empty()) {
                int u = st.back(); st.pop_back();
                for (int id = rg.head[u]; id; id = rg.line[id].next) {
                    int v = rg.line[id].v;
                    if (comp[v] != -1) continue;
                    comp[v] = components; st.push_back(v);
                }
            }
            ++components;
        }
    }
};
```

- 时间与空间 $\mathcal{O}(n+m)$。这里的分量编号按缩点 DAG 的拓扑顺序，跨分量边 $u\to v$ 满足 comp[u]<comp[v]。
- 第二趟必须在反图上按第一趟结束时间的逆序遍历。第一趟需要真正的后序，不能把普通 BFS 序倒过来替代。
- 原图可以非连通、有自环或重边。构造缩点图时忽略同分量边；若做度数或计数，需要按题意去掉重边。

## Tarjan 版本的 low 判定

- 每点记录 dfn、low 和是否在栈。DFS 子节点回来用 low[v] 更新；遇到仍在栈中的已访问点，用 dfn[v] 更新。
- 当 low[u]=dfn[u]，弹栈直到 u，形成一个 SCC。指向已出栈点的边不能继续降低 low[u]。
- Tarjan 弹栈编号常为逆拓扑序，Kosaraju 也可因遍历方向不同而反转编号。使用 2-SAT 赋值前必须核对本实现的顺序。
- Gabow 用两个栈维护候选根，也能线性求 SCC。选择一种已经验证的实现，缩点后的应用相同。

# 图论 · 割点、桥与点双连通分量

## 无向图 DFS，支持重边

每个非自环边有唯一编号，两条弧以 (arc-1)/2 取得同一个无向边号。本模板忽略自环，点双块按去掉自环后的图定义。

```cpp
struct UndirectedCuts {
    int n, timer = 0, top = 0;
    Graph g;
    int tin[GN], low[GN], stamp[GN];
    char cut[GN], bridge[GA/2];
    int edge_stack[GA/2];
    vector<vector<int>> blocks;
    UndirectedCuts(int n)
        : n(n), g(n) {}
    int add_edge(int u,int v) {
        if (u == v) return -1;
        return (g.add_undirected(u,v)-1)/2;
    }
    void dfs(int u,int parent_edge) {
        tin[u] = low[u] = ++timer;
        int children = 0;
        for (int arc = g.head[u]; arc; arc = g.line[arc].next) {
            int v = g.line[arc].v, id = (arc-1)/2;
            if (id == parent_edge) continue;
            if (!tin[v]) {
                ++children; edge_stack[top++] = id; dfs(v,id);
                low[u] = min(low[u],low[v]);
                if (low[v] > tin[u]) bridge[id] = true;
                if (low[v] >= tin[u]) {
                    if (parent_edge != -1) cut[u] = true;
                    int block_id = (int)blocks.size();
                    vector<int> vertices;
                    while (true) {
                        int e = edge_stack[--top];
                        int a = g.line[2*e+2].v, b = g.line[2*e+1].v;
                        for (int x : {a,b}) if (stamp[x] != block_id) {
                            stamp[x] = block_id; vertices.push_back(x);
                        }
                        if (e == id) break;
                    }
                    blocks.push_back(vertices);
                }
            } else if (tin[v] < tin[u]) {
                low[u] = min(low[u],tin[v]); edge_stack[top++] = id;
            }
        }
        if (parent_edge == -1) cut[u] = children > 1;
    }
    void build() {
        timer = top = 0; blocks.clear();
        fill_n(tin,n,0); fill_n(low,n,0);
        fill_n(stamp,n,-1); fill_n(cut,n,0);
        fill_n(bridge,g.cnt/2,0);
        for (int u = 0; u < n; ++u) if (!tin[u]) {
            if (g.head[u] == 0) blocks.push_back(vector<int>{u});
            dfs(u,-1);
        }
    }
};
```

- 时间与空间 $\mathcal{O}(n+m)$。根是割点当且仅当 DFS 树孩子数>1；非根看是否有孩子 low[v]>=tin[u]。
- 树边是桥当且仅当 low[v]>tin[u]。必须按父边编号跳过，重边会提供另一条回到父点的边。

# 图论 · 边双、点双与圆方树

## 边双连通分量

删掉全部桥后，剩下图的连通块就是边双分量。复用上一页的 UndirectedCuts，先调用 build。

```cpp
vector<int> edge_bcc(const UndirectedCuts& graph) {
    vector<int> comp(graph.n,-1);
    int total = 0;
    for (int root = 0; root < graph.n; ++root) if (comp[root] == -1) {
        vector<int> st{root}; comp[root] = total;
        while (!st.empty()) {
            int u = st.back(); st.pop_back();
            for (int arc = graph.g.head[u]; arc; arc = graph.g.line[arc].next) {
                int v = graph.g.line[arc].v, id = (arc-1)/2;
                if (graph.bridge[id] || comp[v] != -1) continue;
                comp[v] = total; st.push_back(v);
            }
        }
        ++total;
    }
    return comp;
}
```

- 桥两端的分量必不同，把桥连接相应分量，得到桥森林。原图连通时缩点后是一棵树。
- 两点之间所有路径都经过的桥，对应缩点树上两分量之间的路径。桥数可用树上距离求。
- 连通无向图若要添加最少边使其无桥，桥树有 L 个叶子时需要 $\lceil L/2\rceil$ 条；原图已经无桥时为 0。
- 非连通图必须先明确是否同时要求连通，不能把各树的叶子数随意套入连通图公式。

## 点双连通分量

- 上一页 blocks 中每项为一个点双块的顶点集合，采用“桥单独为两点块、孤立点为单点块”的约定。
- 一个割点可以属于多个点双块，每条非自环边属于唯一点双块。点双块不应被当作顶点的不相交划分。
- 弹边栈时遇到当前树边才停止；顶点用 stamp 去重，不能在 DFS 搜索过程中把整个点栈随意清空。

## 圆方树（块割森林）

```cpp
void block_cut_forest(const UndirectedCuts& graph,Graph& forest) {
    int blocks = (int)graph.blocks.size();
    forest.init(graph.n + blocks);
    for (int i = 0; i < blocks; ++i)
        for (int u : graph.blocks[i])
            forest.add_undirected(u,graph.n+i);
}
```

- 先调用 graph.build()，按圆方树建成后的点和弧数核对 GN、GA。非连通图得到森林。该“所有原顶点+所有块”的版本中，原图孤立点与它的单点块相连。
- 两原点之间的圆方树路径描述依次经过的割点和点双块。做“必经点”计数时，注意是否排除两个询问端点。
- 块节点的权值或统计含义要单独定义，直接沿树路径求和可能重复计入割点。
- 常见的“只保留割点与块节点”版本省掉非割点圆点，需要记录每个非割点所属块，不能混用两个版本的编号。

# 图论 · 2-SAT

## 文字、蕴含图与赋值

变量 x_i 的两个文字编号为 2i（x_i=false）与 2i+1（x_i=true）。互为否定用 id^1。复用前面的 SCC。

```cpp
struct TwoSAT {
    int n;
    SCC graph;
    TwoSAT(int n) : n(n), graph(2*n) {}
    void add_or(int i,bool a,int j,bool b) {
        int x = 2*i+(int)a, y = 2*j+(int)b;
        graph.add_edge(x^1,y); graph.add_edge(y^1,x);
    }
    void imply(int i,bool a,int j,bool b) {
        add_or(i,!a,j,b);                  // (x_i=a) => (x_j=b)
    }
    void force(int i,bool value) { add_or(i,value,i,value); }
    optional<vector<int>> solve() {
        graph.build();
        vector<int> answer(n);
        for (int i = 0; i < n; ++i) {
            if (graph.comp[2*i] == graph.comp[2*i+1]) return nullopt;
            answer[i] = graph.comp[2*i+1] > graph.comp[2*i];
        }
        return answer;
    }
};
```

- 子句 $A\vee B$ 加边 $\neg A\to B$、$\neg B\to A$。本实现 SCC 编号为正拓扑序，故选择编号更大的文字为真。
- 同一变量与其否定在同一 SCC 中，当且仅当无解。算法为 $\mathcal{O}(n+m)$，m 为子句数。
- `imply` 同时加入逆否蕴含。只手写一条边而漏掉逆否边，可能让 SCC 判定失去逻辑对应。

## 常见约束翻译

- 至少一个为真，用 `add_or(i,true,j,true)`；不能同时为真，用 `add_or(i,false,j,false)`。
- 恰好一个为真，加入上述两个子句。相等 $x_i=x_j$，加入两个方向的蕴含。
- 强制 x_i=true 用 force(i,true)。形如“不能同时选择状态 a,b”用 add_or(i,!a,j,!b)。
- “至少三个里一个为真”不是一般 2-CNF 子句。不能仅凭引入一个变量就无条件转成等价 2-SAT。
- solve 给一组可行解，题目要求字典序最小解时还需要额外处理。

## 例子

```cpp
TwoSAT sat(2);
sat.add_or(0,true,1,true);       // x0 or x1
sat.imply(0,true,1,false);       // x0 => not x1
sat.force(0,true);
auto solution = sat.solve();    // {1,0}
```

# 图论 · 二分图最大匹配

## Hopcroft-Karp，左部与右部分开编号

左点编号 $[0,L)$，右点编号 $[0,R)$，g 只建左到右的边。L、R 均不超过 GN。时间 $\mathcal{O}((L+R+m)\sqrt{L+R})$。

```cpp
struct HopcroftKarp {
    int L, R, shortest;
    const int INF = INT_MAX/4;
    Graph g;
    int left_match[GN], right_match[GN], dist[GN], current[GN];
    HopcroftKarp(int L,int R) : L(L), R(R), g(L) {}
    void add_edge(int u,int v) { g.add(u,v); }
    bool bfs() {
        queue<int> q; shortest = INF;
        for (int u = 0; u < L; ++u) {
            dist[u] = left_match[u] == -1 ? 0 : INF;
            if (dist[u] == 0) q.push(u);
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (dist[u] >= shortest) continue;
            for (int id = g.head[u]; id; id = g.line[id].next) {
                int v = g.line[id].v;
                int mate = right_match[v];
                if (mate == -1) shortest = dist[u]+1;
                else if (dist[mate] == INF) {
                    dist[mate] = dist[u]+1; q.push(mate);
                }
            }
        }
        return shortest != INF;
    }
    bool dfs(int u) {
        for (int& id = current[u]; id; id = g.line[id].next) {
            int v = g.line[id].v, mate = right_match[v];
            bool good = mate == -1 ? dist[u]+1 == shortest
                         : dist[mate] == dist[u]+1 && dfs(mate);
            if (good) {
                left_match[u] = v; right_match[v] = u; return true;
            }
        }
        dist[u] = INF; return false;
    }
    int solve() {
        fill_n(left_match,L,-1);
        fill_n(right_match,R,-1);
        int answer = 0;
        while (bfs()) {
            copy_n(g.head,L,current);
            for (int u = 0; u < L; ++u)
                if (left_match[u] == -1 && dfs(u)) ++answer;
        }
        return answer;
    }
};
```

- BFS 找最短增广路长度，DFS 只走分层边，一轮求多条最短增广路。匹配中每个点最多关联一条匹配边。
- 下一页给出逐个左点找增广路的匈牙利算法（Kuhn）。
- 也可建单位容量流网，源点到左部、左到右、右部到汇点，最大流值就是最大匹配大小。

# 图论 · 二分图最大匹配（匈牙利算法）

## Kuhn，逐个左点找增广路

左部用 Graph(L) 建链式前向星，只加左到右的边。左、右各自从 0 编号，R 不超过 GN。返回最大匹配数及右点匹配到的左点，-1 表示未匹配。

```cpp
pair<int,vector<int>> kuhn(const Graph& g,int R) {
    vector<int> right_match(R,-1);
    static int seen[GN];
    fill_n(seen,R,-1);
    auto augment = [&](auto&& self,int u,int round) -> bool {
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (seen[v] == round) continue;
            seen[v] = round;
            if (right_match[v] == -1 ||
                self(self,right_match[v],round)) {
                right_match[v] = u;
                return true;
            }
        }
        return false;
    };
    int matches = 0;
    for (int u = 0; u < g.n; ++u)
        if (augment(augment,u,u)) ++matches;
    return {matches,right_match};
}
```

- 右点尚未匹配时直接占用。右点已匹配时，先尝试给原左点换一个搭档，递归成功后再改匹配。
- 每个左部起点用自己的编号作时间戳，递归共用这个时间戳。每轮最多访问每个右点一次。
- 时间 $\mathcal{O}(L+R+Lm)$，额外空间 $\mathcal{O}(L+R)$，m 为边数。大图可用前一页的 Hopcroft-Karp。两侧可以交换时，可把点数较少的一侧作为左部。
- 若要恢复左部匹配，从所有已匹配的右点反填即可。带费用的完整指派见带权匹配页。

# 图论 · 二分图判定与匹配结论

## BFS 染色，检查每个连通分量

```cpp
optional<vector<int>> bipartite_color(const Graph& g) {
    int n = g.n;
    vector<int> color(n,-1);
    for (int root = 0; root < n; ++root) if (color[root] == -1) {
        queue<int> q; q.push(root); color[root] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id = g.head[u]; id; id = g.line[id].next) {
                int v = g.line[id].v;
                if (color[v] == -1) {
                    color[v] = color[u]^1; q.push(v);
                } else if (color[v] == color[u]) return nullopt;
            }
        }
    }
    return color;
}
```

- 无向图为二分图当且仅当没有奇环。自环直接破坏二分性，重边不影响判定。时间 $\mathcal{O}(n+m)$。
- 一般图的匹配不能直接使用二分图算法。给定固定左右部时，先核对每条边都跨越左右部。

## 最小点覆盖与最大独立集

- Kőnig 定理，二分图最小点覆盖大小等于最大匹配大小。最大独立集大小为总点数减最小点覆盖大小。
- 求出最大匹配后，从所有未匹配左点出发，沿“左到右走非匹配边，右到左走匹配边”搜索。
- 可达左点集合为 Z_L、可达右点集合为 Z_R，最小点覆盖为 $(L\setminus Z_L)\cup Z_R$。

```cpp
// Call hk.solve() first; return selected left/right vertex sets.
pair<vector<int>,vector<int>> minimum_vertex_cover(const HopcroftKarp& hk) {
    static char a[GN], b[GN];
    fill_n(a,hk.L,0); fill_n(b,hk.R,0);
    queue<int> q;
    for (int u = 0; u < hk.L; ++u)
        if (hk.left_match[u] == -1) { a[u] = true; q.push(u); }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int id = hk.g.head[u]; id; id = hk.g.line[id].next) {
            int v = hk.g.line[id].v;
            if (hk.left_match[u] == v || b[v]) continue;
            b[v] = true;
            int w = hk.right_match[v];
            if (w != -1 && !a[w]) { a[w] = true; q.push(w); }
        }
    }
    vector<int> left, right;
    for (int u = 0; u < hk.L; ++u) if (!a[u]) left.push_back(u);
    for (int v = 0; v < hk.R; ++v) if (b[v]) right.push_back(v);
    return {left,right};
}
```

- Hall 定理，左部存在全覆盖匹配当且仅当任意 $S\subseteq L$ 均满足 $|N(S)|\geq|S|$。
- DAG 最小顶点不交路径覆盖，拆左右副本，每条有向边接左 u 到右 v，答案为 $n-|M|$。一般有向图先满足 DAG 条件。
- DM 分解可按交替可达与强连通性划分最大匹配结构。关键点/边是否属于所有最大匹配，不能仅凭当前一次匹配是否选中判断。

# 图论 · 二分图带权匹配 Hungarian

## 最小费用矩形指派

用 ForwardStar<ll>(n) 建左到右的带权边，右部有 m 点且 m 不超过 GN 和 KM_CAP。要求 $n\leq m$，每行选一个不同的列，求最小总费用。返回总费用与每行所选列。

```cpp
pair<ll,vector<int>> hungarian(const ForwardStar<ll>& g,int m) {
    int n = g.n;
    constexpr int KM_CAP = 505;
    static ll cost[KM_CAP][KM_CAP];
    for (int u = 0; u < n; ++u) {
        for (int id = g.head[u]; id; id = g.line[id].next) {
            const auto& edge = g.line[id];
            cost[u][edge.v] = edge.val;
        }
    }
    const ll INF = LLONG_MAX/4;
    static ll u[GN+1], v[GN+1], best[GN+1];
    static int p[GN+1], way[GN+1];
    static char used[GN+1];
    fill_n(u,n+1,0); fill_n(v,m+1,0); fill_n(p,m+1,0);
    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        int j0 = 0;
        fill_n(best,m+1,INF); fill_n(used,m+1,0);
        do {
            used[j0] = true;
            int i0 = p[j0], j1 = 0;
            ll delta = INF;
            for (int j = 1; j <= m; ++j) if (!used[j]) {
                ll reduced = cost[i0-1][j-1]-u[i0]-v[j];
                if (reduced < best[j]) { best[j] = reduced; way[j] = j0; }
                if (best[j] < delta) { delta = best[j]; j1 = j; }
            }
            for (int j = 0; j <= m; ++j) {
                if (used[j]) { u[p[j]] += delta; v[j] -= delta; }
                else best[j] -= delta;
            }
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int previous = way[j0];
            p[j0] = p[previous]; j0 = previous;
        } while (j0 != 0);
    }
    vector<int> assignment(n,-1);
    for (int j = 1; j <= m; ++j)
        if (p[j]) assignment[p[j]-1] = j-1;
    return {-v[0],assignment};
}
```

- 时间 $\mathcal{O}(n^2m)$，方阵为 $\mathcal{O}(n^3)$，额外空间 $\mathcal{O}(nm+n+m)$。可以有负费用。
- 每个左点到每个右点恰加一条边。内部构造费用矩阵供势能算法使用。费用、势能、费用差与总费用均须能用 ll 表示，并远小于 INF。
- 最大总权值可将权值取反后最小化。LLONG_MIN 不能直接取反，先按题目界限选择类型。
- 本实现要求每条指派边有有限费用。缺边可用足够大的有限惩罚并在答案中检查是否选中惩罚边，或建费用流模型显式省去缺边。
- “每行必须匹配”与“允许不匹配”是不同模型，后者可加虚拟列并按题意给费用，不能只返回当前较小的权值。
- Kuhn-Munkres 的标签算法维护相等子图并调整顶标；上面使用等价的势能与增广方式，适合直接处理矩形费用表。

# 图论 · 网络流 Dinic 最大流

```cpp
#define int long long
int n, m, s, t;
struct edge {
  int v, next, val;
} line[10010];
int cnt = 1, head[210], cur[210];
void add(int u, int v, int w) {
  cnt++;
  line[cnt].v = v;
  line[cnt].val = w;
  line[cnt].next = head[u];
  head[u] = cnt;
}
int depth[210];

bool bfs() {
  std::queue<int> q;
  memset(depth, -1, sizeof(depth));
  q.push(s);
  depth[s] = 0;
  while (!q.empty()) {
    int u = q.front();
    cur[u] = head[u];
    q.pop();
    for (int i = head[u]; i; i = line[i].next) {
      int v = line[i].v;
      if (depth[v] < 0 && line[i].val > 0) {
        depth[v] = depth[u] + 1;
        q.push(v);
      }
    }
  }
  return depth[t] >= 0;
}

int dfs(int u, int flow) {
  int allowflow = 0;
  if (u == t) {
    return flow;
  }
  for (int i = cur[u]; i && flow; i = line[i].next) {
    cur[u] = i;
    int v = line[i].v;
    if (depth[v] == depth[u] + 1 && line[i].val > 0) {
      int tmp = dfs(v, std::min(flow, line[i].val));
      if (tmp == 0) {
        depth[v] = -1;
      }
      line[i].val -= tmp;
      line[i ^ 1].val += tmp;
      allowflow += tmp;
      flow -= tmp;
    }
  }
  return allowflow;
}

int dinic() {
  int ans = 0;
  while (bfs()) {
    ans += dfs(s, LLONG_MAX);
  }
  return ans;
}
```

- 每条有向边连续调用 `add(u,v,w); add(v,u,0);`，保证 `i^1` 找到反向弧。
- 点号为 0 到 209，最多 5004 条原始边。多测清空 head 并设 cnt=1。
- 头文件放在宏前，入口用 signed main()。拼接其他模板时注意宏的作用范围。要求 s 与 t 不同，容量非负，总流量在 long long 范围内。
- 一般时间界 $\mathcal{O}(V^2E)$。dinic 返回本次新增流量，继续调用会沿用残量。

# 图论 · 最小割、建模与流算法选用

## 取出最小割

- 最大流最小割定理，最大 s-t 流值等于最小 s-t 割容量。运行 dinic() 到没有增广路后，再从残量网络取最小割。
- 调用 residual_reachable() 后，seen[u] 表示 u 属于源侧 S。原始边从 S 指向 T 的边构成一个最小割。

```cpp
// Call dinic() first. Read the source side of the final residual graph.
bool seen[210];
void residual_reachable() {
    memset(seen,0,sizeof(seen));
    std::queue<int> q;
    q.push(s); seen[s] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int i = head[u]; i; i = line[i].next) {
            int v = line[i].v;
            if (line[i].val > 0 && !seen[v]) {
                seen[v] = true; q.push(v);
            }
        }
    }
}
```

- 加边前记下正向弧编号 cnt+1，并保存原始容量。枚举割边时只看原始正向弧。
- 反向弧初始容量为 0 时，原始边 i 的流量为 line[i^1].val。也可用原始容量减去 line[i].val。

## 常用模型

- 边容量直接建边；点容量把点 u 拆成 u_in→u_out，容量为点上限，其他边按入出端相接。
- 无向割边容量 c 可建两个方向各为 c 的原始边，跨任一 s-t 割只计算从 S 到 T 的方向。
- 最大权闭合子图，将正权点接源点，负权点接汇点，依赖 u 选中必须选 v 时加 u→v 的足够大容量边。答案为正权和减最小割。
- 二元选择问题通常把两个状态放在割两侧，跨侧惩罚写为边容量。先验证每个状态组合的割贡献，避免源侧/汇侧含义颠倒。
- 若要最小割中割边数最少，可将每条容量改为 $cB+1$，取 B 大于任意割可能包含的边数，确保容量优先；同时检查溢出。
- “无限容量”只需严格大于所有有限可行割的容量上界，按题目构造，不要盲目设为 LLONG_MAX 后参与加法。

## 算法边界

- Ford-Fulkerson 每次找一条增广路，整数容量时间依赖最大流值 F。Edmonds-Karp 每次 BFS 找最短路，$\mathcal{O}(VE^2)$。
- Dinic 一轮分层后求阻塞流。ISAP 用到汇点的距离标号和 GAP；Push-Relabel 维护预流、高度与活动点，HLPP 加最高标号和全局重标号优化。
- 有理/浮点容量须重新设计正残量判断与停止条件。浮点容量还需选定误差阈值。

# 图论 · 最小费用流 SSP

## SPFA 最短增广路，允许负费用边

要求 s 与 t 不同，且从 s 可达的部分没有负费用环。所有可达路径距离与费用差须在 ll 范围内且绝对值远小于 INF。总费用用 i128。

```cpp
struct MinCostFlow {
    struct Edge { int v, next; ll val, cost; } line[GA]{};
    int n, cnt = 1, head[GN]{};
    MinCostFlow(int n) : n(n) {}
    void add(int u,int v,ll w,ll cost) {
        ++cnt; line[cnt] = {v,head[u],w,cost}; head[u] = cnt;
    }
    int add_edge(int u,int v,ll cap,ll cost) {
        int id = cnt+1;
        add(u,v,cap,cost); add(v,u,0,-cost);
        return id;
    }
    pair<ll,i128> flow(int s,int t,ll limit=LLONG_MAX) {
        const ll INF = LLONG_MAX/4;
        ll total = 0;
        i128 cost = 0;
        while (total < limit) {
            static ll dist[GN];
            static int previous[GN];
            static char in_queue[GN];
            fill_n(dist,n,INF); fill_n(previous,n,0);
            fill_n(in_queue,n,0);
            queue<int> q; q.push(s); in_queue[s] = true; dist[s] = 0;
            while (!q.empty()) {
                int u = q.front(); q.pop(); in_queue[u] = false;
                for (int id = head[u]; id; id = line[id].next) {
                    const auto& edge = line[id];
                    if (edge.val > 0 && dist[edge.v] > dist[u]+edge.cost) {
                        dist[edge.v] = dist[u]+edge.cost;
                        previous[edge.v] = id;
                        if (!in_queue[edge.v]) {
                            in_queue[edge.v] = true; q.push(edge.v);
                        }
                    }
                }
            }
            if (previous[t] == 0) break;
            ll add = limit-total;
            for (int v = t; v != s; v = line[previous[v]^1].v)
                add = min(add,line[previous[v]].val);
            for (int v = t; v != s; v = line[previous[v]^1].v) {
                int id = previous[v]; line[id].val -= add; line[id^1].val += add;
            }
            total += add; cost += (i128)add*dist[t];
        }
        return {total,cost};
    }
};
```

- 容量上限 GN、GA 见第 6 页。容量非负，费用可负，反向费用必须取负；输入费用不能为 LLONG_MIN。返回本次新增流量及新增费用。
- limit=K 表示尽量发送 K 单位，返回流量<K 表示无法完成要求。默认发送到无路可增广，得到最小费用最大流。
- SPFA 在本算法中的最坏单轮可很慢，整数流量 F 下常写 $\mathcal{O}(FVE)$。大数据使用下一页的势能 Dijkstra 路线。
- 有可达负费用环时，最短游走无下界，模板可能不终止；有限容量负环需要消圈或适合该模型的费用流算法。

# 图论 · 势能费用流与上下界流

## Primal-Dual 路线

- 势能 h 给边改权 $c'(u,v)=c(u,v)+h(u)-h(v)$。所有可达正残量边的改权非负，才可用 Dijkstra。
- 初始费用全非负时 h=0 即可。存在负费用边时先求一次源点最短路作为初始势能，仍要求无可达负费用环。
- 一轮 Dijkstra 得距离 d，所有可达点更新 $h(v)\leftarrow h(v)+d(v)$。不可达点保持原值，不把 INF 加入势能。
- 从 s 到 t 的原始单位路费为 $d(t)-h(s)+h(t)$，此处 h 是更新前的势能。沿最短路增广，再在残量网络重复。
- 零改权边形成可行最短路结构，可一次发送多条最短路上的流。势能、距离、总费用应按数值范围选宽整数。

## 无源汇上下界可行流

每条原始边满足下界 l、上界 r，$0\leq l\leq r$。先令流量为下界，再为可调整量建容量 r-l 的边。

```text
balance[u] -= l; balance[v] += l for each original u -> v
add residual edge u -> v with capacity r-l; save its edge ID
create super source SS and super sink TT
if balance[v] > 0: add SS -> v with capacity balance[v]
if balance[v] < 0: add v -> TT with capacity -balance[v]
feasible iff maxflow(SS,TT) equals sum of positive balances
original flow on edge = l + reverse residual of its saved edge
```

- balance[v] 为下界造成的“流入减流出”。正值需要额外向外发送，因此由 SS 输入补偿；负值向 TT 输出补偿。
- 每条原边即使容量上界相同也单独保存编号。r<l 可立即判无解，balance 求和与超级源总容量须防溢出。

## 有源汇可行流、最大流、最小流

- 额外加入 t→s 的足够大容量边，再按上面做可行循环流。该辅助边上的流量是找到的一组可行 s-t 流值 base。
- 判可行后，删除 SS、TT 的所有边和 t→s 辅助边，必须把相应正反残量都清零，保留原边当前残量。
- 上下界最大流为 base+原残量网络上的 maxflow(s,t)。
- 上下界最小流为 base-原残量网络上的 maxflow(t,s)。若要求流值非负，可将反向增广 limit 设为 base。
- 上述 t→s 辅助边建模针对非负 s-t 流值。若问题允许负流值，需要重新定义可行流方向与辅助结构。
- 下界还带费用时，先计入固定费用 $\sum_e l_e c_e$，再解剩余模型；普通最大流的可行性过程不自动保证费用最优。
