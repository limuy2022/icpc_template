#include <bits/stdc++.h>
using namespace std;
void XorBasis::ordered_vectors(u64 out[]) const {
    u64 a[64];
    copy_n(b,64,a);
    for (int i = 0; i < 64; ++i) if (a[i])
        for (int j = 0; j < i; ++j)
            if ((a[i]>>j)&1ULL) a[i] ^= a[j];
    int used = 0;
    for (u64 x : a) if (x) out[used++] = x;
}
optional<u64> XorBasis::kth(u128 k,bool nonempty) const {
    u128 total = (u128)1<<rank;
    bool skip_zero = nonempty && !dependent;
    if (k >= total-(u128)skip_zero) return nullopt;
    k += (u128)skip_zero;
    u64 a[64];
    ordered_vectors(a);
    u64 answer = 0;
    for (int i = 0; i < rank; ++i)
        if ((k>>i)&(u128)1) answer ^= a[i];
    return answer;
}
