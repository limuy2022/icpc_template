#include <bits/stdc++.h>
using namespace std;
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
