#include <bits/stdc++.h>
using namespace std;
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
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
