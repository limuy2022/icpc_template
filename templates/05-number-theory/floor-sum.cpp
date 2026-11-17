#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "modular-arithmetic.cpp"
#endif // PDF: endhide
i128 floor_sum(ll n, ll m, ll a0, ll b0) {
    i128 a = a0, b = b0, mod = m, count = n;
    i128 qa = floor_div(a, mod), qb = floor_div(b, mod);
    i128 ans = qa*count*(count-1)/2 + qb*count;
    a -= qa*mod; b -= qb*mod;
    while (true) {
        ans += count*(count-1)/2*(a/mod);
        ans += count*(b/mod);
        a %= mod; b %= mod;
        i128 y = a*count+b;
        if (y < mod) break;
        count = y/mod; b = y%mod;
        swap(a, mod);
    }
    return ans;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
