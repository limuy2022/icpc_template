#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../05-number-theory/modular-arithmetic.cpp"
#include "../02-common/forward-star.cpp"
#endif // PDF: endhide
struct CostEdge {
    int v, next;
    ll val, cost;
} mcf_line[GA]{};
int mcf_n, mcf_cnt = 1, mcf_head[GN]{};
void mcf_init(int vertices) {
    mcf_n = vertices;
    mcf_cnt = 1;
    fill_n(mcf_head + 1, mcf_n, 0);
}
void mcf_arc(int u, int v, ll w, ll cost) {
    ++mcf_cnt;
    mcf_line[mcf_cnt] = {v, mcf_head[u], w, cost};
    mcf_head[u] = mcf_cnt;
}
int mcf_add(int u, int v, ll cap, ll cost) {
    int id = mcf_cnt + 1;
    mcf_arc(u, v, cap, cost);
    mcf_arc(v, u, 0, -cost);
    return id;
}
pair<ll, i128> min_cost_flow(int s, int t, ll limit = LLONG_MAX) {
    const ll INF = LLONG_MAX / 4;
    ll total = 0;
    i128 cost = 0;
    while (total < limit) {
        static ll dist[GN];
        static int previous[GN];
        static char in_queue[GN];
        fill_n(dist + 1, mcf_n, INF);
        fill_n(previous + 1, mcf_n, 0);
        fill_n(in_queue + 1, mcf_n, 0);
        queue<int> q;
        q.push(s);
        in_queue[s] = true;
        dist[s] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in_queue[u] = false;
            for (int id = mcf_head[u]; id; id = mcf_line[id].next) {
                const auto& edge = mcf_line[id];
                if (edge.val > 0 && dist[edge.v] > dist[u] + edge.cost) {
                    dist[edge.v] = dist[u] + edge.cost;
                    previous[edge.v] = id;
                    if (!in_queue[edge.v]) {
                        in_queue[edge.v] = true;
                        q.push(edge.v);
                    }
                }
            }
        }
        if (previous[t] == 0) break;
        ll add = limit - total;
        for (int v = t; v != s; v = mcf_line[previous[v] ^ 1].v)
            add = min(add, mcf_line[previous[v]].val);
        for (int v = t; v != s; v = mcf_line[previous[v] ^ 1].v) {
            int id = previous[v];
            mcf_line[id].val -= add;
            mcf_line[id ^ 1].val += add;
        }
        total += add;
        cost += (i128)add * dist[t];
    }
    return {total, cost};
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
