#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../05-number-theory/modular-arithmetic.cpp"
#endif // PDF: endhide
vector<ll> connected_labeled_graphs(int N, ll mod) {
    vector<vector<ll>> choose(N + 1, vector<ll>(N + 1));
    for (int n = 0; n <= N; ++n) {
        choose[n][0] = choose[n][n] = 1 % mod;
        for (int k = 1; k < n; ++k)
            choose[n][k] = norm((i128)choose[n - 1][k - 1] + choose[n - 1][k], mod);
    }
    vector<ll> all(N + 1), conn(N + 1);
    for (int n = 0; n <= N; ++n) all[n] = pow_mod(2, (u64)n * (n ? n - 1 : 0) / 2, mod);
    for (int n = 1; n <= N; ++n) {
        conn[n] = all[n];
        for (int k = 1; k < n; ++k) {
            ll term = mul_mod(choose[n - 1][k - 1], conn[k], mod);
            term = mul_mod(term, all[n - k], mod);
            conn[n] = norm((i128)conn[n] - term, mod);
        }
    }
    return conn;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
