#include <bits/stdc++.h>
using namespace std;

int main() { // PDF: body
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
}
