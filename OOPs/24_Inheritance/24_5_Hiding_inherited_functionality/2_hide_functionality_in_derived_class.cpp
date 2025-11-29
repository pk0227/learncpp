#include <iostream>

class Base
{
public:
    int m_value{};
};

class Derived : public Base
{
    using Base::m_value;
};

int main()
{
    Base b{};
    std::cout << b.m_value << "\n";

    Derived d{};
    //std::cout << d.m_value << "\n";     // error: ‘int Base::m_value’ is inaccessible within this context
    std::cout << static_cast<const Base&>(d).m_value << "\n";
    
    return 0;
}