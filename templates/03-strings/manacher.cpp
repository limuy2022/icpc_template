vector<int> palindrome_radius(string_view s, bool odd) {
    int n = (int)s.size(), even = !odd;
    vector<int> d(n);
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = i > r ? (int)odd
                      : min(d[l + (r - i) + even], r - i + 1);
        while (i - k - even >= 0 && i + k < n &&
               s[i - k - even] == s[i + k]) ++k;
        d[i] = k;
        if (i + k - 1 > r) { l = i - k + odd; r = i + k - 1; }
    }
    return d;
}
