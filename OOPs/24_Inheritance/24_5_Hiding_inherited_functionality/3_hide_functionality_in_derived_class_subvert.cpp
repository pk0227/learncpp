#include <iostream>

class Base
{
public:
    virtual void func()
    {
        std::cout << "Base - public - func()\n";
    }
};

class Derived : public Base
{
private:
    virtual void func()
    {
        std::cout << "Derived - private - func()\n";
    }
};

int main()
{
    Base b{};
    b.func();

    Derived d{};
    //d.func();     // error: ‘virtual void Derived::func()’ is private within this context
    static_cast<Base&>(d).func();       // okay: Base::func() is public, resolves to private Derived::func() at runtime
                                        // calls Derived func, not Base func. Because it is virtual 

    return 0;
}