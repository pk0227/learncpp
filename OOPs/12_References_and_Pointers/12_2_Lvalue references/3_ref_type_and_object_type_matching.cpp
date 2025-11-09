#include <iostream>

int main()
{
    int i{10};
    double d{12.345};

    const int& invalidRef1{d};    // invalid: conversion of double to int is narrowing conversion, if no const qualifier, will cause compilation error
    const double& invalidRef2{i}; // invalid: non-const lvalue reference can't bind to rvalue (result of converting x to double)

    std::cout << invalidRef1 << std::endl;
    std::cout << invalidRef2 << std::endl;

    return 0;
}