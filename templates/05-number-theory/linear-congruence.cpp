#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "modular-arithmetic.cpp"
#endif // PDF: endhide
optional<pair<ll, ll>> linear_congruence(ll a, ll b, ll m) {
    a = norm(a, m);
    b = norm(b, m);
    ll g = gcd(a, m);
    if (b % g) return nullopt;
    ll period = m / g;
    ll x = mul_mod(b / g, inverse(a / g, period), period);
    return pair<ll, ll>{x, period}; // x + k*period
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
