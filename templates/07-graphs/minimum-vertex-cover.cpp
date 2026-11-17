#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../02-common/forward-star.cpp"
#include "hopcroft-karp.cpp"
#endif // PDF: endhide
// Call hopcroft_karp(g,R) first; return selected left/right vertex sets.
pair<vector<int>, vector<int>> minimum_vertex_cover(const Graph& g, int R) {
    static char a[GN], b[GN];
    fill_n(a, g.n, 0);
    fill_n(b, R, 0);
    queue<int> q;
    for (int u = 0; u < g.n; ++u)
        if (left_match[u] == -1) {
            a[u] = true;
            q.push(u);
        }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (left_match[u] == v || b[v]) continue;
            b[v] = true;
            int w = right_match[v];
            if (w != -1 && !a[w]) {
                a[w] = true;
                q.push(w);
            }
        }
    }
    vector<int> left, right;
    for (int u = 0; u < g.n; ++u)
        if (!a[u]) left.push_back(u);
    for (int v = 0; v < R; ++v)
        if (b[v]) right.push_back(v);
    return {left, right};
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
