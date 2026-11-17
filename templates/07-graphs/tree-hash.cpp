#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../05-number-theory/modular-arithmetic.cpp"
#include "../02-common/forward-star.cpp"
#endif // PDF: endhide
u64 tree_mix(u64 x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}
u64 rooted_tree_hash(const Graph& g, int root, u64 salt) {
    int n = g.n;
    static int parent[GN], order[GN];
    int used = 1;
    order[0] = root;
    parent[root] = root;
    for (int i = 0; i < n; ++i) {
        int u = order[i];
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (v == parent[u]) continue;
            parent[v] = u;
            order[used++] = v;
        }
    }
    static u64 h[GN];
    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        h[u] = tree_mix(salt);
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (parent[v] == u) h[u] += tree_mix(h[v] ^ salt);
        }
    }
    return h[root];
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
