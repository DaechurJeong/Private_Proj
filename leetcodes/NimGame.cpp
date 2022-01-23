#include <iostream>

using namespace std;

bool canWinNim(int n) {
    return n % 4 != 0;
}

int main(void)
{
    string output = canWinNim(8) ? "TRUE" : "FALSE";
    cout << output << endl;
	return 0;
}