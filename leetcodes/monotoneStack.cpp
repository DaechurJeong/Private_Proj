#include <iostream>
#include <vector>
#include <set>

std::vector<std::pair<int, int>> AvailableDirection(std::vector<std::vector<char>> map, std::vector<bool> history, int cur_row, int cur_col)
{
	std::vector<std::pair<int, int>> result;
	if ((cur_col - 1) >= 0) // up
	{
		if (!history[(cur_col - 1) * map.size() + cur_row])
			result.push_back(std::make_pair(0, -1));
	}
	if ((cur_col + 1) < map.size()) // bottom
	{
		if (!history[(cur_col + 1) * map.size() + cur_row])
			result.push_back(std::make_pair(0, 1));
	}
	if ((cur_row - 1) >= 0) // left
	{
		if (!history[cur_col * map.size() + cur_row - 1])
			result.push_back(std::make_pair(-1, 0));
	}
	if ((cur_row + 1) < map.size()) // right
	{
		if (!history[cur_col * map.size() + cur_row + 1])
			result.push_back(std::make_pair(1, 0));
	}
	return result;
}
bool FindAlphabet(std::set<std::string> dic, std::string cur_str)
{
	for(auto it = dic.begin(); it != dic.end(); ++it) {
		if (it->find(cur_str) == 0 && cur_str.length() <= it->length()) {
			return true;
		}
	}
	return false;
}
std::string FindWord_aux(std::vector<std::vector<char>> map, std::set<std::string> dictionary, std::vector<bool>& visited, int row, int col, std::string curr_string)
{
	visited[col * map.size() + row] = true;
	if (FindAlphabet(dictionary, curr_string)) {
		std::set<std::string>::iterator it = std::find(dictionary.begin(), dictionary.end(), curr_string);
		if (it != dictionary.end())
			return *it;

		std::vector<std::pair<int, int>> available_dir = AvailableDirection(map, visited, row, col);
		for (unsigned i = 0; i < available_dir.size(); ++i) {
			row += available_dir[i].first;
			col += available_dir[i].second;
			curr_string.push_back(map[row][col]);

			// find word...	
			std::string result = FindWord_aux(map, dictionary, visited, row, col, curr_string);
			if (!result.empty())
				return result;

			curr_string.pop_back();
			row = row;
			col = col;
		}
	}
	return {};
}
void FindWord(std::vector<std::vector<char>> map, std::set<std::string> dictionary, int row, int col, std::set<std::string>& result_dic)
{
	std::vector<bool> visited(100, false);
	std::string curr_string = "";
	visited[col * map.size() + row] = true;
	curr_string.push_back(map[row][col]);
	std::string result_string = FindWord_aux(map, dictionary, visited, row, col, curr_string);
	if (!result_string.empty())
		result_dic.insert(result_string);
}
int main(void)
{
	// method 1
	/*const char* A = "This is A";
	const char* B = "This is B";

	char* AB = new char[strlen(A) + strlen(B)];

	strcpy(AB, A);
	strcat(AB, B);*/

	// method 2
	/*std::string A = "This is A";
	std::string B = "This is B";

	std::string AB;
	for (unsigned i = 0; i < A.length(); ++i)
		AB.push_back(A[i]);
	for (unsigned i = 0; i < B.length(); ++i)
		AB.push_back(B[i]);

	std::cout << AB << std::endl;*/

	std::vector<std::vector<char>> map;
	for (unsigned i = 0; i < 10; ++i) {
		std::vector<char> row;
		for (unsigned j = 0; j < 10; ++j) {
			row.push_back(rand() % 26 + 97);
		}
		map.push_back(row);
	}
	std::set<std::string> dictionary;

	dictionary.insert("w");
	dictionary.insert("abc");
	dictionary.insert("eng");
	dictionary.insert("is");
	dictionary.insert("the");
	dictionary.insert("app");
	dictionary.insert("we");
	dictionary.insert("you");
	dictionary.insert("im");
	dictionary.insert("www");
	dictionary.insert("emu");
	dictionary.insert("kkk");
	std::set<std::string> result_dic;
	unsigned num_words = 0;
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j)
			FindWord(map, dictionary, i, j, result_dic);
	}
	for (auto it = result_dic.begin(); it != result_dic.end(); ++it)
		std::cout << *it << std::endl;

	return 0;
}