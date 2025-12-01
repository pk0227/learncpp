#include <iostream>
#include <string_view>
#include <memory>

class Base
{
public:
    Base()   { std::cout << "Base Con'tor\n"; }
    std::string_view printType() const { return "Base"; }
};

class Derived : public Base
{
public:
    Derived()   { std::cout << "Derived Con'tor\n"; }
    std::string_view printType() const { return "Derived"; }
};


class A
{
public:
	// This version of getThis() returns a pointer to a Base class
    virtual Base* create() 
    {
        std::cout << "A::create\n"; 
        return new Base{}; 
    }

};

class B : public A
{
public:
	// Normally override functions have to return objects of the same type as the base function
	// However, because Derived is derived from Base, it's okay to return Derived* instead of Base*
    Derived* create() override 
    {
        std::cout << "B::create()\n"; 
        return new Derived{}; 
    }

};

int main()
{
    A* aptr{new B{}};

    std::unique_ptr<Base> bptr{aptr->create()};
    std::cout << bptr->printType() << "\n";

    delete aptr;

    return 0;
}