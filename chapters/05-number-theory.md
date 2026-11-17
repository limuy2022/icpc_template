# 数论 · 范围与速查索引

按 OI Wiki 数学目录的“数论”分组整理，覆盖 33 个目录条目。以下为独立编写的赛场提要，保留定义、关键公式、算法路线与常用模板。

## 基础与同余（第 13 至 23 页）

- 数论基础、模算术简介、最大公约数、裴蜀定理与一次不定方程。
- 素数、欧拉函数、筛法、分解质因数。
- 费马小定理与欧拉定理、模逆元、线性同余方程、中国剩余定理。
- 升幂引理、阶乘取模、卢卡斯定理、一般同余方程。
- 二次剩余、阶与原根、离散对数、高次剩余与单位根。

## 数论函数与求和（第 24 至 29 页）

- 数论分块、狄利克雷卷积、莫比乌斯反演。
- 狄利克雷双曲线法与杜教筛、素数计数。
- Powerful Number 筛、`Min_25` 筛、洲阁筛。

## 有理逼近与代数方程（第 30 至 33 页）

- 类欧几里得算法、连分数、Stern-Brocot 树与 Farey 序列。
- 二次域、Pell 方程。

## 模板共用约定

- GNU C++17，使用 <bits/stdc++.h> 与 using namespace std。超过内置整数范围时须自行实现高精度。
- 下文 ll 为 long long，i128 为 `__int128_t`，u64 为 unsigned long long，u128 为 `__uint128_t`。
- 除特别说明，模数$m \geq  1$。乘法和求和要先提升类型，再运算。整数除法使用非负数或显式向下取整。
- i128 的有效范围仍有限。CRT 模数乘积、Pell 解和几何叉积均要按题目范围选择类型。
- 基础函数 norm、`mul_mod`、`pow_mod`、exgcd、inverse 被后续模板复用，录入时一起带上。

## 如何选算法

- 连续小范围预处理用筛法。单个 64 位整数判素数用 Miller-Rabin，分解用 Pollard Rho。
- 线性方程先看 gcd 整除条件。合并多个余数用扩展 CRT。幂指数的同余先检查底数是否互素。
- 函数前缀和先找卷积恒等式，再选杜教筛；素数点值可筛时考虑 `Min_25`；能消去一次质因子时考虑 PN 筛。
- 固定模数的大量乘法可用 Barrett 或 Montgomery。仅少量 64 位模乘时，u128 更便于核对。

来源范围按 OI Wiki Team 的数论目录核对，整理日期 2026-11-05。公式与模板按赛场用途重新组织。高阶筛法页包含算法递推说明，按题目定制状态与模数。前 1 至 11 页为 PBDS、STL、字符串与霍夫曼树。

# 数论 · 基础、模算术与共用函数

## 整除、分解与取整

- a | b 表示 b = ak。正整数唯一分解 $n=\prod_{p\mid n}p^{e_p}$。约数个数 $\tau(n)=\prod_{p\mid n}(e_p+1)$，约数和 $\sigma(n)=\prod_{p\mid n}(1+p+\cdots+p^{e_p})$。
- $\gcd(a,b)\operatorname{lcm}(a,b)=ab$。求 lcm 先除 gcd 再乘，0 与任意数的 lcm 为 0。
- 对整数 a、b，b>0，$\left\lfloor a/b\right\rfloor$与 C++ 向零截断不同。$\left\lceil\frac{a}{b}\right\rceil=-\left\lfloor-\frac{a}{b}\right\rfloor$。
- gcd(a,b)=1 时才可在模 b 下消去 a。模数为合数时，非零元素可能没有逆元。

## 可复用的模乘、快速幂、扩展 gcd

```cpp
using ll = long long;
using i128 = __int128_t;
using u64 = unsigned long long;
using u128 = __uint128_t;
ll norm(i128 x, ll m) {                 // m >= 1
    x %= m;
    if (x < 0) x += m;
    return (ll)x;
}
ll mul_mod(ll a, ll b, ll m) {
    return norm((i128)a * b, m);
}
ll pow_mod(ll a, u64 e, ll m) {
    ll ans = 1 % m;
    a = norm(a, m);
    for (; e; e >>= 1, a = mul_mod(a, a, m))
        if (e & 1) ans = mul_mod(ans, a, m);
    return ans;
}
i128 exgcd(i128 a, i128 b, i128& x, i128& y) { // a,b >= 0
    if (!b) { x = 1; y = 0; return a; }
    i128 u, v;
    i128 g = exgcd(b, a % b, u, v);
    x = v; y = u - (a / b) * v;
    return g;
}
ll inverse(ll a, ll m) {                 // -1 means not invertible
    if (m == 1) return 0;
    i128 x, y;
    if (exgcd(norm(a, m), m, x, y) != 1) return -1;
    return norm(x, m);
}
i128 floor_div(i128 a, i128 b) {         // b > 0
    i128 q = a / b;
    return q - (a % b < 0);
}
```

- 模$2^w$可用 w 位无符号溢出实现。溢出不等于模任意 m，带符号溢出是未定义行为。
- 加倍法模乘逐位处理乘数，$\mathcal{O}(\log  m)$。加法也须防溢出，不能直接让 a+a 溢出。
- Barrett 用预存的近似倒数估计商，再修正余数。Montgomery 取 $R=2^w$，要求 gcd(R,m)=1，用 aR mod m 表示数。
- Montgomery 的 REDC(T) 取 $q\equiv -T m^{-1}\; (\mathrm{mod}\ R)$，再算 $\frac{T+qm}{R}$ 并修正。进入、退出表示域各需转换。

# 数论 · gcd、裴蜀与线性方程

## 最大公约数与互素结构

- 欧几里得 $\gcd(a,b)=\gcd(b,a\;\mathrm{mod}\; b)$，$\mathcal{O}(\log  \min (a,b))$。gcd(0,b)=|b|，gcd(0,0)=0。
- gcd(a,b,c)=gcd(gcd(a,b),c)。若 g=gcd(a,b)，则 $\gcd\left(\frac{a}{g},\frac{b}{g}\right)=1$。
- 二进制 gcd（Stein）先提取公共 2 因子，再将较大奇数减去较小奇数并去掉 2 因子。ctz 的参数必须非零。
- 等差差分性质 gcd(a,b)=gcd(a,b-a)。区间加与区间 gcd 可转成维护差分。

## ax + by = c

- 当 a、b 不全为 0 时，有整数解当且仅当 g=gcd(|a|,|b|) 整除 c。
- exgcd 得 au+bv=g，特解 $x_0=u\frac{c}{g}$、$y_0=v\frac{c}{g}$。原系数为负时相应调整 u 或 v 的符号。
- 通解 $x=x_0+\frac{b}{g} t$，$y=y_0-\frac{a}{g} t$，t∈Z。先分离 a=0 或 b=0，再对 t 施加区间限制。
- 系数全为 0 时，c=0 则所有整数对都是解，否则无解。
- 设 A>0，要让 $L\leq x_0+At\leq R$，可得 $\left\lceil\frac{L-x_0}{A}\right\rceil\leq t\leq\left\lfloor\frac{R-x_0}{A}\right\rfloor$。A<0 时反向处理。
- 多变量方程 $\sum_{i=1}^n a_i x_i=c$ 的可解条件为 $\gcd(a_1,\ldots,a_n)\mid c$，可逐步用裴蜀合并。

## 非负整数与硬币

- a、b>0 且互素，最大不能写成 ax+by（$x,y\geq 0$）的非负整数是 ab-a-b。
- 正整数不可表示的个数为 $\frac{(a-1)(b-1)}{2}$。a=1 或 b=1 时所有非负整数均可表示。
- 三种及以上面值没有同样简单的闭式公式，可按最小面值取模建图求同余最短路。

## $ax\equiv b\; (\mathrm{mod}\ m)$

- g=gcd(a,m)。若$g\nmid b$则无解，否则除以 g，在模 m/g 下求逆。
- 令 t=m/g，$x_0=\left(\frac{a}{g}\right)^{-1}\frac{b}{g}\;\mathrm{mod}\; t$。模 m 恰有 g 个解，$x=x_0+kt$，$0\leq k<g$。

