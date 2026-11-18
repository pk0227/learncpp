// 14_3_Const_class_objects_n_const_member_functions/15_constexpr_member_not_implicitly_const.cpp
// Demonstrates that constexpr member functions are NOT implicitly const (since C++14).
//
// C++11: a constexpr non-static member function was implicitly const.
// C++14: that rule was removed so constexpr functions could modify members during
//        compile-time evaluation. constexpr and const are now independent.
// Consequence: code that relied on the C++11 rule stops compiling on const objects
// in C++14 and later, unless const is written explicitly.
#include <iostream>

class Point
{
    int m_x{};

public:
    constexpr Point(int x) : m_x{ x } {}

    constexpr int getX() const { return m_x; }      // constexpr AND const: callable on const objects
    constexpr int getXNoConst() { return m_x; }     // C++14+: NOT const
    constexpr void setX(int x) { m_x = x; }         // C++14+: constexpr function that modifies a member
};

constexpr int shifted()
{
    Point p{ 5 };
    p.setX(7);              // modification during compile-time evaluation
    return p.getXNoConst();
}

int main()
{
    constexpr Point cp{ 3 };                 // constexpr objects are implicitly const
    static_assert(cp.getX() == 3);           // OK: getX is const
    // cp.getXNoConst();                     // ERROR (C++14+): passing 'const Point' discards qualifiers

    static_assert(shifted() == 7);
    std::cout << cp.getX() << ' ' << shifted() << '\n';
    return 0;
}
