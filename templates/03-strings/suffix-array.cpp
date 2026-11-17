struct SuffixArray {
    static constexpr int CAP = 200005;
    int sa[CAP], rk[CAP], height[CAP];
    SuffixArray(string_view s) {
        int n = (int)s.size();
        iota(sa,sa+n,0);
        fill_n(height,n,0);
        if (n == 0) return;
        for (int i = 0; i < n; ++i) rk[i] = (unsigned char)s[i];
        static int next[CAP]; // require n <= CAP
        for (long long k = 1; ; k *= 2) {
            auto second = [&](int i) {
                return i + k < n ? rk[i + k] : -1;
            };
            sort(sa,sa+n, [&](int a, int b) {
                if (rk[a] != rk[b]) return rk[a] < rk[b];
                return second(a) < second(b);
            });
            next[sa[0]] = 0;
            for (int i = 1; i < n; ++i) {
                int a = sa[i - 1], b = sa[i];
                bool diff = rk[a] != rk[b] || second(a) != second(b);
                next[b] = next[a] + diff;
            }
            copy_n(next,n,rk);
            if (rk[sa[n-1]] == n - 1) break;
        }
        for (int i = 0, k = 0; i < n; ++i) {
            int r = rk[i];
            if (r == 0) { k = 0; continue; }
            int j = sa[r - 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k]) ++k;
            height[r] = k;
            if (k) --k;
        }
    }
};
