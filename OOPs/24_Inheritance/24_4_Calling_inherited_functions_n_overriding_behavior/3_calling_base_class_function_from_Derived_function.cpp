#include <iostream>

class Base
{
public:
    void whichClass() const { std::cout << "Base class\n"; }
};

class Derived : public Base
{
public:
    void whichClass() const 
    { 
        std::cout << "Derived class calling Base function\n";
        Base::whichClass();         //Calling whichClass function without scope resolution operator causes infinite recursion.
        //whichClass();
    }
};

int main()
{
    Base b{};
    b.whichClass();

    Derived d{};
    d.whichClass();
    
    return 0;
}