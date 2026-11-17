#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../02-common/forward-star.cpp"
#endif // PDF: endhide
int scc_count;
Graph scc_g(0), scc_rg(0);
int scc_id[GN];
void scc_init(int n) { scc_g.init(n); scc_rg.init(n); }
void scc_add(int u,int v) { scc_g.add(u,v); scc_rg.add(v,u); }
void scc_build() {
    int n = scc_g.n;
    static char seen[GN];
    fill_n(seen,n,0);
    static int order[GN], stack[GN], arc[GN];
    int done = 0;
    for (int root = 0; root < n; ++root) if (!seen[root]) {
        int size = 1; stack[0] = root; arc[0] = scc_g.head[root];
        seen[root] = true;
        while (size) {
            int u = stack[size-1], &i = arc[size-1];
            if (i) {
                int v = scc_g.line[i].v; i = scc_g.line[i].next;
                if (!seen[v]) {
                    seen[v] = true; stack[size] = v; arc[size++] = scc_g.head[v];
                }
            } else { order[done++] = u; --size; }
        }
    }
    fill_n(scc_id,n,-1); scc_count = 0;
    while (done) {
        int root = order[--done];
        if (scc_id[root] != -1) continue;
        int size = 1; stack[0] = root; scc_id[root] = scc_count;
        while (size) {
            int u = stack[--size];
            for (int id = scc_rg.head[u]; id; id = scc_rg.line[id].next) {
                int v = scc_rg.line[id].v;
                if (scc_id[v] != -1) continue;
                scc_id[v] = scc_count; stack[size++] = v;
            }
        }
        ++scc_count;
    }
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
