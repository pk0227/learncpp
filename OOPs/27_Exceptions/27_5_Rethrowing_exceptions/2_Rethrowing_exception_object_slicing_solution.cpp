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
            
            // ✅ SOLUTION: Use bare 'throw;' without exception object
            // This rethrows the ORIGINAL exception (Derived object)
            // NO object slicing occurs - full Derived type is preserved!
            throw; 
        }
    }
    catch(const Base& e)  // Outer catch block
    {
            std::cerr << "Caught by Base, which is actually a ";
            // ✅ This will correctly call Derived::print()
            // Because bare 'throw;' preserved the original Derived exception
            // No slicing occurred - polymorphism works as expected!
            e.print();
            std::cerr << "\n";
    }
    
    return 0;
}