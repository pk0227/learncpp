#include <iostream>

// Base class - simple, copyable
class Base
{
public:
    Base() { }
};

// Derived class with DELETED copy constructor
// This demonstrates that exception objects MUST be copyable
class Derived : public Base
{
public:
    Derived() { }
    Derived(const Derived&) = delete;  // Copy constructor explicitly deleted
};

int main()
{
    Derived d{};
    try
    {
        // IMPORTANT: Exception objects must be copyable when throwing lvalues!
        
        // Option 1: throw Derived{};
        // This WORKS even with deleted copy constructor because:
        // - Creates a temporary (rvalue) Derived object
        // - In C++17+, mandatory copy elision means no copy/move is needed
        // - The temporary is constructed directly in exception storage
        // This compiles successfully!
        throw Derived{};
        
        // Option 2: throw d{};
        // This does NOT compile because:
        // - 'd' is an lvalue (named object)
        // - throw must copy 'd' to create the exception object
        // - But Derived's copy constructor is deleted!
        // throw d{}; // COMPILE ERROR: cannot throw lvalue when copy constructor is deleted
    }
    catch(const Derived& d)  // Catch by const reference (no copy needed here)
    {
        std::cerr << "Caught by Derived\n";
    }
    catch(const Base& b)  // Catch by const reference
    {
        std::cerr << "Caught by Base\n";
    }

    return 0;
}