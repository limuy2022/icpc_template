# 线性代数 · 实数高斯消元

## 矩形方程组、秩、特解与自由变量

输入 m 行增广矩阵，每行 n 个系数与一个右端项，使用 ld=long double。返回一个特解与零空间基，全部解为特解加零空间基的任意线性组合。

```cpp
struct RealSystem {
    bool consistent;
    int rank;
    vector<ld> solution;
    vector<vector<ld>> nullspace;
};
RealSystem real_gauss(vector<vector<ld>> a,int n,ld eps=1e-12L) {
    int m = (int)a.size(), row = 0;
    vector<int> where(n,-1);
    for (int col = 0; col < n && row < m; ++col) {
        int pivot = row;
        for (int i = row+1; i < m; ++i)
            if (fabsl(a[i][col]) > fabsl(a[pivot][col])) pivot = i;
        if (fabsl(a[pivot][col]) <= eps) continue;
        swap(a[pivot],a[row]);
        ld value = a[row][col];
        for (int j = col; j <= n; ++j) a[row][j] /= value;
        for (int i = 0; i < m; ++i) if (i != row) {
            ld factor = a[i][col];
            if (fabsl(factor) > eps)
                for (int j = col+1; j <= n; ++j)
                    a[i][j] -= factor*a[row][j];
            a[i][col] = 0;
        }
        where[col] = row++;
    }
    for (int i = row; i < m; ++i)
        if (fabsl(a[i][n]) > eps) return {false,row,{},{}};
    RealSystem result{true,row,vector<ld>(n),{}};
    for (int j = 0; j < n; ++j)
        if (where[j] != -1) result.solution[j] = a[where[j]][n];
    for (int f = 0; f < n; ++f) if (where[f] == -1) {
        vector<ld> basis(n); basis[f] = 1;
        for (int j = 0; j < n; ++j)
            if (where[j] != -1) basis[j] = -a[where[j]][f];
        result.nullspace.push_back(basis);
    }
    return result;
}
```

- consistent=false 表示出现 $0=b\ne0$，无解。有解且 rank=n 为唯一解，rank<n 时有 n-rank 个自由变量。
- 选绝对值最大的主元可减轻误差，eps 必须结合矩阵尺度。数据病态时应缩放并检查原方程残差，数值秩并非精确代数秩。
- 消元时间 $\mathcal{O}(mn\min(m,n))$；显式输出零空间基另需 $\mathcal{O}(n(n-r))$ 空间与时间。原矩阵按值传入，调用时会复制。
- 矩阵求逆可对 [A|I] 同步消元，左侧化成 I 后右侧为逆。缺少任一主元则方阵不可逆。
- 实数行列式用各步消元前主元之积，交换两行取负；将主元行除以主元后，不要误乘已经变成 1 的对角元。

# 线性代数 · 素数模高斯消元

## 有限域上的精确方程组

p 必须为素数，每行 n+1 项。复用 norm、mul_mod、pow_mod，返回特解及零空间基，所有运算在模 p 下进行。

```cpp
struct ModSystem {
    bool consistent;
    int rank;
    vector<ll> solution;
    vector<vector<ll>> nullspace;
};
ModSystem mod_gauss(vector<vector<ll>> a,int n,ll p) {
    int m = (int)a.size(), row = 0;
    for (auto& r : a) for (ll& x : r) x = norm(x,p);
    vector<int> where(n,-1);
    for (int col = 0; col < n && row < m; ++col) {
        int pivot = row;
        while (pivot < m && a[pivot][col] == 0) ++pivot;
        if (pivot == m) continue;
        swap(a[pivot],a[row]);
        ll inv = pow_mod(a[row][col],p-2,p);
        for (int j = col; j <= n; ++j)
            a[row][j] = mul_mod(a[row][j],inv,p);
        for (int i = 0; i < m; ++i) if (i != row && a[i][col] != 0) {
            ll factor = a[i][col];
            for (int j = col+1; j <= n; ++j)
                a[i][j] = norm((i128)a[i][j]-mul_mod(factor,a[row][j],p),p);
            a[i][col] = 0;
        }
        where[col] = row++;
    }
    for (int i = row; i < m; ++i)
        if (a[i][n] != 0) return {false,row,{},{}};
    ModSystem result{true,row,vector<ll>(n),{}};
    for (int j = 0; j < n; ++j)
        if (where[j] != -1) result.solution[j] = a[where[j]][n];
    for (int f = 0; f < n; ++f) if (where[f] == -1) {
        vector<ll> basis(n); basis[f] = 1;
        for (int j = 0; j < n; ++j)
            if (where[j] != -1) basis[j] = norm(-a[where[j]][f],p);
        result.nullspace.push_back(basis);
    }
    return result;
}
```

- 有解时解的个数为 $p^{n-r}$，r 为秩。唯一解等价于 r=n。n=0 时，所有右端为 0 才有唯一的空解。
- 消元不能只交换列却不记录变量置换。此实现只交换行，where 显式记录每个主元列对应的行。
- 合数模的非零数可能没有逆元。本模板不能用于一般合数模方程组，单个线性同余见第 14 页。
- 若只要求秩或一个特解，可以省去零空间基生成。若做模行列式，见第 68 页的 determinant_mod。

# 线性代数 · 异或高斯消元

## GF(2)，bitset 批量异或整行

系数与右端均为 0/1，加减都是异或。增广矩阵第 0..n-1 位为系数，第 n 位为右端，最大变量数为 512。

```cpp
constexpr int XOR_VARIABLES = 512;
struct XorSystem {
    bool consistent;
    int rank;
    bitset<XOR_VARIABLES> solution;
};
XorSystem xor_gauss(vector<bitset<XOR_VARIABLES+1>> a,int n) {
    int m = (int)a.size(), row = 0;
    int where[XOR_VARIABLES];
    fill_n(where,n,-1);
    for (int col = 0; col < n && row < m; ++col) {
        int pivot = row;
        while (pivot < m && !a[pivot][col]) ++pivot;
        if (pivot == m) continue;
        swap(a[pivot],a[row]);
        for (int i = 0; i < m; ++i)
            if (i != row && a[i][col]) a[i] ^= a[row];
        where[col] = row++;
    }
    for (int i = row; i < m; ++i)
        if (a[i][n]) return {false,row,{}};
    XorSystem result{true,row,{}};
    for (int j = 0; j < n; ++j)
        if (where[j] != -1) result.solution[j] = a[where[j]][n];
    return result;
}
```

- 调用前须把无关高位清零，并保证每行的右端位放在同一位置 n。`a[i][n]=rhs` 不是固定放在 bit 512。
- 返回一个自由变量全部取 0 的特解。有解时解数为 $2^{n-r}$，唯一解等价于 r=n。
- 如需完整通解，可用上一页 mod_gauss(p=2)，或逐个令自由变量为 1，从主元行读出对应零空间基。
- 位操作时间约为 $\mathcal{O}(mn\lceil(B+1)/w\rceil)$，B=512，w 为机器字长。更大位数需要修改常量或改手写动态字块。

## 常见模型

- 翻灯、开关、网格翻转，将每个操作是否执行作为一个变量，每个格子的目标状态作为一条异或方程。
- 输入翻转影响是“切换状态”，同一操作执行两次抵消，所以可限制变量为 0/1。
- 最少操作次数并非“求任意一组解”。自由变量少时枚举零空间组合并最小化置位数；自由变量多时通常需要问题额外结构。
- 线性基适合动态插入向量、维护秩或最大异或值。解带右端的完整方程组时，还须保留增广信息。
