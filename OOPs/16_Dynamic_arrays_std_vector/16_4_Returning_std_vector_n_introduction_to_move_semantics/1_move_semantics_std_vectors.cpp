/**
 * Introduction to Move Semantics with std::vector
 * 
 * Key Concepts:
 * 1. Return Value Optimization (RVO) and Named Return Value Optimization (NRVO)
 * 2. Move semantics avoid expensive deep copies when returning containers
 * 3. Temporary objects (rvalues) are automatically moved, not copied
 * 4. Modern C++ compilers optimize away unnecessary copies
 * 
 * This example demonstrates that returning vectors by value is efficient
 * due to move semantics and compiler optimizations.
 */

#include <iostream>
#include <vector>

// Function returning std::vector by value
// Despite returning by value, this is efficient due to move semantics
std::vector<int> getVector()        
{
    // Named local vector object
    // Using a named object here so we can observe move semantics in action
    // (mandatory copy elision applies only to unnamed temporaries)
    std::vector arr1{1, 2, 3, 4, 5};       
    
    // When returning arr1:
    // 1. The compiler may apply NRVO (Named Return Value Optimization)
    // 2. If NRVO doesn't apply, move constructor is used (not copy constructor)
    // 3. Either way, no expensive deep copy of the vector data occurs
    return arr1;  // Move semantics or RVO makes this efficient
}

int main()
{
    // The return value of getVector() is an rvalue (temporary)
    // Move semantics allow arr2 to "steal" the resources from the temporary
    // The temporary object is destroyed immediately after this line
    std::vector arr2{getVector()};      
    
    // After the above line, the temporary return value no longer exists
    // arr2 now owns the vector data - no deep copy was performed
    
    // Modify the first element to demonstrate we have a usable vector
    arr2[0] = 7;

    std::cout << arr2[0] << "\n";  // Output: 7

    return 0;
}