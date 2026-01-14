/**
 * Erase-Remove Idiom - Essential STL Pattern
 *
 * The erase-remove idiom is a common pattern for removing elements from
 * containers. It's one of the most important STL idioms to know for interviews.
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 erase_remove_idiom.cpp -o
 * erase_remove Run: ./erase_remove
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// ============================================================================
// WHY ERASE-REMOVE IDIOM?
// ============================================================================

/*
 * THE PROBLEM:
 *
 * std::remove() is a GENERIC ALGORITHM that works with iterators.
 * Iterators DON'T have the ability to change container size!
 * Only the container itself can change its size.
 *
 * So std::remove() doesn't actually remove elements - it just moves them to the
 * end. You need to call erase() on the container to actually remove them.
 *
 * WRONG WAY:
 * for (auto it = v.begin(); it != v.end(); ++it) {
 *     if (*it == target) {
 *         v.erase(it);  // ❌ Iterator invalidation! Undefined behavior!
 *     }
 * }
 *
 * RIGHT WAY (Erase-Remove Idiom):
 * v.erase(std::remove(v.begin(), v.end(), target), v.end());
 */

// ============================================================================
// HOW std::remove WORKS
// ============================================================================

void demonstrateRemove() {
  std::cout << "=== HOW std::remove WORKS ===\n";

  std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};

  std::cout << "Original: ";
  for (int x : v)
    std::cout << x << " ";
  std::cout << "(size = " << v.size() << ")\n";

  // std::remove moves elements to keep to the front
  // Returns iterator to new "end" (first element to remove)
  auto new_end = std::remove(v.begin(), v.end(), 2);

  std::cout << "After std::remove(2): ";
  for (int x : v)
    std::cout << x << " ";
  std::cout << "(size = " << v.size() << ")\n";

  std::cout << "Elements to keep: ";
  for (auto it = v.begin(); it != new_end; ++it) {
    std::cout << *it << " ";
  }
  std::cout << "\n";

  std::cout << "Elements to remove (garbage): ";
  for (auto it = new_end; it != v.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << "\n";

  // Now actually erase the unwanted elements
  v.erase(new_end, v.end());

  std::cout << "After v.erase(new_end, v.end()): ";
  for (int x : v)
    std::cout << x << " ";
  std::cout << "(size = " << v.size() << ")\n";

  std::cout << "\n";
}

// ============================================================================
// ERASE-REMOVE IDIOM EXAMPLES
// ============================================================================

void eraseRemoveValue() {
  std::cout << "=== ERASE-REMOVE: Remove Specific Value ===\n";

  std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};

  std::cout << "Before: ";
  for (int x : v)
    std::cout << x << " ";
  std::cout << "\n";

  // Remove all 2's
  v.erase(std::remove(v.begin(), v.end(), 2), v.end());

  std::cout << "After removing 2: ";
  for (int x : v)
    std::cout << x << " ";
  std::cout << "\n\n";
}

void eraseRemoveIf() {
  std::cout << "=== ERASE-REMOVE: Remove If Condition ===\n";

  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  std::cout << "Before: ";
  for (int x : v)
    std::cout << x << " ";
  std::cout << "\n";

  // Remove all even numbers
  v.erase(std::remove_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }),
          v.end());

  std::cout << "After removing evens: ";
  for (int x : v)
    std::cout << x << " ";
  std::cout << "\n\n";
}

void eraseRemoveStrings() {
  std::cout << "=== ERASE-REMOVE: With Strings ===\n";

  std::vector<std::string> words = {"apple",  "banana", "cherry",
                                    "banana", "date",   "banana"};

  std::cout << "Before: ";
  for (const auto &w : words)
    std::cout << w << " ";
  std::cout << "\n";

  // Remove all "banana"
  words.erase(std::remove(words.begin(), words.end(), "banana"), words.end());

  std::cout << "After removing 'banana': ";
  for (const auto &w : words)
    std::cout << w << " ";
  std::cout << "\n\n";
}

void eraseRemoveComplex() {
  std::cout << "=== ERASE-REMOVE: Complex Condition ===\n";

  struct Person {
    std::string name;
    int age;
  };

  std::vector<Person> people = {
      {"Alice", 30}, {"Bob", 25}, {"Charlie", 35}, {"David", 20}, {"Eve", 28}};

  std::cout << "Before: ";
  for (const auto &p : people)
    std::cout << p.name << "(" << p.age << ") ";
  std::cout << "\n";

  // Remove people under 25
  people.erase(std::remove_if(people.begin(), people.end(),
                              [](const Person &p) { return p.age < 25; }),
               people.end());

  std::cout << "After removing age < 25: ";
  for (const auto &p : people)
    std::cout << p.name << "(" << p.age << ") ";
  std::cout << "\n\n";
}

