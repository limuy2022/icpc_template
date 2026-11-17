#include <bits/stdc++.h>
using namespace std;
using Complex = complex<long double>;
void fft(vector<Complex>& a,bool inverse_transform) {
    size_t n = a.size();
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n>>1;
        for (; j&bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i],a[j]);
    }
    const long double pi = acosl(-1.0L);
    for (size_t len = 2; len <= n; ) {
        long double angle = (inverse_transform ? -2 : 2)*pi/len;
        Complex root(cosl(angle),sinl(angle));
        for (size_t start = 0; start < n; start += len) {
            Complex w(1,0);
            for (size_t j = 0; j < len/2; ++j) {
                Complex x = a[start+j], y = a[start+j+len/2]*w;
                a[start+j] = x+y; a[start+j+len/2] = x-y;
                w *= root;
            }
        }
        if (len == n) break;
        len <<= 1;
    }
    if (inverse_transform)
        for (Complex& x : a) x /= (long double)n;
}
