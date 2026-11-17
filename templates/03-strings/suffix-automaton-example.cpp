#include <bits/stdc++.h>
using namespace std;
#include "suffix-automaton.cpp"

int main() { // PDF: body
    sam_init();
    for (char c : string("ababa")) sam_extend(c);
    long long distinct = 0;
    for (int v = 1; v < sam_sz; ++v) distinct += sam[v].len - sam[sam[v].link].len; // 9
    sam_propagate();
    int v = 0;
    for (char c : string("aba")) {
        v = sam[v].go[c - 'a'];
        if (v == -1) break;
    }
    long long hits = v == -1 ? 0 : sam[v].occ; // 2
}
