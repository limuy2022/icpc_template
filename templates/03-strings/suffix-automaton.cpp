#include <bits/stdc++.h>
using namespace std;
struct SAMNode {
    int go[26];
    int link = -1, len = 0;
    long long occ = 0;
    SAMNode() { fill_n(go,26,-1); }
};
static constexpr int SAM_CAP = 200005;
SAMNode sam[SAM_CAP];
int sam_last = 0, sam_sz = 1;
void sam_init() {
    sam[0] = SAMNode{}; sam_last = 0; sam_sz = 1;
}
void sam_extend(char ch) {
    int c = ch - 'a', cur = sam_sz++;
    sam[cur] = SAMNode{};
    sam[cur].len = sam[sam_last].len + 1;
    sam[cur].occ = 1;
    int p = sam_last;
    while (p != -1 && sam[p].go[c] == -1) {
        sam[p].go[c] = cur;
        p = sam[p].link;
    }
    if (p == -1) sam[cur].link = 0;
    else {
        int q = sam[p].go[c];
        if (sam[p].len + 1 == sam[q].len) sam[cur].link = q;
        else {
            int clone = sam_sz++; sam[clone] = sam[q];
            sam[clone].len = sam[p].len + 1;
            sam[clone].occ = 0;
            while (p != -1 && sam[p].go[c] == q) {
                sam[p].go[c] = clone;
                p = sam[p].link;
            }
            sam[q].link = sam[cur].link = clone;
        }
    }
    sam_last = cur;
}
void sam_propagate() {                      // call once after all sam_extend
    static int order[SAM_CAP];
    iota(order,order+sam_sz,0);
    sort(order,order+sam_sz, [&](int a, int b) {
        return sam[a].len > sam[b].len;
    });
    for (int i = 0; i < sam_sz; ++i) {
        int v = order[i], p = sam[v].link;
        if (p != -1) sam[p].occ += sam[v].occ;
    }
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
