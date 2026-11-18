// 14_3_Const_class_objects_n_const_member_functions/16_cv_qualified_member_functions.cpp
// Demonstrates volatile and const volatile member functions (cv-qualifiers on member functions).
//
// The trailing qualifier of a member function sets the type of *this:
//   void f();                 -> this is T*
//   void f() const;           -> this is const T*
//   void f() volatile;        -> this is volatile T*
//   void f() const volatile;  -> this is const volatile T*
// All four can be overloaded. A volatile object can only call volatile (or const volatile)
// member functions, exactly like a const object can only call const ones.
//
// Use case: classes that model memory-mapped hardware registers. volatile is NOT a
// thread-synchronization tool; use std::atomic or a mutex for threads.
#include <iostream>

struct Reg
{
    unsigned value{};

    void show()                { std::cout << "plain\n"; }
    void show() const          { std::cout << "const\n"; }
    void show() volatile       { std::cout << "volatile\n"; }
    void show() const volatile { std::cout << "const volatile\n"; }
};

int main()
{
    Reg r{};
    const Reg cr{};
    volatile Reg vr{};
    const volatile Reg cvr{};

    r.show();      // plain
    cr.show();     // const
    vr.show();     // volatile
    cvr.show();    // const volatile
    return 0;
}
