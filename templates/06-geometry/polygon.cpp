#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "point.cpp"
#include "segment.cpp"
#endif // PDF: endhide
int point_in_polygon(Point q, const vector<Point>& p) {
    int winding = 0, n = (int)p.size();
    for (int i = 0; i < n; ++i) {
        Point a = p[i], b = p[(i+1)%n];
        if (on_segment(q,a,b)) return 0;        // boundary
        int side = sgn(cross(b-a,q-a));
        if (a.y <= q.y && b.y > q.y && side > 0) ++winding;
        if (b.y <= q.y && a.y > q.y && side < 0) --winding;
    }
    return winding ? 1 : -1;                   // inside / outside
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
