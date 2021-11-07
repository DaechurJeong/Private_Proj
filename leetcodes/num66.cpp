#include <iostream>
#include <vector>

std::vector<int> plusOne(std::vector<int>& digits)
{
    int n = digits.size();
    std::vector<int> out = digits;
    int add = 1;
    int i = n - 1;
    while (i >= 0 and add > 0) {
        int val = digits[i] + add;
        out[i--] = val % 10;
        add = val / 10;
    }
    if (add > 0) {
        out.push_back(0);
        out[0] = 1;
    }
    return out;
}
int main(void)
{
	std::vector<int> input;
	input.push_back(9);
	input.push_back(9);

	std::vector<int> result = plusOne(input);

	for (size_t i = 0; i < result.size(); ++i)
		std::cout << result[i] << std::endl;

	return 0;
}