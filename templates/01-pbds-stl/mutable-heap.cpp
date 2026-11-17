#include <functional>
#include <utility>
#include <ext/pb_ds/priority_queue.hpp>

using Item = std::pair<long long, int>;  // (distance, vertex)
using Heap = __gnu_pbds::priority_queue<
    Item, std::greater<Item>, __gnu_pbds::pairing_heap_tag>;
