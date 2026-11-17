vector<int> sg_dag(const Graph& g) {
    int n = g.n;
    static int indegree[GN], outdegree[GN];
    fill_n(indegree,n,0); fill_n(outdegree,n,0);
    vector<int> order;
    for (int u = 0; u < n; ++u) {
        for (int id = g.head[u]; id; id = g.line[id].next) {
            ++indegree[g.line[id].v];
            ++outdegree[u];
        }
    }
    queue<int> q;
    for (int u = 0; u < n; ++u) if (indegree[u] == 0) q.push(u);
    while (!q.empty()) {
        int u = q.front(); q.pop(); order.push_back(u);
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (--indegree[v] == 0) q.push(v);
        }
    }
    vector<int> sg(n);
    static int seen[GN+1];
    fill_n(seen,n+1,0);
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        int u = *it, degree = outdegree[u];
        int stamp = (int)(it-order.rbegin())+1;
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            seen[sg[v]] = stamp;
        }
        while (sg[u] <= degree && seen[sg[u]] == stamp)
            ++sg[u];
    }
    return sg;
}
