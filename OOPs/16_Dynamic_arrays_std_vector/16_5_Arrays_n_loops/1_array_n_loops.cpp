/**
 * Demonstration of template functions with std::vector and loops
 * 
 * Key Concepts:
 * 1. Function templates work with different types (int, double, etc.)
 * 2. Index-based loops using std::size_t for array traversal
 * 3. Template argument deduction from function parameters
 * 4. Calculating average of vector elements
 */

#include <iostream>
#include <vector>

/**
 * Template function to calculate average of vector elements
 * 
 * @param ref - Const reference to vector of any numeric type
 * @return Average value of all elements in the vector
 * 
 * Template allows this function to work with vectors of different types
 */
template <typename T>
T calculateAverage(const std::vector<T>& ref)
{
    // Initialize accumulator to zero
    T average{};
    
    // Index-based loop using std::size_t (unsigned type)
    // std::size_t is the return type of size() - avoids sign conversion issues
    for(std::size_t i{}; i < ref.size(); i++)
        average += ref[i];  // Sum all elements

    // Divide by number of elements to get average
    // static_cast ensures proper division (prevents integer division issues)
    average /= static_cast<int>(ref.size());

    return average;
}

int main()
{
    // Vector of integers - template deduced as calculateAverage<int>
    std::vector v1{80, 54, 21, 69, 52};
    std::cout << calculateAverage(v1) << "\n";  // Output: 55 (integer average)

    // Vector of doubles - template deduced as calculateAverage<double>
    std::vector v2{80.21, 54.114, 21.587, 69.3325, 52.1587};
    std::cout << calculateAverage(v2) << "\n";  // Output: 55.4804 (floating-point average)

    return 0;
}