```cpp
optional<pair<ll, ll>> linear_congruence(ll a, ll b, ll m) {
    a = norm(a, m); b = norm(b, m);
    ll g = gcd(a, m);
    if (b % g) return nullopt;
    ll period = m / g;
    ll x = mul_mod(b / g, inverse(a / g, period), period);
    return pair<ll, ll>{x, period};       // x + k*period
}
```

- 素数 p 下，$a\nequiv 0$时 $a^{-1}\equiv a^{p-2}\; (\mathrm{mod}\ p)$。一般模数用 exgcd。
- p 为素数，$1\leq i<p$，inv[1]=1，inv[i]=-(p/i)·inv[p%i] mod p，可线性求一段逆元。
- 一组全可逆元素可先求前缀积，仅求总积一次逆元，再反向推出每项逆元。含 0 或非单位时须先处理。

# 数论 · 素数与 Miller-Rabin

## 素性测试、反素数

- n<2 均非素数。试除至$d\leq n/d$，单次$\mathcal{O}(\sqrt{n})$。费马测试可能被 Carmichael 数骗过。
- Miller-Rabin 将 n-1 写成$d\cdot 2^s$，d 为奇数。对底 a，先求$x=a^d$。x=1 或 -1 则通过，否则反复平方寻找 -1。
- 下列 7 个底配合精确模乘，覆盖整个无符号 64 位范围。判素数为确定性结果。

```cpp
u64 mul64(u64 a, u64 b, u64 m) { return (u128)a * b % m; }
u64 pow64(u64 a, u64 e, u64 m) {
    u64 r = 1 % m;
    for (; e; e >>= 1, a = mul64(a, a, m))
        if (e & 1) r = mul64(r, a, m);
    return r;
}
bool is_prime(u64 n) {
    if (n < 2) return false;
    for (u64 p : {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,
                  23ULL,29ULL,31ULL,37ULL}) {
        if (n % p == 0) return n == p;
    }
    u64 d = n - 1;
    int s = __builtin_ctzll(d);
    d >>= s;
    for (u64 a : {2ULL,325ULL,9375ULL,28178ULL,450775ULL,
                  9780504ULL,1795265022ULL}) {
        if (a % n == 0) continue;
        u64 x = pow64(a % n, d, n);
        if (x == 1 || x == n - 1) continue;
        bool pass = false;
        for (int r = 1; r < s; ++r) {
            x = mul64(x, x, n);
            if (x == n - 1) { pass = true; break; }
        }
        if (!pass) return false;
    }
    return true;
}
```

- 反素数是约数个数严格超过所有更小正整数的数。搜索$n\leq N$内约数最多的最小数时，连续使用最小的质数。
- 反素数分解中的指数非增，$e_2\geq e_3\geq e_5\geq\cdots$。DFS 时枚举下一质数的指数不超过上一个指数，乘前用 N/p 判界。
- 若只要数值最小，约数数目相同时保留较小的 n。1 的约数数目为 1。

# 数论 · 分解质因数 Pollard Rho

## 64 位分解模板

复用上一页的 `mul64`、`pow64`、`is_prime`。输入$n\geq 1$，返回带重数的素因子，之后排序统计指数。

```cpp
mt19937_64 nt_rng(chrono::steady_clock::now().time_since_epoch().count());
u64 rho(u64 n) {                         // n is composite
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    while (true) {
        u64 c = nt_rng() % (n - 1) + 1;
        u64 x = nt_rng() % (n - 2) + 2, y = x, d = 1;
        auto f = [&](u64 v) {
            return (u64)(((u128)mul64(v, v, n) + c) % n);
        };
        while (d == 1) {
            x = f(x); y = f(f(y));
            u64 diff = x > y ? x - y : y - x;
            d = gcd(diff, n);
        }
        if (d != n) return d;
    }
}
void factor(u64 n, vector<u64>& out) {
    if (n == 1) return;
    if (is_prime(n)) { out.push_back(n); return; }
    u64 d = rho(n);
    factor(d, out); factor(n / d, out);
}
```

- Floyd 每次移动一步与两步，借助 gcd(|x-y|,n) 暴露非平凡因子。gcd=n 时换随机参数重来。
- Pollard Rho 是随机算法。找到最小素因子 p 的常见期望步数为$\mathcal{O}(\sqrt{p})$，半素数常写成期望$\mathcal{O}(n^{1/4})$，不保证最坏运行时间。
- Brent 可批量相乘若干 |x-y| 后求 gcd，减少 gcd 次数。批次积为 0 导致 gcd=n 时，回放该批次逐项恢复因子。
- 试除分解时，循环终止条件随剩余 n 缩小；结束后 n>1 仍是一个素因子。

## 由分解恢复常用函数

- $n=\prod_{p\mid n}p^{e_p}$，$\varphi(n)=n\prod_{p\mid n}\left(1-\frac{1}{p}\right)$，$\mu (n)=0$（某个$e\geq 2$），否则 $\mu(n)=(-1)^{\omega(n)}$。
- $\tau(n)=\prod_{p\mid n}(e_p+1)$，$\sigma_k(n)=\prod_{p\mid n}\sum_{j=0}^{e_p}p^{kj}$。模数不保证可逆时用循环累加几何级数。
- 枚举约数从 {1} 开始，对每个$p^e$将已有约数分别乘$p^0,\ldots ,p^e$。约数个数可能很多。
- 1 的质因子列表为空，$\varphi (1)=\mu (1)=τ(1)=σ(1)=1$。0 没有有限的通常质因子分解。

# 数论 · 欧拉函数、筛法与积性函数

## $\varphi $、$\mu $的线性筛

```cpp
struct NumberSieve {
    static constexpr int MAX_N = 1000000;
    int lp[MAX_N+1]{}, phi[MAX_N+1]{}, mu[MAX_N+1]{};
    vector<int> primes;
    int limit;
    NumberSieve(int n) : limit(n) {
        if (n >= 1) phi[1] = mu[1] = 1;
        for (int i = 2; i <= n; ++i) {
            if (!lp[i]) {
                lp[i] = i; primes.push_back(i);
                phi[i] = i - 1; mu[i] = -1;
            }
            for (int p : primes) {
                if (p > n / i) break;
                int v = i * p;
                lp[v] = p;
                if (i % p == 0) {
                    phi[v] = phi[i] * p; mu[v] = 0;
                    break;
                }
                phi[v] = phi[i] * (p - 1); mu[v] = -mu[i];
            }
        }
    }
};
```

- n 不超过 MAX_N，筛表对象放全局或声明为 static。
- Eratosthenes 从$p^{2}$开始标记，$\mathcal{O}(N \log  \log  N)$。线性筛每个合数由其最小素因子产生一次，$\mathcal{O}(N)$。
- 分段筛 [L,R] 先筛到$\sqrt{R}$，对每个 p 从$\max (p^{2},\left\lceil L/p\right\rceilp)$标记。单独排除 0、1，不能把 p 自身筛掉。
- 只筛奇数可省空间。bitset/位压缩适合只需要素性。lp 数组支持$\mathcal{O}(\log  n)$次除法分解范围内的数。

## 欧拉函数与常见递推

- $\varphi(p^e)=p^{e-1}(p-1)$。互素时$\varphi (ab)=\varphi (a)\varphi (b)$。一般有 $\varphi(ab)=\varphi(a)\varphi(b)\frac{g}{\varphi(g)}$，g=gcd(a,b)。
- $\sum_{d\mid n}\varphi(d)=n$。区间$1\leq a,b\leq n$的有序互素对个数为 $2\sum_{i=1}^{n}\varphi(i)-1$。
- 积性函数 f 满足 f(1)=1，gcd(a,b)=1 时 f(ab)=f(a)f(b)。完全积性不要求互素，例如$n^k$。
- 加性函数在互素乘积上变为加法，例如不同素因子个数$\omega (n)$。$\Omega (n)$对任意乘积均可加。

## 线性筛扩充 τ、σ

- 维护 i 的最小质因子 p 的指数 e，以及去掉 p 因子的部分 r。
- 若新质数$q\nmid i$，则 τ(iq)=2τ(i)，σ(iq)=(q+1)σ(i)。
- 若 q=p，则 $\tau(iq)=\frac{\tau(i)}{e+1}(e+2)$，$\sigma(iq)=\sigma(r)(1+p+\cdots+p^{e+1})$。
- 除法在整数域先做精确除法；取模时不要把不可逆的 (e+1) 或 (p-1) 直接求逆。

