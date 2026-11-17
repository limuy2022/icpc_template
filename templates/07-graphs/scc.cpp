#include <bits/stdc++.h>
using namespace std;
int scc_count;
Graph scc_g(0), scc_rg(0);
int scc_id[GN];
void scc_init(int n) { scc_g.init(n); scc_rg.init(n); }
void scc_add(int u,int v) { scc_g.add(u,v); scc_rg.add(v,u); }
void scc_build() {
    int n = scc_g.n;
    static char seen[GN];
    fill_n(seen,n,0);
    vector<int> order;
    for (int root = 0; root < n; ++root) if (!seen[root]) {
        vector<pair<int,int>> st{{root,scc_g.head[root]}}; seen[root] = true;
        while (!st.empty()) {
            int u = st.back().first;
            int& i = st.back().second;
            if (i) {
                int v = scc_g.line[i].v; i = scc_g.line[i].next;
                if (!seen[v]) { seen[v] = true; st.push_back({v,scc_g.head[v]}); }
            } else { order.push_back(u); st.pop_back(); }
        }
    }
    fill_n(scc_id,n,-1); scc_count = 0;
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        int root = *it;
        if (scc_id[root] != -1) continue;
        vector<int> st{root}; scc_id[root] = scc_count;
        while (!st.empty()) {
            int u = st.back(); st.pop_back();
            for (int id = scc_rg.head[u]; id; id = scc_rg.line[id].next) {
                int v = scc_rg.line[id].v;
                if (scc_id[v] != -1) continue;
                scc_id[v] = scc_count; st.push_back(v);
            }
        }
        ++scc_count;
    }
}
