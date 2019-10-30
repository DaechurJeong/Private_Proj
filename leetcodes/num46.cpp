#include <iostream>
#include <vector>
#include <algorithm>

std::vector< std::vector< int > > permute(std::vector<int>& nums)
{
	std::vector< std::vector< int > > result;
    if(nums.empty())
        return result;
    sort(nums.begin(), nums.end());
        
    do{
        result.push_back(nums);
    }while(std::next_permutation(nums.begin(), nums.end()));
    return result;
}

int main(void)
{
	std::vector<int> input;
	input.push_back(1);
	input.push_back(2);
	input.push_back(3);

	std::vector< std::vector< int > > result = permute(input);

	for(unsigned i = 0; i < result.size(); ++i)
	{
		for(unsigned j = 0; j < result[i].size(); ++j)
			std::cout << result[i][j] << " ";
		std::cout << std::endl;
	}

	return 0;
}