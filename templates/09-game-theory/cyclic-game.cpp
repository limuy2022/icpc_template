vector<int> game_outcomes(const Graph& g) {
    int n = g.n;
    static Graph reverse(0);
    reverse.init(n);
    static int remain[GN];
    fill_n(remain,n,0);
    vector<int> result(n);
    queue<int> q;
    for (int u = 0; u < n; ++u) {
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            reverse.add(v,u);
            ++remain[u];
        }
        if (remain[u] == 0) { result[u] = -1; q.push(u); }
    }
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int id = reverse.head[v]; id; id = reverse.line[id].next) {
            int u = reverse.line[id].v;
            if (result[u] != 0) continue;
            if (result[v] == -1) { result[u] = 1; q.push(u); }
            else if (--remain[u] == 0) { result[u] = -1; q.push(u); }
        }
    }
    return result;
}
