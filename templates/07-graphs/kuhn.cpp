#include <bits/stdc++.h>
using namespace std;
int kuhn_match[GN];
int kuhn(const Graph& g,int R) {
    fill_n(kuhn_match,R,-1);
    static int seen[GN];
    fill_n(seen,R,-1);
    auto augment = [&](auto&& self,int u,int round) -> bool {
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            if (seen[v] == round) continue;
            seen[v] = round;
            if (kuhn_match[v] == -1 ||
                self(self,kuhn_match[v],round)) {
                kuhn_match[v] = u;
                return true;
            }
        }
        return false;
    };
    int matches = 0;
    for (int u = 0; u < g.n; ++u)
        if (augment(augment,u,u)) ++matches;
    return matches;
}
