// 14_3_Const_class_objects_n_const_member_functions/8_constness_during_construction_destruction.cpp
// Demonstrates WHEN an object becomes const and when it stops being const.
//
// - const does NOT apply while the constructor runs. The constructor of a const object
//   can (and must) write to the members. Constness begins when the constructor finishes.
// - const stops applying when the destructor starts. The destructor of a const object
//   can modify members (e.g. release resources, reset handles).
// - That is why constructors and destructors can never be declared const.
#include <iostream>

class Counter
{
    int m_value{};

public:
    Counter(int start)
    {
        m_value = start;      // OK even for a const Counter: constness has not started yet
        m_value *= 2;         // still OK: assignment in the body, not only initialization
        std::cout << "ctor: m_value = " << m_value << '\n';
    }

    ~Counter()
    {
        m_value = 0;          // OK even for a const Counter: constness has ended
        std::cout << "dtor: m_value reset to " << m_value << '\n';
    }

    int get() const { return m_value; }
    void set(int v) { m_value = v; }

    // Counter() const;      // ERROR: constructors cannot be const
    // ~Counter() const;     // ERROR: destructors cannot be const
};

int main()
{
    {
        const Counter c{ 21 };                   // constructor runs with a non-const 'this'
        std::cout << "get(): " << c.get() << '\n'; // const from here...
        // c.set(5);                             // ERROR: c is const now
    }                                            // ...until the destructor starts
    return 0;
}
