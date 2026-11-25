# Python · 提交速查：大整数、读入与递归

适合用标准库省掉高精度、解析或约分的实现。先按最大数据估算循环次数和内存。

## 01 · 大整数与常用数学函数

[Python 源码](../templates/14-python/integers.py)

- int 支持任意精度整数和位运算，运算成本随位数增加。`//` 向下取整，`/` 返回浮点数。
- `pow(a, b, m)` 直接算模幂。`pow(a, -1, m)` 求模逆，要求 a 与 m 互质（3.8+）。
- `isqrt(n)` 返回非负整数 n 的平方根向下取整。`comb(n,k)` 返回精确组合数。两者需 3.8+，`lcm` 需 3.9+。
- 部分解释器限制超长十进制字符串与整数互转。大数题用示例里的 `hasattr` 检查后关闭限制，兼容无此接口的版本。

## 02 · 批量读入与递归

```python
import sys

it = iter(sys.stdin.buffer.read().split())
n = int(next(it))
a = [int(next(it)) for _ in range(n)]
```

- 批量读入适合空白分隔的离线输入。输入很大时分行读，保留行边界时用 `sys.stdin.readline()`。交互题逐次读写并刷新输出。
- DFS 或记忆化搜索确实需要深递归时，可按深度设置 `sys.setrecursionlimit(200000)`。这只提高解释器限制，深链优先用显式栈。

# Python · 日期与时间模拟

## 03 · 日期差、闰年、月末、星期几

[Python 源码](../templates/14-python/parse-date.py)

- `weekday()` 从周一 0 到周日 6，`isoweekday()` 从周一 1 到周日 7。求下一个指定星期时，允许当天就用偏移量，严格在将来则用 `offset or 7`。
- 日期相减得到有符号天数。若起止日都算且起日不晚于止日，用 `(end - start).days + 1`。天数和星期计算可直接用序号，无需逐天模拟。
- 按月推进要先确认题目如何处理无效日期。示例把 31 号落到目标月最后一天。每月按原定日期重复时，要保留原定日号。
- `date` 按公历计算，年份范围 1 到 9999。非法日期会触发 `ValueError`，形状正确也可能遇到 2 月 30 日。

## 04 · 时间进位、午夜与持续时间

[Python 源码](../templates/14-python/time-demo.py)

- 格式串中 `%m` 是月份，`%M` 是分钟。跨日加减用 `datetime + timedelta`，或用 `divmod(total, 1440)` 把总分钟拆成天数与当天分钟。
- 算完整时间差要包含天数。只取 `delta.seconds` 得到的是归一化后的秒数部分。示例用整数算总秒数。

# Python · 文本与表达式解析

## 05 · 文本与表达式

[Python 源码](../templates/14-python/parse-text.py)

- `split(sep, 1)` 只切一次，切片用 `s[l:r]`，`re.fullmatch` 检查整段格式，捕获组可直接拆出字段。
- `eval` 会执行 Python 表达式。只有表达式来源可信且题目的运算规则与 Python 一致时才适用。普通输入按题意解析。

# Python · 精确分数（Fraction）

## 06 · 构造、四则运算、比较与输出

[Python 源码](../templates/14-python/fractions-demo.py)

- 支持两个整数构造、分数字符串和十进制字符串。结果自动约分，分母恒为正。构造时分母和除法的除数都须非零。
- 十进制输入用 `Fraction("0.1")`。如果先用 float 读入，Fraction 会精确保存那个已舍入的二进制值。运算时也尽量只混用 int。
- `int(x)` 向零截断，`floor(x)` 向下取整，`ceil(x)` 向上取整。`round(x)` 的正中间值取偶数。
- `limit_denominator(n)` 求分母不超过 n 的近似分数。题目要求精确值时直接输出分子和分母。
- 适合小规模分数迭代、精确几何和有理数高斯消元。留意中间分子分母的位数增长。

# Python · 高精度十进制（Decimal）

## 07 · 有效数字、数学函数与舍入

[Python 源码](../templates/14-python/decimal-demo.py)

- 从字符串或整数构造 Decimal。示例也演示了把精确分数的分子和分母分别转成 Decimal 后相除。除不尽时按当前精度舍入。
- `prec` 是运算的有效数字数目。按整数位数和目标小数位数预留额外精度。`localcontext()` 退出时恢复原设置。
- `quantize(Decimal("0.01"))` 量化到两位小数。`ROUND_HALF_UP` 在正中间时远离零，`ROUND_HALF_EVEN` 取末位为偶数的结果。
- `format(x, "f")` 用普通十进制输出。`f"{x:.100f}"` 固定 100 位小数。若题目指定舍入方式，先用 quantize 显式处理。
- `quantize` 后的有效位数超过 prec 会触发 InvalidOperation。上下文精度要容纳舍入进位后的整数位和尾随零。

# Python · 本地打表找规律

[Python 源码](../templates/14-python/enumerate-sg.py)

- `permutations(a)` 枚举全排列，`combinations(a,k)` 枚举 k 元组合，`product(a,repeat=k)` 枚举可重复取值的 k 元组。
- `@lru_cache(None)` 缓存递归结果，参数须可哈希。多组数据更换规则后清缓存。
- SG 示例每次减 1、3 或 4，打出前 15 项。按题目换转移，用小规模结果猜规律，再证明后提交。
