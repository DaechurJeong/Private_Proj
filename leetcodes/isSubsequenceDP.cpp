#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

bool isSubsequence(string s, string t) {
    int n = s.length();
    int m = t.length();

    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            if (i == 0 || j == 0) dp[i][j] = 0;
        }
    }

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (s[i - 1] == t[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            }
            else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[n][m] == s.length();
}

int main(void)
{
    string input1 = "abc";
    string input2 = "ahbgdc";
    string output = isSubsequence(input1, input2) ? "TRUE" : "FALSE";
    cout << output << endl;
	return 0;
}