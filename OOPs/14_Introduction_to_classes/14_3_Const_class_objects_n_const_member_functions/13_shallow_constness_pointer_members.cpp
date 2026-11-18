// 14_3_Const_class_objects_n_const_member_functions/13_shallow_constness_pointer_members.cpp
// Demonstrates that const is SHALLOW: it protects the members themselves, not what they point or refer to.
//
// Inside a const member function:
//   int* m_ptr               becomes  int* const   -> pointer locked, pointee still writable
//   int& m_ref               the referent is still writable (a reference is never reseatable anyway)
//   std::unique_ptr<int> m_up  becomes const unique_ptr -> operator* still returns int&
//   std::vector<int> m_vec   IS deep-const: vector's const operator[] returns const int&
//
// So a const member function can silently change data the object "owns" through a pointer.
// Fixes:
//   1. Expose access through const/non-const accessor pairs that return const T& / T&.
//   2. std::experimental::propagate_const (Library Fundamentals TS v2) makes the pointee const
//      in const contexts. Available in libstdc++ and libc++, not yet in the C++ standard.
#include <experimental/propagate_const>
#include <iostream>
#include <memory>
#include <vector>

class Widget
{
    int* m_ptr{};
    int& m_ref;
    std::unique_ptr<int> m_up{ std::make_unique<int>(3) };
    std::vector<int> m_vec{ 1, 2, 3 };

public:
    Widget(int* p, int& r) : m_ptr{ p }, m_ref{ r } {}

    void sneaky() const
    {
        *m_ptr = 100;      // compiles: pointee is not const
        m_ref = 200;       // compiles: referent is not const
        *m_up = 300;       // compiles: const unique_ptr still gives int&
        // m_ptr = nullptr;   // ERROR: the pointer itself is const
        // m_vec[0] = 9;      // ERROR: vector is deep-const
    }

    int up() const { return *m_up; }
};

class SafeWidget
{
    std::experimental::propagate_const<std::unique_ptr<int>> m_up{ std::make_unique<int>(3) };

public:
    void tryModify() const
    {
        // *m_up = 300;    // ERROR: propagate_const makes the pointee const inside const functions
    }
    void modify() { *m_up = 300; }   // OK: non-const function
    int get() const { return *m_up; }
};

int main()
{
    int a{ 1 };
    int b{ 2 };
    const Widget w{ &a, b };     // a const object...
    w.sneaky();                  // ...that just changed three values
    std::cout << a << ' ' << b << ' ' << w.up() << '\n';   // 100 200 300

    SafeWidget sw{};
    sw.modify();
    std::cout << sw.get() << '\n';                         // 300
    return 0;
}
