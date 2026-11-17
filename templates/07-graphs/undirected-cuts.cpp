struct UndirectedCuts {
    int n, timer = 0, top = 0;
    Graph g;
    int tin[GN], low[GN], stamp[GN];
    char cut[GN], bridge[GA/2];
    int edge_stack[GA/2];
    vector<vector<int>> blocks;
    UndirectedCuts(int n)
        : n(n), g(n) {}
    int add_edge(int u,int v) {
        if (u == v) return -1;
        return (g.add_undirected(u,v)-1)/2;
    }
    void dfs(int u,int parent_edge) {
        tin[u] = low[u] = ++timer;
        int children = 0;
        for (int arc = g.head[u]; arc; arc = g.line[arc].next) {
            int v = g.line[arc].v, id = (arc-1)/2;
            if (id == parent_edge) continue;
            if (!tin[v]) {
                ++children; edge_stack[top++] = id; dfs(v,id);
                low[u] = min(low[u],low[v]);
                if (low[v] > tin[u]) bridge[id] = true;
                if (low[v] >= tin[u]) {
                    if (parent_edge != -1) cut[u] = true;
                    int block_id = (int)blocks.size();
                    vector<int> vertices;
                    while (true) {
                        int e = edge_stack[--top];
                        int a = g.line[2*e+2].v, b = g.line[2*e+1].v;
                        for (int x : {a,b}) if (stamp[x] != block_id) {
                            stamp[x] = block_id; vertices.push_back(x);
                        }
                        if (e == id) break;
                    }
                    blocks.push_back(vertices);
                }
            } else if (tin[v] < tin[u]) {
                low[u] = min(low[u],tin[v]); edge_stack[top++] = id;
            }
        }
        if (parent_edge == -1) cut[u] = children > 1;
    }
    void build() {
        timer = top = 0; blocks.clear();
        fill_n(tin,n,0); fill_n(low,n,0);
        fill_n(stamp,n,-1); fill_n(cut,n,0);
        fill_n(bridge,g.cnt/2,0);
        for (int u = 0; u < n; ++u) if (!tin[u]) {
            if (g.head[u] == 0) blocks.push_back(vector<int>{u});
            dfs(u,-1);
        }
    }
};
