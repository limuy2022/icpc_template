ll bsgs_coprime(ll a, ll b, ll m) {
    if (m == 1) return 0;
    ll t = (ll)sqrtl((long double)m) + 1;
    unordered_map<ll,ll> baby;
    ll cur = 1 % m;
    for (ll j = 0; j < t; ++j) {
        baby.emplace(cur, j);             // preserve smallest j
        cur = mul_mod(cur, a, m);
    }
    ll step = inverse(pow_mod(a, t, m), m);
    cur = norm(b, m);
    for (ll i = 0; i <= t; ++i) {
        auto it = baby.find(cur);
        if (it != baby.end()) return (ll)((i128)i*t + it->second);
        cur = mul_mod(cur, step, m);
    }
    return -1;
}
ll discrete_log(ll a, ll b, ll m) {
    a = norm(a, m); b = norm(b, m);
    ll removed = 0, coefficient = 1 % m;
    while (true) {
        if (coefficient == b) return removed;
        ll g = gcd(a, m);
        if (g == 1) break;
        if (b % g) return -1;
        m /= g; b /= g;
        coefficient = mul_mod(coefficient, a/g, m);
        ++removed;
    }
    ll target = mul_mod(b, inverse(coefficient, m), m);
    ll tail = bsgs_coprime(norm(a,m), target, m);
    return tail == -1 ? -1 : tail + removed;
}
