#include <iostream>

class Base
{
public:
    int m_public{};
protected:
    int m_protected{};
private:
    int m_private{};
};

class Derived : private Base // note: private inheritance
{
    // Private inheritance means:
    // Public inherited members become private (so m_public is treated as private)
    // Protected inherited members become private (so m_protected is treated as private)
    // Private inherited members stay inaccessible (so m_private is inaccessible)

public:
    Derived()
    {
        m_public = 1;           // okay: m_public was inherited as private
        m_protected = 1;        // okay: m_protected was inherited as private
        //m_private = 1;        // not okay: m_private is inaccessible from derived class
    }
};

int main()
{
    // Outside access uses the access specifiers of the class being accessed.
    // In this case, the access specifiers of base.
    Base b1{};
    b1.m_public = 1;            // okay: m_public is public in Base
    //b1.m_protected = 1;       // not okay: m_protected is protected in Base
    //b1.m_private = 1;         // not okay: m_private is private in Base

    Derived d1{};
    //d1.m_public = 1;          // not okay: m_public is now private in Derived
    //d1.m_protected = 1;       // not okay: m_protected is private in Derived
    //d1.m_private = 1;         // not okay: m_private is inaccessible in Derived

    return 0;
}