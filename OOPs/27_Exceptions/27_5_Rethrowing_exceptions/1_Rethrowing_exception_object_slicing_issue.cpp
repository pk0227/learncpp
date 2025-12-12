#include <iostream>

// Base exception class with virtual print function
// Virtual function allows polymorphic behavior
class Base
{
public:
    virtual void print() const { std::cout << "Base::print()\n"; }
};

// Derived exception class overriding print function
class Derived : public Base
{
public:
    void print() const override { std::cout << "Derived::print()\n"; }
};

int main()
{
    try
    {
        // Inner try-catch block
        try
        {
            // Throw a Derived exception object
            throw Derived{};
        }
        catch(const Base& e)  // Catch by reference - polymorphism works here!
        {
            std::cerr << "Caught by Base, which is actually a ";
            e.print();  // Calls Derived::print() via polymorphism
            std::cerr << "\n";
            
            // ⚠️ PROBLEM: throw e; causes OBJECT SLICING!
            // 'e' is a const Base&, so throw e; copies only the Base part
            // The Derived information is lost (sliced off)
            throw e; 
        }
    }
    catch(const Base& e)  // Outer catch block
    {
            std::cerr << "Caught by Base, which is actually a ";
            // ❌ This will call Base::print() instead of Derived::print()
            // Because the rethrown exception was sliced to Base type
            e.print();
            std::cerr << "\n";
    }
    
    return 0;
}