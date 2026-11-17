struct XorBasis {
    u64 b[64]{};
    int rank = 0;
    bool dependent = false;
    bool insert(u64 x) {
        for (int i = 63; i >= 0; --i) if ((x>>i)&1ULL) {
            if (!b[i]) { b[i] = x; ++rank; return true; }
            x ^= b[i];
        }
        dependent = true;
        return false;
    }
    bool contains(u64 x) const {
        for (int i = 63; i >= 0; --i) if ((x>>i)&1ULL) {
            if (!b[i]) return false;
            x ^= b[i];
        }
        return true;
    }
    u64 max_xor(u64 seed=0) const {
        for (int i = 63; i >= 0; --i) seed = max(seed,seed^b[i]);
        return seed;
    }
    u64 min_xor(u64 seed=0) const {
        for (int i = 63; i >= 0; --i) seed = min(seed,seed^b[i]);
        return seed;
    }
    void merge(const XorBasis& other) {
        dependent = dependent || other.dependent;
        for (u64 x : other.b) if (x) insert(x);
    }
    void ordered_vectors(u64 out[]) const;
    optional<u64> kth(u128 k,bool nonempty=false) const;
};
