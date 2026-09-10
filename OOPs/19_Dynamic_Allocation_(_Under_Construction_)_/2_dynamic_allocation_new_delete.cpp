/**
 * @file 2_dynamic_allocation_new_delete.cpp
 * @brief Demonstrates scalar dynamic memory allocation with new, delete, and std::nothrow
 * 
 * Key Concepts:
 * 1. Dynamic memory is allocated on the heap at runtime via 'new'.
 * 2. Scalar 'delete' returns memory to the OS; always set deleted pointers to nullptr.
 * 3. By default, 'new' throws std::bad_alloc on failure.
 * 4. 'new (std::nothrow)' returns nullptr instead of throwing on allocation failure.
 * 5. Deleting a nullptr is safe and performs a no-op.
 */

#include <iostream>
#include <new> // for std::nothrow

int main()
{
    // 1. Basic dynamic allocation and initialization
    int* ptr{ new int{ 42 } }; // Allocates an int on the heap with value 42
    std::cout << "Dynamically allocated value: " << *ptr << '\n';

    // 2. Deallocation and preventing dangling pointers
    delete ptr;     // Returns memory to the operating system
    ptr = nullptr;  // Reset to nullptr so it is not a dangling pointer

    // Deleting a nullptr is safe and does nothing
    delete ptr;

    // 3. Nothrow allocation (returns nullptr if heap memory exhausted)
    int* safePtr{ new (std::nothrow) int{ 100 } };
    if (!safePtr)
    {
        std::cerr << "Could not allocate memory!\n";
        return 1;
    }

    std::cout << "Nothrow allocated value: " << *safePtr << '\n';
    delete safePtr;
    safePtr = nullptr;

    return 0;
}
