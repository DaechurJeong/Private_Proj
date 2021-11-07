#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

int getSumOfDigits(int a) {
    int sum = 0;
    a = abs(a);
    while (a) {
        sum += (a % 10);
        a /= 10;
    }
    return sum;
}

int getMaxSumEqDigPair(const std::vector<int>& A) {
    std::unordered_map<int, std::priority_queue<int, std::vector<int>, std::greater<int>>> m;
    int maxSum = -1;
    for (auto a : A) {
        int sod = getSumOfDigits(a);
        m[sod].push(a);
        if (m[sod].size() > 2) {
            m[sod].pop();
        }
    }

    for (auto e : m) {
        int sum = 0;
        if (e.second.size() == 2) {
            sum += e.second.top(); e.second.pop();
            sum += e.second.top();
            maxSum = std::max(maxSum, sum);
        }
    }
    return maxSum;
}

void testGetMaxSumEqDigPair() {
    std::vector<int> a = { 51, 71, 17, 42 };
    std::vector<int> b = { 42, 33, 60 };
    std::vector<int> c = { 51, 32, 43 };
    std::cout << "maxSum = " << getMaxSumEqDigPair(a) << std::endl;
    std::cout << "maxSum = " << getMaxSumEqDigPair(b) << std::endl;
    std::cout << "maxSum = " << getMaxSumEqDigPair(c) << std::endl;

}

int main() {
    testGetMaxSumEqDigPair();
}