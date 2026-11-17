#include <bits/stdc++.h>
using namespace std;
struct SAM {
    struct Node {
        int go[26];
        int link = -1, len = 0;
        long long occ = 0;
        Node() { fill_n(go,26,-1); }
    };
    static constexpr int CAP = 200005;
    Node t[CAP];
    int last = 0, sz = 1;
    void extend(char ch) {
        int c = ch - 'a', cur = sz++;
        t[cur].len = t[last].len + 1;
        t[cur].occ = 1;
        int p = last;
        while (p != -1 && t[p].go[c] == -1) {
            t[p].go[c] = cur;
            p = t[p].link;
        }
        if (p == -1) t[cur].link = 0;
        else {
            int q = t[p].go[c];
            if (t[p].len + 1 == t[q].len) t[cur].link = q;
            else {
                int clone = sz++; t[clone] = t[q];
                t[clone].len = t[p].len + 1;
                t[clone].occ = 0;
                while (p != -1 && t[p].go[c] == q) {
                    t[p].go[c] = clone;
                    p = t[p].link;
                }
                t[q].link = t[cur].link = clone;
            }
        }
        last = cur;
    }
    void propagate() {                      // call once after all extend
        static int order[CAP];
        iota(order,order+sz,0);
        sort(order,order+sz, [&](int a, int b) {
            return t[a].len > t[b].len;
        });
        for (int i = 0; i < sz; ++i) {
            int v = order[i], p = t[v].link;
            if (p != -1) t[p].occ += t[v].occ;
        }
    }
};