# 数论 · 欧拉定理、阶与原根

## 指数缩减

- 素数 p 下$a^p\equiv a$，且$p\nmid a$时$a^{p-1}\equiv 1$。一般 m 下仅在 gcd(a,m)=1 时有 $a^{\varphi(m)}\equiv1\; (\mathrm{mod}\ m)$。
- 扩展欧拉定理，$e\geq \varphi (m)$时，$a^e\equiv a^{(e\;\mathrm{mod}\;\varphi(m))+\varphi(m)}\; (\mathrm{mod}\ m)$。$e<\varphi (m)$时用原指数。
- 幂塔递归要同时维护“实际指数是否达到$\varphi (m)$”的标记。仅有取模值无法判断能否加$\varphi (m)$。
- m=1 的答案为 0，指数 0 按空乘积处理。$0^0$的题目定义需要单独确认。

## 阶与原根

- $\operatorname{ord}_m(a)$ 是使$a^k\equiv 1$的最小正 k，仅对 gcd(a,m)=1 定义，且 $\operatorname{ord}_m(a)\mid\varphi(m)$。
- 已知$\varphi (m)$的质因子后，从$k=\varphi (m)$开始，若$a^{k/q}\equiv 1$就不断令 k/=q。
- 有原根的正整数模数恰为 1、2、4、$p^k$、$2p^k$，其中 p 为奇素数；m=1 常单独按平凡群处理。
- g 为原根当且仅当对$\varphi (m)$的每个不同素因子 q，都有$g^{\varphi (m)/q}\nequiv 1$。原根个数为$\varphi (\varphi (m))$。

```cpp
ll multiplicative_order(ll a, ll m, ll phi_m,
                        const vector<ll>& prime_divisors) {
    if (gcd(a, m) != 1) return -1;
    ll order = phi_m;
    for (ll q : prime_divisors)
        while (order % q == 0 && pow_mod(a, order/q, m) == 1)
            order /= q;
    return order;
}
```

## Carmichael 函数

- $\lambda (m)$是所有可逆元素阶的最小公倍数。可用$\lambda (m)$替代$\varphi (m)$缩减互素底数的指数。
- $\lambda (p^e)=\varphi (p^e)$对奇素数成立。$\lambda (2)=1$，$\lambda (4)=2$，$e\geq 3$时 $\lambda(2^e)=2^{e-2}$。
- $\lambda\left(\prod_{p\mid m}p^{e_p}\right)=\operatorname{lcm}_{p\mid m}\lambda(p^{e_p})$。Carmichael 合数无平方因子，且每个 p|n 都满足 p-1|n-1。
- $\operatorname{ord}(a^k)=\frac{\operatorname{ord}(a)}{\gcd(\operatorname{ord}(a),k)}$。互素的两个阶对应元素相乘后，阶等于这两个阶的乘积。

# 数论 · 中国剩余定理与 Garner

## 互素 CRT

- $x\equiv a_i\; (\mathrm{mod}\ m_i)$，模数两两互素时唯一解模 $M=\prod_i m_i$。
- 设 $M_i=\frac{M}{m_i}$，$x\equiv\sum_i a_iM_i\left(M_i^{-1}\;\mathrm{mod}\; m_i\right)\; (\mathrm{mod}\ M)$。中间乘积需宽整数。

## 扩展 CRT，逐个合并任意正模数

当前$x\equiv r \;\mathrm{mod}\; M$，新方程$x\equiv a \;\mathrm{mod}\; m$。令 g=gcd(M,m)，有解当且仅当 g|(a-r)。令 x=r+Mt，解 $\frac{M}{g} t\equiv\frac{a-r}{g}\; (\mathrm{mod}\ m/g)$。

```cpp
// Uses i128 internally; every combined modulus must fit in long long.
optional<pair<ll,ll>> crt_merge(ll r, ll M, ll a, ll m) {
    r = norm(r, M); a = norm(a, m);
    i128 x, y;
    ll g = (ll)exgcd(M, m, x, y);
    i128 diff = (i128)a - r;
    if (diff % g) return nullopt;
    ll q = m / g;
    ll step = norm((diff / g) * x, q);
    i128 mod = (i128)M * q;
    ll value = norm((i128)r + (i128)M * step, (ll)mod);
    return pair<ll,ll>{value, (ll)mod};
}
```

- 初始 r=0、M=1。空方程组表示全部整数。最终周期为$\mathrm{lcm}(m_i)$，不一定是模数乘积。
- $m_i=1$不增加限制。不允许模数为 0；“x 等于某值”的约束须另行处理。
- 使用 i128 作结果时，还要确保所有中间乘积在 i128 范围内。范围更大时须自行实现高精度。

## Garner 混合进制

- 模数两两互素时，写 $x=c_0+c_1m_0+c_2m_0m_1+\cdots$，在各个模数下逐步确定$c_i$。
- 求$c_i$时，用前缀积在$m_i$下的逆元消去系数，$c_i$规范到$[0,m_i)$。
- 若只要 x mod P，可将同样的加乘同步在 P 下执行，避免显式构造巨大 M。P 与其他模数不必互素，因为不对 P 求逆。
- Garner 不能直接接受任意非互素方程组。先用扩展 CRT 或将相容约束整理成互素形式。

# 数论 · 升幂引理与阶乘取模

## p-adic 赋值与 LTE

$v_p(n)$是 p 在非零整数 n 中的指数。$v_p(ab)=v_p(a)+v_p(b)$，$v_p(a+b)\geq\min\{v_p(a),v_p(b)\}$，两侧赋值不同时取等。

- p 为奇素数，p|(a-b)、$p\nmid ab$，$n\geq 1$，则 $v_p(a^n-b^n)=v_p(a-b)+v_p(n)$。
- p 为奇素数，p|(a+b)、$p\nmid ab$，n 为正奇数，则 $v_p(a^n+b^n)=v_p(a+b)+v_p(n)$。
- a、b 均奇，n 为偶数，$v_2(a^n-b^n)=v_2(a-b)+v_2(a+b)+v_2(n)-1$。
- a、b 均奇，n 为奇数，$v_2(a^n-b^n)=v_2(a-b)$。偶 n 时$a^n+b^n\equiv 2 \;\mathrm{mod}\; 8$，赋值为 1。
- LTE 不处理$a^n-b^n=0$的有限赋值，先排除零。使用前逐项检查$p\nmid ab$和奇偶条件。

## Wilson、Legendre、Kummer

- p 为素数，$(p-1)!\equiv -1 \;\mathrm{mod}\; p$。$n\geq p$时$n!\equiv 0 \;\mathrm{mod}\; p$，但“去掉 p 因子后的阶乘”通常非零。
- $v_p(n!)=\sum_{k\geq1}\left\lfloor\frac{n}{p^k}\right\rfloor$，循环令 n/=p 累加即可，不要显式计算溢出的$p^k$。
- $v_p\binom{n}{k}=v_p(n!)-v_p(k!)-v_p((n-k)!)$，也是 p 进制相加 k+(n-k) 的进位次数。
- 模 p 的去 p 阶乘，按长度 p 的块乘积 -1 递归。素数幂模下整块乘积必须按$p^q$计算。

## 模素数的组合数与 Lucas

- 当$0\leq n<p$，fac[n]、ifac[n] 预处理后 C(n,k)=fac[n]ifac[k]ifac[n-k] mod p。
- 一般 n,k 写成 p 进制，$\binom{n}{k}\equiv\prod_i\binom{n_i}{k_i}\; (\mathrm{mod}\ p)$。任一位$k_i>n_i$则答案为 0。

```cpp
// fac/ifac have indices 0..p-1; p is prime and small enough to store.
ll lucas(u64 n, u64 k, ll p, const vector<ll>& fac,
         const vector<ll>& ifac) {
    ll ans = 1;
    while (n || k) {
        ll a = n % p, b = k % p;
        if (b > a) return 0;
        ans = mul_mod(ans, mul_mod(fac[a],
              mul_mod(ifac[b], ifac[a-b], p), p), p);
        n /= p; k /= p;
    }
    return ans;
}
```

- 当 p 很大，不能为 Lucas 直接开 p 个元素。单个小位组合数可用乘法式，但耗时依赖$\min (k_i,n_i-k_i)$。
- Lucas 要求模数为素数。复合模数使用下一页的素数幂方案，再 CRT。

