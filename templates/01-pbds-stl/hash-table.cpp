#include <bits/stdc++.h>
using namespace std;
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
