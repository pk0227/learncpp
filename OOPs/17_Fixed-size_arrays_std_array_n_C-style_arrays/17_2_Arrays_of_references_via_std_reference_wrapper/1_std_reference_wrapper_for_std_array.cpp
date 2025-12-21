/*
 * Demonstration: Arrays of References using std::reference_wrapper
 * 
 * Problem: std::array cannot directly hold references (std::array<int&, 3> is invalid)
 * Solution: Use std::reference_wrapper<T> to wrap references in the array
 * 
 * Key Concepts:
 * 1. std::reference_wrapper allows references to be stored in containers
 * 2. Implicit conversion from T to std::reference_wrapper<T>
 * 3. Use .get() to access the underlying reference for modification
 * 4. std::ref() and std::cref() helper functions create wrappers
 * 5. Assignment of reference_wrapper rebinds the wrapper, not the referenced value
 */

#include <iostream>
#include <array>
#include <functional>  // for std::reference_wrapper, std::ref, std::cref

int main()
{
    int x{1}, y{2}, z{3};
    
    // ILLEGAL: Cannot create array of native references
    // std::array<int&, 3> arr{x, y, z};  // compilation error

    // Approach 1: CTAD with references - deduces to std::array<int, 3> (copies, not references!)
    //int &x_ref{x}, &y_ref{y}, &z_ref{z};
    //std::array arr1{x_ref, y_ref, z_ref};  // Deduces to std::array<int, 3>, not references
    
    // Approach 2: Use std::reference_wrapper to store references in array
    std::array<std::reference_wrapper<int>, 3> arr2{x, y, z};
    
    // Reading: Implicit conversion makes reading easy
    std::cout << arr2[1] << "\n";   // Prints 2 (value of y)
    
    // Writing: Cannot assign directly to wrapper
    //arr2[1] = 5;                    // ERROR: Tries to assign int to reference_wrapper
    arr2[1].get() = 5;              // CORRECT: Use .get() to access underlying reference
    std::cout << arr2[1] << "  :  " << y << "\n";  // Both print 5 (y was modified)
    
    // std::ref() and std::cref() helper functions
    auto r1{std::ref(x)};       // Creates reference_wrapper<int> to x
    auto r2{std::cref(y)};      // Creates reference_wrapper<const int> to y
                                // Note: 'auto' drops top-level const, so r2 is not const itself
    
    std::cout << "r2 : " << r2 << " || y :" << y << "\n";  // r2 refers to y (value 5)
    
    // Assignment to reference_wrapper rebinds the wrapper (doesn't modify referenced value)
    r2 = r1;  // Now r2 refers to x instead of y
    
    std::cout << "r2 : " << r2 << " || y :" << y << "\n";  // r2 now shows x's value, y unchanged
    
    return 0;
}