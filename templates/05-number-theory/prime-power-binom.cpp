#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "modular-arithmetic.cpp"
#endif // PDF: endhide
struct PrimePowerBinom {
    ll p, mod;
    vector<ll> pref;
    PrimePowerBinom(ll prime, ll power) : p(prime), mod(power), pref(power + 1, 1) {
        for (ll i = 1; i <= mod; ++i)
            pref[i] = mul_mod(pref[i - 1], i % p ? i : 1, mod);
    }
    u64 valuation(u64 n) const {
        u64 e = 0;
        while (n) {
            n /= p;
            e += n;
        }
        return e;
    }
    ll unit_factorial(u64 n) const {
        ll ans = 1;
        while (n) {
            ans = mul_mod(ans, pow_mod(pref[mod], n / mod, mod), mod);
            ans = mul_mod(ans, pref[n % mod], mod);
            n /= p;
        }
        return ans;
    }
    ll choose(u64 n, u64 k) const {
        if (k > n) return 0;
        u64 e = valuation(n) - valuation(k) - valuation(n - k);
        ll pe = pow_mod(p, e, mod);
        if (pe == 0) return 0;
        ll num = unit_factorial(n);
        ll den = mul_mod(unit_factorial(k), unit_factorial(n - k), mod);
        return mul_mod(mul_mod(num, inverse(den, mod), mod), pe, mod);
    }
};
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
