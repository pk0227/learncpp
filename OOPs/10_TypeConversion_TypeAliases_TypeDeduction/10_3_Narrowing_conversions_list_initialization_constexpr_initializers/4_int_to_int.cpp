#include <iostream>

void print(unsigned int u) // note: unsigned
{
    std::cout << u << '\n';
}

int main()
{
/*
    std::cout << "Enter an integral value: ";
    int n{};
    std::cin >> n; // enter 5 or -5
    print(n);      // conversion to unsigned may or may not preserve value
*/
/*
    constexpr int n1{ 5 };   // note: constexpr
    unsigned int u1 { n1 };  // okay: conversion is not narrowing due to exclusion clause

    constexpr int n2 { -5 }; // note: constexpr
    unsigned int u2 { n2 };  // compile error: conversion is narrowing due to value change
    
    std::cout << "u1 : " << u1 << std::endl;
    std::cout << "u2 : " << u2 << std::endl;
*/
    constexpr int big{1000};
    // char small{big}; // COMPILE ERROR: narrowing conversion in list-initialization (1000 out of char range)
    char small{ static_cast<char>(big) }; // OK: explicit static_cast

    std::cout << "small : " << static_cast<int>(small) << std::endl;

    return 0;
}