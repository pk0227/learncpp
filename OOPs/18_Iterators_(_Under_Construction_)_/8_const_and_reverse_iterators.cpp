/**
 * @file 8_const_and_reverse_iterators.cpp
 * @brief Demonstrates const iterators (cbegin/cend) and reverse iterators (rbegin/rend)
 * 
 * Key Concepts:
 * 1. cbegin() / cend() return const_iterator: read-only access prevents accidental modification.
 * 2. rbegin() / rend() return reverse_iterator: traverses backwards from last to first element.
 *    - rbegin() points to the last valid element.
 *    - rend() points to the theoretical element preceding the first element.
 *    - Moving forward with ++it on a reverse iterator actually steps backwards through the container!
 * 3. crbegin() / crend() return const_reverse_iterator: backwards and read-only.
 * 4. Non-member functions std::cbegin(), std::rbegin(), etc. are available in <iterator>.
 */

#include <iostream>
#include <vector>
#include <iterator>

int main()
{
    const std::vector nums{ 1, 2, 3, 4, 5 };

    // 1. Const iterator: read-only traversal
    std::cout << "Forward traversal using const iterator (cbegin/cend):\n";
    for (auto it{ nums.cbegin() }; it != nums.cend(); ++it)
    {
        std::cout << *it << ' ';
        // *it = 10; // COMPILE ERROR: cannot modify through const_iterator!
    }
    std::cout << "\n\n";

    // 2. Reverse iterator: backwards traversal
    std::cout << "Backward traversal using reverse iterator (rbegin/rend):\n";
    for (auto it{ nums.rbegin() }; it != nums.rend(); ++it)
    {
        std::cout << *it << ' '; // ++it moves backwards towards the first element!
    }
    std::cout << "\n\n";

    // 3. Non-member std::crbegin / std::crend
    std::cout << "Backward traversal using non-member std::crbegin/std::crend:\n";
    for (auto it{ std::crbegin(nums) }; it != std::crend(nums); ++it)
    {
        std::cout << *it << ' ';
    }
    std::cout << '\n';

    return 0;
}
