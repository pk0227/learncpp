/**
 * @file 2_c_style_array_decay.cpp
 * @brief Demonstrates C-style array decay to pointer and its pitfalls
 * 
 * Key Concepts:
 * 1. In most expressions (including function calls), a C-style array decays into a pointer to its first element (int[] -> int*).
 * 2. Function parameter 'int arr[]' is syntactic sugar for 'int* arr'.
 * 3. Array bounds inside parameter brackets (e.g. 'int arr[100]') are completely ignored by the compiler!
 * 4. Decay strips length information: sizeof(decayed_arr) gives sizeof(int*), not array size!
 * 5. Modern alternatives: std::span<const int> (C++20) or std::array.
 */

#include <iostream>
#include <span>

// Parameter syntax 'int arr[]' is identical to 'int* arr'
void printDecayed(const int arr[100]) // The '100' is completely ignored!
{
    std::cout << "Inside printDecayed, sizeof(arr) (pointer size): " << sizeof(arr) << " bytes\n";
    std::cout << "First element: " << arr[0] << '\n';
    // std::size(arr); // COMPILE ERROR: cannot get size of decayed pointer!
}

// Safer modern C++20 alternative: std::span preserves length info without copying
void printSpan(std::span<const int> sp)
{
    std::cout << "Inside printSpan, size is preserved: " << sp.size() << " elements: ";
    for (int x : sp)
    {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

int main()
{
    int numbers[]{ 10, 20, 30, 40, 50 };

    std::cout << "In main, sizeof(numbers) (array size): " << sizeof(numbers) << " bytes\n";
    std::cout << "In main, std::size(numbers): " << std::size(numbers) << " elements\n\n";

    // Array decays to int* when passed to function
    printDecayed(numbers);

    // Notice we can pass an array with only 2 elements to a parameter declared as arr[100]!
    int smallArr[]{ 1, 2 };
    printDecayed(smallArr); // Compiles with no warning! Danger of out-of-bounds access.

    std::cout << "\nUsing std::span (C++20):\n";
    printSpan(numbers);
    printSpan(smallArr);

    return 0;
}
