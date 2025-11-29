#include <iostream>

class Base
{
public:
    void func() { std::cout << "Base - func()\n"; }
    void func(int) { std::cout << "Base - func(int)\n"; }
    void func(double) { std::cout << "Base - func(double)\n"; }
};

class Derived : public Base
{
    using Base::func;
};

int main()
{
    Base b{};
    b.func();
    b.func(5);
    b.func(1.23);

    Derived d{};
    //d.func();     // error: ‘void Base::func()’ is inaccessible within this context
    //d.func(5);     // error: ‘void Base::func(int)’ is inaccessible within this context
    //d.func(1.23);     // error: ‘void Base::func(double)’ is inaccessible within this context
    
    static_cast<Base&>(d).func();
    static_cast<Base&>(d).func(5);
    static_cast<Base&>(d).func(1.23);
    
    return 0;
}