#include <iostream>

int main()
{
/*
    // narrowing conversion as x is not constexpr and x int value cant be fit in y float.
    int x = 16777217;
    float y{x};
*/

    // Narrowing conversion: constexpr int value 16777217 (2^24 + 1) cannot fit exactly into 24-bit float mantissa
    constexpr int x = 16777217;
    // float y{x}; // COMPILE ERROR: narrowing conversion in list-initialization
    float y{ static_cast<float>(x) }; // OK: explicit static_cast acknowledges loss of precision

    std::cout << "y : " << std::fixed << y << std::endl;
    return 0;
}