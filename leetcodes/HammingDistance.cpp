//
//  HammingDistance.cpp
//  Testing
//
//  Created by Daechul Jung on 12/28/21.
//

#include <iostream>

int hammingDistance(int x, int y)
{
    int xDiff = 0;
    int yDiff = 0;
    int count = 0;
    
    while(x > 0 || y > 0)
    {
        xDiff = x & 1;
        x = x >> 1;
        yDiff = y & 1;
        y = y >> 1;
        if(xDiff != yDiff) {
            ++count;
        }
    }
    return count;
}

int main(void)
{
    std::cout << hammingDistance(93, 73) << std::endl;
    return 0;
}
