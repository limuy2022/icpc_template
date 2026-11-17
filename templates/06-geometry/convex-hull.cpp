#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "point.cpp"
#endif // PDF: endhide
vector<Point> convex_hull(vector<Point> p) {
    sort(p.begin(), p.end(), lex_less);
    p.erase(unique(p.begin(), p.end(),
                   [](Point a, Point b) { return a.x == b.x && a.y == b.y; }),
            p.end());
    if (p.size() <= 1) return p;
    vector<Point> lower, upper;
    for (Point a : p) {
        while (lower.size() >= 2 && sgn(cross(lower.back() - lower[lower.size() - 2],
                                              a - lower.back())) <= 0)
            lower.pop_back();
        lower.push_back(a);
    }
    for (auto it = p.rbegin(); it != p.rend(); ++it) {
        Point a = *it;
        while (upper.size() >= 2 && sgn(cross(upper.back() - upper[upper.size() - 2],
                                              a - upper.back())) <= 0)
            upper.pop_back();
        upper.push_back(a);
    }
    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
