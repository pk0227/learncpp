// Function try blocks can catch both base and the current class exceptions.
// This example shows BOTH Base and Derived using function try blocks - demonstrating exception propagation.

#include <iostream>

// Base class using function try block
class Base
{
    int m_x{};
public:
    // ⚡ Function try block in Base constructor
    Base(int x=0) try : m_x{x}  // Initialize m_x
    {
        // Validate after initialization
        if(x <= 0)
            throw -1;  // Throw int exception from constructor body
    }
    catch(...)  // Catches exceptions from initialization and body
    {
        std::cerr << "Base :: Exception caught\n";    
        // ⚠️ IMPLICIT RETHROW: Base's function try block automatically rethrows
        // Even without explicit 'throw;', the exception propagates to Derived
        // implicitly throw  
    }
};

// Derived class also using function try block
class Derived : public Base
{
public:
    // ⚡ Function try block in Derived constructor
    Derived(int x=0) try : Base{x}  // Calls Base{x}, which may throw
    {
        // Constructor body is empty
    }
    catch(...)  // Catches the rethrown exception from Base
    {
        std::cerr << "Derived :: Exception caught\n";
        
        // 🔁 EXPLICIT THROW: This 'throw;' is redundant!
        // Constructor function try blocks ALWAYS implicitly rethrow
        // Whether you write 'throw;' or not, the exception propagates
        throw;              // already implicitly throw (this line is redundant)
    }
};

int main()
{
    try
    {
        // Attempt to create Derived with x=0
        // 🔀 Exception propagation chain:
        // 1. Derived{0} calls Base{0}
        // 2. Base constructor: m_x initialized, then x <= 0 throws -1
        // 3. Base's catch(...) catches it, prints "Base :: Exception caught"
        // 4. Base implicitly rethrows (mandatory behavior)
        // 5. Derived's catch(...) catches it, prints "Derived :: Exception caught"
        // 6. Derived implicitly rethrows (mandatory, even though explicit 'throw;' is present)
        // 7. Exception propagates to main, caught here
        Derived d{0};
    }
    catch(int)  // Finally catches the cascaded exception
    {
        // Output sequence:
        // "Base :: Exception caught"
        // "Derived :: Exception caught"
        // "Oops...!"
        std::cerr << "Oops...!\n";
    }

    return 0;
}