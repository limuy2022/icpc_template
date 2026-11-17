struct Rat {
    ll p, q;
    Rat(i128 a=0,i128 b=1) {
        if (b < 0) { a = -a; b = -b; }
        i128 g = std::__gcd(a < 0 ? -a : a,b);
        p = a/g; q = b/g;
    }
    Rat operator+(Rat b) const {
        return {(i128)p*b.q + (i128)b.p*q,(i128)q*b.q};
    }
    Rat operator-(Rat b) const {
        return {(i128)p*b.q - (i128)b.p*q,(i128)q*b.q};
    }
    Rat operator*(Rat b) const { return {(i128)p*b.p,(i128)q*b.q}; }
    Rat operator/(Rat b) const { return {(i128)p*b.q,(i128)q*b.p}; }
    bool operator==(Rat b) const { return p == b.p && q == b.q; }
    bool operator<(Rat b) const { return (i128)p*b.q < (i128)b.p*q; }
};
