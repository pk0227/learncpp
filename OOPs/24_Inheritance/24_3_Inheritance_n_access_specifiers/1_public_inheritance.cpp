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

class Derived : public Base // note: public inheritance
{
    // Public inheritance means:
    // Public inherited members stay public (so m_public is treated as public)
    // Protected inherited members stay protected (so m_protected is treated as protected)
    // Private inherited members stay inaccessible (so m_private is inaccessible)

public:
    Derived()
    {
        m_public = 1;           // okay: m_public was inherited as public
        m_protected = 1;        // okay: m_protected was inherited as protected
        //m_private = 1;        // not okay: m_private is inaccessible from derived class
    }
};

int main()
{
    // Outside access uses the access specifiers of the class being accessed.
    Base b1{};
    b1.m_public = 1;            // okay: m_public is public in Base
    //b1.m_protected = 1;       // not okay: m_protected is protected in Base
    //b1.m_private = 1;         // not okay: m_private is private in Base

    Derived d1{};
    d1.m_public = 1;            // okay: m_public is public in Derived
    //d1.m_protected = 1;       // not okay: m_protected is protected in Derived
    //d1.m_private = 1;         // not okay: m_private is inaccessible in Derived

    return 0;
}