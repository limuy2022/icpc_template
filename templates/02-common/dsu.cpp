#include <bits/stdc++.h>
using namespace std;
int dsu_parent[GN], dsu_size[GN];
void dsu_init(int n) {
    iota(dsu_parent,dsu_parent+n,0);
    fill_n(dsu_size,n,1);
}
int dsu_find(int x) {
    if (dsu_parent[x] != x) dsu_parent[x] = dsu_find(dsu_parent[x]);
    return dsu_parent[x];
}
bool unite(int a, int b) {
    a = dsu_find(a); b = dsu_find(b);
    if (a == b) return false;
    if (dsu_size[a] < dsu_size[b]) swap(a, b);
    dsu_parent[b] = a;
    dsu_size[a] += dsu_size[b];
    return true;
}
