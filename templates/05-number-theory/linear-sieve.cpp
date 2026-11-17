struct NumberSieve {
    static constexpr int MAX_N = 1000000;
    int lp[MAX_N+1]{}, phi[MAX_N+1]{}, mu[MAX_N+1]{};
    vector<int> primes;
    int limit;
    NumberSieve(int n) : limit(n) {
        if (n >= 1) phi[1] = mu[1] = 1;
        for (int i = 2; i <= n; ++i) {
            if (!lp[i]) {
                lp[i] = i; primes.push_back(i);
                phi[i] = i - 1; mu[i] = -1;
            }
            for (int p : primes) {
                if (p > n / i) break;
                int v = i * p;
                lp[v] = p;
                if (i % p == 0) {
                    phi[v] = phi[i] * p; mu[v] = 0;
                    break;
                }
                phi[v] = phi[i] * (p - 1); mu[v] = -mu[i];
            }
        }
    }
};
