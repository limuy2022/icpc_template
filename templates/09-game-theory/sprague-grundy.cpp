#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../02-common/forward-star.cpp"
#endif // PDF: endhide
int sg[GN];
void sg_dag(const Graph& g) {
    int n = g.n;
    static int indegree[GN], outdegree[GN];
    fill_n(indegree,n,0); fill_n(outdegree,n,0);
    static int order[GN];
    int used = 0;
    for (int u = 0; u < n; ++u) {
        for (int id = g.head[u]; id; id = g.line[id].next) {
            ++indegree[g.line[id].v];
            ++outdegree[u];
        }
    }
    queue<int> q;
    for (int u = 0; u < n; ++u) if (indegree[u] == 0) q.push(u);
    while (!q.empty()) {
        int u = q.front(); q.pop(); order[used++] = u;
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (--indegree[v] == 0) q.push(v);
        }
    }
    static int seen[GN+1];
    fill_n(seen,n+1,0);
    for (int i = used-1; i >= 0; --i) {
        int u = order[i], degree = outdegree[u], stamp = i+1;
        sg[u] = 0;
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            seen[sg[v]] = stamp;
        }
        while (sg[u] <= degree && seen[sg[u]] == stamp)
            ++sg[u];
    }
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
