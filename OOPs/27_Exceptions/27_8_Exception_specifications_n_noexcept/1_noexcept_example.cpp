// Demonstrates noexcept specification and what happens when exceptions violate it
// noexcept functions that throw cause std::terminate() to be called - program crashes!

#include <iostream>

// Simple class to track destructor calls during stack unwinding
class Doomed
{
public:
    ~Doomed()
    {
        std::cout << "~Doomed() called...!\n";  // Shows cleanup during unwinding
    }
};

// Helper function that always throws
void thrower()
{
    std::cout << "Throwing exception\n";
    throw 1;  // Throws int exception
}

// Potentially throwing function (NO noexcept specifier)
void pt()
{
    std::cout << "pt (potentially throwing)  called\n";
    Doomed d{};  // Local object - destructor will be called during unwinding
    thrower();   // Throws exception
    std::cout << "This never prints...\n";  // Unreachable - exception thrown above
}

// ⚠️ noexcept function that VIOLATES its contract by throwing
void nt() noexcept
{
    std::cout << "nt (noexcept)  called\n";
    Doomed d{};  // Local object
    thrower();   // ☠️ DANGER: This throws, but nt() is marked noexcept!
                 // Result: std::terminate() is called - program CRASHES!
                 // Destructor of 'd' is NOT called - no stack unwinding
    std::cout << "This never prints...\n";  // Unreachable
}

// Test harness function (also marked noexcept)
void tester(int c) noexcept
{
    std::cout << "tester (noexcept) case " << c << " called\n";
    try
    {
        // Case 1: Calls pt() - exception can be caught (no noexcept violation)
        // Case 2: Calls nt() - exception violates noexcept, std::terminate() called
        (c == 1) ? pt() : nt();
    }
    catch(...)  // This catch block works for case 1, but NOT for case 2!
    {
        // Case 1: Exception from pt() is caught here - normal exception handling
        // Case 2: Never reached - std::terminate() called before catch can execute
        std::cout << "tester caught exception\n";
    }
}

int main()
{
    std::cout << std::unitbuf;   // Unbuffered output - see messages immediately
    std::cout << std::boolalpha; // Print bool as true/false instead of 1/0
    
    // ✅ Case 1: pt() throws, but is NOT noexcept
    // Expected output:
    // - "tester (noexcept) case 1 called"
    // - "pt (potentially throwing) called"
    // - "Throwing exception"
    // - "~Doomed() called...!" (stack unwinding happens)
    // - "tester caught exception"
    // - "Test successful"
    tester(1);
    std::cout << "Test successful\n\n";
    
    // ☠️ Case 2: nt() is noexcept but throws - PROGRAM TERMINATES!
    // Expected output:
    // - "tester (noexcept) case 2 called"
    // - "nt (noexcept) called"
    // - "Throwing exception"
    // - Program crashes with std::terminate() - NO destructor call, NO catch!
    // - "Test successful" NEVER prints
    tester(2);
    std::cout << "Test successful\n\n";  // Unreachable - program terminated above

    return 0;
}