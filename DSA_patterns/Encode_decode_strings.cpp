#include <vector>
#include <string>
#include <iostream>

using namespace std;
// Encode & Decode Strings — NeetCode #6
// Approach: length-prefix encoding — "word" -> "4#word"
// Time: O(n)  Space: O(n)
string encode(vector<string>& strs) {
    string result = "";
    for (auto& s : strs)
        result += to_string(s.size()) + "#" + s;
    return result;
}

vector<string> decode(string s) {
    vector<string> result;
    int i = 0;
    while (i < s.size()) {
        int j = s.find('#', i);          // find the delimiter
        int len = stoi(s.substr(i, j-i)); // number before '#'
        result.push_back(s.substr(j+1, len)); // grab exactly len chars
        i = j + 1 + len;                 // advance past this word
    }
    return result;
}

int main() {
    vector<string> input = {"neet", "code", "love", "you"};
    string encoded = encode(input);
    cout << "Encoded: " << encoded << "\n";

    vector<string> decoded = decode(encoded);
    cout << "Decoded: ";
    for (auto& s : decoded) cout << s << " ";
    cout << "\n";

    // edge case: string containing '#'
    vector<string> tricky = {"he#llo", "wor#ld"};
    encoded = encode(tricky);
    decoded = decode(encoded);
    cout << "Tricky: ";
    for (auto& s : decoded) cout << s << " ";
    cout << "\n";
}
