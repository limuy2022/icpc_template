#include <bits/stdc++.h>
using namespace std;
struct MinCostFlow {
    struct Edge { int v, next; ll val, cost; } line[GA]{};
    int n, cnt = 1, head[GN]{};
    MinCostFlow(int n) : n(n) {}
    void add(int u,int v,ll w,ll cost) {
        ++cnt; line[cnt] = {v,head[u],w,cost}; head[u] = cnt;
    }
    int add_edge(int u,int v,ll cap,ll cost) {
        int id = cnt+1;
        add(u,v,cap,cost); add(v,u,0,-cost);
        return id;
    }
    pair<ll,i128> flow(int s,int t,ll limit=LLONG_MAX) {
        const ll INF = LLONG_MAX/4;
        ll total = 0;
        i128 cost = 0;
        while (total < limit) {
            static ll dist[GN];
            static int previous[GN];
            static char in_queue[GN];
            fill_n(dist,n,INF); fill_n(previous,n,0);
            fill_n(in_queue,n,0);
            queue<int> q; q.push(s); in_queue[s] = true; dist[s] = 0;
            while (!q.empty()) {
                int u = q.front(); q.pop(); in_queue[u] = false;
                for (int id = head[u]; id; id = line[id].next) {
                    const auto& edge = line[id];
                    if (edge.val > 0 && dist[edge.v] > dist[u]+edge.cost) {
                        dist[edge.v] = dist[u]+edge.cost;
                        previous[edge.v] = id;
                        if (!in_queue[edge.v]) {
                            in_queue[edge.v] = true; q.push(edge.v);
                        }
                    }
                }
            }
            if (previous[t] == 0) break;
            ll add = limit-total;
            for (int v = t; v != s; v = line[previous[v]^1].v)
                add = min(add,line[previous[v]].val);
            for (int v = t; v != s; v = line[previous[v]^1].v) {
                int id = previous[v]; line[id].val -= add; line[id^1].val += add;
            }
            total += add; cost += (i128)add*dist[t];
        }
        return {total,cost};
    }
};
