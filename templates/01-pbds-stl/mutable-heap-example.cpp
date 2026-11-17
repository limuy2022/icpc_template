#include <bits/stdc++.h>
using namespace std;
#include "mutable-heap.cpp"

int main() { // PDF: body
    Heap q, other;
    Heap::point_iterator h = q.push({10, 1});
    q.push({5, 2});
    q.modify(h, {3, 1});           // top() == (3, 1)
    q.erase(h);                    // h is now invalid
    other.push({2, 3});
    q.join(other);                 // other.empty(), top() == (2, 3)
    if (!q.empty()) q.pop();       // removes (2, 3)
}
