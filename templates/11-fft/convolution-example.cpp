#include <bits/stdc++.h>
using namespace std;
#include "../05-number-theory/modular-arithmetic.cpp"
#include "fft.cpp"
#include "integer-convolution.cpp"

int main() { // PDF: body
    vector<ll> a{1,-2,3}, b{4,5};
    auto c = convolution_fft(a,b);             // {4,-3,2,15}
}
