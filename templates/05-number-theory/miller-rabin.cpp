u64 mul64(u64 a, u64 b, u64 m) { return (u128)a * b % m; }
u64 pow64(u64 a, u64 e, u64 m) {
    u64 r = 1 % m;
    for (; e; e >>= 1, a = mul64(a, a, m))
        if (e & 1) r = mul64(r, a, m);
    return r;
}
bool is_prime(u64 n) {
    if (n < 2) return false;
    for (u64 p : {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,
                  23ULL,29ULL,31ULL,37ULL}) {
        if (n % p == 0) return n == p;
    }
    u64 d = n - 1;
    int s = __builtin_ctzll(d);
    d >>= s;
    for (u64 a : {2ULL,325ULL,9375ULL,28178ULL,450775ULL,
                  9780504ULL,1795265022ULL}) {
        if (a % n == 0) continue;
        u64 x = pow64(a % n, d, n);
        if (x == 1 || x == n - 1) continue;
        bool pass = false;
        for (int r = 1; r < s; ++r) {
            x = mul64(x, x, n);
            if (x == n - 1) { pass = true; break; }
        }
        if (!pass) return false;
    }
    return true;
}
