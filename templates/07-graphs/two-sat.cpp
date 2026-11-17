#include <bits/stdc++.h>
using namespace std;
void add_or(int i,bool a,int j,bool b) {
    int x = 2*i+(int)a, y = 2*j+(int)b;
    scc_add(x^1,y); scc_add(y^1,x);
}
void imply(int i,bool a,int j,bool b) {
    add_or(i,!a,j,b);                  // (x_i=a) => (x_j=b)
}
void force(int i,bool value) { add_or(i,value,i,value); }
optional<vector<int>> solve_2sat(int n) {
    scc_build();
    vector<int> answer(n);
    for (int i = 0; i < n; ++i) {
        if (scc_id[2*i] == scc_id[2*i+1]) return nullopt;
        answer[i] = scc_id[2*i+1] > scc_id[2*i];
    }
    return answer;
}
