#include <bits/stdc++.h>
using namespace std;

int main() { // PDF: body
    unsigned long long mask = (1ULL << 2) | (1ULL << 5);
    int count = __builtin_popcountll(mask); // 2
    for (auto m = mask; m; m &= m - 1) {
        int bit = __builtin_ctzll(m); // indices: 2, 5
        (void)bit;
    }
}
