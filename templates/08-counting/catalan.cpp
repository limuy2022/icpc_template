#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../05-number-theory/modular-arithmetic.cpp"
#endif // PDF: endhide
void catalan_numbers(int n,ll mod,ll c[]) { // c has at least n+1 entries
    c[0] = 1%mod;
    for (int k = 1; k <= n; ++k) {
        c[k] = 0;
        for (int i = 0; i < k; ++i)
            c[k] = norm((i128)c[k]+mul_mod(c[i],c[k-1-i],mod),mod);
    }
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
