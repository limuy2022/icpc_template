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
