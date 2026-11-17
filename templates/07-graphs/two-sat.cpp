#include <bits/stdc++.h>
using namespace std;
struct TwoSAT {
    int n;
    SCC graph;
    TwoSAT(int n) : n(n), graph(2*n) {}
    void add_or(int i,bool a,int j,bool b) {
        int x = 2*i+(int)a, y = 2*j+(int)b;
        graph.add_edge(x^1,y); graph.add_edge(y^1,x);
    }
    void imply(int i,bool a,int j,bool b) {
        add_or(i,!a,j,b);                  // (x_i=a) => (x_j=b)
    }
    void force(int i,bool value) { add_or(i,value,i,value); }
    optional<vector<int>> solve() {
        graph.build();
        vector<int> answer(n);
        for (int i = 0; i < n; ++i) {
            if (graph.comp[2*i] == graph.comp[2*i+1]) return nullopt;
            answer[i] = graph.comp[2*i+1] > graph.comp[2*i];
        }
        return answer;
    }
};
