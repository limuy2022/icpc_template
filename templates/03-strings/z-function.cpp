#include <bits/stdc++.h>
using namespace std;
constexpr int Z_CAP = 200005;
int z[Z_CAP];
void z_function(string_view s) {
    int n = (int)s.size();
    fill_n(z,n,0);                          // z[0] = 0 by convention
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] > r) { l = i; r = i + z[i]; }
    }
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
