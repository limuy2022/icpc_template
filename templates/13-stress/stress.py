import argparse
from pathlib import Path
import subprocess

from gen import CASES, make_case


def run(program, data, timeout):
    try:
        p = subprocess.run([program], input=data, capture_output=True,
                           timeout=timeout)
        return p.returncode, p.stdout, p.stderr
    except subprocess.TimeoutExpired as e:
        return "timeout", e.stdout or b"", e.stderr or b""


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("candidate", type=Path)
    parser.add_argument("brute", type=Path)
    parser.add_argument("--kind", choices=CASES, default="array")
    parser.add_argument("--seed", type=int, default=0)
    parser.add_argument("--rounds", type=int, default=200)
    parser.add_argument("--timeout", type=float, default=2)
    args = parser.parse_args()
    if args.rounds <= 0 or args.timeout <= 0:
        parser.error("--rounds and --timeout must be positive")

    candidate, brute = args.candidate.resolve(), args.brute.resolve()
    for seed in range(args.seed, args.seed + args.rounds):
        data = make_case(seed, args.kind).encode()
        try:
            a, x, err_x = run(candidate, data, args.timeout)
            b, y, err_y = run(brute, data, args.timeout)
        except OSError as e:
            parser.exit(2, f"{e}\n")
        if a == b == 0 and x.split() == y.split():
            continue
        Path("fail.in").write_bytes(data)
        Path("fail.candidate.out").write_bytes(x)
        Path("fail.brute.out").write_bytes(y)
        Path("fail.candidate.err").write_bytes(err_x)
        Path("fail.brute.err").write_bytes(err_y)
        print(f"FAIL seed={seed} kind={args.kind}: candidate={a}, brute={b}")
        return 1
    print(f"OK {args.rounds} cases")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
