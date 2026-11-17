#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../05-number-theory/modular-arithmetic.cpp"
#endif // PDF: endhide
bool wythoff_losing(u64 a, u64 b) { // a,b <= 10^18
    if (a > b) swap(a, b);
    u64 k = b - a, low = 0, high = 3 * k + 1;
    u128 target = (u128)5 * k * k;
    while (low + 1 < high) {
        u64 mid = low + (high - low) / 2;
        if ((u128)mid * mid <= target)
            low = mid;
        else
            high = mid;
    }
    return a == (k + low) / 2;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
