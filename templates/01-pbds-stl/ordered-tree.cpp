#include <functional>
#include <utility>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

namespace pb = __gnu_pbds;

template<class Key>
using OrderedSet = pb::tree<Key, pb::null_type, std::less<Key>,
                            pb::rb_tree_tag,
                            pb::tree_order_statistics_node_update>;
