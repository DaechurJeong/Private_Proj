#include <iostream>
#include <string>

/* Way 1 */
bool isAnagram_ONE(std::string s, std::string t) {
	if (s == t) return true;
	if (s.length() != t.length()) return false;

	bool eraseFlag = false;
	for (auto it = s.begin(); it != s.end(); ++it) {
		if (eraseFlag) {
			it--;
			eraseFlag = false;
		}
		for (auto it2 = t.begin(); it2 != t.end(); ++it2) {
			if ((*it) == (*it2)) {
				// found s[i] character in t[j]
				// pop both
				s.erase(it);
				t.erase(it2);
				eraseFlag = true;
				break;
			}
		}
		if (!eraseFlag) return false;
	}
	if (s == t) return true;
	return false;
}
/* Way 2 */
bool isAnagram_TWO(std::string s, std::string t)
{
	if (s == t) return true;
	if (s.length() != t.length()) return false;
	
	int frequency[26] = { 0 };
	for (int i = 0; i < s.length(); ++i) {
		++frequency[s[i] - 'a'];
	}
	for (int i = 0; i < t.length(); ++i) {
		if (--frequency[t[i] - 'a'] < 0) {
			return false;
		}
	}
	return true;
}

int main(void)
{
	std::string s = "anagtam";
	std::string t = "nbgbram";

	std::string res = isAnagram_ONE(s, t) ? "TRUE" : "FALSE";
	std::string res2 = isAnagram_TWO(s, t) ? "TRUE" : "FALSE";
	std::cout << res << std::endl;
	std::cout << res2 << std::endl;

	return 0;
}