/**
 * Solutions to signed/unsigned indexing challenges when iterating in reverse
 * 
 * Key Concepts:
 * 1. Problem: size() returns unsigned type, can't safely decrement to check >= 0
 * 2. Solution 1: Use type alias for signed index type (std::ptrdiff_t)
 * 3. Solution 2: Use std::ssize() which returns signed size
 * 4. Using .data() to access elements via pointer arithmetic
 * 
 * Why unsigned indices are problematic in reverse iteration:
 * - Unsigned types wrap around when decremented below 0
 * - Condition (index >= 0) is always true for unsigned types
 */

#include <iostream>
#include <vector>

// Type alias for signed index type (recommended for loop indices)
// std::ptrdiff_t is a signed integer type used for pointer arithmetic
using Index = std::ptrdiff_t;

/**
 * Template function to print vector elements in reverse order
 * Demonstrates solutions to signed/unsigned indexing issues
 */
template <typename T>
void printReverse(const std::vector<T>& ref)
{
    // PROBLEM APPROACHES (commented out):
    // Approach 1: Cast unsigned size to signed Index type
    // for(auto index{static_cast<Index>(ref.size()-1)}; index >= 0; index--)
    
    // Approach 2: Cast std::size() result to signed Index type
    // for(auto index{static_cast<Index>(std::size(ref)-1)}; index >= 0; index--)
    
    // RECOMMENDED SOLUTION:
    // Use std::ssize() which returns signed size (std::ptrdiff_t)
    // This eliminates the need for casting and prevents unsigned wrap-around
    for(auto index{std::ssize(ref)-1}; index >= 0; index--)
    {
        // Access element using .data() which returns pointer to underlying array
        // Can use pointer arithmetic with signed index safely
        // Alternative: ref[static_cast<std::size_t>(index)]
        std::cout << ref.data()[index] << " ";
    }
    std::cout << "\n";
}

int main()
{
    // Create vector with test data
    std::vector<int> vi{10, 20, 30, 40, 50};
    
    // Print in reverse: 50 40 30 20 10
    printReverse(vi);
    
    return 0;
}