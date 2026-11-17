#include <bits/stdc++.h>
using namespace std;
struct RangeAddSum {
    int n;
    ll sum[4*GN+4]{}, lazy[4*GN+4]{};
    RangeAddSum(int n) : n(n) {}
    void apply(int o,int l,int r,ll v) {
        sum[o] += v*(r-l); lazy[o] += v;
    }
    void push(int o,int l,int r) {
        if (r-l <= 1 || lazy[o] == 0) return;
        int mid = (l+r)/2;
        apply(o*2,l,mid,lazy[o]); apply(o*2+1,mid,r,lazy[o]);
        lazy[o] = 0;
    }
    void update(int o,int l,int r,int ql,int qr,ll v) {
        if (ql <= l && r <= qr) { apply(o,l,r,v); return; }
        push(o,l,r);
        int mid = (l+r)/2;
        if (ql < mid) update(o*2,l,mid,ql,qr,v);
        if (qr > mid) update(o*2+1,mid,r,ql,qr,v);
        sum[o] = sum[o*2]+sum[o*2+1];
    }
    ll query(int o,int l,int r,int ql,int qr) {
        if (ql <= l && r <= qr) return sum[o];
        push(o,l,r);
        int mid = (l+r)/2;
        ll ans = 0;
        if (ql < mid) ans += query(o*2,l,mid,ql,qr);
        if (qr > mid) ans += query(o*2+1,mid,r,ql,qr);
        return ans;
    }
    void add(int l,int r,ll v) { if (l < r) update(1,0,n,l,r,v); }
    ll get(int l,int r) { return l < r ? query(1,0,n,l,r) : 0; }
};
