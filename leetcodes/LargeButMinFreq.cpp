#include <iostream>
#include <map>
#include <algorithm>

using namespace std;

int LargButMinFreq(int arr[], int n) {
    // code here
    // value, frequency
    map<int, int> table;
    for(int i = 0; i < n; ++i)
    {
        ++table[arr[i]];
    }
    int ans = INT_MAX, bns = 0;
    for(auto val : table) {
        ans = min(ans, val.second);
    }
    for(auto val : table) {
        if(val.second == ans) {
            bns = max(bns, val.first);
        }
    }
    return bns;
}

int main(void)
{
    int sz = 5;
    int input[5] = {2,2,5,50,1};

    cout << LargButMinFreq(input, sz) << endl;

    return 0;
}