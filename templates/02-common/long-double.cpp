#include <bits/stdc++.h>
using namespace std;

int main() { // PDF: body
    long double x;
    if (!(cin >> x)) return 0;
    long double y = sqrtl(x + 0.1L);
    cout << fixed << setprecision(18) << y << '\n';
}
