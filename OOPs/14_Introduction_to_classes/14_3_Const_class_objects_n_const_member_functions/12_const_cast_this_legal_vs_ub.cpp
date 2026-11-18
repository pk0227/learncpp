// 14_3_Const_class_objects_n_const_member_functions/12_const_cast_this_legal_vs_ub.cpp
// Demonstrates when casting const away from 'this' is legal and when it is undefined behavior (UB).
//
// Inside a const member function, 'this' has type const T*.
// const_cast<T*>(this) compiles in every case. Whether MODIFYING through it is legal depends on
// the object, not on the function:
//   - object created NON-const, called through a const function -> modification is legal
//   - object created const                                      -> modification is UB
// A const member function cannot tell which case it is in, so modifying through
// const_cast<T*>(this) is a bug waiting to happen. Use mutable for hidden state instead.
#include <iostream>

class Sensor
{
    int m_reads{ 0 };

public:
    int read() const
    {
        // BAD PRACTICE: compiles, but is only legal if *this is not really const
        ++const_cast<Sensor*>(this)->m_reads;
        return 42;
    }

    int reads() const { return m_reads; }
};

int main()
{
    Sensor s{};                     // created NON-const
    const Sensor& view{ s };        // const view of a non-const object
    view.read();                    // legal: the object itself was never const
    std::cout << "reads: " << s.reads() << '\n';   // 1

    const Sensor cs{};              // created const: the compiler may place it in read-only memory
    // cs.read();                   // UB: modifies an object that was created const
                                    // (may crash, may appear to work, may be optimized away)

    std::cout << "cs.reads(): " << cs.reads() << '\n';
    return 0;
}
