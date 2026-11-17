#include <bits/stdc++.h>
using namespace std;
#include "../02-common/forward-star.cpp"
#include "scc.cpp"
#include "two-sat.cpp"

int main() { // PDF: body
    scc_init(4);
    add_or(0,true,1,true);       // x0 or x1
    imply(0,true,1,false);       // x0 => not x1
    force(0,true);
    bool ok = solve_2sat(2);        // true, sat_value = {1,0}
}
