#include <bits/stdc++.h>
using namespace std;
#include "../02-common/forward-star.cpp"
#include "scc.cpp"
#include "two-sat.cpp"

int main() { // PDF: body
    TwoSAT sat(2);
    sat.add_or(0,true,1,true);       // x0 or x1
    sat.imply(0,true,1,false);       // x0 => not x1
    sat.force(0,true);
    auto solution = sat.solve();    // {1,0}
}
