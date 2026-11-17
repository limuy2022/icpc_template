#include <bits/stdc++.h>
using namespace std;
struct ACNode {
    int go[26]{};
    int fail = 0;
};
static constexpr int AC_CAP = 200005;
ACNode ac[AC_CAP];
int ac_sz = 1, ac_order[AC_CAP], ac_used = 0;
void ac_init() {
    ac[0] = {};
    ac_sz = 1;
    ac_used = 0;
}
int ac_insert(string_view s) {
    int u = 0;
    for (char ch : s) {
        int c = ch - 'a';
        if (!ac[u].go[c]) {
            ac[ac_sz] = {};
            ac[u].go[c] = ac_sz++;
        }
        u = ac[u].go[c];
    }
    return u; // save endpoint for this pattern
}
void ac_build() {
    for (int c = 0; c < 26; ++c)
        if (ac[0].go[c]) ac_order[ac_used++] = ac[0].go[c];
    for (int i = 0; i < ac_used; ++i) {
        int u = ac_order[i], f = ac[u].fail;
        for (int c = 0; c < 26; ++c) {
            int v = ac[u].go[c];
            if (v) {
                ac[v].fail = ac[f].go[c];
                ac_order[ac_used++] = v;
            } else
                ac[u].go[c] = ac[f].go[c];
        }
    }
}
long long ac_occ[AC_CAP];
void ac_count(string_view text) {
    fill_n(ac_occ, ac_sz, 0);
    int u = 0;
    for (char ch : text) {
        u = ac[u].go[ch - 'a'];
        ++ac_occ[u];
    }
    for (int i = ac_used - 1; i >= 0; --i) {
        int v = ac_order[i];
        ac_occ[ac[v].fail] += ac_occ[v];
    }
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
