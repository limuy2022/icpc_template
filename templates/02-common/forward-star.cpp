#include <bits/stdc++.h>
using namespace std;
constexpr int GN = 2005, GA = 10010; // vertices, arc slots
template <class W = int> struct ForwardStar {
    struct Edge {
        int v, next;
        W val;
    } line[GA]{};
    int n, cnt = 0, head[GN]{};
    ForwardStar(int n) : n(n) {}
    void init(int vertices) {
        n = vertices;
        cnt = 0;
        fill_n(head + 1, n, 0);
    }
    int add(int u, int v, W w = W{}) {
        ++cnt;
        line[cnt] = {v, head[u], w};
        head[u] = cnt;
        return cnt;
    }
    int add_undirected(int u, int v, W w = W{}) {
        int id = add(u, v, w);
        add(v, u, w);
        return id;
    }
};
using Graph = ForwardStar<>;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
