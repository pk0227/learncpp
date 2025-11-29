/*
    Note that when you redefine a function in the derived class, the derived function does not inherit the access specifier of the function with the same name in the base class. 
    It uses whatever access specifier it is defined under in the derived class. 
    Therefore, a function that is defined as private in the base class can be redefined as public in the derived class, or vice-versa!
*/

#include <iostream>

class Base
{
public:
    void whichClass() const { std::cout << "Base class\n"; }
};

class Derived : public Base
{
public:
    void whichClass() const { std::cout << "Derived class\n"; }
};

int main()
{
    Base b{};
    b.whichClass();

    Derived d{};
    d.whichClass();
    
    return 0;
}