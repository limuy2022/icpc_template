#include <bits/stdc++.h>
using namespace std;
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
