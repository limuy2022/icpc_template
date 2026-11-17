#include <bits/stdc++.h>
using namespace std;
#include "aho-corasick.cpp"

int main() { // PDF: body
    ac_init();
    int a = ac_insert("a"), aa = ac_insert("aa");
    int same = ac_insert("a");
    ac_build();
    auto cnt = ac_count("aaa");          // cnt[a] = 3, cnt[aa] = 2
    // cnt[same] == cnt[a]; repeated patterns share an endpoint
}
