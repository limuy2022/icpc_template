#include <bits/stdc++.h>
using namespace std;
void block_cut_forest(const Graph& g,Graph& forest) {
    int count = (int)blocks.size();
    forest.init(g.n + count);
    for (int i = 0; i < count; ++i)
        for (int u : blocks[i])
            forest.add_undirected(u,g.n+i);
}
