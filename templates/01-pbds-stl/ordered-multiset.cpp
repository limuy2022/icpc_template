using Key = std::pair<long long, int>;
OrderedSet<Key> ms;
int next_id = 0;                  // IDs: [0, next_id)
long long x = 7;
int id = next_id++;
ms.insert({x, id});
ms.insert({x, next_id++});

auto lt = ms.order_of_key({x, 0});        // count < x
auto le = ms.order_of_key({x, next_id});  // count <= x
auto eq = le - lt;                        // count == x
auto kth = ms.find_by_order(1);
if (kth != ms.end()) { (void)kth->first; }  // value of the 2nd key
ms.erase({x, id});                       // erase one known copy

auto pos = ms.lower_bound({x, 0});
if (pos != ms.end() && pos->first == x)
    ms.erase(*pos);                       // erase any one copy
