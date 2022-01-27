#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

string minWindow(string s, string t) {
    if (!s.length() || !t.length()) return "";

    map<char, int> dict;
    for (int i = 0; i < t.length(); ++i) {
        dict[t[i]]++;
    }
    int minSize = dict.size();
    int left = 0, right = 0, formed = 0;

    map<char, int> windowCounts;

    // length, left, right
    vector<int> ans{ -1, 0, 0 };

    while (right < s.length()) {
        char c = s[right];
        windowCounts[c]++;
        if (dict[c] && windowCounts[c] == dict[c]) {
            ++formed;
        }
        while (left <= right && formed == minSize) {
            c = s[left];
            if (ans[0] == -1 || right - left + 1 < ans[0]) {
                ans[0] = right - left + 1;
                ans[1] = left;
                ans[2] = right;
            }
            windowCounts[c]--;
            if (dict[c] && windowCounts[c] < dict[c]) {
                --formed;
            }
            ++left;
        }
        ++right;
    }
    return ans[0] == -1 ? "" : s.substr(ans[1], ans[2] + 1);
}

int main(void)
{
    string a = "ADOBECODEBANC";
    string b = "ABC";
    cout << minWindow(a, b);
	return 0;
}