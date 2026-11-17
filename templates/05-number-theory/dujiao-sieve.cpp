#include <bits/stdc++.h>
using namespace std;
struct Dujiao {
    ll small_phi[NumberSieve::MAX_N+1]{};
    ll small_mu[NumberSieve::MAX_N+1]{};
    int limit;
    map<ll, pair<i128,i128>> memo;
    Dujiao(const NumberSieve& s) : limit(s.limit) {
        for (int i = 1; i <= limit; ++i) {
            small_phi[i] = small_phi[i-1] + s.phi[i];
            small_mu[i] = small_mu[i-1] + s.mu[i];
        }
    }
    pair<i128,i128> sum(ll n) {
        if (n <= limit)
            return {small_phi[n], small_mu[n]};
        auto it = memo.find(n);
        if (it != memo.end()) return it->second;
        i128 a = (i128)n*((i128)n+1)/2, b = 1;
        for (ll l = 2, r; l <= n; ) {
            ll q = n/l; r = n/q;
            auto [x,y] = sum(q);
            a -= (i128)(r-l+1)*x; b -= (i128)(r-l+1)*y;
            if (r == n) break;
            l = r+1;
        }
        return memo[n] = {a,b};
    }
};
