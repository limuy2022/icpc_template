#include <bits/stdc++.h>
using namespace std;
#include "../02-common/forward-star.cpp"
#include "scc.cpp"
#include "two-sat.cpp"

int main() { // PDF: body
    scc_init(4);
    add_or(1, true, 2, true); // x1 or x2
    imply(1, true, 2, false); // x1 => not x2
    force(1, true);
    bool ok = solve_2sat(2); // true, sat_value[1..2] = {1,0}
}
