/**
 * @file 3_passing_and_returning_std_array.cpp
 * @brief Demonstrates passing and returning std::array to/from functions
 * 
 * Key Concepts:
 * 1. Pass by const reference to avoid expensive copies.
 * 2. Use function templates to accept arrays of any element type or length:
 *    template <typename T, std::size_t N>
 * 3. In C++20, non-type template parameters can use 'auto N'.
 * 4. static_assert on template parameter N for compile-time size guarantees.
 * 5. Return by value for small arrays (RVO / move); out-parameters for large arrays.
 */

#include <iostream>
#include <array>

// 1. Function template accepting any element type T and any length N
template <typename T, std::size_t N>
void printArray(const std::array<T, N>& arr)
{
    static_assert(N > 0, "Cannot print empty array");
    for (const auto& elem : arr)
    {
        std::cout << elem << ' ';
    }
    std::cout << '\n';
}

// 2. C++20 auto non-type template parameter
template <typename T, auto N>
void printFirstAndLast(const std::array<T, N>& arr)
{
    static_assert(N >= 2, "Array must have at least 2 elements");
    std::cout << "First: " << arr[0] << ", Last: " << arr[N - 1] << '\n';
}

// 3. Return by value (ideal for small arrays)
template <typename T, std::size_t N>
std::array<T, N> createFilledArray(T value)
{
    std::array<T, N> arr{};
    arr.fill(value);
    return arr; // Copy elision (RVO) avoids unnecessary copies
}

int main()
{
    constexpr std::array intArr{ 10, 20, 30, 40 };
    constexpr std::array dblArr{ 1.1, 2.2, 3.3 };

    std::cout << "intArr: ";
    printArray(intArr);

    std::cout << "dblArr: ";
    printArray(dblArr);

    printFirstAndLast(intArr);

    auto fives = createFilledArray<int, 4>(5);
    std::cout << "fives: ";
    printArray(fives);

    return 0;
}
