#include <bits/stdc++.h>
using namespace std;
#include "ordered-tree.cpp"

int main() { // PDF: body
    OrderedSet<int> a, b;
    a.insert(1); a.insert(3); a.insert(5);
    a.split(3, b);  // a = {1, 3}, b = {5}
    a.join(b);      // a = {1, 3, 5}, b is empty
}
