/**
 * Demonstration of std::vector::resize() function
 * 
 * Key Concepts:
 * 1. resize() changes the number of elements in a vector
 * 2. Resizing larger adds default-initialized elements (0 for int)
 * 3. Resizing smaller removes elements from the end
 * 4. Capacity may remain larger than size after shrinking
 * 
 * Important behaviors:
 * - Growing: New elements are value-initialized (0 for built-in types)
 * - Shrinking: Excess elements are destroyed
 * - Size vs Capacity: resize() affects size, capacity may not decrease
 */

#include <iostream>
#include <vector>
#include <cmath>  // for std::sqrt

// Template function to print all vector elements
template <typename T>
void printVector(const std::vector<T>& ref)
{
    for(auto i{0Z}; i < std::ssize(ref); i++)
        std::cout << ref.data()[i] << " ";
    
    std::cout << "\n";
}

int main()
{
    // Initial vector with 4 elements
    std::vector<int> vi{1, 21, 31, 41};     // size is 4
    printVector(vi);  // Output: 1 21 31 41

    // Resize to double the current size
    // New elements (positions 4-7) are value-initialized to 0
    vi.resize(2*vi.size());                 // size is 8 (2*4)
    printVector(vi);  // Output: 1 21 31 41 0 0 0 0

    // Resize to square root of current size (truncated to integer)
    // Elements beyond index 1 are removed
    // sqrt(8) ≈ 2.83, truncates to 2
    vi.resize(std::sqrt(vi.size()));        // size is sqrt(8) ~= 2
    printVector(vi);  // Output: 1 21

    return 0;
}