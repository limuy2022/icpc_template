#include <bits/stdc++.h>
using namespace std;
struct Huffman {
    struct Node {
        long long weight;
        int left = -1, right = -1;
    };
    vector<Node> t;
    int root = -1;
    long long cost = 0;
    Huffman(const vector<long long>& weights) {
        using Item = pair<long long, int>;
        priority_queue<Item, vector<Item>, greater<Item>> q;
        for (long long w : weights) {
            int id = (int)t.size();
            t.push_back({w});
            q.push({w, id});
        }
        while (q.size() > 1) {
            auto [a, u] = q.top(); q.pop();
            auto [b, v] = q.top(); q.pop();
            int id = (int)t.size();
            t.push_back({a + b, u, v});
            cost += a + b;
            q.push({a + b, id});
        }
        if (!q.empty()) root = q.top().second;
    }
    vector<string> codes(int leaves) const {
        vector<string> result(leaves);
        if (root == -1) return result;
        vector<pair<int, string>> todo{{root, ""}};
        while (!todo.empty()) {
            auto [u, code] = todo.back(); todo.pop_back();
            if (t[u].left == -1) result[u] = code;
            else {
                todo.push_back({t[u].right, code + '1'});
                todo.push_back({t[u].left, code + '0'});
            }
        }
        return result;
    }
};
