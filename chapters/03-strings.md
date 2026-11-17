# 字符串算法 · KMP、Z 函数、Manacher

示例使用 GNU C++17，函数返回的数组与匹配位置均为 0-based。

## 01 · KMP，找模式串的所有出现位置

[C++ 源码](../templates/03-strings/kmp.cpp)

- `pi[i]` 是前缀 `s[0..i]` 的最长相等真前后缀长度。KMP 时间$\mathcal{O}(n + m)$，匹配后回退以保留重叠结果。

## 02 · Z 函数，求每个后缀与整串的最长公共前缀

[C++ 源码](../templates/03-strings/z-function.cpp)

- 维护匹配区间 `[l, r)`，时间$\mathcal{O}(n)$。若用 `pat + sep + text` 匹配，分隔符必须在两串中都未出现。

## 03 · Manacher，奇偶回文半径

[C++ 源码](../templates/03-strings/manacher.cpp)

- 奇回文以 i 为中心，长度 `2*d[i]-1`。偶回文中心在 i-1 与 i 之间，长度 `2*d[i]`。
- 两种各跑一次，时间$\mathcal{O}(n)$。两组半径之和就是回文子串出现总数，累加用 long long。

# 字符串算法 · AC 自动机

小写英文字母，离线插入全部非空模式串，再 build 一次。每次查询返回各节点代表的模式出现次数。

## 01 · Trie、失败指针、补全转移

[C++ 源码](../templates/03-strings/aho-corasick.cpp)

## 02 · 查询与易错点

[C++ 源码](../templates/03-strings/aho-corasick-example.cpp)

- 保存每个模式的终点编号，查询后用 `cnt[endpoint]` 读答案。逆 BFS 序沿 fail 累加，包含重叠出现。
- 节点数不得超过 CAP，含根。build 会把缺失转移补全，之后禁止插入模式或再次 build。重复查询可重新调用 count。
- 设模式总长度为 L、节点数为 S、文本长度为 N。插入$\mathcal{O}(L)$，建图$\mathcal{O}(26S)$，单次计数$\mathcal{O}(N + S)$。
- 适合统计所有模式各自的出现次数。枚举每个匹配的具体位置还需要维护输出链或模式终点信息。

# 字符串算法 · 后缀自动机 SAM

小写英文字母，逐字符 extend。根为 0，根的 link 为 -1。状态 v 代表长度在 (len[link[v]], len[v]] 内的一组子串。

## 01 · 构造与克隆

[C++ 源码](../templates/03-strings/suffix-automaton.cpp)

## 02 · 不同子串数与模式出现次数

[C++ 源码](../templates/03-strings/suffix-automaton-example.cpp)

- 查询模式非空。走不到转移即不存在，走完后状态的 occ 是出现次数，包含重叠。
- 新状态 occ = 1，克隆 occ = 0；按 len 降序向 link 汇总。propagate 后禁止继续 extend 或重复汇总。
- 构造$\mathcal{O}(n)$，状态数为$\mathcal{O}(n)$。此处用排序汇总，$\mathcal{O}(S \log  S)$，空间$\mathcal{O}(26S)$。预留 CAP 至少为 2n+1。

# 字符串算法 · 后缀数组 SA、LCP

0-based。sa[i] 是第 i 小后缀的起点，rk[pos] 是后缀 pos 的排名。此版倍增用 sort，构造$\mathcal{O}(n \log ^{2} n)$。

## 01 · 倍增排序 + Kasai 求相邻后缀 LCP

[C++ 源码](../templates/03-strings/suffix-array.cpp)

## 02 · 常用结论与下标

[C++ 源码](../templates/03-strings/suffix-array-example.cpp)

- 串长不超过 CAP，大对象放全局或声明为 static。
- `height[i] = LCP(sa[i-1], sa[i])`，且 height[0] = 0。Kasai 为$\mathcal{O}(n)$，额外空间$\mathcal{O}(n)$。
- 任意两后缀 x、y，若 rk[x] < rk[y]，LCP 为 height[rk[x]+1 .. rk[y]] 的最小值，可配 RMQ。
- x == y 时 LCP = n-x。不同子串总数 = n(n+1)/2 - sum(height)，使用 64 位整数。
- 最长重复子串允许重叠，长度是 max(height)。本模板未追加哨兵字符，空串返回空数组。
