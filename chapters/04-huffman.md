# 贪心算法 · 霍夫曼树 Huffman

用于最优前缀编码和二叉最优合并。每次取两个最小权值合并，将和放回小根堆，直到只剩一个节点。

## 01 · 建树、最小带权路径长度 WPL

```cpp
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
```

## 02 · 使用与边界约定

```cpp
vector<long long> weights{1, 2, 3, 4};
Huffman h(weights);
long long answer = h.cost;             // 19
auto code = h.codes((int)weights.size());
// Leaf IDs [0, n) match the original weight order.
// WPL = sum(weights[i] * code[i].size()) == sum of merge costs
```

- 权值须非负。建树$\mathcal{O}(n \log  n)$、空间$\mathcal{O}(n)$。权值和及 cost 均须能用 long long 表示。
- 空输入 root = -1、cost = 0。仅一个叶子时深度为 0，模板给空编码、cost = 0。
- 若题目要求每个符号至少编码一位，单叶子时改成 "0"，编码总长为该符号频次；合并代价仍为 0。
- 相同权值可能产生多种最优树。左右子树的 0/1 分配可互换。生成全部编码的时间和空间随编码总长度增长。
- 仅求最优合并代价时可只存权值，省掉节点和 codes。k 叉合并（$n \geq  1$、$k \geq  2$）先补零，使 (n-1) 能被 (k-1) 整除，再每次合并 k 个最小权值。
