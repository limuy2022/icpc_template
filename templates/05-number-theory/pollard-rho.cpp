#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "modular-arithmetic.cpp"
#include "miller-rabin.cpp"
#endif // PDF: endhide
mt19937_64 nt_rng(chrono::steady_clock::now().time_since_epoch().count());
u64 rho(u64 n) { // n is composite
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    while (true) {
        u64 c = nt_rng() % (n - 1) + 1;
        u64 x = nt_rng() % (n - 2) + 2, y = x, d = 1;
        auto f = [&](u64 v) { return (u64)(((u128)mul64(v, v, n) + c) % n); };
        while (d == 1) {
            x = f(x);
            y = f(f(y));
            u64 diff = x > y ? x - y : y - x;
            d = gcd(diff, n);
        }
        if (d != n) return d;
    }
}
void factor(u64 n, vector<u64>& out) {
    if (n == 1) return;
    if (is_prime(n)) {
        out.push_back(n);
        return;
    }
    u64 d = rho(n);
    factor(d, out);
    factor(n / d, out);
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
