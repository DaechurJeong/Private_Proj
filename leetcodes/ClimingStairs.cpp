#include <iostream>
#include <vector>

using namespace std;

int climbStairs(int n) 
{
	vector<int> container(n+1, 0);
	container[0] = 1;
	container[1] = 1;
	for (int i = 2; i <= n; ++i)
	{
		container[i] = container[i - 1] + container[i - 2];
	}
	return container[n];
}

int main(void)
{
	std::cout << climbStairs(2) << std::endl;
	return 0;
}