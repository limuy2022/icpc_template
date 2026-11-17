# 字符串算法 · KMP、Z 函数、Manacher

示例使用 GNU C++17，函数返回的数组与匹配位置均为 0-based。

## 01 · KMP，找模式串的所有出现位置

```cpp
vector<int> prefix_function(string_view s) {
    int n = (int)s.size();
    vector<int> pi(n);
    for (int i = 1, j = 0; i < n; ++i) {
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
    return pi;
}

vector<int> kmp(string_view text, string_view pat) {
    int n = (int)text.size(), m = (int)pat.size();
    vector<int> hits;
    if (m == 0) return hits;                 // empty pattern: no hits
    auto pi = prefix_function(pat);
    for (int i = 0, j = 0; i < n; ++i) {
        while (j && text[i] != pat[j]) j = pi[j - 1];
        if (text[i] == pat[j]) ++j;
        if (j == m) {
            hits.push_back(i - m + 1);
            j = pi[j - 1];                   // allow overlapping hits
        }
    }
    return hits;
}
```

- `pi[i]` 是前缀 `s[0..i]` 的最长相等真前后缀长度。KMP 时间$\mathcal{O}(n + m)$，匹配后回退以保留重叠结果。

## 02 · Z 函数，求每个后缀与整串的最长公共前缀

```cpp
vector<int> z_function(string_view s) {
    int n = (int)s.size();
    vector<int> z(n);                          // z[0] = 0 by convention
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] > r) { l = i; r = i + z[i]; }
    }
    return z;
}
```

- 维护匹配区间 `[l, r)`，时间$\mathcal{O}(n)$。若用 `pat + sep + text` 匹配，分隔符必须在两串中都未出现。

## 03 · Manacher，奇偶回文半径

```cpp
vector<int> palindrome_radius(string_view s, bool odd) {
    int n = (int)s.size(), even = !odd;
    vector<int> d(n);
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = i > r ? (int)odd
                      : min(d[l + (r - i) + even], r - i + 1);
        while (i - k - even >= 0 && i + k < n &&
               s[i - k - even] == s[i + k]) ++k;
        d[i] = k;
        if (i + k - 1 > r) { l = i - k + odd; r = i + k - 1; }
    }
    return d;
}
```

- 奇回文以 i 为中心，长度 `2*d[i]-1`。偶回文中心在 i-1 与 i 之间，长度 `2*d[i]`。
- 两种各跑一次，时间$\mathcal{O}(n)$。两组半径之和就是回文子串出现总数，累加用 long long。

# 字符串算法 · AC 自动机

小写英文字母，离线插入全部非空模式串，再 build 一次。每次查询返回各节点代表的模式出现次数。

## 01 · Trie、失败指针、补全转移

```cpp
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
```

## 02 · 查询与易错点

```cpp
static AhoCorasick ac;
int a = ac.insert("a"), aa = ac.insert("aa");
int same = ac.insert("a");
ac.build();
auto cnt = ac.count("aaa");          // cnt[a] = 3, cnt[aa] = 2
// cnt[same] == cnt[a]; repeated patterns share an endpoint
```

- 保存每个模式的终点编号，查询后用 `cnt[endpoint]` 读答案。逆 BFS 序沿 fail 累加，包含重叠出现。
- 节点数不得超过 CAP，含根。build 会把缺失转移补全，之后禁止插入模式或再次 build。重复查询可重新调用 count。
- 设模式总长度为 L、节点数为 S、文本长度为 N。插入$\mathcal{O}(L)$，建图$\mathcal{O}(26S)$，单次计数$\mathcal{O}(N + S)$。
- 适合统计所有模式各自的出现次数。枚举每个匹配的具体位置还需要维护输出链或模式终点信息。

# 字符串算法 · 后缀自动机 SAM

小写英文字母，逐字符 extend。根为 0，根的 link 为 -1。状态 v 代表长度在 (len[link[v]], len[v]] 内的一组子串。

## 01 · 构造与克隆

```cpp
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
```

## 02 · 不同子串数与模式出现次数

```cpp
static SAM sam;
for (char c : string("ababa")) sam.extend(c);
long long distinct = 0;
for (int v = 1; v < sam.sz; ++v)
    distinct += sam.t[v].len - sam.t[sam.t[v].link].len;  // 9
sam.propagate();
int v = 0;
for (char c : string("aba")) {
    v = sam.t[v].go[c - 'a'];
    if (v == -1) break;
}
long long hits = v == -1 ? 0 : sam.t[v].occ;             // 2
```

- 查询模式非空。走不到转移即不存在，走完后状态的 occ 是出现次数，包含重叠。
- 新状态 occ = 1，克隆 occ = 0；按 len 降序向 link 汇总。propagate 后禁止继续 extend 或重复汇总。
- 构造$\mathcal{O}(n)$，状态数为$\mathcal{O}(n)$。此处用排序汇总，$\mathcal{O}(S \log  S)$，空间$\mathcal{O}(26S)$。预留 CAP 至少为 2n+1。

# 字符串算法 · 后缀数组 SA、LCP

0-based。sa[i] 是第 i 小后缀的起点，rk[pos] 是后缀 pos 的排名。此版倍增用 sort，构造$\mathcal{O}(n \log ^{2} n)$。

## 01 · 倍增排序 + Kasai 求相邻后缀 LCP

```cpp
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
```

## 02 · 常用结论与下标

```cpp
string s = "banana";
static SuffixArray a(s);
// sa = {5, 3, 1, 0, 4, 2}; height = {0, 1, 3, 0, 0, 2}
long long n = (long long)s.size();
long long distinct = n * (n + 1) / 2
                   - accumulate(a.height, a.height+n, 0LL);
int longest_repeat = n == 0 ? 0 : *max_element(a.height, a.height+n);
// distinct = 15; longest_repeat = 3
```

- 串长不超过 CAP，大对象放全局或声明为 static。
- `height[i] = LCP(sa[i-1], sa[i])`，且 height[0] = 0。Kasai 为$\mathcal{O}(n)$，额外空间$\mathcal{O}(n)$。
- 任意两后缀 x、y，若 rk[x] < rk[y]，LCP 为 height[rk[x]+1 .. rk[y]] 的最小值，可配 RMQ。
- x == y 时 LCP = n-x。不同子串总数 = n(n+1)/2 - sum(height)，使用 64 位整数。
- 最长重复子串允许重叠，长度是 max(height)。本模板未追加哨兵字符，空串返回空数组。
