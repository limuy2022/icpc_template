import os

from gen import make_case

candidate, brute = "./main", "./brute"

while True:
    with open("fail.in", "w") as f:
        f.write(make_case())
    if (
        os.system(f"{candidate} < fail.in > candidate.out")
        or os.system(f"{brute} < fail.in > brute.out")
        or os.system("diff candidate.out brute.out")
    ):
        print("FAIL")
        break
