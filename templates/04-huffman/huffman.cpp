#include <bits/stdc++.h>
using namespace std;
struct HuffNode {
    long long weight;
    int left = -1, right = -1;
};
vector<HuffNode> huff;
int huff_root = -1;
long long huff_cost = 0;
void build_huffman(const vector<long long>& weights) {
    huff.clear(); huff_root = -1; huff_cost = 0;
    using Item = pair<long long, int>;
    priority_queue<Item, vector<Item>, greater<Item>> q;
    for (long long w : weights) {
        int id = (int)huff.size();
        huff.push_back({w});
        q.push({w, id});
    }
    while (q.size() > 1) {
        auto [a, u] = q.top(); q.pop();
        auto [b, v] = q.top(); q.pop();
        int id = (int)huff.size();
        huff.push_back({a + b, u, v});
        huff_cost += a + b;
        q.push({a + b, id});
    }
    if (!q.empty()) huff_root = q.top().second;
}
vector<string> huffman_codes(int leaves) {
    vector<string> result(leaves);
    if (huff_root == -1) return result;
    vector<pair<int, string>> todo{{huff_root, ""}};
    while (!todo.empty()) {
        auto [u, code] = todo.back(); todo.pop_back();
        if (huff[u].left == -1) result[u] = code;
        else {
            todo.push_back({huff[u].right, code + '1'});
            todo.push_back({huff[u].left, code + '0'});
        }
    }
    return result;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
