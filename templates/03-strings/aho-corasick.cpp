struct AhoCorasick {
    struct Node {
        int go[26]{};
        int fail = 0;
    };
    static constexpr int CAP = 200005;
    Node t[CAP];
    int sz = 1, order[CAP], used = 0;
    int insert(string_view s) {
        int u = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (!t[u].go[c]) {
                t[u].go[c] = sz++;
            }
            u = t[u].go[c];
        }
        return u;                        // save endpoint for this pattern
    }
    void build() {
        for (int c = 0; c < 26; ++c)
            if (t[0].go[c]) order[used++] = t[0].go[c];
        for (int i = 0; i < used; ++i) {
            int u = order[i], f = t[u].fail;
            for (int c = 0; c < 26; ++c) {
                int v = t[u].go[c];
                if (v) {
                    t[v].fail = t[f].go[c];
                    order[used++] = v;
                } else t[u].go[c] = t[f].go[c];
            }
        }
    }
    vector<long long> count(string_view text) const {
        vector<long long> cnt(sz);
        int u = 0;
        for (char ch : text) {
            u = t[u].go[ch - 'a'];
            ++cnt[u];
        }
        for (int i = used - 1; i >= 0; --i) {
            int v = order[i];
            cnt[t[v].fail] += cnt[v];
        }
        return cnt;
    }
};
