# PBDS 赛场速查 · 有序树

PBDS 是 GNU libstdc++ 扩展，命名空间为 `__gnu_pbds`。示例默认升序红黑树。

## 01 · 去重集合、排名、第 k 小

```cpp
#include <functional>
#include <utility>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

namespace pb = __gnu_pbds;

template<class Key>
using OrderedSet = pb::tree<Key, pb::null_type, std::less<Key>,
                            pb::rb_tree_tag,
                            pb::tree_order_statistics_node_update>;
```

```cpp
OrderedSet<int> s;
s.insert(10);
s.insert(30);
s.insert(20);
s.insert(20);                     // {10, 20, 30}
auto cnt = s.order_of_key(20);    // 1
auto it = s.find_by_order(1);     // points to 20
if (it != s.end()) s.erase(*it);  // {10, 30}
```

- `order_of_key(x)` 数严格小于 `x` 的元素，`x` 可以不在树里。
- `find_by_order(k)` 从 0 开始。题目给 1-based 的 `k` 时先检查 `1 <= k <= size()`，再查 `k - 1`。
- 越界返回 `end()`。插入、按键删除、查排名和查第 k 小均为$\mathcal{O}(\log  n)$。
- `lower_bound(x)` 找第一个$\geq  x$的键，`upper_bound(x)` 找第一个 > x 的键。

## 02 · 保留重复值，用 (值, 唯一编号) 作键

```cpp
using Key = std::pair<long long, int>;
OrderedSet<Key> ms;
int next_id = 0;                  // IDs: [0, next_id)
long long x = 7;
int id = next_id++;
ms.insert({x, id});
ms.insert({x, next_id++});

auto lt = ms.order_of_key({x, 0});        // count < x
auto le = ms.order_of_key({x, next_id});  // count <= x
auto eq = le - lt;                        // count == x
auto kth = ms.find_by_order(1);
if (kth != ms.end()) { (void)kth->first; }  // value of the 2nd key
ms.erase({x, id});                       // erase one known copy

auto pos = ms.lower_bound({x, 0});
if (pos != ms.end() && pos->first == x)
    ms.erase(*pos);                       // erase any one copy
```

- 编号从 0 递增，每次插入取新编号，并保证计数器足够大。用编号边界避免计算 `x + 1`。
- 比较器须满足严格弱序，保留重复值时用唯一编号区分。

## 03 · 有序树拆分与合并

```cpp
OrderedSet<int> a, b;
a.insert(1); a.insert(3); a.insert(5);
a.split(3, b);  // a = {1, 3}, b = {5}
a.join(b);      // a = {1, 3, 5}, b is empty
```

- `a.split(x, b)` 先清空 b，再把严格大于 x 的键移到 b。
- `a.join(b)` 要求一棵树的所有键严格小于另一棵的所有键。任一树为空也可以。
- 键值范围交错或重叠时，跳过 join。两棵树都非空时，可比较各自的最小键和最大键。
- `find(x) != end()` 检查键是否存在。`begin()` / `rbegin()` 取最小 / 最大键，解引用前先判空。

## 04 · 树的其他接口

- `erase(it)` 删当前元素并返回下一迭代器。`erase_if(pred)` 扫描全树，返回删除数。
- 有序映射把模板里的 `pb::null_type` 换成值类型。用 `t[key]` 访问值，缺键会插入。
- `splay_tree_tag` 同样支持按键拆分。先 `t.split(R, hi)` 再 `t.split(L, mid)`，取出 (L, R]。
- 按下标做区间翻转或区间加需要隐式 Splay 或 Treap。PBDS 的 `split` 按键拆分。

# PBDS 赛场速查 · 堆与哈希表

## 05 · 可修改堆，保存 push 返回的句柄

```cpp
#include <functional>
#include <utility>
#include <ext/pb_ds/priority_queue.hpp>

using Item = std::pair<long long, int>;  // (distance, vertex)
using Heap = __gnu_pbds::priority_queue<
    Item, std::greater<Item>, __gnu_pbds::pairing_heap_tag>;
```

```cpp
Heap q, other;
Heap::point_iterator h = q.push({10, 1});
q.push({5, 2});
q.modify(h, {3, 1});           // top() == (3, 1)
q.erase(h);                    // h is now invalid
other.push({2, 3});
q.join(other);                 // other.empty(), top() == (2, 3)
if (!q.empty()) q.pop();       // removes (2, 3)
```

- `std::greater<Item>` 是小根堆，换成 `std::less<Item>` 就是大根堆。pair 先按第一项比较，再按第二项。
- `modify(h, value)` 更新整个元素。Dijkstra 可按顶点存句柄，并单独记录该顶点是否还在堆中。
- `pop()`、`erase(h)` 和 `clear()` 会让被删除元素的句柄失效。操作句柄时，用当前持有该节点的堆。
- 这里使用 `pairing_heap_tag`，其句柄可跨其他元素的插入和删除使用。

## 06 · 堆的批量操作与遍历

