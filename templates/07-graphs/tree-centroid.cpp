#include <bits/stdc++.h>
using namespace std;
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
