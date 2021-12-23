#include <iostream>
#include <vector>

int gcd(int x,int y)
{
    return y == 0 ? x : gcd(y, x % y);
}

int solve(std::vector<int> v,int K)
{
    int g = v[0];
    int max = v[0];
    for(int x : v)
    {
        g = gcd(g,x);
        max = std::max(max,x);
    }
    
    // ans=m-(K-1)*g>0
    long ans = max;
    ans -= (long)(K - 1) * g;
    return (ans > 0 ? ans : -1);
}

int main(void)
{
    std::cout << solve({1,3,5,6,7},2) << std::endl;
    std::cout << solve({1},2) << std::endl;
    std::cout << solve({1,3,5,6,7},6) << std::endl;
	return 0;
}
