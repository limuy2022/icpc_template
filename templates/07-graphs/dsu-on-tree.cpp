#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../02-common/forward-star.cpp"
#include "hld.cpp"
#endif // PDF: endhide
int subtree_colors[GN];
void subtree_distinct_colors(const Graph& g,const int col[],int C) {
    build_hld(g);
    int n = g.n;
    static int count[GN];
    fill_n(count,C,0);
    int distinct = 0;
    auto add_vertex = [&](int u,int delta) {
        int c = col[u];
        if (count[c] == 0) ++distinct;
        count[c] += delta;
        if (count[c] == 0) --distinct;
    };
    auto add_subtree = [&](int u,int delta) {
        for (int i = pos[u]; i < pos[u]+sub[u]; ++i)
            add_vertex(rev[i],delta);
    };
    auto dfs = [&](auto&& self,int u,bool keep) -> void {
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (fa[v] == u && v != son[u]) self(self,v,false);
        }
        if (son[u] != -1) self(self,son[u],true);
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (fa[v] == u && v != son[u]) add_subtree(v,1);
        }
        add_vertex(u,1);
        subtree_colors[u] = distinct;
        if (!keep) add_subtree(u,-1);
    };
    if (n) dfs(dfs,0,false);
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
