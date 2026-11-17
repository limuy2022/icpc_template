string s = "ababa";
auto i = s.find("ba");              // 1
auto j = s.find("ba", 2);           // 3; search from index 2
auto k = s.rfind("ba");             // 3; last occurrence
auto p = s.find_first_of("bx");     // 1; any char in "bx"
string sub = s.substr(1, 2);        // "ba"; position, length
bool starts = s.compare(0, 2, "ab") == 0;
s.insert(2, "XY");                  // "abXYaba"
s.replace(2, 2, "Z");               // "abZaba"
s.erase(2, 1);                     // "ababa"
s += "!"; s.push_back('?'); s.pop_back();
string bits = "101";
const char* last = bits.data() + bits.size();
long long x;
auto [end, ec] = from_chars(bits.data(), last, x, 2);
if (ec == errc{} && end == last) { string num = to_string(x); }
istringstream in("one two");        // split by whitespace
string token;
while (in >> token) { /* use token */ }