```cpp
Heap q, moved;
q.push({3, 1});
q.push({8, 2});
q.split([](const Item& item) { return item.first > 5; }, moved);
// q contains (3, 1); moved contains (8, 2)
```

- `q.split(pred, moved)` 先清空目标堆，再将使 `pred` 返回 true 的元素移过去。
- `q.erase_if(pred)` 删除满足条件的元素并返回删除数。这两个操作都要扫描整个堆。
- `begin()` / `end()` 可以遍历堆，遍历顺序与优先级无关。按优先级取出时循环 `top()` / `pop()`。

## 07 · 哈希表，常用映射接口

```cpp
#include <ext/pb_ds/assoc_container.hpp>
```

```cpp
__gnu_pbds::gp_hash_table<long long, int> freq;
long long x = 42;
++freq[x];                     // missing key starts at 0
auto it = freq.find(x);        // lookup without insertion
if (it != freq.end()) {
    int count = it->second;
    (void)count;
}
freq.erase(x);
bool inserted = freq.insert({x, 9}).second;
(void)inserted;
```

- `insert({key, value}).second` 表示是否插入成功。已有键保留原值，覆盖时用 `freq[key] = value`。
- `size()` 取元素数，`empty()` 判空，`clear()` 清空。`swap(other)` 交换同类型容器。

- `gp_hash_table` 用探测法解决冲突。查找、插入和删除的表现取决于散列质量，最坏可达线性。
- 默认模板直接使用自动扩容。需要显式 `resize()` 时，先配置允许外部调整容量的 resize policy。

# STL 赛场速查 · 字符串

GNU C++ 可用 `#include <bits/stdc++.h>`。下方示例假定已写 `using namespace std;`。

## 01 · string 查找、切片、编辑、转换

```cpp
string s = "ababa";
auto i = s.find("ba");              // 1
auto j = s.find("ba", 2);           // 3; search from index 2
auto k = s.rfind("ba");             // 3; last occurrence
auto p = s.find_first_of("bx");     // 1; any char in "bx"
string sub = s.substr(1, 2);        // "ba"; position, length
bool starts = s.compare(0, 2, "ab") == 0;
s.insert(2, "XY");                  // "abXYaba"
s.replace(2, 2, "Z");               // "abZaba"
s.erase(2, 1);                     // "ababa"
s += "!"; s.push_back('?'); s.pop_back();
string bits = "101";
const char* last = bits.data() + bits.size();
long long x;
auto [end, ec] = from_chars(bits.data(), last, x, 2);
if (ec == errc{} && end == last) { string num = to_string(x); }
istringstream in("one two");        // split by whitespace
string token;
while (in >> token) { /* use token */ }
```

- 查找失败返回 `string::npos`。先判断是否找到，再做下标运算。`substr(pos, len)` 的长度会截到末尾。
- `getline(cin, s)` 读整行。前面用过 `cin >> x` 时，可用 `cin.ignore(numeric_limits<streamsize>::max(), '\n')` 丢掉该行余下内容。
- 整数转换用 `from_chars`。ec 判断转换是否成功，end 指向首个未读字符。完整解析还需 end==last。输入开头的空白和加号需自行处理。

## 02 · 按逗号拆分、拼接、零拷贝子串

```cpp
string csv = "a,,b,";
istringstream in(csv);
string field, joined;
bool first = true;
while (getline(in, field, ',')) {
    if (!first) joined += '|';
    joined += field;
    first = false;
}
if (!csv.empty() && csv.back() == ',') joined += '|';
// joined == "a||b|"; getline alone skips the final empty field
string_view view = csv;
auto part = view.substr(3, 1);          // "b", no copy
```

- `string_view` 借用原字符，原串销毁或改动导致存储失效后就不能再用。view 的 `data()` 不保证子串末尾有 NUL。

# STL 赛场速查 · 容器

## 01 · set、multiset、map、`unordered_map`

```cpp
set<int> st{2, 5, 8};
auto it = st.lower_bound(4);         // 5; check against st.end()
if (it != st.begin()) { auto pre = prev(it); (void)pre; }
multiset<int> ms{2, 2, 5};
auto hit = ms.find(2);
if (hit != ms.end()) ms.erase(hit);  // remove one
ms.erase(2);                         // remove all remaining 2s
map<int, int> freq;
++freq[7];                           // missing key inserts 0
if (auto f = freq.find(7); f != freq.end()) f->second += 2;
unordered_map<int, int> seen;
seen.reserve(1000);                   // reserve for 1000 elements
```

- 有序容器查找、插入和按键定位$\mathcal{O}(\log  n)$。`lower_bound` 找$\geq  x$，`upper_bound` 找 > x。
- 在 set / map 上用成员 `lower_bound`。通用算法虽只比较$\mathcal{O}(\log  n)$次，迭代器移动可达$\mathcal{O}(n)$。
- `multiset.count(x)` 和按键删全部副本的复杂度还要加上匹配数。map 的 `find` 查询不会插入。
- `unordered_map` / `unordered_set` 平均$\mathcal{O}(1)$，最坏$\mathcal{O}(n)$。重哈希使迭代器失效。

