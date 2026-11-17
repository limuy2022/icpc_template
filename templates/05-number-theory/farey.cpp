#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "modular-arithmetic.cpp"
#endif // PDF: endhide
vector<pair<ll, ll>> farey(ll N) { // N >= 1; output can be O(N^2)
    vector<pair<ll, ll>> out{{0, 1}};
    ll a = 0, b = 1, c = 1, d = N;
    while (true) {
        out.push_back({c, d});
        if (c == 1 && d == 1) break;
        ll k = (ll)(((i128)N + b) / d);
        ll e = (ll)((i128)k * c - a), f = (ll)((i128)k * d - b);
        a = c;
        b = d;
        c = e;
        d = f;
    }
    return out;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
