#include <bits/stdc++.h>
using namespace std;
constexpr int SIEVE_MAX = 1000000;
int lp[SIEVE_MAX + 1], phi[SIEVE_MAX + 1], mu[SIEVE_MAX + 1];
int primes[SIEVE_MAX], prime_cnt;
int sieve_limit;
void linear_sieve(int n) {
    sieve_limit = n;
    prime_cnt = 0;
    fill_n(lp, n + 1, 0);
    if (n >= 1) phi[1] = mu[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (!lp[i]) {
            lp[i] = i;
            primes[prime_cnt++] = i;
            phi[i] = i - 1;
            mu[i] = -1;
        }
        for (int j = 0; j < prime_cnt; ++j) {
            int p = primes[j];
            if (p > n / i) break;
            int v = i * p;
            lp[v] = p;
            if (i % p == 0) {
                phi[v] = phi[i] * p;
                mu[v] = 0;
                break;
            }
            phi[v] = phi[i] * (p - 1);
            mu[v] = -mu[i];
        }
    }
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
