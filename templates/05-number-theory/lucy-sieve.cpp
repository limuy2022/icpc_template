#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "modular-arithmetic.cpp"
#endif // PDF: endhide
ll prime_count(ll n) {                  // n >= 0; O(sqrt(n)) states
    if (n < 2) return 0;
    vector<ll> values, g;
    unordered_map<ll,int> id;
    for (ll l = 1, r; l <= n; ) {
        ll v = n/l; r = n/v;
        id[v] = (int)values.size();
        values.push_back(v); g.push_back(v-1);
        if (r == n) break;
        l = r+1;
    }
    for (ll p = 2; p <= n/p; ++p) {
        ll before = g[id[p-1]];
        if (g[id[p]] == before) continue;
        for (int i = 0; i < (int)values.size() &&
                        p <= values[i]/p; ++i)
            g[i] -= g[id[values[i]/p]] - before;
    }
    return g[0];
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
