#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<string> SplitBySpace(string input)
{
	vector<string> output;
	string curr;
	bool isPreserved = false;
	for (int i = 0; i < input.length(); ++i)
	{
		if(input[i] != '\'')
			curr += input[i];
		if (input[i] == ' ') {
			if (!isPreserved) {
				if (curr != " ") {
					output.push_back(curr);
				}
				curr = "";
			}
		}
		else if (input[i] == '\'') {
			isPreserved = !isPreserved;
		}
	}
	output.push_back(curr);
	return output;
}

int main(void)
{
	//string input = "foo hello  'hello world'  blah";
	string input = "   foo hello 'hello world' blah";
	vector<string> output = SplitBySpace(input);
	for (string st : output) {
		cout << st << endl;
	}
	return 0;
}