#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "point.cpp"
#endif // PDF: endhide
ld convex_diameter2(const vector<Point>& p) {
    int n = (int)p.size();
    if (n < 2) return 0;
    if (n == 2) return norm2(p[1]-p[0]);
    ld best = 0;
    int j = 1;
    for (int i = 0; i < n; ++i) {
        int ni = (i+1)%n;
        Point edge = p[ni]-p[i];
        while (cross(edge,p[(j+1)%n]-p[i]) >
               cross(edge,p[j]-p[i])+EPS) j = (j+1)%n;
        for (int v : {j,(j+1)%n}) {
            best = max(best,norm2(p[v]-p[i]));
            best = max(best,norm2(p[v]-p[ni]));
        }
    }
    return best;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
