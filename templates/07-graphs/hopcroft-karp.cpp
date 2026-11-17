#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../02-common/forward-star.cpp"
#endif // PDF: endhide
int hk_shortest;
const int HK_INF = INT_MAX/4;
int left_match[GN], right_match[GN], hk_dist[GN], hk_cur[GN];
bool hk_bfs(const Graph& g) {
    int L = g.n;
    queue<int> q; hk_shortest = HK_INF;
    for (int u = 0; u < L; ++u) {
        hk_dist[u] = left_match[u] == -1 ? 0 : HK_INF;
        if (hk_dist[u] == 0) q.push(u);
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (hk_dist[u] >= hk_shortest) continue;
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            int mate = right_match[v];
            if (mate == -1) hk_shortest = hk_dist[u]+1;
            else if (hk_dist[mate] == HK_INF) {
                hk_dist[mate] = hk_dist[u]+1; q.push(mate);
            }
        }
    }
    return hk_shortest != HK_INF;
}
bool hk_dfs(const Graph& g,int u) {
    for (int& id = hk_cur[u]; id; id = g.line[id].next) {
        int v = g.line[id].v, mate = right_match[v];
        bool good = mate == -1 ? hk_dist[u]+1 == hk_shortest
                     : hk_dist[mate] == hk_dist[u]+1 && hk_dfs(g,mate);
        if (good) {
            left_match[u] = v; right_match[v] = u; return true;
        }
    }
    hk_dist[u] = HK_INF; return false;
}
int hopcroft_karp(const Graph& g,int R) {
    int L = g.n;
    fill_n(left_match,L,-1);
    fill_n(right_match,R,-1);
    int answer = 0;
    while (hk_bfs(g)) {
        copy_n(g.head,L,hk_cur);
        for (int u = 0; u < L; ++u)
            if (left_match[u] == -1 && hk_dfs(g,u)) ++answer;
    }
    return answer;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
