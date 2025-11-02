#include <iostream>

int main()
{
/*
    // narrowing conversion as x is not constexpr and x int value cant be fit in y float.
    int x = 16777217;
    float y{x};
*/

    // narrowing conversion as x is not constexpr and x int value cant be fit in y float.
    constexpr int x = 16777217;
    float y{x};

    std::cout << "y : " << std::fixed << y << std::endl;
    return 0;
}