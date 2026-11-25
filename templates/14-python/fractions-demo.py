from fractions import Fraction
from math import ceil, floor

a, b = Fraction("0.1"), Fraction(2, 3)
print(a + b, a - b, a * b, a / b)
print(a < b, sorted([b, a, Fraction("1/4")]))

x = Fraction(7, -3)
print(f"{x.numerator}/{x.denominator}")
print(int(x), floor(x), ceil(x), round(x))
print(Fraction("3.14159265").limit_denominator(100))