// ============================================================================
// C++20: std::erase and std::erase_if
// ============================================================================

void cpp20Erase() {
  std::cout << "=== C++20: std::erase and std::erase_if ===\n";

  std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};

  std::cout << "Before: ";
  for (int x : v)
    std::cout << x << " ";
  std::cout << "\n";

  // C++20: Simpler syntax!
  std::erase(v, 2); // Remove all 2's

  std::cout << "After std::erase(v, 2): ";
  for (int x : v)
    std::cout << x << " ";
  std::cout << "\n";

  v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  // C++20: Remove if condition
  std::erase_if(v, [](int x) { return x % 2 == 0; });

  std::cout << "After std::erase_if (evens): ";
  for (int x : v)
    std::cout << x << " ";
  std::cout << "\n\n";
}

// ============================================================================
// COMMON MISTAKES
// ============================================================================

void commonMistakes() {
  std::cout << "=== COMMON MISTAKES ===\n";

  // Mistake 1: Forgetting to call erase
  {
    std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};
    std::cout << "Mistake 1: Only calling std::remove\n";
    std::cout << "Before: size = " << v.size() << "\n";

    std::remove(v.begin(), v.end(), 2); // ❌ Doesn't change size!

    std::cout << "After std::remove: size = " << v.size() << " (unchanged!)\n";
    std::cout << "Elements: ";
    for (int x : v)
      std::cout << x << " ";
    std::cout << "(garbage at end)\n\n";
  }

  // Mistake 2: Erasing while iterating (wrong way)
  {
    std::cout << "Mistake 2: Erasing while iterating (WRONG)\n";
    std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};

    // ❌ WRONG: Iterator invalidation!
    // for (auto it = v.begin(); it != v.end(); ++it) {
    //     if (*it == 2) {
    //         v.erase(it);  // it is now invalid!
    //         // ++it in loop header uses invalid iterator!
    //     }
    // }

    std::cout << "Don't do this! Use erase-remove idiom instead.\n\n";
  }

  // Correct way: Use erase return value
  {
    std::cout << "Correct alternative: Use erase return value\n";
    std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};

    for (auto it = v.begin(); it != v.end();) {
      if (*it == 2) {
        it = v.erase(it); // erase returns next valid iterator
      } else {
        ++it;
      }
    }

    std::cout << "Result: ";
    for (int x : v)
      std::cout << x << " ";
    std::cout << "\n\n";
  }
}

// ============================================================================
// PERFORMANCE COMPARISON
// ============================================================================

void performanceComparison() {
  std::cout << "=== PERFORMANCE COMPARISON ===\n";

  std::cout << "Erase-while-iterating:\n";
  std::cout << "  - Time: O(n²) in worst case (each erase shifts elements)\n";
  std::cout << "  - Example: Remove all evens from [1,2,3,4,5,6,7,8,9]\n";
  std::cout << "  - Each erase is O(n), and we do it n/2 times = O(n²)\n\n";

  std::cout << "Erase-remove idiom:\n";
  std::cout
      << "  - Time: O(n) - single pass to move elements, then one erase\n";
  std::cout << "  - std::remove: O(n) - one pass through container\n";
  std::cout << "  - erase: O(n) - removes range at end\n";
  std::cout << "  - Total: O(n) - much faster!\n\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
  std::cout << "Erase-Remove Idiom - Essential STL Pattern\n";
  std::cout << "===========================================\n\n";

  demonstrateRemove();
  eraseRemoveValue();
  eraseRemoveIf();
  eraseRemoveStrings();
  eraseRemoveComplex();
  cpp20Erase();
  commonMistakes();
  performanceComparison();

  std::cout << "KEY TAKEAWAYS:\n";
  std::cout
      << "1. std::remove doesn't actually remove - it moves elements to end\n";
  std::cout << "2. You must call erase() to actually remove elements\n";
  std::cout << "3. Erase-remove idiom: v.erase(std::remove(...), v.end())\n";
  std::cout
      << "4. C++20: std::erase(v, value) and std::erase_if(v, predicate)\n";
  std::cout << "5. Erase-remove is O(n), erase-while-iterating is O(n²)\n";

  return 0;
}
