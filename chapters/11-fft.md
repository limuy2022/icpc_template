# 多项式 · 快速傅里叶变换 FFT

## DFT、单位根与卷积

- 对长度 N 的系数，令 $\omega_N=\exp\left(\frac{2\pi i}{N}\right)$，DFT 为 $A_k=\sum_{j=0}^{N-1}a_j\omega_N^{jk}$，逆变换使用负角度并除以 N。
- 偶奇拆分得到 $A_k=E_k+\omega_N^kO_k$、$A_{k+N/2}=E_k-\omega_N^kO_k$，递归到长度 1，时间 $\mathcal{O}(N\log N)$。
- 两多项式系数卷积对应点值逐项相乘，变换长度必须至少为 len(a)+len(b)-1，再向上补为 2 的幂。

## 原地迭代 FFT

输入长度必须是正的 2 的幂。

```cpp
using Complex = complex<long double>;
void fft(vector<Complex>& a,bool inverse_transform) {
    size_t n = a.size();
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n>>1;
        for (; j&bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i],a[j]);
    }
    const long double pi = acosl(-1.0L);
    for (size_t len = 2; len <= n; ) {
        long double angle = (inverse_transform ? -2 : 2)*pi/len;
        Complex root(cosl(angle),sinl(angle));
        for (size_t start = 0; start < n; start += len) {
            Complex w(1,0);
            for (size_t j = 0; j < len/2; ++j) {
                Complex x = a[start+j], y = a[start+j+len/2]*w;
                a[start+j] = x+y; a[start+j+len/2] = x-y;
                w *= root;
            }
        }
        if (len == n) break;
        len <<= 1;
    }
    if (inverse_transform)
        for (Complex& x : a) x /= (long double)n;
}
```

- 正变换传 false，逆变换传 true。角度符号可整体对调，但正逆必须相反，且逆变换要恰好除 N 一次。
- 位逆序置换对应递归的偶奇拆分顺序，不能只改蝶形循环顺序而省略此步骤。
- 变换长度为 1 时不做蝶形，正逆均保持原值。空输入由卷积接口处理，底层 fft 不接受空数组。
- 空间 $\mathcal{O}(N)$ 用于系数数组，原地变换额外空间 $\mathcal{O}(1)$。浮点误差会随长度、系数大小和运算次数增长。

# 多项式 · FFT 整数卷积与精度

## 带负系数的卷积模板

返回长度 len(a)+len(b)-1 的整数卷积，要求结果可用 ll 表示，并且浮点误差小于正确舍入所允许的范围。

```cpp
vector<ll> convolution_fft(const vector<ll>& a,const vector<ll>& b) {
    if (a.empty() || b.empty()) return {};
    size_t need = a.size()+b.size()-1, n = 1;
    while (n < need) n <<= 1;
    vector<Complex> fa(n), fb(n);
    for (size_t i = 0; i < a.size(); ++i) fa[i] = (long double)a[i];
    for (size_t i = 0; i < b.size(); ++i) fb[i] = (long double)b[i];
    fft(fa,false); fft(fb,false);
    for (size_t i = 0; i < n; ++i) fa[i] *= fb[i];
    fft(fa,true);
    vector<ll> result(need);
    for (size_t i = 0; i < need; ++i) result[i] = llroundl(fa[i].real());
    return result;
}
```

## 使用例子与下标

```cpp
vector<ll> a{1,-2,3}, b{4,5};
auto c = convolution_fft(a,b);             // {4,-3,2,15}
```

- $c_k=\sum_{i+j=k}a_ib_j$。a、b 是次数最高分别为 A、B 的多项式时，结果有 A+B+1 个系数。
- N 不足时得到循环卷积，最高项折回低项。不能把 N 只补到 max(len(a),len(b))。
- 有负数时使用 llroundl，不能统一写成 `(ll)(x+0.5)`。结果溢出 ll 或误差达到 0.5 时不能保证正确。
- 粗略系数幅度上界为 $\min(|a|,|b|)\max_i|a_i|\max_j|b_j|$。仅满足 ll 范围不代表浮点能够精确恢复。

## 常见应用与扩展

- 相关运算可先反转一组序列，再做卷积，按定义把卷积下标换成位移。重复元素的计数可先建频率多项式。
- 大整数乘法按小进制拆成系数，卷积后从低位向高位进位。进制越大，精度风险越高，最终还可能多出若干进位位。
- 拆分系数 $a=a_0+Ba_1$、$b=b_0+Bb_1$，通过若干次较小系数卷积合成，可改善精度，但合并中间乘积要防溢出。
- NTT 在有限域进行相同的蝶形，需 N 整除 p-1 且具有相应阶单位根。常见素数 $998244353=119\cdot2^{23}+1$，本原根为 3。
- 多模 NTT 加 CRT 重构整数系数时，非负系数要求模数乘积大于系数上界；有符号还原要求大于两倍绝对值界。任意模数卷积也可另用拆分方案。
- FFT 模板的 long double 精度依编译平台而异；正式使用前按题目最大长度与系数范围对拍。需要严格整数结果时应选满足条件的精确模变换。

来源范围按 OI Wiki Team 的错位排列、卡特兰数、图论计数、博弈论、高斯消元与 FFT 条目核对，整理日期 2026-11-05。
