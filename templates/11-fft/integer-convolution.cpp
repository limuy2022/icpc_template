#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../05-number-theory/modular-arithmetic.cpp"
#include "fft.cpp"
#endif // PDF: endhide
vector<ll> convolution_fft(const vector<ll>& a,const vector<ll>& b) {
    if (a.empty() || b.empty()) return {};
    size_t need = a.size()+b.size()-1, n = 1;
    while (n < need) n <<= 1;
    vector<Complex> fa(n), fb(n);
    for (size_t i = 0; i < a.size(); ++i) fa[i] = (long double)a[i];
    for (size_t i = 0; i < b.size(); ++i) fb[i] = (long double)b[i];
    fft(fa,false); fft(fb,false);
    for (size_t i = 0; i < n; ++i) fa[i] *= fb[i];
    fft(fa,true);
    vector<ll> result(need);
    for (size_t i = 0; i < need; ++i) result[i] = llroundl(fa[i].real());
    return result;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
