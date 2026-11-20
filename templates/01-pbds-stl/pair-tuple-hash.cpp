#include <bits/stdc++.h>
using namespace std;

struct TupleHash {
    static uint64_t mix(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    template <class Key> size_t operator()(const Key& key) const {
        static const uint64_t seed =
            chrono::steady_clock::now().time_since_epoch().count();
        uint64_t h = seed;
        apply(
            [&](const auto&... x) {
                ((h = mix(h + hash<decay_t<decltype(x)>>{}(x))), ...);
            },
            key);
        return h;
    }
};

#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
