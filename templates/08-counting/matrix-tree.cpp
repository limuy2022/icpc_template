#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../05-number-theory/modular-arithmetic.cpp"
#include "../02-common/forward-star.cpp"
#endif // PDF: endhide
ll determinant_mod(vector<vector<ll>> a,ll p) { // square; p prime
    int n = (int)a.size();
    for (auto& row : a) for (ll& x : row) x = norm(x,p);
    ll answer = 1%p;
    for (int col = 0; col < n; ++col) {
        int pivot = col;
        while (pivot < n && a[pivot][col] == 0) ++pivot;
        if (pivot == n) return 0;
        if (pivot != col) { swap(a[pivot],a[col]); answer = norm(-answer,p); }
        ll value = a[col][col], inv = pow_mod(value,p-2,p);
        answer = mul_mod(answer,value,p);
        for (int i = col+1; i < n; ++i) {
            ll factor = mul_mod(a[i][col],inv,p);
            for (int j = col+1; j < n; ++j)
                a[i][j] = norm((i128)a[i][j]-mul_mod(factor,a[col][j],p),p);
            a[i][col] = 0;
        }
    }
    return answer;
}
ll spanning_tree_count(const ForwardStar<ll>& g,ll p) {
    int n = g.n;
    if (n == 0) return 0;
    int k = n-1;
    vector<vector<ll>> a(k,vector<ll>(k));
    for (int u = 0; u < k; ++u) {
        for (int id = g.head[u]; id; id = g.line[id].next) {
            int v = g.line[id].v;
            ll w = g.line[id].val;
            if (u == v) continue;
            a[u][u] = norm((i128)a[u][u]+w,p);
            if (v < k) a[u][v] = norm((i128)a[u][v]-w,p);
        }
    }
    return determinant_mod(a,p);
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
