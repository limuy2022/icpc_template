import argparse
import random


def array_case(rng):
    n = rng.randint(1, 12)
    a = [rng.randint(-10, 10) for _ in range(n)]
    return f"{n}\n" + " ".join(map(str, a)) + "\n"


def permutation_case(rng):
    n = rng.randint(1, 12)
    a = list(range(1, n + 1))
    rng.shuffle(a)
    return f"{n}\n" + " ".join(map(str, a)) + "\n"


def string_case(rng):
    n = rng.randint(1, 20)
    s = "".join(rng.choice("abc") for _ in range(n))
    return f"{n}\n{s}\n"


def tree_case(rng):
    n = rng.randint(1, 12)
    edges = [(rng.randint(1, v - 1), v) for v in range(2, n + 1)]
    return f"{n}\n" + "".join(f"{u} {v}\n" for u, v in edges)


def graph_case(rng):
    n = rng.randint(1, 12)
    pairs = [(u, v) for u in range(1, n + 1) for v in range(u + 1, n + 1)]
    m = rng.randint(0, len(pairs))
    edges = rng.sample(pairs, m)
    return f"{n} {m}\n" + "".join(f"{u} {v}\n" for u, v in edges)


CASES = dict(array=array_case, perm=permutation_case, string=string_case,
             tree=tree_case, graph=graph_case)


def make_case(seed, kind="array"):
    return CASES[kind](random.Random(seed))


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("seed", type=int)
    parser.add_argument("--kind", choices=CASES, default="array")
    args = parser.parse_args()
    print(make_case(args.seed, args.kind), end="")
