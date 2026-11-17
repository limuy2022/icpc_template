OrderedSet<int> s;
s.insert(10);
s.insert(30);
s.insert(20);
s.insert(20);                     // {10, 20, 30}
auto cnt = s.order_of_key(20);    // 1
auto it = s.find_by_order(1);     // points to 20
if (it != s.end()) s.erase(*it);  // {10, 30}
