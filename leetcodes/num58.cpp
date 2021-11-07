#include <iostream>
#include <sstream>
#include <string>

int lengthOfLastWord(std::string s) {
    std::stringstream ss(s);
    std::string str;

    while (ss >> str) {
        continue;
    }

    return str.length();
}

int main(void)
{
	std::string input = "Hello World";

	int result = lengthOfLastWord(input);

	std::cout << result << std::endl;

	return 0;
}