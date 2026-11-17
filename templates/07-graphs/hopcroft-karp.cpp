struct HopcroftKarp {
    int L, R, shortest;
    const int INF = INT_MAX/4;
    Graph g;
    int left_match[GN], right_match[GN], dist[GN], current[GN];
    HopcroftKarp(int L,int R) : L(L), R(R), g(L) {}
    void add_edge(int u,int v) { g.add(u,v); }
    bool bfs() {
        queue<int> q; shortest = INF;
        for (int u = 0; u < L; ++u) {
            dist[u] = left_match[u] == -1 ? 0 : INF;
            if (dist[u] == 0) q.push(u);
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (dist[u] >= shortest) continue;
            for (int id = g.head[u]; id; id = g.line[id].next) {
                int v = g.line[id].v;
                int mate = right_match[v];
                if (mate == -1) shortest = dist[u]+1;
                else if (dist[mate] == INF) {
                    dist[mate] = dist[u]+1; q.push(mate);
                }
            }
        }
        return shortest != INF;
    }
    bool dfs(int u) {
        for (int& id = current[u]; id; id = g.line[id].next) {
            int v = g.line[id].v, mate = right_match[v];
            bool good = mate == -1 ? dist[u]+1 == shortest
                         : dist[mate] == dist[u]+1 && dfs(mate);
            if (good) {
                left_match[u] = v; right_match[v] = u; return true;
            }
        }
        dist[u] = INF; return false;
    }
    int solve() {
        fill_n(left_match,L,-1);
        fill_n(right_match,R,-1);
        int answer = 0;
        while (bfs()) {
            copy_n(g.head,L,current);
            for (int u = 0; u < L; ++u)
                if (left_match[u] == -1 && dfs(u)) ++answer;
        }
        return answer;
    }
};
