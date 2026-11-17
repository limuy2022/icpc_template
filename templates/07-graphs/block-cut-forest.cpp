#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../02-common/forward-star.cpp"
#include "undirected-cuts.cpp"
#endif // PDF: endhide
void block_cut_forest(const Graph& g,Graph& forest) {
    int count = (int)blocks.size();
    forest.init(g.n + count);
    for (int i = 0; i < count; ++i)
        for (int u : blocks[i])
            forest.add_undirected(u,g.n+i);
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
