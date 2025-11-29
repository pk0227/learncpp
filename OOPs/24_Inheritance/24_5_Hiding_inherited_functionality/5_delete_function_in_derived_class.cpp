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
public:
    void func() = delete;   // error: use of deleted function ‘void Derived::func()’
    void func(int) = delete;   // error: use of deleted function ‘void Derived::func(int)’
    void func(double) = delete;   // error: use of deleted function ‘void Derived::func(double)’
};

int main()
{
    Base b{};
    b.func();
    b.func(5);
    b.func(1.23);

    Derived d{};
    //d.func();         // error: use of deleted function ‘void Derived::func()’
    //d.func(5);        // error: use of deleted function ‘void Derived::func(int)’
    //d.func(1.23);     // error: use of deleted function ‘void Derived::func(double)’
    
    static_cast<Base&>(d).func();
    static_cast<Base&>(d).func(5);
    static_cast<Base&>(d).func(1.23);
    
    return 0;
}