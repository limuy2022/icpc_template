#include <bits/stdc++.h>
using namespace std;

int main() { // PDF: body
    string csv = "a,,b,";
    istringstream in(csv);
    string field, joined;
    bool first = true;
    while (getline(in, field, ',')) {
        if (!first) joined += '|';
        joined += field;
        first = false;
    }
    if (!csv.empty() && csv.back() == ',') joined += '|';
    // joined == "a||b|"; getline alone skips the final empty field
    string_view view = csv;
    auto part = view.substr(3, 1); // "b", no copy
}
