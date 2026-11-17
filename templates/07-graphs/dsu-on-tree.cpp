#include <bits/stdc++.h>
using namespace std;
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
