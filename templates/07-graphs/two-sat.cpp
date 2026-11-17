#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../02-common/forward-star.cpp"
#include "scc.cpp"
#endif // PDF: endhide
void add_or(int i, bool a, int j, bool b) {
    int x = 2 * i - 1 + a, y = 2 * j - 1 + b;
    scc_add(2 * i - a, y);
    scc_add(2 * j - b, x);
}
void imply(int i, bool a, int j, bool b) {
    add_or(i, !a, j, b); // (x_i=a) => (x_j=b)
}
void force(int i, bool value) {
    add_or(i, value, i, value);
}
int sat_value[GN];
bool solve_2sat(int n) {
    scc_build();
    for (int i = 1; i <= n; ++i) {
        if (scc_id[2 * i] == scc_id[2 * i - 1]) return false;
        sat_value[i] = scc_id[2 * i] > scc_id[2 * i - 1];
    }
    return true;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
