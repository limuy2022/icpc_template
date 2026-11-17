#include <bits/stdc++.h>
using namespace std;
#include "suffix-array.cpp"

int main() { // PDF: body
    string s = "banana";
    build_sa(s);
    // sa = {5, 3, 1, 0, 4, 2}; height = {0, 1, 3, 0, 0, 2}
    long long n = (long long)s.size();
    long long distinct = n * (n + 1) / 2 - accumulate(height, height + n, 0LL);
    int longest_repeat = n == 0 ? 0 : *max_element(height, height + n);
    // distinct = 15; longest_repeat = 3
}
