#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../02-common/forward-star.cpp"
#endif // PDF: endhide
int color[GN];
bool bipartite_color(const Graph& g) {
    int n = g.n;
    fill_n(color, n, -1);
    for (int root = 0; root < n; ++root)
        if (color[root] == -1) {
            queue<int> q;
            q.push(root);
            color[root] = 0;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int id = g.head[u]; id; id = g.line[id].next) {
                    int v = g.line[id].v;
                    if (color[v] == -1) {
                        color[v] = color[u] ^ 1;
                        q.push(v);
                    } else if (color[v] == color[u])
                        return false;
                }
            }
        }
    return true;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
