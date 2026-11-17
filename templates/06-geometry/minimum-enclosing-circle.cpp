#include <bits/stdc++.h>
using namespace std;
struct Circle { Point o; ld r; };
Circle diameter_circle(Point a,Point b) {
    return {(a+b)/2,length(a-b)/2};
}
bool outside(Circle c,Point p) {
    return c.r < 0 || norm2(p-c.o) > (c.r+EPS)*(c.r+EPS);
}
Circle through_three(Point a,Point b,Point c) {
    Point u = b-a, v = c-a;
    ld den = 2*cross(u,v);
    if (den == 0) {
        Circle best{{0,0},numeric_limits<ld>::infinity()};
        for (Circle q : {diameter_circle(a,b),diameter_circle(a,c),
                         diameter_circle(b,c)})
            if (!outside(q,a) && !outside(q,b) && !outside(q,c) &&
                q.r < best.r) best = q;
        return best;
    }
    Point o = a+(perp(u)*norm2(v)-perp(v)*norm2(u))/den;
    return {o,length(o-a)};
}
Circle smallest_circle(vector<Point> p) {
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    shuffle(p.begin(),p.end(),rng);
    Circle c{{0,0},-1};
    for (int i = 0; i < (int)p.size(); ++i) if (outside(c,p[i])) {
        c = {p[i],0};
        for (int j = 0; j < i; ++j) if (outside(c,p[j])) {
            c = diameter_circle(p[i],p[j]);
            for (int k = 0; k < j; ++k) if (outside(c,p[k]))
                c = through_three(p[i],p[j],p[k]);
        }
    }
    return c;
}
