#include <iostream>

double myPow_(double x, long long n) {
	return n == 0 ? 1 : n < 0 ? 1 / myPow_(x, -n) : (n & 1 ? x : 1) * (x = myPow_(x, n / 2)) * x;
}
double myPow(double x, int n)
{
	return myPow_(x,n);
}

int main(void)
{
	std::cout << myPow(0.00001, 2147483647) << std::endl;

	return 0;
}