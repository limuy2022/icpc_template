#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "point.cpp"
#endif // PDF: endhide
bool on_segment(Point p, Point a, Point b) {
    return sgn(cross(b-a,p-a)) == 0 && sgn(dot(p-a,p-b)) <= 0;
}
bool segments_intersect(Point a, Point b, Point c, Point d) {
    int x = sgn(cross(b-a,c-a)), y = sgn(cross(b-a,d-a));
    int z = sgn(cross(d-c,a-c)), w = sgn(cross(d-c,b-c));
    if (x*y < 0 && z*w < 0) return true;
    return on_segment(a,c,d) || on_segment(b,c,d) ||
           on_segment(c,a,b) || on_segment(d,a,b);
}
optional<Point> line_intersection(Point a, Point v, Point b, Point w) {
    ld den = cross(v,w);
    if (sgn(den) == 0) return nullopt;
    return a + v*(cross(b-a,w)/den);
}
Point projection(Point p, Point a, Point b) { // a != b
    Point v = b-a;
    return a+v*(dot(p-a,v)/norm2(v));
}
ld point_segment_distance(Point p, Point a, Point b) {
    Point v = b-a;
    if (norm2(v) == 0) return length(p-a);
    ld t = clamp(dot(p-a,v)/norm2(v),0.0L,1.0L);
    return length(p-(a+v*t));
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
