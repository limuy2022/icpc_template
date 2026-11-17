pair<ll,vector<int>> hungarian(const ForwardStar<ll>& g,int m) {
    int n = g.n;
    constexpr int KM_CAP = 505;
    static ll cost[KM_CAP][KM_CAP];
    for (int u = 0; u < n; ++u) {
        for (int id = g.head[u]; id; id = g.line[id].next) {
            const auto& edge = g.line[id];
            cost[u][edge.v] = edge.val;
        }
    }
    const ll INF = LLONG_MAX/4;
    static ll u[GN+1], v[GN+1], best[GN+1];
    static int p[GN+1], way[GN+1];
    static char used[GN+1];
    fill_n(u,n+1,0); fill_n(v,m+1,0); fill_n(p,m+1,0);
    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        int j0 = 0;
        fill_n(best,m+1,INF); fill_n(used,m+1,0);
        do {
            used[j0] = true;
            int i0 = p[j0], j1 = 0;
            ll delta = INF;
            for (int j = 1; j <= m; ++j) if (!used[j]) {
                ll reduced = cost[i0-1][j-1]-u[i0]-v[j];
                if (reduced < best[j]) { best[j] = reduced; way[j] = j0; }
                if (best[j] < delta) { delta = best[j]; j1 = j; }
            }
            for (int j = 0; j <= m; ++j) {
                if (used[j]) { u[p[j]] += delta; v[j] -= delta; }
                else best[j] -= delta;
            }
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int previous = way[j0];
            p[j0] = p[previous]; j0 = previous;
        } while (j0 != 0);
    }
    vector<int> assignment(n,-1);
    for (int j = 1; j <= m; ++j)
        if (p[j]) assignment[p[j]-1] = j-1;
    return {-v[0],assignment};
}
