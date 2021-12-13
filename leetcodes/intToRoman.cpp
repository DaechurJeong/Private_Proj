#include <iostream>
#include <string>
#include <vector>
#include <map>
/*std::string intToRoman(int num) {
    std::string m[] = { "", "M", "MM", "MMM" };
    std::string c[] = { "", "C", "CC", "CCC", "CD", "D",
                       "DC", "DCC", "DCCC", "CM" };
    std::string x[] = { "", "X", "XX", "XXX", "XL", "L",
                       "LX", "LXX", "LXXX", "XC" };
    std::string i[] = { "", "I", "II", "III", "IV", "V",
                       "VI", "VII", "VIII", "IX" };

    // Converting to roman 
    std::string thousands = m[num / 1000];
    std::string hundereds = c[(num % 1000) / 100];
    std::string tens = x[(num % 100) / 10];
    std::string ones = i[num % 10];

    std::string ans = thousands + hundereds + tens + ones;

    return ans;
}*/
std::string intToRoman(int num) {

    std::map<int, std::string, std::greater <int>> m;
    m.insert({ 1000,"M" });
    m.insert({ 900,"CM" });
    m.insert({ 500,"D" });
    m.insert({ 400,"CD" });
    m.insert({ 100,"C" });
    m.insert({ 90,"XC" });
    m.insert({ 50,"L" });
    m.insert({ 40,"XL" });
    m.insert({ 10,"X" });
    m.insert({ 9,"IX" });
    m.insert({ 5,"V" });
    m.insert({ 4,"IV" });
    m.insert({ 1,"I" });
    std::string ret = "";
    for (std::pair<int, std::string> x : m) {
        while (num >= x.first) {
            ret += x.second;
            num -= x.first;
        }
    }

    return ret;
}

int main(void)
{
    std::cout << intToRoman(12) << std::endl;
    std::cout << intToRoman(4) << std::endl;
    std::cout << intToRoman(9) << std::endl;
    std::cout << intToRoman(58) << std::endl;
    std::cout << intToRoman(1994) << std::endl;

	return 0;
}