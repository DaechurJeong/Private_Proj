#include <iostream>
#include <string>


std::string multiply_string(std::string num1, std::string num2)
{
	if (num1 == "0" || num2 == "0")
		return "0";
	int len1 = num1.length(), len2 = num2.length();
	std::string res(len1 + len2, '0');
	int carry = 0;
	for (int i = len1 - 1; i >= 0; --i)
	{
		for (int j = len2 - 1; j >= 0; --j)
		{
			int multiplication = (num1[i] - '0') * (num2[j] - '0') + carry + res[i + j + 1] - '0';
			res[i + j + 1] = multiplication % 10 + '0';
			carry = multiplication / 10;
		}
		res[i] += carry;
		carry = 0;
	}
	int start = 0;
	// erase all front zeros
	while (start < res.length() && res[start] == '0')
		start++;
	if (start == res.length())
		return "0";
	return res;
}

int main(void)
{
	std::string num1 = "123456789";
	std::string num2 = "987654321";

	std::string output = multiply_string(num1, num2);

	std::cout << output << std::endl;

	return 0;
}