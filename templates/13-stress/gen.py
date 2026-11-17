import sys
from random import choice, randint, sample, shuffle


def array_case():
    n = randint(1, 12)
    a = [randint(-10, 10) for _ in range(n)]
    return f"{n}\n" + " ".join(map(str, a)) + "\n"


def permutation_case():
    n = randint(1, 12)
    a = list(range(1, n + 1))
    shuffle(a)
    return f"{n}\n" + " ".join(map(str, a)) + "\n"


def string_case():
    n = randint(1, 20)
    s = "".join(choice("abc") for _ in range(n))
    return f"{n}\n{s}\n"


def tree_case():
    n = randint(1, 12)
    edges = [(randint(1, v - 1), v) for v in range(2, n + 1)]
    return f"{n}\n" + "".join(f"{u} {v}\n" for u, v in edges)


def graph_case():
    n = randint(1, 12)
    pairs = [(u, v) for u in range(1, n + 1) for v in range(u + 1, n + 1)]
    m = randint(0, len(pairs))
    edges = sample(pairs, m)
    return f"{n} {m}\n" + "".join(f"{u} {v}\n" for u, v in edges)


CASES = dict(array=array_case, perm=permutation_case, string=string_case,
             tree=tree_case, graph=graph_case)


def make_case(kind="array"):
    return CASES[kind]()


if __name__ == "__main__":
    print(make_case(*sys.argv[1:]), end="")
