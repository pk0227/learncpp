// Include necessary headers for input/output and vector container
#include <iostream>
#include <vector>

/**
 * Function to demonstrate passing std::vector by const reference
 *
 * @param ref - A const reference to any type (using abbreviated function
 * template)
 *
 * Using 'const auto&' is a C++20 abbreviated function template syntax that
 * allows the compiler to deduce the type automatically. This is equivalent to:
 * template <typename T>
 * void passByRef(const T& ref)
 *
 * Benefits of passing by const reference:
 * 1. No copying - efficient for large vectors
 * 2. const prevents modification of the original vector
 * 3. Reference avoids slicing and maintains object identity
 */
void passByRef(const auto &ref)
// Alternative template syntax (commented out for reference):
// template <typename T>
// void passByRef(const std::vector<T>& ref)  // More specific: only accepts
// std::vector void passByRef(const T& ref)                // Generic: accepts
// any type T
{
  // Access element at index 3 using subscript operator (alternative approach)
  // std::cout << ref[3] << "\n";

  // Access element at index 3 using .data() method
  // .data() returns a pointer to the underlying array used by the vector
  // This demonstrates direct memory access to vector elements
  std::cout << ref.data()[3] << "\n";
}

int main() {
  // Create a vector of integers initialized with the first 5 prime numbers
  // Uses initializer list syntax for direct initialization
  std::vector<int> primes{2, 3, 5, 7, 11};

  // Pass the vector by const reference to the function
  // This is efficient as it avoids copying all elements
  // Output: 7 (the element at index 3)
  passByRef(primes);

  return 0;
}