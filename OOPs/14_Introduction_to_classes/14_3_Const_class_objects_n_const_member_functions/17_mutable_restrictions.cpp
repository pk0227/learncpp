// 14_3_Const_class_objects_n_const_member_functions/17_mutable_restrictions.cpp
// Demonstrates where the mutable keyword may and may not be used.
//
// mutable is allowed only on non-static, non-const, non-reference data members.
//   - static:    a static member does not belong to any object, so object constness never applies
//   - const:     "mutable const" contradicts itself
//   - reference: a reference can never be reseated, so there is nothing to exempt
//                (the referent is already modifiable through a reference member, see shallow constness)
//   - functions: mutable cannot be applied to member functions
// A mutable member of class type may itself be modified via its non-const member functions.
#include <iostream>
#include <string>

struct Example
{
    mutable int counter{};                 // OK
    mutable std::string note{};            // OK: class-type member, can call non-const functions on it

    // mutable static int s_count;         // ERROR: static members cannot be mutable
    // mutable const int c{};              // ERROR: const members cannot be mutable
    // mutable int& ref;                   // ERROR: reference members cannot be mutable
    // mutable void f();                   // ERROR: mutable applies only to data members

    void touch() const
    {
        ++counter;
        note.append("x");                  // non-const std::string member function, allowed
    }
};

int main()
{
    const Example e{};
    e.touch();
    e.touch();
    std::cout << e.counter << ' ' << e.note << '\n';   // 2 xx
    return 0;
}
