#include <bits/stdc++.h>
using namespace std;
constexpr int SIEVE_MAX = 1000000;
int lp[SIEVE_MAX+1], phi[SIEVE_MAX+1], mu[SIEVE_MAX+1];
vector<int> primes;
int sieve_limit;
void linear_sieve(int n) {
    sieve_limit = n;
    primes.clear(); fill_n(lp,n+1,0);
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
