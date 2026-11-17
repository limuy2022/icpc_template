Heap q, moved;
q.push({3, 1});
q.push({8, 2});
q.split([](const Item& item) { return item.first > 5; }, moved);
// q contains (3, 1); moved contains (8, 2)
