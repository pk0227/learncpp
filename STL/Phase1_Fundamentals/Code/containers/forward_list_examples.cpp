/**
 * std::forward_list Examples - Singly Linked List
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 forward_list_examples.cpp -o
 * forward_list_examples Run: ./forward_list_examples
 */

#include <algorithm>
#include <forward_list>
#include <iostream>

int main() {
  std::cout << "=== std::forward_list Examples ===\n\n";

  // 1. Declaration and Initialization
  {
    std::cout << "1. Declaration and Initialization\n";
    std::forward_list<int> fl1;
    std::forward_list<int> fl2(5, 42);
    std::forward_list<int> fl3 = {1, 2, 3, 4, 5};

    std::cout << "   fl3: ";
    for (int x : fl3)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 2. NO size() method! (Key difference from list)
  {
    std::cout << "2. NO size() method! (Key difference from list)\n";
    std::forward_list<int> fl = {1, 2, 3, 4, 5};

    // fl.size();  // ❌ Won't compile!

    // To get size, use std::distance (O(n))
    int size = std::distance(fl.begin(), fl.end());
    std::cout << "   Size (using std::distance): " << size << "\n";
    std::cout << "   Why no size()? To maintain O(1) space overhead\n\n";
  }

  // 3. Front operations only (no back operations)
  {
    std::cout << "3. Front operations only (no back operations)\n";
    std::forward_list<int> fl;

    fl.push_front(3);
    fl.push_front(2);
    fl.push_front(1);

    std::cout << "   After push_front: ";
    for (int x : fl)
      std::cout << x << " ";

    std::cout << "\n   Front: " << fl.front() << "\n";
    // fl.back();  // ❌ No back() - would be O(n)!

    fl.pop_front();
    std::cout << "   After pop_front: ";
    for (int x : fl)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 4. insert_after and erase_after (KEY FEATURE!)
  {
    std::cout << "4. insert_after and erase_after (KEY FEATURE!)\n";
    std::forward_list<int> fl = {1, 2, 4, 5};

    std::cout << "   Before: ";
    for (int x : fl)
      std::cout << x << " ";

    // Insert 3 after position 1 (which contains 2)
    auto it = fl.begin();
    ++it; // Now points to 2
    fl.insert_after(it, 3);

    std::cout << "\n   After insert_after: ";
    for (int x : fl)
      std::cout << x << " ";

    // Erase element after position 0 (erase 2)
    fl.erase_after(fl.begin());

    std::cout << "\n   After erase_after: ";
    for (int x : fl)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 5. before_begin() - Special iterator before first element
  {
    std::cout << "5. before_begin() - Special iterator\n";
    std::forward_list<int> fl = {2, 3, 4};

    std::cout << "   Before: ";
    for (int x : fl)
      std::cout << x << " ";

    // Insert at beginning using before_begin()
    fl.insert_after(fl.before_begin(), 1);

    std::cout << "\n   After insert_after(before_begin(), 1): ";
    for (int x : fl)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 6. forward_list::sort() - Merge sort
  {
    std::cout << "6. forward_list::sort() - Merge sort\n";
    std::forward_list<int> fl = {5, 2, 8, 1, 9, 3, 7};

    std::cout << "   Before sort: ";
    for (int x : fl)
      std::cout << x << " ";

    fl.sort(); // Can't use std::sort (needs random access)!

    std::cout << "\n   After sort: ";
    for (int x : fl)
      std::cout << x << " ";

    fl.sort(std::greater<int>()); // Descending
    std::cout << "\n   Descending: ";
    for (int x : fl)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 7. forward_list::reverse()
  {
    std::cout << "7. forward_list::reverse()\n";
    std::forward_list<int> fl = {1, 2, 3, 4, 5};

    fl.reverse();
    std::cout << "   Reversed: ";
    for (int x : fl)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 8. forward_list::unique() - Remove consecutive duplicates
  {
    std::cout << "8. forward_list::unique()\n";
    std::forward_list<int> fl = {1, 1, 2, 2, 2, 3, 3, 4, 5, 5};

    std::cout << "   Before unique: ";
    for (int x : fl)
      std::cout << x << " ";

    fl.unique();

    std::cout << "\n   After unique: ";
    for (int x : fl)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 9. forward_list::remove() - Remove all occurrences
  {
    std::cout << "9. forward_list::remove()\n";
    std::forward_list<int> fl = {1, 2, 3, 2, 4, 2, 5};

    std::cout << "   Before remove(2): ";
    for (int x : fl)
      std::cout << x << " ";

    fl.remove(2);

    std::cout << "\n   After remove(2): ";
    for (int x : fl)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 10. forward_list::remove_if() - Remove with predicate
  {
    std::cout << "10. forward_list::remove_if()\n";
    std::forward_list<int> fl = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    fl.remove_if([](int x) { return x % 2 == 0; });

    std::cout << "   After removing evens: ";
    for (int x : fl)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 11. forward_list::splice_after() - Move elements
  {
    std::cout << "11. forward_list::splice_after()\n";
    std::forward_list<int> fl1 = {1, 2, 3};
    std::forward_list<int> fl2 = {10, 20, 30};

    std::cout << "   fl1 before: ";
    for (int x : fl1)
      std::cout << x << " ";
    std::cout << "\n   fl2 before: ";
    for (int x : fl2)
      std::cout << x << " ";

    // Splice fl2 after first element of fl1
    fl1.splice_after(fl1.begin(), fl2);

    std::cout << "\n   fl1 after splice: ";
    for (int x : fl1)
      std::cout << x << " ";
    std::cout << "\n   fl2 after splice: ";
    for (int x : fl2)
      std::cout << x << " ";
    std::cout << " (empty)\n\n";
  }

  // 12. Memory efficiency comparison
  {
    std::cout << "12. Memory efficiency comparison\n";
    std::cout << "   list (doubly linked):\n";
    std::cout << "   - 2 pointers per node (prev + next) = 16 bytes\n";
    std::cout << "   - Plus data\n";
    std::cout << "\n   forward_list (singly linked):\n";
    std::cout << "   - 1 pointer per node (next only) = 8 bytes\n";
    std::cout << "   - Plus data\n";
    std::cout << "   - 50% less pointer overhead!\n\n";
  }

  // 13. Interview Use Case: When to use forward_list
  {
    std::cout << "13. Interview Use Case: When to use forward_list\n";
    std::cout << "   Use forward_list when:\n";
    std::cout << "   - Memory is extremely constrained\n";
    std::cout << "   - Only need forward traversal\n";
    std::cout << "   - Don't need size()\n";
    std::cout << "   - Implementing custom data structures (e.g., hash table "
                 "buckets)\n";
    std::cout << "\n   Avoid forward_list when:\n";
    std::cout << "   - Need bidirectional traversal\n";
    std::cout << "   - Need to know size frequently\n";
    std::cout << "   - Need back operations\n";
    std::cout << "   - list would work just as well (usually the case)\n\n";
  }

  // 14. Comparison: forward_list vs list vs vector
  {
    std::cout << "14. Comparison: forward_list vs list vs vector\n";
    std::cout << "   forward_list:\n";
    std::cout << "   - Minimal memory (1 pointer per node)\n";
    std::cout << "   - Forward iteration only\n";
    std::cout << "   - No size()\n";
    std::cout << "   - insert_after/erase_after\n";
    std::cout << "\n   list:\n";
    std::cout << "   - More memory (2 pointers per node)\n";
    std::cout << "   - Bidirectional iteration\n";
    std::cout << "   - Has size()\n";
    std::cout << "   - Regular insert/erase\n";
    std::cout << "\n   vector:\n";
    std::cout << "   - Contiguous memory\n";
    std::cout << "   - Random access\n";
    std::cout << "   - Best cache performance\n";
    std::cout << "   - Usually the best choice!\n\n";
  }

  std::cout << "Key Points:\n";
  std::cout << "- Singly linked list (only next pointer)\n";
  std::cout << "- Forward iterators only (no backward traversal)\n";
  std::cout << "- No size() method (O(1) space overhead)\n";
  std::cout << "- insert_after/erase_after instead of insert/erase\n";
  std::cout << "- before_begin() for inserting at front\n";
  std::cout << "- Minimal memory overhead (50% less than list)\n";
  std::cout << "- Rarely used in practice (list or vector usually better)\n";

  return 0;
}
