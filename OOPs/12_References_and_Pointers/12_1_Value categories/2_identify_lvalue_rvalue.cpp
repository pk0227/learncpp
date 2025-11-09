#include <iostream>
#include <string>
#include <iomanip>

// T& is an lvalue reference, so this overload will be preferred for lvalues
template <typename T>
constexpr bool is_lvalue(T&)
{
    return true;
}

// T&& is an rvalue reference, so this overload will be preferred for rvalues
template <typename T>
constexpr bool is_lvalue(T&&)
{
    return false;
}

int getInt()
{
    return 5;
}

// A helper macro (#expr prints whatever is passed in for expr as text)
#define PRINTVCAT(exp)  {std::cout << std::left << std::setw(25) << #exp << " is an " << (is_lvalue(exp) ? "lvalue\n" : "rvalue\n");}

int main()
{
    PRINTVCAT(5);
    PRINTVCAT(getInt());

    int x{6};
    PRINTVCAT(x);
    PRINTVCAT(std::string {"Hello"});
    PRINTVCAT("Hello");
    PRINTVCAT(x++);
    PRINTVCAT(++x); 

    return 0;
}