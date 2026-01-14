/**
 * std::list Examples - Doubly Linked List
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 list_examples.cpp -o list_examples
 * Run: ./list_examples
 */

#include <algorithm>
#include <iostream>
#include <list>

int main() {
  std::cout << "=== std::list Examples ===\n\n";

  // 1. Declaration and Initialization
  {
    std::cout << "1. Declaration and Initialization\n";
    std::list<int> l1;
    std::list<int> l2(5, 42);
    std::list<int> l3 = {1, 2, 3, 4, 5};

    std::cout << "   l3: ";
    for (int x : l3)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 2. Front and Back Operations
  {
    std::cout << "2. Front and Back Operations\n";
    std::list<int> l;

    l.push_back(3);
    l.push_back(4);
    l.push_front(2);
    l.push_front(1);

    std::cout << "   After pushes: ";
    for (int x : l)
      std::cout << x << " ";

    l.pop_front();
    l.pop_back();

    std::cout << "\n   After pops: ";
    for (int x : l)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 3. Insert and Erase (O(1) if you have iterator!)
  {
    std::cout << "3. Insert and Erase (O(1) with iterator!)\n";
    std::list<int> l = {1, 2, 4, 5};

    auto it = l.begin();
    ++it;
    ++it;            // Point to 4
    l.insert(it, 3); // Insert 3 before 4

    std::cout << "   After insert: ";
    for (int x : l)
      std::cout << x << " ";

    it = l.begin();
    ++it;        // Point to 2
    l.erase(it); // Erase 2

    std::cout << "\n   After erase: ";
    for (int x : l)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 4. list::sort() - Uses merge sort (works with bidirectional iterators)
  {
    std::cout << "4. list::sort() - Merge Sort\n";
    std::list<int> l = {5, 2, 8, 1, 9, 3, 7};

    std::cout << "   Before sort: ";
    for (int x : l)
      std::cout << x << " ";

    l.sort(); // Can't use std::sort (needs random access)!

    std::cout << "\n   After sort: ";
    for (int x : l)
      std::cout << x << " ";

    l.sort(std::greater<int>()); // Descending
    std::cout << "\n   Descending: ";
    for (int x : l)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 5. list::reverse()
  {
    std::cout << "5. list::reverse()\n";
    std::list<int> l = {1, 2, 3, 4, 5};

    l.reverse();
    std::cout << "   Reversed: ";
    for (int x : l)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 6. list::unique() - Remove consecutive duplicates
  {
    std::cout << "6. list::unique()\n";
    std::list<int> l = {1, 1, 2, 2, 2, 3, 3, 4, 5, 5};

    std::cout << "   Before unique: ";
    for (int x : l)
      std::cout << x << " ";

    l.unique();

    std::cout << "\n   After unique: ";
    for (int x : l)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 7. list::remove() - Remove all occurrences
  {
    std::cout << "7. list::remove()\n";
    std::list<int> l = {1, 2, 3, 2, 4, 2, 5};

    std::cout << "   Before remove(2): ";
    for (int x : l)
      std::cout << x << " ";

    l.remove(2);

    std::cout << "\n   After remove(2): ";
    for (int x : l)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 8. list::remove_if() - Remove with predicate
  {
    std::cout << "8. list::remove_if()\n";
    std::list<int> l = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    l.remove_if([](int x) { return x % 2 == 0; });

    std::cout << "   After removing evens: ";
    for (int x : l)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 9. list::splice() - Move elements from another list (O(1)!)
  {
    std::cout << "9. list::splice() - Move elements (O(1)!)\n";
    std::list<int> l1 = {1, 2, 3};
    std::list<int> l2 = {10, 20, 30};

    std::cout << "   l1 before: ";
    for (int x : l1)
      std::cout << x << " ";
    std::cout << "\n   l2 before: ";
    for (int x : l2)
      std::cout << x << " ";

    l1.splice(l1.end(), l2); // Move all of l2 to end of l1

    std::cout << "\n   l1 after splice: ";
    for (int x : l1)
      std::cout << x << " ";
    std::cout << "\n   l2 after splice: ";
    for (int x : l2)
      std::cout << x << " ";
    std::cout << " (empty)\n\n";
  }

  // 10. Iterator Stability (KEY ADVANTAGE!)
  {
    std::cout << "10. Iterator Stability (KEY ADVANTAGE!)\n";
    std::list<int> l = {1, 2, 3, 4, 5};

    auto it1 = l.begin();   // Points to 1
    auto it2 = ++l.begin(); // Points to 2

    l.push_back(6);
    l.push_front(0);
    l.insert(l.end(), 7);

    std::cout << "   After many insertions:\n";
    std::cout << "   it1 still points to: " << *it1 << "\n";
    std::cout << "   it2 still points to: " << *it2 << "\n";
    std::cout << "   (Iterators remain valid!)\n\n";
  }

  // 11. Interview Use Case: When to use list
  {
    std::cout << "11. Interview Use Case: When to use list\n";
    std::cout << "   Use list when:\n";
    std::cout << "   - Frequent insertions/deletions in middle\n";
    std::cout << "   - Need iterator stability\n";
    std::cout << "   - Splicing operations needed\n";
    std::cout << "   - Container is LARGE (>10,000 elements)\n";
    std::cout << "\n   Avoid list when:\n";
    std::cout << "   - Need random access\n";
    std::cout << "   - Cache performance matters\n";
    std::cout << "   - Container is small (<1,000 elements)\n";
    std::cout << "   - Memory overhead is concern (16 bytes per element!)\n\n";
  }

  std::cout << "Key Points:\n";
  std::cout << "- O(1) insert/erase with iterator\n";
  std::cout << "- Iterator stability (never invalidated except erased)\n";
  std::cout << "- Has own sort() using merge sort\n";
  std::cout << "- Splice operation is unique and powerful\n";
  std::cout << "- High memory overhead (2 pointers per element)\n";
  std::cout << "- Poor cache performance (non-contiguous)\n";

  return 0;
}
