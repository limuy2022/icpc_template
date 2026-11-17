#include <bits/stdc++.h>
using namespace std;
static SAM sam;
for (char c : string("ababa")) sam.extend(c);
long long distinct = 0;
for (int v = 1; v < sam.sz; ++v)
    distinct += sam.t[v].len - sam.t[sam.t[v].link].len;  // 9
sam.propagate();
int v = 0;
for (char c : string("aba")) {
    v = sam.t[v].go[c - 'a'];
    if (v == -1) break;
}
long long hits = v == -1 ? 0 : sam.t[v].occ;             // 2
