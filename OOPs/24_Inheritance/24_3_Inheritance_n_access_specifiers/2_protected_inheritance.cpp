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

class Derived : protected Base // note: protected inheritance
{
public:
    Derived()
    {
        m_public = 1;           
        m_protected = 1;        
        //m_private = 1;   
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
    //d1.m_public = 1;          // not okay: m_public is protected in Derived
    //d1.m_protected = 1;       // not okay: m_protected is protected in Derived
    //d1.m_private = 1;         // not okay: m_private is inaccessible in Derived

    return 0;
}