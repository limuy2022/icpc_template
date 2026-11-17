ld closest_pair2(vector<Point> p) {
    if (p.size() < 2) return numeric_limits<ld>::infinity();
    sort(p.begin(),p.end(),lex_less);
    vector<Point> buffer(p.size());
    auto by_y = [](Point a,Point b) {
        return a.y != b.y ? a.y < b.y : a.x < b.x;
    };
    auto solve = [&](auto&& self,int l,int r) -> ld {
        if (r-l <= 3) {
            ld best = numeric_limits<ld>::infinity();
            for (int i = l; i < r; ++i)
                for (int j = l; j < i; ++j)
                    best = min(best,norm2(p[i]-p[j]));
            sort(p.begin()+l,p.begin()+r,by_y);
            return best;
        }
        int mid = (l+r)/2;
        ld middle_x = p[mid].x;              // save before recursion
        ld best = min(self(self,l,mid),self(self,mid,r));
        merge(p.begin()+l,p.begin()+mid,p.begin()+mid,p.begin()+r,
              buffer.begin()+l,by_y);
        copy(buffer.begin()+l,buffer.begin()+r,p.begin()+l);
        vector<Point> strip;
        for (int i = l; i < r; ++i) {
            ld dx = p[i].x-middle_x;
            if (dx*dx >= best) continue;
            for (int j = (int)strip.size()-1; j >= 0; --j) {
                ld dy = p[i].y-strip[j].y;
                if (dy*dy >= best) break;
                best = min(best,norm2(p[i]-strip[j]));
            }
            strip.push_back(p[i]);
        }
        return best;
    };
    return solve(solve,0,(int)p.size());
}
