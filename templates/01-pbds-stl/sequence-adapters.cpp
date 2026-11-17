deque<int> d; d.push_front(1); d.push_back(2);
d.pop_front(); d.pop_back();
stack<int> st; st.push(1); (void)st.top(); st.pop();
queue<int> q; q.push(1); (void)q.front(); q.pop();
priority_queue<int> mx;               // max heap
priority_queue<int, vector<int>, greater<int>> mn;
mn.push(3); mn.push(1);
int x = mn.top(); mn.pop();           // x = 1
bitset<128> b; b.set(3); b.flip(7); b.reset(3);
auto k = b.count();                   // 1
