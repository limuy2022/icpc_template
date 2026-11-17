#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "point.cpp"
#endif // PDF: endhide
Point invert_point(Point p,Point o,ld k) {
    Point v = p-o;
    return o+v*(k/norm2(v));
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
