#include <iostream>
#include <string>

int maxPower(std::string s) {
    int count = 1;
    int maxCount = 1;
    char prev = ' ';
    for (int i = 0; i < s.length(); ++i) {
        s[i] == prev ? ++count : count = 1;
        if (count > maxCount) {
            maxCount = count;
        }
        prev = s[i];
    }
    return maxCount;
}

int main(void) {
    std::string input = "abbcccddddeeeeedcba";

    std::cout << maxPower(input) << std::endl;

	return 0;
}