## 02 · 双端队列、栈、队列、优先队列、bitset

```cpp
deque<int> d; d.push_front(1); d.push_back(2);
d.pop_front(); d.pop_back();
stack<int> st; st.push(1); (void)st.top(); st.pop();
queue<int> q; q.push(1); (void)q.front(); q.pop();
priority_queue<int> mx;               // max heap
priority_queue<int, vector<int>, greater<int>> mn;
mn.push(3); mn.push(1);
int x = mn.top(); mn.pop();           // x = 1
bitset<128> b; b.set(3); b.flip(7); b.reset(3);
auto k = b.count();                   // 1
```

- deque 随机访问和两端插删为$\mathcal{O}(1)$。
- 栈和队列的 push / pop 为$\mathcal{O}(1)$。优先队列的 top 为$\mathcal{O}(1)$，push / pop 为$\mathcal{O}(\log  n)$。
- `pop()` 不返回元素。先判空，再读 `top()` / `front()`，最后删除。
- bitset 的大小编译期固定，支持 `& | ^ ~ << >>`、`test(i)`、`any()`、`none()`。

# STL 赛场速查 · 算法

大多数算法使用半开区间 `[first, last)`。`end()` 指向尾后，不能解引用。

## 01 · 排序、去重、第 k 小

```cpp
vector<int> a{4, 1, 3, 1};
sort(a.begin(), a.end());
a.erase(unique(a.begin(), a.end()), a.end());   // {1, 3, 4}
sort(a.begin(), a.end(), greater<int>{});       // descending
int k = 1;                                     // zero-based
if (0 <= k && k < (int)a.size())
    nth_element(a.begin(), a.begin() + k, a.end());
```

- `sort` 为$\mathcal{O}(n \log  n)$。同键要保留原相对次序时，用 `stable_sort`。
- `unique` 合并相邻重复项，返回新逻辑尾。任意位置去重先排序，再 erase。
- `nth_element` 平均$\mathcal{O}(n)$。第 k 项就位，两边各自无需有序。比较器要用严格弱序。

## 02 · 二分查找、最值、计数

```cpp
int a[]{1, 2, 2, 5};
auto lo = lower_bound(a, a+4, 2);  // first >= 2
auto hi = upper_bound(a, a+4, 2);  // first > 2
auto copies = hi - lo;                         // 2
bool ok = binary_search(a, a+4, 5);
auto mx = max_element(a, a+4);
auto pos = find(a, a+4, 5);
auto odds = count_if(a, a+4, [](int x){return x%2;});
```

- 二分前保证区间按同一比较器排序。随机访问迭代器上的二分是$\mathcal{O}(\log  n)$。
- `find`、`count_if`、`min_element`、`max_element` 都扫描区间。解引用查找或最值的结果前，先检查是否等于 end。

## 03 · 前缀和、排列、原地调整

```cpp
vector<int> a{3, 1, 2};
long long sum = accumulate(a.begin(), a.end(), 0LL);
long long pre[4]{};
for (size_t i = 0; i < a.size(); ++i) pre[i + 1] = pre[i] + a[i];
// sum on [l, r) = pre[r] - pre[l]
int ids[3];
iota(ids, ids+3, 0);               // 0, 1, 2
do { /* use ids */ } while (next_permutation(ids, ids+3));
reverse(a.begin(), a.end());
rotate(a.begin(), a.begin() + 1, a.end());     // left by 1
a.erase(remove_if(a.begin(), a.end(), [](int x){return x < 2;}),
        a.end());
```

- `accumulate` 的累加类型由初值决定，用 `0LL` 做 64 位求和。`gcd(a,b)` / `lcm(a,b)` 在 <numeric>，结果须可表示。
- 枚举所有不同排列时从升序开始。最后一次 `next_permutation` 返回 false 并恢复升序。
- `remove_if` 返回新逻辑尾，配合 erase 真正删除。rotate 的中间参数指向旋转后的首元素。

## 04 · Top k、判定、归并、堆算法

```cpp
int a[]{4, 1, 3, 2, 5};
partial_sort(a, a + 2, a+5);  // first: 1, 2
bool positive = all_of(a, a+5, [](int x){return x>0;});
int x[]{1, 2, 4}, y[]{2, 3, 4};
int out[6];
merge(x, x+3, y, y+3, out);
// out = {1, 2, 2, 3, 4, 4}
make_heap(a, a+5);       // max heap; O(n)
pop_heap(a, a+5);        // moves max to a[4]
int largest = a[4];              // 5; heap is [begin, end-1)
```

- `partial_sort` 把最小的 k 项排好，$\mathcal{O}(n \log  k)$（$k \geq  2$）。只求第 k 小可用 `nth_element`。
- `any_of / all_of / none_of` 对空区间分别返回 false / true / true。
- `merge` 合并两段有序区间。求交、并、差用 `set_intersection / set_union / set_difference`。
- 堆追加元素后对新区间调用 `push_heap`。`pop_heap` 后还要 `pop_back` 才会删元素。
