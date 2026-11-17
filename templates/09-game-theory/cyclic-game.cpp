#include <bits/stdc++.h>
using namespace std;
int outcome[GN];
void game_outcomes(const Graph& g) {
    int n = g.n;
    static Graph reverse(0);
    reverse.init(n);
    static int remain[GN];
    fill_n(remain,n,0);
    fill_n(outcome,n,0);
    queue<int> q;
    for (int u = 0; u < n; ++u) {
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            reverse.add(v,u);
            ++remain[u];
        }
        if (remain[u] == 0) { outcome[u] = -1; q.push(u); }
    }
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int id = reverse.head[v]; id; id = reverse.line[id].next) {
            int u = reverse.line[id].v;
            if (outcome[u] != 0) continue;
            if (outcome[v] == -1) { outcome[u] = 1; q.push(u); }
            else if (--remain[u] == 0) { outcome[u] = -1; q.push(u); }
        }
    }
}
