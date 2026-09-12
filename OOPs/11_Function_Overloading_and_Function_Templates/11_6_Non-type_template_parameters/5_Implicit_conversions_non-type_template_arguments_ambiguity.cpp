#include <iostream>

template <int N>
void print()
{
    std::cout << "N : " << N << '\n';
}

template <char C>
void print()
{
    std::cout << "C : " << C << '\n';
}

int main()
{
    // print<5>();   // COMPILE ERROR: call of overloaded 'print<5>()' is ambiguous (5 can be int N or char C)
    // print<'A'>(); // COMPILE ERROR: call of overloaded 'print<'A'>()' is ambiguous ('A' can be char C or int N)

    // Ambiguity occurs because non-type template arguments allow conversions between integral types.
    // To resolve, avoid identically named function templates differing solely by convertible non-type parameter types.
    return 0;
}