/**
 * @file 1_pointer_arithmetic_and_traversal.cpp
 * @brief Demonstrates pointer arithmetic, subscript equivalence, and array traversal
 * 
 * Key Concepts:
 * 1. Pointer arithmetic moves in units of the pointed-to type, not bytes (p + 1 steps sizeof(T) bytes).
 * 2. Subscript equivalence: arr[i] == *(arr + i) == *(i + arr) == i[arr].
 * 3. Range-based for loops over C-style arrays literally compile to pointer traversal:
 *    for (const int* p = arr; p != arr + size; ++p)
 * 4. Relative indexing: if p = &arr[2], then p[-1] accesses arr[1], p[1] accesses arr[3].
 * 5. Pointer arithmetic is only defined within the array or one-past-the-end.
 */

#include <iostream>

int main()
{
    const int arr[]{ 10, 20, 30, 40, 50 };

    // 1. Pointer arithmetic and subscript equivalence
    const int* ptr{ arr }; // decays to &arr[0]
    std::cout << "ptr[2]:       " << ptr[2] << '\n';
    std::cout << "*(ptr + 2):   " << *(ptr + 2) << '\n';
    std::cout << "*(2 + ptr):   " << *(2 + ptr) << '\n';
    std::cout << "2[ptr]:       " << 2[ptr] << " (C language quirk)\n\n";

    // 2. Relative indexing
    const int* mid{ &arr[2] }; // Points to 30
    std::cout << "mid[0]  (current):  " << mid[0] << '\n';
    std::cout << "mid[1]  (next):     " << mid[1] << '\n';
    std::cout << "mid[-1] (previous): " << mid[-1] << "\n\n";

    // 3. Array traversal using begin and end pointers
    std::cout << "Traversing with pointer loop (under the hood of range-for):\n";
    const int* begin{ arr };
    const int* end{ arr + std::size(arr) }; // One-past-the-end pointer

    for (const int* p{ begin }; p != end; ++p)
    {
        std::cout << *p << ' ';
    }
    std::cout << '\n';

    return 0;
}
