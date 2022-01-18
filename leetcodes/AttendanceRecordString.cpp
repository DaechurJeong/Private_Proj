#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool checkRecord(string s) {
	int Acount = 0;
	char prev = ' ';
	bool trigger = false;
	for (char c : s) {
		if (c == 'A') {
			++Acount;
			if (Acount >= 2) {
				return false;
			}
		}
		if (prev == 'L' && c == prev) {
			if (trigger) return false;
			trigger = true;
		}
		else {
			trigger = false;
			prev = ' ';
		}
		prev = c;
	}
	return true;
}

int main(void)
{
	string input = "LLPPPLL";
	string output = checkRecord(input) ? "TRUE" : "FALSE";
	cout << output << endl;

	return 0;
}
