#include <cstdio>
#include <iostream>
#include <optional>

// use c++17
// g++ -std=c++17 -Wall -O0  -g main.cpp  -o test && ./test

int main()
{
    std::optional<double> price_abet;
    if (price_abet) {
        std::cout << "has value:" << *price_abet << std::endl;
    } else {
        std::cout << "not value! will exception: " << price_abet.value() << std::endl;
    }
    price_abet = 100;
    if (price_abet) {
        std::cout << "has value:" << price_abet.value() << std::endl;
    } else {
        std::cout << "not value!" << std::endl;
    }
    return 0;
}
