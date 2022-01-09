#include <iostream>
#include <vector>
#include <map>

using namespace std;

string word = "";

int sum(vector<int> count)
{
	int cnt = 0;
	for (auto i : count)
		cnt += i;
	return cnt;
}

void dfs(string ans, vector<int> count, vector<string> options, int index, int max)
{
	if (ans.length() == max)
	{
		if (sum(count) == 0)
		{
			cout << "Output: " << ans << endl;
			word = ans;
		}
		return;
	}
	map <char, vector<int>> m;
	for (int i = 0; i < options.size(); i++)
	{
		if (count[i] > 0)
		{
			vector <int> arr = count;
			char ch = options[i].at(index);

			if (m.find(ch) != m.end())
				arr = m[ch];

			arr[i] -= 1;
			m[ch] = arr;
		}
	}

	for (auto i : m)
		dfs(ans + i.first, i.second, options, index + 1, max);
}

void solve(vector<int> count, vector<string> options, int max)
{
	dfs("", count, options, 0, max);
	return;
}
int main(void)
{
	vector<int> count = { 3, 0, 2, 4, 1 };
	vector<string> options = { "MOXTE", "AXCDG", "MOQRT", "FOUSE", "POWER" };

	int max = options[0].length();

	solve(count, options, max);

	cout << word << endl;

	return 0;
}