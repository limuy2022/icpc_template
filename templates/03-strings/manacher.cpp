#include <bits/stdc++.h>
using namespace std;
constexpr int MAN_CAP = 200005;
int rad[2][MAN_CAP]; // 0: even, 1: odd
void palindrome_radius(string_view s, bool odd) {
    int n = (int)s.size(), even = !odd;
    int* d = rad[odd];
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = i > r ? (int)odd : min(d[l + (r - i) + even], r - i + 1);
        while (i - k - even >= 0 && i + k < n && s[i - k - even] == s[i + k]) ++k;
        d[i] = k;
        if (i + k - 1 > r) {
            l = i - k + odd;
            r = i + k - 1;
        }
    }
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
