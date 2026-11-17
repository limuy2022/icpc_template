#include <bits/stdc++.h>
using namespace std;
#include "huffman.cpp"

int main() { // PDF: body
    vector<long long> weights{1, 2, 3, 4};
    build_huffman(weights);
    long long answer = huff_cost;             // 19
    auto code = huffman_codes((int)weights.size());
    // Leaf IDs [0, n) match the original weight order.
    // WPL = sum(weights[i] * code[i].size()) == sum of merge costs
}
