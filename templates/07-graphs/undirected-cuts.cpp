#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../02-common/forward-star.cpp"
#endif // PDF: endhide
int cut_timer, cut_top;
int tin[GN], low[GN], stamp[GN];
char cut[GN], bridge[GA / 2];
int edge_stack[GA / 2];
vector<vector<int>> blocks;
int cut_add(Graph& g, int u, int v) {
    if (u == v) return -1;
    return (g.add_undirected(u, v) - 1) / 2;
}
void cut_dfs(const Graph& g, int u, int parent_edge) {
    tin[u] = low[u] = ++cut_timer;
    int children = 0;
    for (int arc = g.head[u]; arc; arc = g.line[arc].next) {
        int v = g.line[arc].v, id = (arc - 1) / 2;
        if (id == parent_edge) continue;
        if (!tin[v]) {
            ++children;
            edge_stack[cut_top++] = id;
            cut_dfs(g, v, id);
            low[u] = min(low[u], low[v]);
            if (low[v] > tin[u]) bridge[id] = true;
            if (low[v] >= tin[u]) {
                if (parent_edge != -1) cut[u] = true;
                int block_id = (int)blocks.size();
                vector<int> vertices;
                while (true) {
                    int e = edge_stack[--cut_top];
                    int a = g.line[2 * e + 2].v, b = g.line[2 * e + 1].v;
                    for (int x : {a, b})
                        if (stamp[x] != block_id) {
                            stamp[x] = block_id;
                            vertices.push_back(x);
                        }
                    if (e == id) break;
                }
                blocks.push_back(vertices);
            }
        } else if (tin[v] < tin[u]) {
            low[u] = min(low[u], tin[v]);
            edge_stack[cut_top++] = id;
        }
    }
    if (parent_edge == -1) cut[u] = children > 1;
}
void build_cuts(const Graph& g) {
    int n = g.n;
    cut_timer = cut_top = 0;
    blocks.clear();
    fill_n(tin, n, 0);
    fill_n(low, n, 0);
    fill_n(stamp, n, -1);
    fill_n(cut, n, 0);
    fill_n(bridge, g.cnt / 2, 0);
    for (int u = 0; u < n; ++u)
        if (!tin[u]) {
            if (g.head[u] == 0) blocks.push_back(vector<int>{u});
            cut_dfs(g, u, -1);
        }
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
