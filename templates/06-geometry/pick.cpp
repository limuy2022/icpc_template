#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../05-number-theory/modular-arithmetic.cpp"
#endif // PDF: endhide
struct IPoint { ll x, y; };
pair<i128,i128> lattice_polygon(const vector<IPoint>& p) {
    i128 area2 = 0, boundary = 0;
    int n = (int)p.size();
    for (int i = 0; i < n; ++i) {
        auto a = p[i], b = p[(i+1)%n];
        area2 += (i128)a.x*b.y-(i128)a.y*b.x;
        // This version assumes each coordinate difference fits in ll.
        boundary += gcd(llabs(b.x-a.x),llabs(b.y-a.y));
    }
    if (area2 < 0) area2 = -area2;
    i128 interior = (area2-boundary+2)/2;
    return {interior,boundary};
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
