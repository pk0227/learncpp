/**
 * Understanding vector capacity vs size with resizing operations
 * 
 * Key Concepts:
 * 1. Size: Number of elements currently in the vector
 * 2. Capacity: Total allocated storage (can hold this many elements without reallocation)
 * 3. resize(): Changes size, may increase capacity if needed
 * 4. shrink_to_fit(): Requests reduction of capacity to match size
 * 
 * Performance implications:
 * - Capacity >= size always
 * - Growing beyond capacity triggers reallocation (expensive)
 * - Shrinking size doesn't automatically reduce capacity (avoid reallocations)
 * - shrink_to_fit() is a non-binding request to free unused memory
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <format>

// Print all vector elements
template <typename T>
void printVector(const std::vector<T>& ref)
{
    for(auto i{0Z}; i < std::ssize(ref); i++)
        std::cout << ref.data()[i] << " ";
    
    std::cout << "\n";
}

// Print capacity and size information
template <typename T>
void printCapLen(const std::vector<T>& ref)
{
    std::cout << std::format("{:<10} : {}\t{:<10} : {}", 
                             "Capacity", ref.capacity(), 
                             "Length", ref.size()) << "\n";
}

int main()
{
    // Initial vector: size = 4, capacity >= 4
    std::vector<int> vi{1, 21, 31, 41};     
    printCapLen(vi);   // Typical: Capacity: 4, Length: 4
    printVector(vi);
    std::cout << "=======================================\n";

    // Resize to 8 elements - capacity increases to accommodate
    vi.resize(2*vi.size());                 // size is 8 (2*4)
    printCapLen(vi);   // Typical: Capacity: 8, Length: 8
    printVector(vi);   // Output: 1 21 31 41 0 0 0 0
    std::cout << "=======================================\n";

    // Resize to 2 elements - size decreases but capacity stays at 8
    // Capacity doesn't shrink automatically (performance optimization)
    vi.resize(std::sqrt(vi.size()));        // size is sqrt(8) ~= 2
    printCapLen(vi);   // Typical: Capacity: 8, Length: 2 (capacity unchanged!)
    printVector(vi);   // Output: 1 21
    std::cout << "=======================================\n";

    // Resize to 5 elements - capacity remains 8 (no reallocation needed)
    vi.resize(5);                           
    printCapLen(vi);   // Typical: Capacity: 8, Length: 5
    printVector(vi);   // Output: 1 21 0 0 0
    std::cout << "=======================================\n";

    // Request to reduce capacity to match size
    // This is a non-binding request - implementation may or may not honor it
    // Frees unused memory if the implementation chooses to
    vi.shrink_to_fit();
    printCapLen(vi);   // Typical: Capacity: 5, Length: 5 (capacity reduced)
    printVector(vi);   // Output: 1 21 0 0 0
    std::cout << "=======================================\n";

    return 0;
}