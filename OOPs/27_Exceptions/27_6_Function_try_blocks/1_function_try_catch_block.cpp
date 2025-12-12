// Function try blocks can catch both base and the current class exceptions.
// They are primarily used to catch exceptions thrown during member initialization
// in constructors, including base class constructor calls.

#include <iostream>

// Base class with constructor that may throw
class Base
{
    int m_x{};
public:
    Base(int x=0) : m_x{x}
    {
        // Validate input - throw exception if invalid
        if(x <= 0)
            throw -1;  // Throws int exception
    }
};

// Derived class using function try block syntax
class Derived : public Base
{
public:
    // ⚡ Function try block: 'try' keyword placed before member initializer list
    // This allows catching exceptions thrown during base class construction
    Derived(int x=0) try : Base{x}  // Base{x} may throw -1 if x <= 0
    {
        // Constructor body (empty in this case)
    }
    catch(...)  // Catches ANY exception thrown during initialization or body
    {
        std::cerr << "Derived :: Exception caught\n";
        
        // ⚠️ IMPORTANT: Constructor function try blocks IMPLICITLY RETHROW!
        // Even though 'throw;' is commented out, the exception is still rethrown
        // This is mandatory behavior - you cannot "swallow" exceptions in constructor try blocks
        //throw;       // This line is redundant - implicit rethrow happens anyway!
    }
};

int main()
{
    try
    {
        // Attempt to create Derived with x=0
        // Flow: Derived{0} → Base{0} → x <= 0 → throw -1
        Derived d{0};
    }
    catch(int)  // Catches the int exception (-1)
    {
        // This catch block executes because:
        // 1. Base constructor throws -1
        // 2. Derived's function try block catches it, prints message
        // 3. Derived IMPLICITLY rethrows the exception
        // 4. Exception propagates to main, caught here
        std::cerr << "Oops...!\n";
    }

    return 0;
}