from functools import lru_cache
from itertools import combinations, permutations, product

a = [1, 2, 3]
print(list(permutations(a)))
print(list(combinations(a, 2)))
print(list(product(a, repeat=2)))


@lru_cache(None)
def sg(n):
    seen = {sg(n - d) for d in (1, 3, 4) if d <= n}
    mex = 0
    while mex in seen:
        mex += 1
    return mex


print([sg(n) for n in range(15)])
