#include <bits/stdc++.h>
using namespace std;

int main() { // PDF: body
    string s;
    if (!(cin >> s)) return 0;
    __float128 x = strtof128(s.c_str(), nullptr);
    __float128 y = sqrtf128(x + 0.1Q);
    char buf[128];
    strfromf128(buf, sizeof buf, "%.36g", y);
    cout << buf << '\n';
}
