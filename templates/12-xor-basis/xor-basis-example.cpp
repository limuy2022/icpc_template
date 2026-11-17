XorBasis basis;
basis.insert(3); basis.insert(5); basis.insert(6);
// all distinct results: {0,3,5,6}; 3^5^6 == 0
bool can = basis.contains(7);             // false
auto first = basis.kth(0,true);           // 0, from a nonempty subset
auto third = basis.kth(2);                // 5
u64 largest = basis.max_xor();            // 6
