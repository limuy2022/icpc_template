#include <bits/stdc++.h>
using namespace std;
vector<ll> derangements(int n,ll mod) {        // n >= 0, mod >= 1
    vector<ll> d(n+1);
    d[0] = 1%mod;
    if (n >= 1) d[1] = 0;
    for (int i = 2; i <= n; ++i)
        d[i] = mul_mod(i-1,norm((i128)d[i-1]+d[i-2],mod),mod);
    return d;
}
