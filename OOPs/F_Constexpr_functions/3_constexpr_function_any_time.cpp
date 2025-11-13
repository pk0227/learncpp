#include <iostream>

constexpr int getValue(int x)
{
    return x;
}

int main()
{
    int x { getValue(5) }; // may evaluate at runtime or compile-time

    return 0;
}