#include <iostream>
#include <vector>

int main()
{
    std::vector v{ 10, 20, 30, 40, 50 };
    auto it{ v.begin() };
    ++it; // Move to second element (20)
    std::cout << "Original element at it: " << *it << "\n";

    // PITFALL:
    // v.erase(it); // Erases 20, but invalidates 'it'!
    // ++it;        // UNDEFINED BEHAVIOR: accessing invalidated iterator!

    // CORRECT IDIOM:
    // erase() returns an iterator pointing to the element that followed the erased element
    it = v.erase(it); // 'it' now points to 30!
    std::cout << "Element at it after revalidation via erase(): " << *it << "\n";

    std::cout << "Remaining elements: ";
    for (int num : v)
    {
        std::cout << num << ' ';
    }
    std::cout << "\n";

    return 0;
}