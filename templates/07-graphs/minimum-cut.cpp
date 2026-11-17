// Call dinic() first. Read the source side of the final residual graph.
bool seen[210];
void residual_reachable() {
    memset(seen,0,sizeof(seen));
    std::queue<int> q;
    q.push(s); seen[s] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int i = head[u]; i; i = line[i].next) {
            int v = line[i].v;
            if (line[i].val > 0 && !seen[v]) {
                seen[v] = true; q.push(v);
            }
        }
    }
}
