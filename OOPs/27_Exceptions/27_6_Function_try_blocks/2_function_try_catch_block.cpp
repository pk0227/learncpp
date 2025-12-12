// Function try blocks can catch both base and the current class exceptions.
// This example shows catching exceptions from the constructor BODY (not just initialization).

#include <iostream>

// Base class with simple constructor (no exceptions)
class Base
{
    int m_x{};
public:
    Base(int x=0) : m_x{x}
    {
        // Base constructor completes successfully
    }
};

// Derived class using function try block to catch exceptions from constructor body
class Derived : public Base
{
public:
    // ⚡ Function try block syntax: 'try' before member initializer list
    Derived(int x=0) try : Base{x}  // Base{x} completes successfully
    {
        // Exception thrown from constructor BODY (after initialization completes)
        if(x <= 0)
            throw -1;  // Validation throws int exception
    }
    catch(...)  // Catches exceptions from BOTH initialization AND body
    {
        std::cerr << "Derived :: Exception caught\n";
        
        // ⚠️ IMPLICIT RETHROW: Constructor function try blocks ALWAYS rethrow!
        // The exception is automatically rethrown even without explicit 'throw;'
        // This is mandatory - cannot swallow exceptions in constructor try blocks
        //throw;            // Redundant - implicit rethrow happens automatically
    }
};

int main()
{
    try
    {
        // Attempt to create Derived with x=0
        // Flow: Derived{0} → Base{0} (succeeds) → Derived body → x <= 0 → throw -1
        Derived d{0};
    }
    catch(int)  // Catches the implicitly rethrown int exception (-1)
    {
        // This catch block executes because:
        // 1. Derived constructor body throws -1
        // 2. Function try block catches it, prints "Derived :: Exception caught"
        // 3. Exception is IMPLICITLY rethrown (mandatory behavior)
        // 4. Exception propagates to main, caught here
        std::cerr << "Oops...!\n";
    }

    return 0;
}