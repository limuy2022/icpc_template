import sys
from math import comb, gcd, isqrt, lcm

if hasattr(sys, "set_int_max_str_digits"):
    sys.set_int_max_str_digits(0)

n = int(sys.stdin.buffer.readline())
print(pow(n, 100, 10**9 + 7))
print(isqrt(n), gcd(n, 12), lcm(n, 12))
print(comb(n, 2))
