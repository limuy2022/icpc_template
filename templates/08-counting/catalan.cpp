vector<ll> catalan_numbers(int n,ll mod) {     // works for composite mod
    vector<ll> c(n+1);
    c[0] = 1%mod;
    for (int k = 1; k <= n; ++k)
        for (int i = 0; i < k; ++i)
            c[k] = norm((i128)c[k]+mul_mod(c[i],c[k-1-i],mod),mod);
    return c;
}
