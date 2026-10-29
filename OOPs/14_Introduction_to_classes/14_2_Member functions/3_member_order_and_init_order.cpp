// Member order rules: the complete-class context vs. initialization order
//
// Two DIFFERENT rules that are easy to confuse:
//   1. Inside a member function BODY -> order-independent (complete-class context).
//      You may use a data member or call a member function declared textually BELOW.
//   2. Inside a default member initializer / member initializer list -> order-DEPENDENT.
//      Using a member declared later reads it before it is initialized -> undefined behavior.

#include <iostream>

// ---------------------------------------------------------------------------
// Rule 1: complete-class context -- member function bodies are order-independent
// ---------------------------------------------------------------------------
struct Widget
{
    // 'value' is declared LATER (below), yet we can use it here. This is legal
    // because a member function body is analyzed as if the whole class is complete.
    int getDouble() const { return value * 2; }

    // a() calls b(), which is declared below it -- also legal.
    int a() { return b(); }
    int b() { return 42; }

    int value{ 5 }; // declared after the functions that use it
};

// ---------------------------------------------------------------------------
// Rule 2: initialization order IS declaration order (constructor / initializers)
// ---------------------------------------------------------------------------
struct Good
{
    int m_x{ 3 };
    int m_y{ m_x + 1 }; // OK: m_x is declared BEFORE m_y, so it is already initialized
};

struct Bad
{
    // m_a is initialized FIRST (declared first), but its initializer reads m_b,
    // which has NOT been initialized yet -> undefined behavior. Avoid this.
    int m_a{ /* m_b */ 0 }; // (left as 0 here; using m_b would be UB)
    int m_b{ 10 };
};

int main()
{
    Widget w{};
    std::cout << "getDouble() (uses member declared later): " << w.getDouble() << '\n';
    std::cout << "a() -> b() (calls member declared later):  " << w.a() << '\n';

    Good g{};
    std::cout << "Good: m_x=" << g.m_x << " m_y=" << g.m_y << " (m_y safely used m_x)\n";

    Bad bad{};
    std::cout << "Bad:  m_a=" << bad.m_a << " m_b=" << bad.m_b
              << " (reading m_b inside m_a's initializer would be UB)\n";

    return 0;
}
