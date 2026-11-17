#include <bits/stdc++.h>
using namespace std;
using ld = long double;
const ld EPS = 1e-12L;
const ld PI = acosl(-1.0L);
int sgn(ld x) {
    return (x > EPS) - (x < -EPS);
}
struct Point {
    ld x, y;
    Point operator+(Point p) const { return {x + p.x, y + p.y}; }
    Point operator-(Point p) const { return {x - p.x, y - p.y}; }
    Point operator*(ld k) const { return {x * k, y * k}; }
    Point operator/(ld k) const { return {x / k, y / k}; }
};
ld dot(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}
ld cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}
ld norm2(Point a) {
    return dot(a, a);
}
ld length(Point a) {
    return sqrtl(norm2(a));
}
Point perp(Point a) {
    return {-a.y, a.x};
}
Point rotate_point(Point a, ld rad) {
    return {a.x * cosl(rad) - a.y * sinl(rad), a.x * sinl(rad) + a.y * cosl(rad)};
}
bool lex_less(Point a, Point b) {
    return a.x != b.x ? a.x < b.x : a.y < b.y;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
