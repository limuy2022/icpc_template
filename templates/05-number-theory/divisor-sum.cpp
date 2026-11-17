#include <bits/stdc++.h>
using namespace std;
i128 divisor_sum(ll n) {               // sum floor(n/i), n >= 0
    i128 ans = 0;
    for (ll l = 1, r; l <= n; ) {
        ll q = n / l;
        r = n / q;
        ans += (i128)(r-l+1) * q;
        if (r == n) break;
        l = r + 1;
    }
    return ans;
}
