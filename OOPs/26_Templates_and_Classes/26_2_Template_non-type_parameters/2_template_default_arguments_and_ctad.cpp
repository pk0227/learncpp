/**
 * @file 2_template_default_arguments_and_ctad.cpp
 * @brief Demonstrates template default arguments and C++17 Class Template Argument Deduction (CTAD).
 *
 * Concepts covered:
 * 1. Default template type parameters and non-type parameters.
 * 2. Instantiating with partial defaults and full defaults using empty angle brackets '<>'.
 * 3. C++17 CTAD: deducing class template parameters from constructor arguments.
 * 4. User-defined deduction guides: Custom mapping from constructor args to template args.
 */

#include <iostream>
#include <string>
#include <string_view>

// 1. Class template with default arguments for type and non-type parameters:
template <typename T = int, int size = 5>
class StaticBuffer
{
    T m_data[size]{};

public:
    int getSize() const { return size; }
    void set(int index, const T& val) { if (index >= 0 && index < size) m_data[index] = val; }
    const T& get(int index) const { return m_data[index]; }
};

// 2. Class template demonstrating CTAD and a custom Deduction Guide
template <typename T, typename U>
class SimplePair
{
    T m_first{};
    U m_second{};

public:
    SimplePair(const T& first, const U& second) : m_first{first}, m_second{second} {}

    void print() const
    {
        std::cout << "Pair(" << m_first << ", " << m_second << ")\n";
    }
};

// User-defined Deduction Guide for SimplePair (C++17):
template <typename T, typename U>
SimplePair(T, U) -> SimplePair<T, U>;

int main()
{
    std::cout << "--- 1. Template Default Arguments ---\n";
    // Full override
    StaticBuffer<double, 10> buf1;
    std::cout << "buf1 size: " << buf1.getSize() << " (double, 10)\n";

    // Override only type, size uses default (5)
    StaticBuffer<std::string> buf2;
    std::cout << "buf2 size: " << buf2.getSize() << " (string, default 5)\n";

    // All defaults: T = int, size = 5
    StaticBuffer<> buf3;
    std::cout << "buf3 size: " << buf3.getSize() << " (default int, default 5)\n\n";

    std::cout << "--- 2. Class Template Argument Deduction (CTAD) (C++17) ---\n";
    // Compiler deduces SimplePair<int, double> without explicit <int, double>!
    SimplePair pair1{ 42, 3.14159 };
    pair1.print();

    // Compiler deduces SimplePair<const char*, int>
    SimplePair pair2{ "Antigravity", 100 };
    pair2.print();

    return 0;
}
