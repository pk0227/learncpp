#include <iostream>

int add(int x, int y)
{
    return x + y;
}

int main()
{
    auto sum { add(5, 6) }; // add() returns an int, so sum's type will be deduced as an int

    auto a1{1.23f}; // f suffix causes a to be deduced to float
    auto b1{5u};    // u suffix causes b to be deduced to unsigned int

    int a2 { 5 };            // a is an int
    const auto b2 { 5 };     // b is a const int
    constexpr auto c2 { 5 }; // c is a constexpr int

    auto s1 { "Hello, world" }; // s will be type const char*, not std::string

    using namespace std::literals; // easiest way to access the s and sv suffixes
    auto s2 { "goo"s };  // "goo"s is a std::string literal, so s1 will be deduced as a std::string
    auto s3 { "moo"sv }; // "moo"sv is a std::string_view literal, so s2 will be deduced as a std::string_view

    constexpr double a3 { 3.4 };  // a has type const double (constexpr not part of type, const is implicit)
    auto b3 { a3 };                // b has type double (const dropped)
    const auto c3 { a3 };          // c has type const double (const dropped but reapplied)
    constexpr auto d3 { a3 };      // d has type const double (const dropped but implicitly reapplied by constexpr)

    return 0;
}