# 数论 · 素数幂阶乘与 exLucas

## $C(n,k) \;\mathrm{mod}\; p^q$

令 $P=p^q$，$E(n)=v_p(n!)$，$U(n)=\frac{n!}{p^{E(n)}}\;\mathrm{mod}\; P$。设 $\mathrm{pref}[t]=\prod_{1\leq i\leq t,\,p\nmid i}i\;\mathrm{mod}\; P$，则 $U(n)\equiv U\left(\left\lfloor\frac{n}{p}\right\rfloor\right)\mathrm{pref}[P]^{\lfloor n/P\rfloor}\mathrm{pref}[n\;\mathrm{mod}\; P]\; (\mathrm{mod}\ P)$。

```cpp
struct PrimePowerBinom {
    ll p, mod;
    vector<ll> pref;
    PrimePowerBinom(ll prime, ll power) : p(prime), mod(power),
                                          pref(power+1, 1) {
        for (ll i = 1; i <= mod; ++i)
            pref[i] = mul_mod(pref[i-1], i % p ? i : 1, mod);
    }
    u64 valuation(u64 n) const {
        u64 e = 0;
        while (n) { n /= p; e += n; }
        return e;
    }
    ll unit_factorial(u64 n) const {
        ll ans = 1;
        while (n) {
            ans = mul_mod(ans, pow_mod(pref[mod], n/mod, mod), mod);
            ans = mul_mod(ans, pref[n%mod], mod);
            n /= p;
        }
        return ans;
    }
    ll choose(u64 n, u64 k) const {
        if (k > n) return 0;
        u64 e = valuation(n)-valuation(k)-valuation(n-k);
        ll pe = pow_mod(p, e, mod);
        if (pe == 0) return 0;
        ll num = unit_factorial(n);
        ll den = mul_mod(unit_factorial(k), unit_factorial(n-k), mod);
        return mul_mod(mul_mod(num, inverse(den, mod), mod), pe, mod);
    }
};
```

- p 必须为素数，mod 必须是它的正整数次幂，且 mod 的大小允许$\mathcal{O}(\;\mathrm{mod}\;)$预处理和存储。
- 原阶乘可能含 p 因子，不能对原阶乘直接求逆。去 p 后的分母与 mod 互素，可求逆。
- 若指数$e\geq q$，$p^e \;\mathrm{mod}\; P=0$，组合数为 0。否则先算单位部分，再乘回$p^e$。
- 模数 $m=\prod_i p_i^{q_i}$，分别计算各素数幂模下结果，再用 CRT 合并。这是 exLucas 的基本路线。
- q=1 时可转 Lucas；本模板仍可用。m=1 时直接返回 0。

## 高阶阶乘技巧

- 模素数 p，去 p 阶乘的块积只可能为 ±1，可将递归迭代到$\mathcal{O}(\log _p n)$个块。
- 模$p^q$的单位块积，奇素数时为 -1；p=2、$q\geq 3$时为 1。通用实现直接用 pref[P]。
- 一般整数 n 的所有可逆剩余类乘积取决于单位群中的 2 阶元素，不能把 Wilson 无条件推广到合数模。

# 数论 · 同余方程、二次剩余与开方

## 多项式同余与 Hensel 提升

- 模合数先分解成素数幂，分别求根再 CRT。模素数 p 的非零 d 次多项式最多 d 个根，模合数不适用此结论。
- 模 p 的函数值可按$x^p=x$降次，即对$x^p-x$取多项式余数；不能把所有指数一律对 p-1 取模而忽略 x=0。
- 已知$f(r)\equiv 0 \;\mathrm{mod}\; p^e$，设新根$r+t\cdot p^e$。求 $f'(r)t\equiv-\frac{f(r)}{p^e}\; (\mathrm{mod}\ p)$。
- $f'(r)\nequiv 0$时唯一提升；导数为 0 且$f(r)\equiv 0 \;\mathrm{mod}\; p^{e+1}$时有 p 个提升；否则无提升。

## 二次剩余、Legendre 与 Jacobi

- 奇素数 p，$a\nequiv 0$，a 是平方当且仅当 $a^{(p-1)/2}\equiv1\; (\mathrm{mod}\ p)$。a=0 的根为 0。
- Legendre 符号 (a/p) 取 0、1、-1，完全积性。$\left(\frac{-1}{p}\right)=(-1)^{(p-1)/2}$，$\left(\frac{2}{p}\right)=(-1)^{(p^2-1)/8}$。
- 不同奇素数 p,q，$\left(\frac{p}{q}\right)\left(\frac{q}{p}\right)=(-1)^{\frac{p-1}{2}\frac{q-1}{2}}$，可反复取模计算符号。
- Jacobi (a/n) 对奇合数 n 按素因子重数相乘。值为 1 仅为平方的必要条件，不足以判有根。
- $p\equiv 3 \;\mathrm{mod}\; 4$时一个平方根为 $a^{(p+1)/4}$。一般奇素数使用 Tonelli-Shanks 或 Cipolla。

## Tonelli-Shanks（素数模，返回一个根或 -1）

```cpp
ll sqrt_mod(ll a, ll p) {
    a = norm(a, p);
    if (p == 2 || a == 0) return a;
    if (pow_mod(a, (p-1)/2, p) != 1) return -1;
    ll q = p - 1;
    int s = 0;
    while (q % 2 == 0) { q /= 2; ++s; }
    ll z = 2;
    while (pow_mod(z, (p-1)/2, p) != p-1) ++z;
    ll c = pow_mod(z, q, p), t = pow_mod(a, q, p);
    ll x = pow_mod(a, (q+1)/2, p);
    int m = s;
    while (t != 1) {
        int i = 0;
        ll v = t;
        while (v != 1) { v = mul_mod(v, v, p); ++i; }
        ll b = c;
        for (int j = 0; j < m-i-1; ++j) b = mul_mod(b, b, p);
        x = mul_mod(x, b, p);
        c = mul_mod(b, b, p);
        t = mul_mod(t, c, p); m = i;
    }
    return min(x, p-x);
}
```

- 非零根成 ±x 一对。Tonelli-Shanks 的平方链部分为$\mathcal{O}(\log ^{2}p)$，寻找非剩余的耗时另计。
- Cipolla 随机取 b 使$w=b^{2}-a$为非剩余，在扩域$i^{2}=w$中求 $(b+i)^{(p+1)/2}$，实部即根，期望$\mathcal{O}(\log  p)$。
- Atkin 对$p\equiv 5 \;\mathrm{mod}\; 8$有专用公式。其他开方路线还包括有限域多项式算法和 Bostan-Mori 型递推；使用时核对域与次数条件。

# 数论 · 离散对数与高次剩余

## BSGS / 扩展 BSGS

求最小 $x\geq0$ 使 $a^x\equiv b\; (\mathrm{mod}\ m)$。互素时令 x=it+j，表存最小 j 对应的$a^j$，扫描 $ba^{-it}$。

```cpp
ll bsgs_coprime(ll a, ll b, ll m) {
    if (m == 1) return 0;
    ll t = (ll)sqrtl((long double)m) + 1;
    unordered_map<ll,ll> baby;
    ll cur = 1 % m;
    for (ll j = 0; j < t; ++j) {
        baby.emplace(cur, j);             // preserve smallest j
        cur = mul_mod(cur, a, m);
    }
    ll step = inverse(pow_mod(a, t, m), m);
    cur = norm(b, m);
    for (ll i = 0; i <= t; ++i) {
        auto it = baby.find(cur);
        if (it != baby.end()) return (ll)((i128)i*t + it->second);
        cur = mul_mod(cur, step, m);
    }
    return -1;
}
ll discrete_log(ll a, ll b, ll m) {
    a = norm(a, m); b = norm(b, m);
    ll removed = 0, coefficient = 1 % m;
    while (true) {
        if (coefficient == b) return removed;
        ll g = gcd(a, m);
        if (g == 1) break;
        if (b % g) return -1;
        m /= g; b /= g;
        coefficient = mul_mod(coefficient, a/g, m);
        ++removed;
    }
    ll target = mul_mod(b, inverse(coefficient, m), m);
    ll tail = bsgs_coprime(norm(a,m), target, m);
    return tail == -1 ? -1 : tail + removed;
}
```

