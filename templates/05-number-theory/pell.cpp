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
