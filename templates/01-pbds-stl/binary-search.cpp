#include <bits/stdc++.h>
using namespace std;

int main() { // PDF: body
    int a[]{1, 2, 2, 5};
    auto lo = lower_bound(a, a+4, 2);  // first >= 2
    auto hi = upper_bound(a, a+4, 2);  // first > 2
    auto copies = hi - lo;                         // 2
    bool ok = binary_search(a, a+4, 5);
    auto mx = max_element(a, a+4);
    auto pos = find(a, a+4, 5);
    auto odds = count_if(a, a+4, [](int x){return x%2;});
}