- BSGS 期望$\mathcal{O}(\sqrt{m})$时间和空间，m 接近 64 位上限时内存不可承受。表大小按题目限制选择。
- 扩展算法每轮先判断当前短指数是否已经满足，再去 gcd。a=0、b=1、m=1 都按此模板的空乘积约定处理。

## 高次剩余与单位根

- 在有原根的单位群中，解$x^k=a$，令$x=g^y$、$a=g^A$，转成 $ky\equiv A\; (\mathrm{mod}\ \varphi(m))$。
- 设$d=\gcd (k,\varphi (m))$，a 可逆时有根当且仅当 $a^{\varphi(m)/d}\equiv1\; (\mathrm{mod}\ m)$；有根时恰有 d 个，$y=y_0+j\frac{\varphi(m)}{d}$。
- $x^k=1$的单位根同样有 d 个。模素数 p，k|p-1 时取 $\omega=g^{(p-1)/k}$，为 k 阶本原单位根。
- 对本原 n 阶根，$\sum_{j=0}^{n-1}\omega^{ij}=n$（n|i）或 0（否则），可作单位根过滤。
- 模$2^e$（$e\geq 3$）的单位群为 $C_2\times C_{2^{e-2}}$，不能假设有单个原根，要分别处理符号与 5 的幂。
- a 不可逆时先拆素数幂并分析$v_p(a)$，再解单位部分与零根，最后 CRT。改良 Tonelli-Shanks 可处理特定高次开方。

# 数论 · 分块、卷积与莫比乌斯反演

## 数论分块

- 对正整数 n，$\left\lfloor n/i\right\rfloor$的不同值只有$\mathcal{O}(\sqrt{n})$个。当前左端 l，值 q=n/l，相同商的右端 r=n/q。
- 双维分块同时求$\left\lfloor n/i\right\rfloor$、$\left\lfloor m/i\right\rfloor$，右端为 min(n/(n/l),m/(m/l))，循环到 min(n,m)。
- $\left\lceil n/i\right\rceil=\left\lfloor (n-1)/i\right\rfloor+1$，可转分块处理；n=1 时 n-1=0 的零商段需单独处理。
- 任意幂次或加权分块需重新推导等商边界，不能把 n/(n/l) 无条件套给所有取整形式。

```cpp
i128 divisor_sum(ll n) {               // sum floor(n/i), n >= 0
    i128 ans = 0;
    for (ll l = 1, r; l <= n; ) {
        ll q = n / l;
        r = n / q;
        ans += (i128)(r-l+1) * q;
        if (r == n) break;
        l = r + 1;
    }
    return ans;
}
```

## 狄利克雷卷积

- $(f*g)(n)=\sum_{d\mid n}f(d)g\left(\frac{n}{d}\right)$。交换、结合、分配律成立，单位元$\varepsilon (1)=1$、$\varepsilon (n>1)=0$。
- 常用函数 1(n)=1、id(n)=n。$\mu *1=\varepsilon $，$\varphi *1=\mathrm{id}$，1*1=τ，id*1=σ，$\mu *\mathrm{id}=\varphi $。
- f(1) 可逆时存在卷积逆元。逆元在素数幂处用卷积系数逐项求，f 为积性则逆元也为积性。
- 积性函数由所有素数幂值唯一确定。Bell 级数 $B_{f,p}(t)=\sum_{e\geq0}f(p^e)t^e$，卷积对应同一 p 的级数相乘。
- Dirichlet 生成函数 $\sum_{n\geq1}\frac{f(n)}{n^s}$ 中，卷积对应乘法；积性函数在绝对收敛区可写为各素数的 Euler 乘积。
- 一般卷积双循环 d、k 累加到 h[dk]，$\mathcal{O}(n \log  n)$。若能由素数幂递推，可用线性筛。

## 莫比乌斯反演

- $F(n)=\sum_{d\mid n}f(d)$ 等价于 $f(n)=\sum_{d\mid n}\mu(d)F\left(\frac{n}{d}\right)$。
- 倍数形式 $F(n)=\sum_{n\mid d}f(d)$，则 $f(n)=\sum_{n\mid d}\mu(d/n)F(d)$，有限范围或收敛条件下成立。
- $[\gcd(a,b)=1]=\sum_{d\mid\gcd(a,b)}\mu(d)$。互素对计数为 $\sum_{d=1}^{\min(n,m)}\mu(d)\left\lfloor\frac{n}{d}\right\rfloor\left\lfloor\frac{m}{d}\right\rfloor$。
- gcd 恰为 k 时先将 n,m 除以 k。加权 gcd 求和常先令$h=\mu *f$，将 f(gcd) 改写成 $\sum_{d\mid\gcd(a,b)}h(d)$。
- 预处理$\mu $前缀和 M，分块 [l,r] 的$\mu $权重为 M(r)-M(l-1)，再乘两个整数商。

# 数论 · 双曲线法与杜教筛

## 双曲线法

令 $F(x)=\sum_{i\leq x}f(i)$、$G(x)=\sum_{i\leq x}g(i)$，h=f*g。取$A=\left\lfloor \sqrt{n}\right\rfloor$，$B=\left\lfloor n/(A+1)\right\rfloor$，则

- $\sum_{i\leq n}h(i)=\sum_{a\leq A}f(a)G\left(\left\lfloor\frac{n}{a}\right\rfloor\right)+\sum_{b\leq B}g(b)F\left(\left\lfloor\frac{n}{b}\right\rfloor\right)-F(A)G(B)$。
- 选择满足 (A+1)(B+1)>n、$AB\leq n$的边界，确保双曲线下的每个格点至少被覆盖一次，再减交叠矩形。
- 块筛卷积将上述点值求和扩展到所有$\left\lfloor n/i\right\rfloor$位置；需要同时维护小点值与大点值的索引。

## 杜教筛通式

已知 h=f*g，g(1)≠0，且 h 的前缀和 H 与 g 的区间和容易求。把卷积前缀和按 g 的参数展开，有

- $g(1)F(n)=H(n)-\sum_{l=2}^{n}g(l)F\left(\left\lfloor\frac{n}{l}\right\rfloor\right)$。
- 用数论分块将每段 g(l) 相加，递归的参数变为$\left\lfloor n/l\right\rfloor$。小范围线性筛，较大点值记忆化。
- $\varphi *1=\mathrm{id}$得 $\Phi(n)=\frac{n(n+1)}{2}-\sum_{l=2}^{n}\Phi\left(\left\lfloor\frac{n}{l}\right\rfloor\right)$。
- $\mu *1=\varepsilon $得 $M(n)=1-\sum_{l=2}^{n}M\left(\left\lfloor\frac{n}{l}\right\rfloor\right)$，$n\geq 1$。$\Phi (0)=M(0)=0$。

```cpp
struct Dujiao {
    ll small_phi[NumberSieve::MAX_N+1]{};
    ll small_mu[NumberSieve::MAX_N+1]{};
    int limit;
    map<ll, pair<i128,i128>> memo;
    Dujiao(const NumberSieve& s) : limit(s.limit) {
        for (int i = 1; i <= limit; ++i) {
            small_phi[i] = small_phi[i-1] + s.phi[i];
            small_mu[i] = small_mu[i-1] + s.mu[i];
        }
    }
    pair<i128,i128> sum(ll n) {
        if (n <= limit)
            return {small_phi[n], small_mu[n]};
        auto it = memo.find(n);
        if (it != memo.end()) return it->second;
        i128 a = (i128)n*((i128)n+1)/2, b = 1;
        for (ll l = 2, r; l <= n; ) {
            ll q = n/l; r = n/q;
            auto [x,y] = sum(q);
            a -= (i128)(r-l+1)*x; b -= (i128)(r-l+1)*y;
            if (r == n) break;
            l = r+1;
        }
        return memo[n] = {a,b};
    }
};
```

- 输入$n\geq 0$，预筛上限至少为 1，Dujiao 对象放全局或声明为 static。预筛内的答案用前缀表读取，最终结果与累加项须能用 i128 表示。
- 通常取预筛$B\approx n^{2/3}$，忽略字典额外开销时总成本可到$\mathcal{O}(n^{2/3})$量级。内存不足时调小 B 会增加递归工作。

