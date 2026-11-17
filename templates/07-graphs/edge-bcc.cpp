#include <bits/stdc++.h>
using namespace std;
vector<int> edge_bcc(const Graph& g) {
    vector<int> comp(g.n,-1);
    int total = 0;
    for (int root = 0; root < g.n; ++root) if (comp[root] == -1) {
        vector<int> st{root}; comp[root] = total;
        while (!st.empty()) {
            int u = st.back(); st.pop_back();
            for (int arc = g.head[u]; arc; arc = g.line[arc].next) {
                int v = g.line[arc].v, id = (arc-1)/2;
                if (bridge[id] || comp[v] != -1) continue;
                comp[v] = total; st.push_back(v);
            }
        }
        ++total;
    }
    return comp;
}
