struct SCC {
    int n, components = 0;
    Graph g, rg;
    int comp[GN];
    SCC(int n) : n(n), g(n), rg(n) {}
    void add_edge(int u,int v) { g.add(u,v); rg.add(v,u); }
    void build() {
        static char seen[GN];
        fill_n(seen,n,0);
        vector<int> order;
        for (int root = 0; root < n; ++root) if (!seen[root]) {
            vector<pair<int,int>> st{{root,g.head[root]}}; seen[root] = true;
            while (!st.empty()) {
                int u = st.back().first;
                int& i = st.back().second;
                if (i) {
                    int v = g.line[i].v; i = g.line[i].next;
                    if (!seen[v]) { seen[v] = true; st.push_back({v,g.head[v]}); }
                } else { order.push_back(u); st.pop_back(); }
            }
        }
        fill_n(comp,n,-1); components = 0;
        for (auto it = order.rbegin(); it != order.rend(); ++it) {
            int root = *it;
            if (comp[root] != -1) continue;
            vector<int> st{root}; comp[root] = components;
            while (!st.empty()) {
                int u = st.back(); st.pop_back();
                for (int id = rg.head[u]; id; id = rg.line[id].next) {
                    int v = rg.line[id].v;
                    if (comp[v] != -1) continue;
                    comp[v] = components; st.push_back(v);
                }
            }
            ++components;
        }
    }
};
