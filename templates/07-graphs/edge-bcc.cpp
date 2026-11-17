#include <bits/stdc++.h>
using namespace std;
vector<int> edge_bcc(const UndirectedCuts& graph) {
    vector<int> comp(graph.n,-1);
    int total = 0;
    for (int root = 0; root < graph.n; ++root) if (comp[root] == -1) {
        vector<int> st{root}; comp[root] = total;
        while (!st.empty()) {
            int u = st.back(); st.pop_back();
            for (int arc = graph.g.head[u]; arc; arc = graph.g.line[arc].next) {
                int v = graph.g.line[arc].v, id = (arc-1)/2;
                if (graph.bridge[id] || comp[v] != -1) continue;
                comp[v] = total; st.push_back(v);
            }
        }
        ++total;
    }
    return comp;
}
