#include <iostream>
#include <string>
#include <map>
#include <vector>

std::string ReadThreeDigit(int num) {
    std::map<int, std::string> SecondDigit;
    SecondDigit.emplace(1, "Ten");
    SecondDigit.emplace(2, "Twenty");
    SecondDigit.emplace(3, "Thirty");
    SecondDigit.emplace(4, "Forty");
    SecondDigit.emplace(5, "Fifty");
    SecondDigit.emplace(6, "Sixty");
    SecondDigit.emplace(7, "Seventy");
    SecondDigit.emplace(8, "Eighty");
    SecondDigit.emplace(9, "Ninety");
    SecondDigit.emplace(11, "Eleven");
    SecondDigit.emplace(12, "Twelve");
    SecondDigit.emplace(13, "Thirteen");
    SecondDigit.emplace(14, "Fourteen");
    SecondDigit.emplace(15, "Fifteen");
    SecondDigit.emplace(16, "Sixteen");
    SecondDigit.emplace(17, "Seventeen");
    SecondDigit.emplace(18, "Eighteen");
    SecondDigit.emplace(19, "Nineteen");

    std::map<int, std::string> FirstThirdDigit;
    FirstThirdDigit.emplace(1, "One");
    FirstThirdDigit.emplace(2, "Two");
    FirstThirdDigit.emplace(3, "Three");
    FirstThirdDigit.emplace(4, "Four");
    FirstThirdDigit.emplace(5, "Five");
    FirstThirdDigit.emplace(6, "Six");
    FirstThirdDigit.emplace(7, "Seven");
    FirstThirdDigit.emplace(8, "Eight");
    FirstThirdDigit.emplace(9, "Nine");

    std::string ret = "";
    // 3 read
    for (int j = 2; j >= 0; --j) {
        if (num < 20 && num > 10) {
            ret += SecondDigit[num];
            ret += " ";
            break;
        }
        int digit = num / pow(10, j);
        if (digit == 0) continue;
        num -= (digit * pow(10, j));
        switch (j) {
        case 0:
            // FirstThirdDigit
            ret += FirstThirdDigit[digit];
            break;
        case 1:
            // Second Digit
            ret += SecondDigit[digit];
            break;
        case 2:
            // FirstThirdDigit
            ret += FirstThirdDigit[digit];
            ret += " Hundred";
            break;
        default:
            break;
        }
        ret += " ";
    }
    ret.pop_back();
    return ret;
}
std::string numberToWords(int num) {
    if (num == 0) return "Zero";
    std::string ret = "";
    std::vector<std::string> BigDigits{ "", " Thousand", " Million", " Billion" };
    // 3 BILLION, MILLION, THOUSAND
    int tmp = num;
    for (int i = 3; i >= 0; --i) {
        int bdigit = num / pow(1000, i);
        if (bdigit == 0) continue;
        ret += ReadThreeDigit(bdigit);
        ret += BigDigits[i];
        if (i > 0) {
            num -= bdigit * pow(1000, i);
        }
        ret += " ";
    }
    ret.pop_back();
    return ret;
}

int main(void)
{
    std::cout << numberToWords(1234567891);
	return 0;
}