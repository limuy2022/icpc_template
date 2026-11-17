#include <bits/stdc++.h>
using namespace std;
void block_cut_forest(const UndirectedCuts& graph,Graph& forest) {
    int blocks = (int)graph.blocks.size();
    forest.init(graph.n + blocks);
    for (int i = 0; i < blocks; ++i)
        for (int u : graph.blocks[i])
            forest.add_undirected(u,graph.n+i);
}
