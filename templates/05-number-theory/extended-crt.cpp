// Uses i128 internally; every combined modulus must fit in long long.
optional<pair<ll,ll>> crt_merge(ll r, ll M, ll a, ll m) {
    r = norm(r, M); a = norm(a, m);
    i128 x, y;
    ll g = (ll)exgcd(M, m, x, y);
    i128 diff = (i128)a - r;
    if (diff % g) return nullopt;
    ll q = m / g;
    ll step = norm((diff / g) * x, q);
    i128 mod = (i128)M * q;
    ll value = norm((i128)r + (i128)M * step, (ll)mod);
    return pair<ll,ll>{value, (ll)mod};
}
