#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "modular-arithmetic.cpp"
#endif // PDF: endhide
vector<ll> continued_fraction(ll a, ll b) { // a>=0, b>0
    vector<ll> terms;
    while (b) {
        terms.push_back(a / b);
        ll r = a % b;
        a = b;
        b = r;
    }
    return terms;
}
pair<i128, i128> convergent(const vector<ll>& terms) {
    i128 p0 = 0, p1 = 1, q0 = 1, q1 = 0;
    for (ll a : terms) {
        i128 p = a * p1 + p0, q = a * q1 + q0;
        p0 = p1;
        p1 = p;
        q0 = q1;
        q1 = q;
    }
    return {p1, q1};
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
