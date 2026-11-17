#include <bits/stdc++.h>
using namespace std;
#include "aho-corasick.cpp"

int main() { // PDF: body
    static AhoCorasick ac;
    int a = ac.insert("a"), aa = ac.insert("aa");
    int same = ac.insert("a");
    ac.build();
    auto cnt = ac.count("aaa");          // cnt[a] = 3, cnt[aa] = 2
    // cnt[same] == cnt[a]; repeated patterns share an endpoint
}
