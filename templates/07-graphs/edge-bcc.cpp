#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../02-common/forward-star.cpp"
#include "undirected-cuts.cpp"
#endif // PDF: endhide
int ebcc_id[GN];
int edge_bcc(const Graph& g) {
    fill_n(ebcc_id,g.n,-1);
    static int stack[GN];
    int total = 0;
    for (int root = 0; root < g.n; ++root) if (ebcc_id[root] == -1) {
        int size = 1; stack[0] = root; ebcc_id[root] = total;
        while (size) {
            int u = stack[--size];
            for (int arc = g.head[u]; arc; arc = g.line[arc].next) {
                int v = g.line[arc].v, id = (arc-1)/2;
                if (bridge[id] || ebcc_id[v] != -1) continue;
                ebcc_id[v] = total; stack[size++] = v;
            }
        }
        ++total;
    }
    return total;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
