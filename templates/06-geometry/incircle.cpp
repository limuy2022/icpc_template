#include <bits/stdc++.h>
using namespace std;
ld incircle(Point a,Point b,Point c,Point d) {
    a = a-d; b = b-d; c = c-d;
    return norm2(a)*cross(b,c)-norm2(b)*cross(a,c)
           +norm2(c)*cross(a,b);
}
