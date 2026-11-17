#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
using u128 = __uint128_t;
ll norm(i128 x, ll m) { // m >= 1
    x %= m;
    if (x < 0) x += m;
    return (ll)x;
}
ll mul_mod(ll a, ll b, ll m) {
    return norm((i128)a * b, m);
}
ll pow_mod(ll a, u64 e, ll m) {
    ll ans = 1 % m;
    a = norm(a, m);
    for (; e; e >>= 1, a = mul_mod(a, a, m))
        if (e & 1) ans = mul_mod(ans, a, m);
    return ans;
}
i128 exgcd(i128 a, i128 b, i128& x, i128& y) { // a,b >= 0
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }
    i128 u, v;
    i128 g = exgcd(b, a % b, u, v);
    x = v;
    y = u - (a / b) * v;
    return g;
}
ll inverse(ll a, ll m) { // -1 means not invertible
    if (m == 1) return 0;
    i128 x, y;
    if (exgcd(norm(a, m), m, x, y) != 1) return -1;
    return norm(x, m);
}
i128 floor_div(i128 a, i128 b) { // b > 0
    i128 q = a / b;
    return q - (a % b < 0);
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
