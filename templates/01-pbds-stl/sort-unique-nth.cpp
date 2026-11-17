#include <bits/stdc++.h>
using namespace std;

int main() { // PDF: body
    vector<int> a{4, 1, 3, 1};
    sort(a.begin(), a.end());
    a.erase(unique(a.begin(), a.end()), a.end());   // {1, 3, 4}
    sort(a.begin(), a.end(), greater<int>{});       // descending
    int k = 1;                                     // zero-based
    if (0 <= k && k < (int)a.size())
        nth_element(a.begin(), a.begin() + k, a.end());
}
