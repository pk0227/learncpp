/*
 * Demonstration: Multidimensional C-Style Arrays
 * 
 * Key Concepts:
 * 1. C-style arrays can be multidimensional (e.g., int arr[3][4])
 * 2. Template functions can deduce dimensions via reference parameters
 * 3. Range-based for loops work naturally with multidimensional arrays
 * 4. Arrays decay to pointers but references preserve dimensions
 * 
 * Template Parameter Deduction:
 * - T: element type
 * - R: number of rows
 * - C: number of columns
 */

#include <iostream>

// Function template to load 2D array from user input
// Reference parameter (T (&ref)[R][C]) preserves array dimensions
template <typename T, std::size_t R, std::size_t C>
void loadArray(T (&ref)[R][C])
{
    for(std::size_t r{0}; r < R; ++r)
    {
        for(std::size_t c{0}; c < C; ++c)
        {
            std::cout << "Enter element for arr[" << r << "][" << c << "]: ";
            std::cin >> ref[r][c];
        }
    }
}

// Function template to print 2D array with formatting
template <typename T, std::size_t R, std::size_t C>
void printArray(T (&ref)[R][C])
{
    // Approach 1: Index-based nested loops (commented out)
    /*
    for(auto r{0Z}; r < R; r++)
    {
        for(auto c{0Z}; c < C; c++)
        {
            std::cout << std::format("{:<5}", ref[r][c]);
        }
        std::cout << "\n";
    }
    */
    
    // Approach 2: Range-based for loops (cleaner, safer)
    // Outer loop: r is a reference to each row (1D array)
    for(const auto& r : ref)
    {
        // Inner loop: c is each element in the row
        for(const auto& c : r)
            std::cout << c << '\t';
        std::cout << "\n";
    }
}


int main()
{
    // Declare 3x4 2D array (3 rows, 4 columns), zero-initialized
    int arr[3][4]{};

    loadArray(arr);   // Compiler deduces T=int, R=3, C=4
    printArray(arr);  // Same template deduction
    
    return 0;
}