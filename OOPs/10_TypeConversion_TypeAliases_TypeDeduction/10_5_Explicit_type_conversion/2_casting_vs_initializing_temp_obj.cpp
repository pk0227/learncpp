#include <iostream>

int main()
{
    // 9223372036854998 : within double range.
    // 9223372036854999 : beyond double range.
    constexpr long long x = 9223372036854998; // constant expression of type long long will make compiler be aware of its value at compile time and 
                                              // able to catch narrowing conversions at compile time for direct list initialization.
    int y { 4 };

    // We want to do floating point division, so one of the operands needs to be a floating point type
    std::cout << double{x} / y << '\n'; // okay if x is 32-bit or within double range, narrowing if x is 64-bit or beyond double range.

    // std::cout << double{9223372036854999} / y << '\n'; // COMPILE ERROR: direct list initialization disallows narrowing conversion!
    std::cout << static_cast<double>(9223372036854999) / y << '\n'; // OK: static_cast explicitly permits narrowing conversion

    return 0;
}