# 数论 · 素数计数与素数幂和

## 商集合上的 Lucy 筛

$\pi (n)$表示$\leq n$的素数个数。对所有不同$v=\left\lfloor n/i\right\rfloor$，初始 g(v)=v-1。枚举素数 p，降序更新$v\geq p^{2}$，有$g(v)-=g(\left\lfloor v/p\right\rfloor)-g(p-1)$。

```cpp
ll prime_count(ll n) {                  // n >= 0; O(sqrt(n)) states
    if (n < 2) return 0;
    vector<ll> values, g;
    unordered_map<ll,int> id;
    for (ll l = 1, r; l <= n; ) {
        ll v = n/l; r = n/v;
        id[v] = (int)values.size();
        values.push_back(v); g.push_back(v-1);
        if (r == n) break;
        l = r+1;
    }
    for (ll p = 2; p <= n/p; ++p) {
        ll before = g[id[p-1]];
        if (g[id[p]] == before) continue;
        for (int i = 0; i < (int)values.size() &&
                        p <= values[i]/p; ++i)
            g[i] -= g[id[values[i]/p]] - before;
    }
    return g[0];
}
```

- values 由大到小，保证右侧仍是筛 p 之前的值。反向更新会重复筛除。
- 查询值$\left\lfloor \left\lfloor n/i\right\rfloor/p\right\rfloor=\left\lfloor n/(ip)\right\rfloor$仍在商集合。所有$\leq \sqrt{n}$的正整数也在集合中。
- 这里字典索引为期望$\mathcal{O}(1)$，可换两个数组 small[v]、large[n/v] 减常数。工作量保守估计$\mathcal{O}(n^{3/4})$，空间$\mathcal{O}(\sqrt{n})$。

## Meissel-Lehmer 与 LMO 路线

- $\varphi (x,a)$计数$\leq x$且不含前 a 个素因子的整数，$\varphi(x,a)=\varphi(x,a-1)-\varphi(\lfloor x/p_a\rfloor,a-1)$，$\varphi (x,0)=\left\lfloor x\right\rfloor$。
- 取$a=\pi (\sqrt{x})$时，$\pi (x)=\varphi (x,a)+a-1$。Lehmer 取$a=\pi (x^{1/4})$，再减去双素数、三素数的贡献。
- 记$b=\pi (\sqrt{x})$、$c=\pi (x^{1/3})$，Lehmer 公式为$\varphi (x,a)+(b+a-2)(b-a+1)/2$。
- 再减 $\sum_{i=a+1}^{b}\pi\left(\frac{x}{p_i}\right)$，以及 $\sum_{i=a+1}^{c}\sum_{j=i}^{\pi(\sqrt{x/p_i})}\left(\pi\left(\frac{x}{p_ip_j}\right)-j+1\right)$，此处素数下标从 1 开始。
- LMO 用更细的部分筛截断与特殊叶子求和减少工作；需按目标规模选择预筛、分段与缓存，不能只替换一个截断指数。

## 素数幂和的推广

- 求 $\sum_{p\leq v}p^k$，把初值换成 $\sum_{i=2}^{v}i^k$，更新为$g(v)-=p^k(g(v/p)-$`prefix_prime_power`(p-1))。
- k=0 是素数计数，k=1 用 v(v+1)/2-1。计算幂和先做精确整除或使用可逆的分母。
- 小数次浮点开方可能误差，整数根要用乘法比较修正；用 i128 或除法避免溢出。

# 数论 · Powerful Number 筛

## 稀疏卷积结构

- Powerful Number（PN）中每个素因子的指数至少为 2。可写成$a^{2}b^{3}$，其中 b 无平方因子，因此$\leq n$的 PN 只有$\mathcal{O}(\sqrt{n})$个。
- 目标 f 为积性函数。挑选前缀和 G 容易求、且 g(p)=f(p) 的积性函数 g，定义 f=g*h。
- h(1)=1，h(p)=0。由素数幂卷积递推 $h(p^e)=f(p^e)-\sum_{j=0}^{e-1}h(p^j)g(p^{e-j})$。
- h 仅可能在 PN 上非零，故 $F(n)=\sum_{d\leq n,\,d\in\mathrm{PN}}h(d)G\left(\left\lfloor\frac{n}{d}\right\rfloor\right)$。

## 枚举路线与伪代码

```text
prepare primes up to floor(sqrt(n))
prepare h(p^e) for p^e <= n, e >= 2
DFS(product d, coefficient h(d), first allowed prime index):
    answer += h(d) * G(floor(n/d))
    for each next prime p with p*p <= n/d:
        for each power p^e <= n/d, e >= 2:
            DFS(d*p^e, h(d)*h(p^e), next index after p)
start with DFS(1, 1, 0)
```

- 只使用递增的不同素数，不会重复枚举 PN。乘之前比较$p\leq n/d/p$，并给幂乘设置 n/p 的边界。
- 递推$h(p^e)$时 e=0 的值是 1，e=1 的值是 0。若$h(p^e)=0$可跳过该分支。
- 总成本是素数预处理、素数幂系数计算和所有 G(n/d) 查询之和。G 为$\mathcal{O}(1)$时，PN 枚举部分为$\mathcal{O}(\sqrt{n})$量级。

## 构造例子

- $f(p^e)=p^e(p^e-1)$。取$g(n)=n\varphi (n)$，在素数上二者均为 p(p-1)。
- 此时$h(p^e)=(e-1)(p-1)p^e$，$e\geq 1$，特别地 h(p)=0。
- 因为$(\mathrm{id}\cdot \varphi )*\mathrm{id}=\mathrm{id}^{2}$，$G(n)=\sum_{i=1}^{n}i^2-\sum_{d=2}^{n}dG\left(\left\lfloor\frac{n}{d}\right\rfloor\right)$。权重区间和为等差数列和，可杜教筛。
- Bell 级数提供选择 g 的方法，使$B_f,p/B_g,p$的一次项为 0，即可把修正项支撑限制到 PN。
- 若 f 与 g 的素数点值不同，h(p) 不为 0，PN 枚举遗漏普通质数及其乘积，公式不能按此方式加速。

# 数论 · Min_25 筛

## 第一步，筛出素数点值前缀和

- 适用 f 为积性，f(p) 可拆成少量完全积性函数之和（常见为 p 的低次多项式），$f(p^e)$可快速求。
- 存所有$v=\left\lfloor n/i\right\rfloor$，$\mathcal{O}(\sqrt{n})$个。对每个完全积性分量 h（如$h(x)=x^k$），初始化 $G(v)=\sum_{i=2}^{v}h(i)$。
- 按素数 p 递增，对$v\geq p^{2}$降序更新 $G(v)\leftarrow G(v)-h(p)\left(G(\lfloor v/p\rfloor)-\sum_{q<p,\,q\ \mathrm{prime}}h(q)\right)$。
- 所有素数筛完后按系数组合，得到 $P(v)=\sum_{p\leq v}f(p)$。上一页 `prime_count` 是 h=1 的特例。

## 第二步，枚举最小质因子与指数

定义 S(x,j) 为$\leq x$、$\geq 2$且所有质因子编号$\geq j$的整数 f 值和，素数$p_0=2$。从 x=n、j=0 开始，答案为 1+S(n,0)。

```text
S(x, j):
    if no allowed prime <= x: return 0
    result = P(x) - sum(f(p_i), i < j)
    for i >= j while p_i*p_i <= x:
        power = p_i
        e = 1
        while power*p_i <= x:
            result += f(power) * S(floor(x/power), i+1)
            result += f(power*p_i)
            power *= p_i
            e += 1
    return result
```

- f(power) 表示题目给定的素数幂函数$f(p_i^e)$，不能误用$f(p_i)^e$，除非 f 完全积性。
- 素数贡献已经放在 P(x) 中；内层额外加$f(p^{e+1})$计入纯素数幂，递归项负责再乘更大的质因子。
- 只在$\mathrm{power}*p\leq x$时转移，因为最后一段的 x/power<p，无更大质因子可乘。
- 边界$x<p_j$返回 0。j 超过预存的$\leq \sqrt{n}$素数列表时，仍可能有大素数贡献，可由 P(x)-prefix[j] 计算。
- 递归不应仅按 x 缓存，因为 j 也影响答案。普通记忆化 (x,j) 可能耗费大量内存。

