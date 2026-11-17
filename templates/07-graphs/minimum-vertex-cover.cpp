#include <bits/stdc++.h>
using namespace std;
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
