vector<int> a{3, 1, 2};
long long sum = accumulate(a.begin(), a.end(), 0LL);
long long pre[4]{};
for (size_t i = 0; i < a.size(); ++i) pre[i + 1] = pre[i] + a[i];
// sum on [l, r) = pre[r] - pre[l]
int ids[3];
iota(ids, ids+3, 0);               // 0, 1, 2
do { /* use ids */ } while (next_permutation(ids, ids+3));
reverse(a.begin(), a.end());
rotate(a.begin(), a.begin() + 1, a.end());     // left by 1
a.erase(remove_if(a.begin(), a.end(), [](int x){return x < 2;}),
        a.end());
