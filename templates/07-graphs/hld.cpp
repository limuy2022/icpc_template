#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../02-common/forward-star.cpp"
#endif // PDF: endhide
int fa[GN], dep[GN], sub[GN], son[GN];
int top[GN], pos[GN], rev[GN];
void build_hld(const Graph& g, int root = 0) {
    int n = g.n;
    fill_n(sub, n, 1);
    fill_n(son, n, -1);
    if (n == 0) return;
    static int order[GN];
    int used = 1;
    order[0] = root;
    fa[root] = root;
    dep[root] = 0;
    for (int i = 0; i < n; ++i) {
        int u = order[i];
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (v == fa[u]) continue;
            fa[v] = u;
            dep[v] = dep[u] + 1;
            order[used++] = v;
        }
    }
    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (fa[v] != u) continue;
            sub[u] += sub[v];
            if (son[u] == -1 || sub[v] > sub[son[u]]) son[u] = v;
        }
    }
    int timer = 0;
    static int tasks[GN];
    int pending = 0;
    tasks[pending++] = root;
    while (pending) {
        int u = tasks[--pending], chain = u;
        for (; u != -1; u = son[u]) {
            top[u] = chain;
            pos[u] = timer;
            rev[timer++] = u;
            for (int id = g.head[u]; id; id = g.line[id].next) {
                int v = g.line[id].v;
                if (fa[v] == u && v != son[u]) tasks[pending++] = v;
            }
        }
    }
}
int hld_lca(int u, int v) {
    while (top[u] != top[v]) {
        if (dep[top[u]] < dep[top[v]]) swap(u, v);
        u = fa[top[u]];
    }
    return dep[u] < dep[v] ? u : v;
}
template <class Visit>
void hld_path(int u, int v, Visit visit, bool edge_values = false) {
    while (top[u] != top[v]) {
        if (dep[top[u]] < dep[top[v]]) swap(u, v);
        visit(pos[top[u]], pos[u] + 1);
        u = fa[top[u]];
    }
    if (dep[u] > dep[v]) swap(u, v);
    int l = pos[u] + (int)edge_values, r = pos[v] + 1;
    if (l < r) visit(l, r);
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
