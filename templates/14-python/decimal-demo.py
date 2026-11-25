from decimal import Decimal, ROUND_HALF_EVEN, ROUND_HALF_UP, localcontext
from fractions import Fraction

with localcontext() as ctx:
    ctx.prec = 120
    x = Decimal("2")
    print(f"{x.sqrt():.100f}")
    print(f"{x.ln():.20f}", f"{x.exp():.20f}")

    r = Fraction("2/3")
    val = Decimal(r.numerator) / Decimal(r.denominator)
    print(f"{val:.30f}")

    x, step = Decimal("2.345"), Decimal("0.01")
    print(x.quantize(step, rounding=ROUND_HALF_UP))
    print(x.quantize(step, rounding=ROUND_HALF_EVEN))
