// Demonstrates exception safety and how smart pointers prevent memory leaks
// When exceptions are thrown, automatic cleanup (RAII) prevents resource leaks

#include <iostream>
#include <memory>

// Validation function that throws on invalid input
void validate(int *ptr)
{
    if(*ptr < 0)
        throw *ptr;  // Throw the invalid value as an exception
}

int main()
{
    try
    {    
        // Allocate memory on the heap
        int *iptr{new int{-1}};
        
        // ✅ RAII (Resource Acquisition Is Initialization)
        // std::unique_ptr takes ownership of iptr
        // When uptr goes out of scope (due to exception or normal flow),
        // the destructor automatically calls delete on iptr
        std::unique_ptr<int> uptr{iptr};
        
        // This throws an exception because *iptr == -1 (negative)
        validate(iptr);
        
        // ⚠️ These manual cleanup lines are NOT NEEDED (and commented out)
        // Smart pointers handle cleanup automatically via RAII
        // If we used raw pointers without smart pointers and an exception occurred,
        // these lines would be skipped, causing a MEMORY LEAK!
        //delete iptr;
        //iptr = nullptr;
    }
    catch(int e)
    {
        // Exception caught: uptr's destructor has already run (automatic cleanup)
        // Memory is freed - no leak!
        std::cerr << "Error : " << e << " is not allowed\n";
    }

    return 0;
}