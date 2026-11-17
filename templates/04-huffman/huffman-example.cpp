vector<long long> weights{1, 2, 3, 4};
Huffman h(weights);
long long answer = h.cost;             // 19
auto code = h.codes((int)weights.size());
// Leaf IDs [0, n) match the original weight order.
// WPL = sum(weights[i] * code[i].size()) == sum of merge costs
