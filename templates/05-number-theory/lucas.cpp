#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "modular-arithmetic.cpp"
#endif // PDF: endhide
// fac/ifac have indices 0..p-1; p is prime and small enough to store.
ll lucas(u64 n, u64 k, ll p, const ll fac[], const ll ifac[]) {
    ll ans = 1;
    while (n || k) {
        ll a = n % p, b = k % p;
        if (b > a) return 0;
        ans = mul_mod(ans, mul_mod(fac[a], mul_mod(ifac[b], ifac[a - b], p), p), p);
        n /= p;
        k /= p;
    }
    return ans;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
