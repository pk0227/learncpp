// 14_3_Const_class_objects_n_const_member_functions/14_return_by_const_value_inhibits_move.cpp
// Demonstrates why returning BY CONST VALUE is an anti-pattern in modern C++.
//
//   const std::string getName() const;   // BAD: the returned temporary is const
//   std::string getName() const;         // GOOD
//
// A const temporary cannot bind to T&& (the move constructor / move assignment parameter),
// so the caller's  x = obj.getName();  falls back to a COPY.
// Note: the trailing const (on the function) is fine. Only the const on the RETURN TYPE hurts.
//
// Historical note: pre-C++11 advice was to return const values from operators such as
// operator+ so that  (a + b) = c;  would not compile. In C++11 and later, prefer
// ref-qualifying the assignment operator instead:  T& operator=(const T&) &;
#include <iostream>

struct Tracer
{
    Tracer() = default;
    Tracer(const Tracer&) { std::cout << "  copy ctor\n"; }
    Tracer(Tracer&&) noexcept { std::cout << "  move ctor\n"; }
    Tracer& operator=(const Tracer&) { std::cout << "  copy assign\n"; return *this; }
    Tracer& operator=(Tracer&&) noexcept { std::cout << "  move assign\n"; return *this; }
};

class Holder
{
    Tracer m_t{};

public:
    const Tracer getConst() const { return m_t; }   // BAD: return type is const
    Tracer get() const { return m_t; }              // GOOD
};

int main()
{
    Holder h{};
    Tracer target{};

    std::cout << "assign from const-value return:\n";
    target = h.getConst();      // copy ctor (making the return value) + COPY assign

    std::cout << "assign from plain-value return:\n";
    target = h.get();           // copy ctor (making the return value) + MOVE assign
    return 0;
}
