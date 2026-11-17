#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "point.cpp"
#endif // PDF: endhide
ld incircle(Point a, Point b, Point c, Point d) {
    a = a - d;
    b = b - d;
    c = c - d;
    return norm2(a) * cross(b, c) - norm2(b) * cross(a, c) + norm2(c) * cross(a, b);
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
