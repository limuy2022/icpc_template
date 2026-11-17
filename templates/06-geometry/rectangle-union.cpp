#include <bits/stdc++.h>
using namespace std;
struct Rect { ll x1,y1,x2,y2; };
i128 rectangle_union(const vector<Rect>& rectangles) {
    struct Event { ll x,lo,hi; int delta; };
    vector<Event> events;
    vector<ll> ys;
    for (auto r : rectangles) {
        if (r.x1 == r.x2 || r.y1 == r.y2) continue;
        events.push_back({r.x1,r.y1,r.y2,1});
        events.push_back({r.x2,r.y1,r.y2,-1});
        ys.push_back(r.y1); ys.push_back(r.y2);
    }
    if (events.empty()) return 0;
    sort(ys.begin(),ys.end());
    ys.erase(unique(ys.begin(),ys.end()),ys.end());
    sort(events.begin(),events.end(),[](Event a,Event b) {
        return a.x < b.x;
    });
    int m = (int)ys.size()-1;
    constexpr int Y_CAP = 200005; // max unique y coordinates
    static int cover[4*Y_CAP+4];
    static i128 len[4*Y_CAP+4];
    fill_n(cover,4*m+4,0); fill_n(len,4*m+4,0);
    auto update = [&](auto&& self,int o,int l,int r,
                      int ql,int qr,int delta) -> void {
        if (ql <= l && r <= qr) cover[o] += delta;
        else {
            int mid = (l+r)/2;
            if (ql <= mid) self(self,o*2,l,mid,ql,qr,delta);
            if (qr > mid) self(self,o*2+1,mid+1,r,ql,qr,delta);
        }
        if (cover[o]) len[o] = (i128)ys[r+1]-ys[l];
        else len[o] = l == r ? 0 : len[o*2]+len[o*2+1];
    };
    i128 area = 0;
    ll previous = events.front().x;
    size_t i = 0;
    while (i < events.size()) {
        ll x = events[i].x;
        area += ((i128)x-previous)*len[1];
        while (i < events.size() && events[i].x == x) {
            auto e = events[i++];
            int l = lower_bound(ys.begin(),ys.end(),e.lo)-ys.begin();
            int r = lower_bound(ys.begin(),ys.end(),e.hi)-ys.begin()-1;
            update(update,1,0,m-1,l,r,e.delta);
        }
        previous = x;
    }
    return area;
}
