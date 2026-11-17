// fac/ifac have indices 0..p-1; p is prime and small enough to store.
ll lucas(u64 n, u64 k, ll p, const vector<ll>& fac,
         const vector<ll>& ifac) {
    ll ans = 1;
    while (n || k) {
        ll a = n % p, b = k % p;
        if (b > a) return 0;
        ans = mul_mod(ans, mul_mod(fac[a],
              mul_mod(ifac[b], ifac[a-b], p), p), p);
        n /= p; k /= p;
    }
    return ans;
}
