#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs)
{
	std::unordered_map<std::string, std::vector<std::string>> hashmap;
	for (auto& s : strs) {
		std::string sorted = s;
		std::sort(sorted.begin(), sorted.end());
		hashmap[sorted].push_back(s);
	}
	std::vector<std::vector<std::string>> result;
	for (auto& [key, group] : hashmap) {
		result.emplace_back(std::move(group));
	}
	return result;
}

int main(void)
{
	std::vector<std::string> input;
	input.push_back("eat");
	input.push_back("tea");
	input.push_back("tan");
	input.push_back("ate");
	input.push_back("nat");
	input.push_back("bat");

	std::vector<std::vector<std::string>> result = groupAnagrams(input);

	for (unsigned i = 0; i < result.size(); ++i)
	{
		for (unsigned j = 0; j < result[i].size(); ++j)
		{
			std::cout << result[i][j] << " ";
		}
		std::cout << std::endl;
	}

	return 0;
}