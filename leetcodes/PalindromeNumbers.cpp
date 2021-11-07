#include <iostream>
#include <vector>

bool isPalindrome(int num)
{
	// avoid one digit numbers
	if (num < 10) return false;
	int rev = 0, val = num;
	while (val > 0)
	{
		// swap the digit of number
		rev = rev * 10 + val % 10;
		val /= 10;
	}
	if (num == rev)
		return true;
	return false;
}
bool isPalindrome_ver2(int num)
{
	std::vector<int> digits;
	int val = num;
	while (val > 0) {
		int digit = val % 10;
		digits.push_back(digit);
		val /= 10;
	}
	int sz = (int)digits.size();
	if (sz < 2) return false;
	for (int i = 0; i < sz; ++i) {
		if (digits[i] != digits[sz - i - 1]) return false;
	}
	return true;
}
std::vector<int> palindromeNumbers(int max) {
	std::vector<int> res;
	for (int i = 0; i < max; ++i) {
		if (isPalindrome(i)) {
			res.push_back(i);
		}
	}
	return res;
}

int main(void)
{
	std::vector<int> res = palindromeNumbers(10000);

	for (int i = 0; i < res.size(); ++i) {
		std::cout << res[i] << std::endl;
	}
	std::cout << "The number of palindrome number which is less than 10000 is : " << res.size() << std::endl;

	return 0;
}