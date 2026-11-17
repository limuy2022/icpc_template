#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "modular-arithmetic.cpp"
#endif // PDF: endhide
ll multiplicative_order(ll a, ll m, ll phi_m,
                        const vector<ll>& prime_divisors) {
    if (gcd(a, m) != 1) return -1;
    ll order = phi_m;
    for (ll q : prime_divisors)
        while (order % q == 0 && pow_mod(a, order/q, m) == 1)
            order /= q;
    return order;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
