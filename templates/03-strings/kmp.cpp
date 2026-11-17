#include <bits/stdc++.h>
using namespace std;
constexpr int KMP_CAP = 200005;
int pi[KMP_CAP];
void prefix_function(string_view s) {
    int n = (int)s.size();
    pi[0] = 0;
    for (int i = 1, j = 0; i < n; ++i) {
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
}

vector<int> kmp(string_view text, string_view pat) {
    int n = (int)text.size(), m = (int)pat.size();
    vector<int> hits;
    if (m == 0) return hits;                 // empty pattern: no hits
    prefix_function(pat);
    for (int i = 0, j = 0; i < n; ++i) {
        while (j && text[i] != pat[j]) j = pi[j - 1];
        if (text[i] == pat[j]) ++j;
        if (j == m) {
            hits.push_back(i - m + 1);
            j = pi[j - 1];                   // allow overlapping hits
        }
    }
    return hits;
}
#if __INCLUDE_LEVEL__ == 0 // PDF: hide
int main() {}
#endif // PDF: endhide