## 实现与复杂度

- 预处理$\mathrm{primes}\leq \sqrt{n}$，素数 f 值前缀和，以及商集合索引。small[v] 与 large[n/v] 两组数组可$\mathcal{O}(1)$定位有效 v。
- 素数筛部分常记为$\mathcal{O}(n^{3/4}/\log  n)$，$\mathcal{O}(\sqrt{n})$主数组。第二步递归的成本另依具体 f 和枚举方式分析。
- 若用迭代后缀 DP 改写第二步，可控制在相同量级；勿直接缓存全部“质数编号×商值”的矩阵。
- 计算$f=\varphi $时 f(p)=p-1，$f(p^e)=p^{e-1}(p-1)$。计算$f=\mu $时 f(p)=-1，$e\geq 2$时为 0。
- 模数与整数函数分开处理。幂和初值中的除法必须在整数域先约分，或确认模逆元存在。

# 数论 · 洲阁筛

## 按是否含大质因子划分

令$B=\left\lfloor \sqrt{n}\right\rfloor$，小质数为$p_0,\ldots ,p_{m-1}$。n 以内整数至多含一个 >B 的质因子，该因子的指数只能是 1。

- 记 $P(x)=\sum_{p\leq x}f(p)$，由素数点值筛得到。大质因子贡献为 $A=\sum_{a=1}^{B}f(a)\left(P\left(\left\lfloor\frac{n}{a}\right\rfloor\right)-P(B)\right)$。
- 对每个 a，$\left\lfloor n/a\right\rfloor\geq B$，所以差值非负（f 非负时）。若 p>B 且$ap\leq n$，则 gcd(a,p)=1，可用积性。
- 剩余贡献 C 来自全部质因子$\leq B$的光滑数，包括 1。最终 F(n)=A+C。

## 光滑数 DP 的确切递推

定义 H(j,x) 为$\leq x$且仅含$p_j,\ldots ,p_{m-1}$的数的 f 值和，包含 1。

```text
H(m, x) = 1                  (x >= 1)
H(j, x) = H(j+1, x)
          + sum over e>=1, p_j^e<=x:
                f(p_j^e) * H(j+1, floor(x/p_j^e))
C = H(0, n)
```

- 若$p_j>x$，H(j,x)=1。若$p_j^{2}>x$，所有允许合数都超过 x，因此 $H(j,x)=1+\sum_{i\geq j,\,p_i\leq x}f(p_i)$。
- 尾部可用小质数 f 值前缀和直接补齐，不再枚举每个素数；对每个 x 只执行$p^{2}\leq x$的非平凡转移。
- x 只会在$\left\lfloor n/i\right\rfloor$的商集合中出现。逆序处理 j，结合每个商的转移截止位置与素数前缀和压缩状态。
- 暴力存下全部 H(j,x) 或不加截止地递归会退化。空间优化必须保证查询到的确为上一层 j+1 的状态。

## 素数点值筛的条件

- $f(p^e)$对 p 常为低次多项式。求 P 可拆成$p^k$分量，分别用完全积性$h(x)=x^k$的扩展埃筛，再线性组合。
- 递推$G_j(x)=G_{j-1}(x)-h(p_j)G_{j-1}(\left\lfloor x/p_j\right\rfloor)$只在对应的完全积性筛状态中成立。
- 不能把一般积性 f 直接当作完全积性函数套减法，尤其当$f(p^{2})\ne f(p)^{2}$时。
- 标准优化的目标工作量约为$\mathcal{O}(n^{3/4}/\log  n)$，商集合主空间$\mathcal{O}(\sqrt{n})$，需要把两阶段的尾部都压缩。
- 赛场定制时按 A 与 C 分别对拍，重点检查 f(1)=1、p>B 的严格边界和$p^{2}=x$的等号归属。

# 数论 · 类欧几里得与万能欧几里得

## `floor_sum`，含负系数的安全定义

求 $\sum_{i=0}^{n-1}\left\lfloor\frac{ai+b}{m}\right\rfloor$，$n\geq 0$、m>0。用 `floor_div` 将负数归一化后，再执行无符号几何交换。

```cpp
i128 floor_sum(ll n, ll m, ll a0, ll b0) {
    i128 a = a0, b = b0, mod = m, count = n;
    i128 qa = floor_div(a, mod), qb = floor_div(b, mod);
    i128 ans = qa*count*(count-1)/2 + qb*count;
    a -= qa*mod; b -= qb*mod;
    while (true) {
        ans += count*(count-1)/2*(a/mod);
        ans += count*(b/mod);
        a %= mod; b %= mod;
        i128 y = a*count+b;
        if (y < mod) break;
        count = y/mod; b = y%mod;
        swap(a, mod);
    }
    return ans;
}
```

- 时间为欧几里得量级$\mathcal{O}(\log  m)$，前提是 i128 的中间项可表示。区间 [L,R] 可先令 b'=aL+b、n=R-L+1。
- 本页定义上界为 n-1。若题目求 i=0..n，调用时传 n+1。
- 非负 a,b<c 的闭区间版本 F(a,b,c,n)，令$y=\left\lfloor (an+b)/c\right\rfloor$，则 F=ny-F(c,c-b-1,a,y-1)。y=0 时直接返回 0。

## 加权类欧递推

- 需要 $\sum_i i\left\lfloor\frac{ai+b}{c}\right\rfloor$、$\sum_i\left\lfloor\frac{ai+b}{c}\right\rfloor^2$ 时，可在交换求和后同步维护 F、一次加权和与平方和。
- 商部分 a=qc+a'、b=rc+b' 展开后只出现 $\sum_i i$、$\sum_i i^2$ 与较小问题的这些矩。先展开多项式再代入，避免遗漏交叉项。
- 平方可用 $y^2=\sum_{j=0}^{y-1}(2j+1)$，把“高度的平方”转为竖向加权计数，再交换横纵坐标。

## 万能欧几里得的路径视角

- 把直线下的格点边界编码为横步 R 和竖步 U。一次商拆分把长串相同步压成一个块，交换坐标则交换 R/U 的角色。
- 状态是可结合的路径信息，可选矩阵积、计数及带权矩。需要定义 concat(A,B) 与 repeat(A,k)，并保留非交换乘法的顺序。
- 对每个欧几里得商，按路径顺序拼接整块，再递归处理余段。不能仅凭交换 a,c 就交换已累积的矩阵因子。
- `floor_sum` 是这种方法的标量特例。更复杂的 DP 先在小范围展开 R/U 序列对拍，再套块合并。

# 数论 · 连分数与有理逼近

## 有限与循环连分数

- 简单连分数$x=[a_{0};a_{1},a_{2},\ldots ]$，$a_{0}$为整数，后续$a_i$为正整数。有理数用欧几里得算法求有限展开。
- 有限展开末项若为 1，可合并到前一项；[...;a]=[...;a-1,1]，规范表示通常要求末项$\geq 2$（仅一项除外）。
- $p_{-2}=0$、$p_{-1}=1$，$q_{-2}=1$、$q_{-1}=0$；$p_k=a_kp_{k-1}+p_{k-2}$，q 同理。
- $p_kq_{k-1}-p_{k-1}q_k=(-1)^{k-1}$，因此每个渐近分数既约，邻项交叉积绝对值为 1。
- 偶下标渐近分数不超过 x，奇下标不小于 x；有限展开末项可取等。

```cpp
vector<ll> continued_fraction(ll a, ll b) { // a>=0, b>0
    vector<ll> terms;
    while (b) {
        terms.push_back(a/b);
        ll r = a%b; a = b; b = r;
    }
    return terms;
}
pair<i128,i128> convergent(const vector<ll>& terms) {
    i128 p0 = 0, p1 = 1, q0 = 1, q1 = 0;
    for (ll a : terms) {
        i128 p = a*p1+p0, q = a*q1+q0;
        p0 = p1; p1 = p; q0 = q1; q1 = q;
    }
    return {p1,q1};
}
```

## 误差与最佳逼近

