#include <iostream>
#include <string_view>

class Base
{
public:
	// This version of getThis() returns a pointer to a Base class
    virtual Base* getThis() 
    {
        std::cout << "Base::getThis()\n"; 
        return this; 
    }
    std::string_view printType() const { return "Base"; }
};

class Derived : public Base
{
public:
	// Normally override functions have to return objects of the same type as the base function
	// However, because Derived is derived from Base, it's okay to return Derived* instead of Base*
    Derived* getThis() override 
    {
        std::cout << "Derived::getThis()\n"; 
        return this; 
    }
    std::string_view printType() const { return "Derived"; }
};

int main()
{
    Derived d{};
    Derived* dptr{&d};
    Base *bptr{&d};

    std::cout << dptr->getThis()->printType() << "\n";  // calls Derived::getThis(), returns a Derived*, calls Derived::printType
    std::cout << "--------------------------------\n";
    std::cout << bptr->getThis()->printType() << "\n";  // calls Derived::getThis(), returns a Base*, calls Base::printType

    return 0;
}