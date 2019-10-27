#include <iostream>

int divide(int dividend, int divisor) {
	return dividend / divisor;
}
int main(void)
{
	int dividend = 7;
	int divisor = -3;

	int result = divide(dividend, divisor);

	std::cout << result << std::endl;

	return 0;
}