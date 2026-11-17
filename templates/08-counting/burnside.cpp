ll unlabeled_simple_graphs(int n,ll p) { // p prime, p > n
    vector<ll> fac(n+1,1);
    for (int i = 1; i <= n; ++i) fac[i] = mul_mod(fac[i-1],i,p);
    vector<int> cycles;
    ll answer = 0;
    auto dfs = [&](auto&& self,int left,int smallest) -> void {
        if (left == 0) {
            ll orbit = 0, z = 1;
            vector<int> count(n+1);
            for (int i = 0; i < (int)cycles.size(); ++i) {
                int a = cycles[i]; ++count[a]; z = mul_mod(z,a,p);
                orbit += a/2;
                for (int j = 0; j < i; ++j) orbit += gcd(a,cycles[j]);
            }
            for (int c : count) z = mul_mod(z,fac[c],p);
            ll term = mul_mod(pow_mod(2,orbit,p),pow_mod(z,p-2,p),p);
            answer = norm((i128)answer+term,p); return;
        }
        for (int a = smallest; a <= left; ++a) {
            cycles.push_back(a); self(self,left-a,a); cycles.pop_back();
        }
    };
    dfs(dfs,n,1);
    return answer;
}
