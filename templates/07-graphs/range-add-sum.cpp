#include <bits/stdc++.h>
using namespace std;
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
#include "../05-number-theory/modular-arithmetic.cpp"
#include "../02-common/forward-star.cpp"
#endif // PDF: endhide
int seg_n;
ll seg_sum[4*GN+4]{}, seg_lazy[4*GN+4]{};
void seg_init(int n_) {
    seg_n = n_;
    fill_n(seg_sum,4*seg_n+4,0); fill_n(seg_lazy,4*seg_n+4,0);
}
void seg_apply(int o,int l,int r,ll v) {
    seg_sum[o] += v*(r-l); seg_lazy[o] += v;
}
void seg_push(int o,int l,int r) {
    if (r-l <= 1 || seg_lazy[o] == 0) return;
    int mid = (l+r)/2;
    seg_apply(o*2,l,mid,seg_lazy[o]); seg_apply(o*2+1,mid,r,seg_lazy[o]);
    seg_lazy[o] = 0;
}
void seg_update(int o,int l,int r,int ql,int qr,ll v) {
    if (ql <= l && r <= qr) { seg_apply(o,l,r,v); return; }
    seg_push(o,l,r);
    int mid = (l+r)/2;
    if (ql < mid) seg_update(o*2,l,mid,ql,qr,v);
    if (qr > mid) seg_update(o*2+1,mid,r,ql,qr,v);
    seg_sum[o] = seg_sum[o*2]+seg_sum[o*2+1];
}
ll seg_query(int o,int l,int r,int ql,int qr) {
    if (ql <= l && r <= qr) return seg_sum[o];
    seg_push(o,l,r);
    int mid = (l+r)/2;
    ll ans = 0;
    if (ql < mid) ans += seg_query(o*2,l,mid,ql,qr);
    if (qr > mid) ans += seg_query(o*2+1,mid,r,ql,qr);
    return ans;
}
void seg_add(int l,int r,ll v) { if (l < r) seg_update(1,0,seg_n,l,r,v); }
ll seg_get(int l,int r) { return l < r ? seg_query(1,0,seg_n,l,r) : 0; }
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
