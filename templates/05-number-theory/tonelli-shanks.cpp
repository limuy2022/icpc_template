ll sqrt_mod(ll a, ll p) {
    a = norm(a, p);
    if (p == 2 || a == 0) return a;
    if (pow_mod(a, (p-1)/2, p) != 1) return -1;
    ll q = p - 1;
    int s = 0;
    while (q % 2 == 0) { q /= 2; ++s; }
    ll z = 2;
    while (pow_mod(z, (p-1)/2, p) != p-1) ++z;
    ll c = pow_mod(z, q, p), t = pow_mod(a, q, p);
    ll x = pow_mod(a, (q+1)/2, p);
    int m = s;
    while (t != 1) {
        int i = 0;
        ll v = t;
        while (v != 1) { v = mul_mod(v, v, p); ++i; }
        ll b = c;
        for (int j = 0; j < m-i-1; ++j) b = mul_mod(b, b, p);
        x = mul_mod(x, b, p);
        c = mul_mod(b, b, p);
        t = mul_mod(t, c, p); m = i;
    }
    return min(x, p-x);
}
