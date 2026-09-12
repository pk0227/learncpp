// Demonstrates dual-path compile-time vs runtime execution in constexpr functions
// Illustrates std::is_constant_evaluated() (C++20) and if consteval (C++23)

#include <iostream>
#include <type_traits>

constexpr int power(int base, int exp)
{
#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    // C++23 if consteval syntax:
    if consteval
    {
        int result = 1;
        for (int i = 0; i < exp; ++i)
            result *= base;
        return result;
    }
    else
    {
        std::cout << "[Runtime path via if consteval]\n";
        int result = 1;
        for (int i = 0; i < exp; ++i)
            result *= base;
        return result;
    }
#else
    // C++20 std::is_constant_evaluated() approach:
    if (std::is_constant_evaluated())
    {
        int result = 1;
        for (int i = 0; i < exp; ++i)
            result *= base;
        return result;
    }
    else
    {
        std::cout << "[Runtime path via std::is_constant_evaluated()]\n";
        int result = 1;
        for (int i = 0; i < exp; ++i)
            result *= base;
        return result;
    }
#endif
}

int main()
{
    // Compile-time evaluation:
    constexpr int compileTimeResult = power(2, 8);
    static_assert(compileTimeResult == 256, "Compile-time calculation failed");
    std::cout << "Compile-time 2^8: " << compileTimeResult << '\n';

    // Runtime evaluation:
    int b = 3;
    int e = 4;
    int runtimeResult = power(b, e);
    std::cout << "Runtime 3^4: " << runtimeResult << '\n';

    return 0;
}
