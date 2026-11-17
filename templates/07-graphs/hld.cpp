#include <bits/stdc++.h>
using namespace std;
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
