ll multiplicative_order(ll a, ll m, ll phi_m,
                        const vector<ll>& prime_divisors) {
    if (gcd(a, m) != 1) return -1;
    ll order = phi_m;
    for (ll q : prime_divisors)
        while (order % q == 0 && pow_mod(a, order/q, m) == 1)
            order /= q;
    return order;
}
