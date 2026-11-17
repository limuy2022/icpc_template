#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../02-common/forward-star.cpp"
#endif // PDF: endhide
int outcome[GN];
void game_outcomes(const Graph& g) {
    int n = g.n;
    static Graph reverse(0);
    reverse.init(n);
    static int remain[GN];
    fill_n(remain + 1, n, 0);
    fill_n(outcome + 1, n, 0);
    queue<int> q;
    for (int u = 1; u <= n; ++u) {
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            reverse.add(v, u);
            ++remain[u];
        }
        if (remain[u] == 0) {
            outcome[u] = -1;
            q.push(u);
        }
    }
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int id = reverse.head[v]; id; id = reverse.line[id].next) {
            int u = reverse.line[id].v;
            if (outcome[u] != 0) continue;
            if (outcome[v] == -1) {
                outcome[u] = 1;
                q.push(u);
            } else if (--remain[u] == 0) {
                outcome[u] = -1;
                q.push(u);
            }
        }
    }
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
