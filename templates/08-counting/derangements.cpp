#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../05-number-theory/modular-arithmetic.cpp"
#endif // PDF: endhide
void derangements(int n,ll mod,ll d[]) {   // d has at least n+1 entries
    d[0] = 1%mod;
    if (n >= 1) d[1] = 0;
    for (int i = 2; i <= n; ++i)
        d[i] = mul_mod(i-1,norm((i128)d[i-1]+d[i-2],mod),mod);
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
