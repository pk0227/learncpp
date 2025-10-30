#include <iostream>

float doSomething()
{
    return 3.0; // double value 3.0 implicitly converted to type float
}

void doSomething(long l)
{
}

int main()
{
    double d{ 3 };  // int value 3 implicitly converted to type double
    d = 6;          // int value 6 implicitly converted to type double
    double division{ 4.0 / 3 }; // int value 3 implicitly converted to type double

    if (5) // int value 5 implicitly converted to type bool
    {
    }

    doSomething(3); // int value 3 implicitly converted to type long

    int x { 3.5 }; // brace-initialization disallows conversions that result in data loss
    
    return 0;
}