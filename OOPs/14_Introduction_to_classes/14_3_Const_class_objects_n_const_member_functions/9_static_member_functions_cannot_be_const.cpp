// 14_3_Const_class_objects_n_const_member_functions/9_static_member_functions_cannot_be_const.cpp
// Demonstrates that static member functions cannot be const, yet const objects may call them.
//
// The trailing const of a member function qualifies the implicit object (*this).
// A static member function has no 'this' pointer, so there is nothing to make const.
// Static member functions also cannot be volatile or ref-qualified (& / &&).
// Because a static function cannot touch any object's non-static members, calling it
// through a const object is always safe, so the compiler allows it.
#include <iostream>

class Config
{
    int m_level{ 3 };
    static inline int s_instances{ 0 };   // C++17 inline static data member

public:
    Config() { ++s_instances; }

    int level() const { return m_level; }                // const: qualifies *this

    static int instances() { return s_instances; }       // no 'this', so no const
    // static int instances() const { return s_instances; }  // ERROR: static member function cannot have cv-qualifier
};

int main()
{
    const Config cfg{};
    std::cout << "level: " << cfg.level() << '\n';
    std::cout << "instances via const object: " << cfg.instances() << '\n';  // OK
    std::cout << "instances via class name:  " << Config::instances() << '\n'; // preferred
    return 0;
}