- 对无理 x，$\left|x-\frac{p_k}{q_k}\right|<\frac{1}{q_kq_{k+1}}<\frac{1}{q_k^2}$。
- 若既约 a/b 满足 $\left|x-\frac{a}{b}\right|<\frac{1}{2b^2}$，则它一定是某个渐近分数（Legendre 判据）。
- 中间分数 $\frac{tp_{k-1}+p_{k-2}}{tq_{k-1}+q_{k-2}}$，$1\leq t\leq a_k$。分母限额下的最佳误差候选需考虑它们。
- 第二类最佳逼近按 |qx-p| 比较；第一类按 |x-p/q| 比较。比较标准不同，不能只凭分母较小选答案。
- 给定分母上限 Q，取最后可行渐近分数，再对下一项 $t\leq\left\lfloor\frac{Q-q_{k-2}}{q_{k-1}}\right\rfloor$ 截断，比较边界候选。

## 矩阵与二次无理数

- 每项 a 对应矩阵 $\left[\genfrac{}{}{0}{}{a\quad1}{1\quad0}\right]$，连乘得到 $\left[\genfrac{}{}{0}{}{p_k\quad p_{k-1}}{q_k\quad q_{k-1}}\right]$。可用矩阵快速幂处理重复块。
- 分式线性变换 x→(ax+b)/(cx+d) 同样用 2×2 矩阵表示，复合对应矩阵相乘，分母为 0 需另判。
- 实二次无理数的简单连分数最终循环，反向也成立。$\sqrt{D}$的状态可用 Pell 页的 (P,Q,a) 递推精确生成。
- 连分数四则运算需要维护区间或双线性状态，只有输出项已由两端界唯一确定时才可输出，否则继续读输入项。

# 数论 · Stern-Brocot 与 Farey 序列

## 中位数、查找与树

- 以左界 a/b=0/1、右界 c/d=1/0 开始，当前点为 $\frac{a+c}{b+d}$。小于目标就替换左界，大于目标就替换右界。
- 始终有 bc-ad=1，中位数既约且严格位于两界之间。所有正有理数在树中恰出现一次，中序递增。
- 向左/向右一步对应一个幺模矩阵。连续同方向的步数可按欧几里得商批量处理，避免目标分子很大时逐步走。
- 目标 p/q 若大于中位数，连续更新左界 a+=tc、b+=td，选最大 t 使新界仍严格小于 p/q；另一方向对称。
- 比较两个分数用 i128 交叉乘，分母规范为正。右界 1/0 是无穷，不能直接做整数除法。

## 有理区间内找分母小的分数

- 当前两界的中位数若在目标开区间内，它给出该 Stern-Brocot 区间的最小分母候选。
- 若还在区间左侧或右侧，整批跳过相同方向的步数。开闭端点决定严格比较还是允许等号。
- 逐层展开树深 d 的节点数为指数级，按路径查找才适合大分子分母。

## Farey 序列

- $F_N$为 [0,1] 内分母$\leq N$的全部最简分数，升序排列，含 0/1 与 1/1。
- 相邻 a/b<c/d 满足 bc-ad=1，且 b+d>N。它们的中位数是下一次可插入的最小分母分数。
- 已知邻项 a/b、c/d，令$k=\left\lfloor (N+b)/d\right\rfloor$，下一项为 $\frac{kc-a}{kd-b}$。
- 长度 $|F_N|=1+\sum_{i=1}^{N}\varphi(i)$。N=1 时恰有 0/1、1/1。

```cpp
vector<pair<ll,ll>> farey(ll N) {         // N >= 1; output can be O(N^2)
    vector<pair<ll,ll>> out{{0,1}};
    ll a = 0, b = 1, c = 1, d = N;
    while (true) {
        out.push_back({c,d});
        if (c == 1 && d == 1) break;
        ll k = (ll)(((i128)N+b)/d);
        ll e = (ll)((i128)k*c-a), f = (ll)((i128)k*d-b);
        a = c; b = d; c = e; d = f;
    }
    return out;
}
```

- Calkin-Wilf 树的 a/b 两子节点为 a/(a+b)、(a+b)/b，层序也枚举全部正有理数，顺序与 Stern-Brocot 不同。
- Stern 双原子序列 s(0)=0、s(1)=1，s(2n)=s(n)、$s(2n+1)=s(n)+s(n+1)$，连续项比值联系上述枚举。

# 数论 · 二次域与 Pell 方程

## 共轭、范数与二次整数

- $Q(\sqrt{D})$的数写为$a+b\sqrt{D}$，共轭$a-b\sqrt{D}$，迹 2a、范数$a^{2}-Db^{2}$。范数可乘。
- $(a+b\sqrt{D})(c+d\sqrt{D})=(ac+Dbd)+(ad+bc)\sqrt{D}$，除法可乘共轭再除范数。适用于快速幂和 Pell 解复合。
- D 无平方因子时，整数环$D\equiv 1 \;\mathrm{mod}\; 4$用$Z[(1+\sqrt{D})/2]$，其余用$Z[\sqrt{D}]$，不能统一当作整数对。
- 单位数范数为 ±1，相伴元素之比为单位。一般二次整数环未必唯一分解，不能直接照搬整数质因数分解。
- Gauss 整数 Z[i] 的范数为$a^{2}+b^{2}$，可四舍五入商做欧几里得除法。$p\equiv 3 \;\mathrm{mod}\; 4$仍为素数，$p\equiv 1 \;\mathrm{mod}\; 4$分裂为共轭因子，2 与$(1+i)^{2}$相伴。
- n 能写为两个整数平方和，当且仅当分解中所有$p\equiv 3 \;\mathrm{mod}\; 4$的指数为偶数。
- 本原勾股数组可写为$(u^{2}-v^{2},2uv,u^{2}+v^{2})$，u>v>0、互素且奇偶不同。Eisenstein 整数满足$\omega ^{2}+\omega +1=0$，范数$a^{2}-ab+b^{2}$；$p\equiv 1 \;\mathrm{mod}\; 3$分裂，$p\equiv 2 \;\mathrm{mod}\; 3$保持素数，3 分歧。

## $\mathrm{Pell} x^{2}-Dy^{2}=1$，求最小正解

D 为非平方整数，$2\leq D\leq 10^9$。使用 GNU 内置 i128，超出其正数范围返回 nullopt。通过连分数状态判定范数为 1，避免直接计算$x^{2}-Dy^{2}$溢出。

```cpp
optional<pair<i128,i128>> pell(ll D) {
    ll a0 = (ll)sqrtl((long double)D);
    while ((a0+1)*(a0+1) <= D) ++a0;
    while (a0*a0 > D) --a0;
    ll P = 0, Q = 1, a = a0;
    i128 p0 = 0, p1 = 1, q0 = 1, q1 = 0;
    const i128 LIMIT = (i128)(((u128)1 << 127)-1);
    bool positive_norm = false;             // index 0 has negative norm
    while (true) {
        if (p1 > (LIMIT-p0)/a || q1 > (LIMIT-q0)/a)
            return nullopt;
        i128 p = a*p1+p0, q = a*q1+q0;
        P = a*Q-P; Q = (D-P*P)/Q; a = (a0+P)/Q;
        // p_k^2-D*q_k^2 = (-1)^(k+1) * Q_(k+1)
        if (positive_norm && Q == 1) return pair<i128,i128>{p,q};
        p0 = p1; p1 = p; q0 = q1; q1 = q;
        positive_norm = !positive_norm;
    }
}
```

- 先检查返回值是否为空，再读取 first、second。i128 没有标准流输出运算符，按十进制反复取余自行输出。
- 全部正解 $x_k+y_k\sqrt{D}=(x_1+y_1\sqrt{D})^k$。还可用 $x_{k+2}=2x_1x_{k+1}-x_k$，y 同理。
- 设$\sqrt{D}$周期长度为 L。+1 方程基本解在第 L-1 项（L 偶）或第 2L-1 项（L 奇），下标从 0 开始。
- -1 方程可解当且仅当 L 为奇数，其基本解在第 L-1 项。±4 方程与完整二次整数环的单位也相关，不能复用相同终止位置。
- 广义$x^{2}-Dy^{2}=N$可能无解，也可能有多个不等价的基本解。一个种子乘 +1 方程的单位只能生成它自己的解轨道。
- PQa 从 $\frac{P+\sqrt{D}}{Q}$ 展开，$a=\left\lfloor\frac{P+\sqrt{D}}{Q}\right\rfloor$，P'=aQ-P、$Q'=\frac{D-(P')^2}{Q}$；枚举适当的初始同余根处理广义方程。
