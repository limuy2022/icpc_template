#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "modular-arithmetic.cpp"
#include "linear-sieve.cpp"
#endif // PDF: endhide
ll small_phi[SIEVE_MAX + 1];
ll small_mu[SIEVE_MAX + 1];
map<ll, pair<i128, i128>> memo;
void dujiao_init() {
    memo.clear();
    for (int i = 1; i <= sieve_limit; ++i) {
        small_phi[i] = small_phi[i - 1] + phi[i];
        small_mu[i] = small_mu[i - 1] + mu[i];
    }
}
pair<i128, i128> dujiao_sum(ll n) {
    if (n <= sieve_limit) return {small_phi[n], small_mu[n]};
    auto it = memo.find(n);
    if (it != memo.end()) return it->second;
    i128 a = (i128)n * ((i128)n + 1) / 2, b = 1;
    for (ll l = 2, r; l <= n;) {
        ll q = n / l;
        r = n / q;
        auto [x, y] = dujiao_sum(q);
        a -= (i128)(r - l + 1) * x;
        b -= (i128)(r - l + 1) * y;
        if (r == n) break;
        l = r + 1;
    }
    return memo[n] = {a, b};
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
