#include <bits/stdc++.h>
using namespace std;
vector<Point> clip_left(const vector<Point>& polygon, Point a, Point b) {
    vector<Point> result;
    int n = (int)polygon.size();
    Point v = b-a;                          // nonzero direction
    for (int i = 0; i < n; ++i) {
        Point p = polygon[i], q = polygon[(i+1)%n];
        ld sp = cross(v,p-a), sq = cross(v,q-a);
        bool inside_p = sp >= 0, inside_q = sq >= 0;
        if (inside_p) result.push_back(p);
        if (inside_p != inside_q) {
            ld t = sp/(sp-sq);
            result.push_back(p+(q-p)*t);
        }
    }
    return result;
}
