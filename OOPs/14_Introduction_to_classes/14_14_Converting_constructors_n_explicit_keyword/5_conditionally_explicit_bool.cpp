// 14_14_Converting_constructors_n_explicit_keyword/5_conditionally_explicit_bool.cpp
// Demonstrates C++20 conditionally explicit constructors using explicit(bool)

#include <iostream>
#include <type_traits>

template <typename T>
class Wrapper
{
    T m_val{};

public:
    // In C++20, explicit(bool) allows conditional explicitness:
    // If T is not convertible from U, or conversion is narrowing/explicit, mark constructor explicit.
    template <typename U>
    explicit(!std::is_convertible_v<U, T>) Wrapper(U&& u)
        : m_val(std::forward<U>(u))
    {
    }

    void print() const
    {
        std::cout << "Wrapper holds: " << m_val << '\n';
    }
};

struct ExplicitBoolDemo
{
    int m_value{};

    // explicit(true) behaves as explicit
    explicit(true) ExplicitBoolDemo(int v) : m_value{v} {}
};

void takeWrapperInt(Wrapper<int> w)
{
    w.print();
}

int main()
{
    // 1. explicit(false) allows implicit conversion:
    takeWrapperInt(42); // int is convertible to int -> explicit(false) -> implicit conversion allowed

    // 2. explicit(true) prevents implicit copy initialization:
    // ExplicitBoolDemo demo1 = 10; // COMPILE ERROR: constructor is explicit(true)
    ExplicitBoolDemo demo2{10};    // OK: direct list-initialization

    std::cout << "ExplicitBoolDemo value: " << demo2.m_value << '\n';

    return 0;
}
