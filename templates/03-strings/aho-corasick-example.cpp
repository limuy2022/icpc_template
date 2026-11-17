#include <bits/stdc++.h>
using namespace std;
#include "aho-corasick.cpp"

int main() { // PDF: body
    ac_init();
    int a = ac_insert("a"), aa = ac_insert("aa");
    int same = ac_insert("a");
    ac_build();
    ac_count("aaa"); // ac_occ[a] = 3, ac_occ[aa] = 2
    // ac_occ[same] == ac_occ[a]; repeated patterns share an endpoint
}
