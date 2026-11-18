// 14_3_Const_class_objects_n_const_member_functions/7_const_default_initialization.cpp
// Demonstrates when a const object of class type can be created WITHOUT an initializer.
//
// Rule: "const T obj;" (default-initialization) compiles only if T is const-default-constructible:
//   1. T has a USER-PROVIDED default constructor, OR
//   2. every data member of T has a default member initializer.
// Otherwise the members would hold indeterminate values that could never be changed.
// Adding {} (value-initialization) always works.
#include <iostream>

struct WithInitializers          // every member has a default member initializer
{
    int x{};
    int y{ 5 };
};

struct NoInitializers            // members are left uninitialized by default-initialization
{
    int x;
    int y;
};

struct UserProvidedCtor          // user-provided default constructor
{
    int x;
    UserProvidedCtor() : x{ 42 } {}
};

struct DefaultedCtor             // "= default" on first declaration is NOT user-provided
{
    int x;
    DefaultedCtor() = default;
};

int main()
{
    const WithInitializers a;    // OK: rule 2
    const UserProvidedCtor b;    // OK: rule 1
    // const NoInitializers c;   // ERROR: uninitialized 'const c'
    // const DefaultedCtor d;    // ERROR: defaulted ctor is not user-provided, x has no initializer

    const NoInitializers c{};    // OK: value-initialization zeroes x and y
    const DefaultedCtor d{};     // OK: value-initialization zeroes x

    std::cout << a.x << ' ' << a.y << ' ' << b.x << ' ' << c.x << ' ' << d.x << '\n';
    return 0;
}
