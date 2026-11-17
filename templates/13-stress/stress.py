import os

from gen import make_case

candidate, brute = "./main", "./brute"

while True:
    with open("fail.in", "w") as f:
        f.write(make_case("array"))
    if (os.system(f"{candidate} < fail.in > candidate.out")
        or os.system(f"{brute} < fail.in > brute.out")
        or open("candidate.out", "rb").read().split()
        != open("brute.out", "rb").read().split()):
        raise SystemExit("FAIL: fail.in")
