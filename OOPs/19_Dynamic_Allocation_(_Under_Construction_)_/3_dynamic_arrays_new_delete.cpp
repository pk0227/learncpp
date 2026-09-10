/**
 * @file 3_dynamic_arrays_new_delete.cpp
 * @brief Demonstrates dynamically allocated C-style arrays with new[] and delete[]
 * 
 * Key Concepts:
 * 1. new[] allocates an array dynamically where length can be determined at runtime.
 * 2. Dynamic arrays can be initialized using list initialization: new int[n]{ 1, 2, 3 }.
 * 3. Dynamic arrays decay immediately to pointers; sizeof() returns pointer size, not array length.
 * 4. Must ALWAYS pair new[] with delete[]; using scalar delete results in undefined behavior.
 * 5. In modern C++, prefer std::vector over raw dynamic arrays for automatic memory management.
 */

#include <iostream>

int main()
{
    std::size_t length{ 5 }; // Length can be a runtime variable

    // 1. Allocating and initializing dynamic array
    int* array{ new int[length]{ 10, 20, 30, 40, 50 } };

    // 2. Accessing elements
    std::cout << "Dynamic array elements: ";
    for (std::size_t i{ 0 }; i < length; ++i)
    {
        std::cout << array[i] << ' ';
    }
    std::cout << '\n';

    // 3. sizeof on dynamic array returns pointer size (8 bytes on 64-bit platforms)
    std::cout << "sizeof(array) (pointer size, not array size): " << sizeof(array) << " bytes\n";

    // 4. Proper cleanup with delete[]
    delete[] array;
    array = nullptr;

    return 0;
}
