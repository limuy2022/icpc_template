import os
from pathlib import Path
from time import time_ns

from gen import make_case

candidate, brute = "./main", "./brute"
inp, out, ans = map(Path, ("fail.in", "candidate.out", "brute.out"))

while True:
    seed = time_ns()
    inp.write_text(make_case(seed, "array"))
    if (os.system(f"{candidate} < {inp} > {out}")
        or os.system(f"{brute} < {inp} > {ans}")
        or out.read_bytes().split() != ans.read_bytes().split()):
        print("FAIL seed =", seed)
        raise SystemExit(1)
