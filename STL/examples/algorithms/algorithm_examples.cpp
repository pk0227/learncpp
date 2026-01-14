/**
 * STL Algorithms - Complete Examples
 * Comprehensive guide to frequently used STL algorithms for interviews
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 algorithm_examples.cpp -o
 * algorithm_examples Run: ./algorithm_examples
 */

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

void printVector(const std::vector<int> &v, const std::string &label = "") {
  if (!label.empty())
    std::cout << label << ": ";
  for (int x : v)
    std::cout << x << " ";
  std::cout << "\n";
}

// ============================================================================
// NON-MODIFYING ALGORITHMS
// ============================================================================

void nonModifyingAlgorithms() {
  std::cout << "=== NON-MODIFYING ALGORITHMS ===\n\n";

  // std::find - Find first occurrence
  {
    std::cout << "1. std::find\n";
    std::vector<int> v = {1, 2, 3, 4, 5, 3, 6};

    auto it = std::find(v.begin(), v.end(), 3);
    if (it != v.end()) {
      std::cout << "   Found 3 at position: " << (it - v.begin()) << "\n";
    }

    auto it2 = std::find(v.begin(), v.end(), 99);
    if (it2 == v.end()) {
      std::cout << "   99 not found\n";
    }
    std::cout << "\n";
  }

  // std::find_if - Find with predicate
  {
    std::cout << "2. std::find_if\n";
    std::vector<int> v = {1, 2, 3, 4, 5, 6};

    auto it = std::find_if(v.begin(), v.end(), [](int x) { return x > 3; });
    if (it != v.end()) {
      std::cout << "   First element > 3: " << *it << "\n";
    }
    std::cout << "\n";
  }

  // std::count - Count occurrences
  {
    std::cout << "3. std::count\n";
    std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};

    int count = std::count(v.begin(), v.end(), 2);
    std::cout << "   Count of 2: " << count << "\n";
    std::cout << "\n";
  }

  // std::count_if - Count with predicate
  {
    std::cout << "4. std::count_if\n";
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    int count =
        std::count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
    std::cout << "   Count of even numbers: " << count << "\n";
    std::cout << "\n";
  }

  // std::all_of, std::any_of, std::none_of
  {
    std::cout << "5. std::all_of / any_of / none_of\n";
    std::vector<int> v = {2, 4, 6, 8, 10};

    bool all_even =
        std::all_of(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
    std::cout << "   All even? " << (all_even ? "Yes" : "No") << "\n";

    bool any_odd =
        std::any_of(v.begin(), v.end(), [](int x) { return x % 2 != 0; });
    std::cout << "   Any odd? " << (any_odd ? "Yes" : "No") << "\n";

    bool none_negative =
        std::none_of(v.begin(), v.end(), [](int x) { return x < 0; });
    std::cout << "   None negative? " << (none_negative ? "Yes" : "No") << "\n";
    std::cout << "\n";
  }

  // std::for_each - Apply function to each element
  {
    std::cout << "6. std::for_each\n";
    std::vector<int> v = {1, 2, 3, 4, 5};

    std::cout << "   Squared values: ";
    std::for_each(v.begin(), v.end(), [](int x) { std::cout << x * x << " "; });
    std::cout << "\n\n";
  }
}

// ============================================================================
// MODIFYING ALGORITHMS
// ============================================================================

void modifyingAlgorithms() {
  std::cout << "=== MODIFYING ALGORITHMS ===\n\n";

  // std::copy
  {
    std::cout << "1. std::copy\n";
    std::vector<int> src = {1, 2, 3, 4, 5};
    std::vector<int> dest(5);

    std::copy(src.begin(), src.end(), dest.begin());
    printVector(dest, "   Copied");
    std::cout << "\n";
  }

  // std::copy_if - Copy with condition
  {
    std::cout << "2. std::copy_if\n";
    std::vector<int> src = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> dest;

    std::copy_if(src.begin(), src.end(), std::back_inserter(dest),
                 [](int x) { return x % 2 == 0; });
    printVector(dest, "   Even numbers");
    std::cout << "\n";
  }

  // std::move - Move elements
  {
    std::cout << "3. std::move\n";
    std::vector<std::string> src = {"hello", "world", "test"};
    std::vector<std::string> dest(3);

    std::move(src.begin(), src.end(), dest.begin());
    std::cout << "   Moved strings to dest\n";
    std::cout << "   Source after move: ";
    for (const auto &s : src)
      std::cout << "'" << s << "' ";
    std::cout << "(empty strings)\n\n";
  }

  // std::transform - Apply function and store result
  {
    std::cout << "4. std::transform\n";
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::vector<int> result(5);

    std::transform(v.begin(), v.end(), result.begin(),
                   [](int x) { return x * x; });
    printVector(result, "   Squared");

    // Binary transform
    std::vector<int> v2 = {10, 20, 30, 40, 50};
    std::transform(v.begin(), v.end(), v2.begin(), result.begin(),
                   [](int a, int b) { return a + b; });
    printVector(result, "   Sum of two vectors");
    std::cout << "\n";
  }

  // std::fill - Fill with value
  {
    std::cout << "5. std::fill\n";
    std::vector<int> v(5);
    std::fill(v.begin(), v.end(), 42);
    printVector(v, "   Filled with 42");
    std::cout << "\n";
  }

  // std::generate - Fill with generated values
  {
    std::cout << "6. std::generate\n";
    std::vector<int> v(5);
    int n = 0;
    std::generate(v.begin(), v.end(), [&n]() { return n++; });
    printVector(v, "   Generated sequence");
    std::cout << "\n";
  }

  // std::replace - Replace value
  {
    std::cout << "7. std::replace\n";
    std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};
    std::replace(v.begin(), v.end(), 2, 99);
    printVector(v, "   After replacing 2 with 99");
    std::cout << "\n";
  }

  // std::replace_if - Replace with condition
  {
    std::cout << "8. std::replace_if\n";
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::replace_if(
        v.begin(), v.end(), [](int x) { return x % 2 == 0; }, 0);
    printVector(v, "   Even numbers replaced with 0");
    std::cout << "\n";
  }

  // std::remove - Move elements to remove to end
  {
    std::cout << "9. std::remove (use with erase!)\n";
    std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};
    printVector(v, "   Before");

    auto new_end = std::remove(v.begin(), v.end(), 2);
    std::cout << "   After std::remove (before erase): ";
    for (auto it = v.begin(); it != v.end(); ++it) {
      if (it == new_end)
        std::cout << "| ";
      std::cout << *it << " ";
    }
    std::cout << "\n";

    v.erase(new_end, v.end());
    printVector(v, "   After erase");
    std::cout << "\n";
  }

  // std::remove_if - Remove with condition
  {
    std::cout << "10. std::remove_if (erase-remove idiom)\n";
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    printVector(v, "   Before");

    v.erase(
        std::remove_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }),
        v.end());
    printVector(v, "   After removing evens");
    std::cout << "\n";
  }

  // std::unique - Remove consecutive duplicates
  {
    std::cout << "11. std::unique\n";
    std::vector<int> v = {1, 1, 2, 2, 2, 3, 3, 4, 5, 5};
    printVector(v, "   Before");

    auto new_end = std::unique(v.begin(), v.end());
    v.erase(new_end, v.end());
    printVector(v, "   After unique");
    std::cout << "\n";
  }

  // std::reverse - Reverse elements
  {
    std::cout << "12. std::reverse\n";
    std::vector<int> v = {1, 2, 3, 4, 5};
    printVector(v, "   Before");

    std::reverse(v.begin(), v.end());
    printVector(v, "   After reverse");
    std::cout << "\n";
  }
}

// ============================================================================
// SORTING & SEARCHING ALGORITHMS
// ============================================================================

void sortingSearchingAlgorithms() {
  std::cout << "=== SORTING & SEARCHING ALGORITHMS ===\n\n";

  // std::sort
  {
    std::cout << "1. std::sort\n";
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7};
    printVector(v, "   Before");

    std::sort(v.begin(), v.end());
    printVector(v, "   After sort (ascending)");

    std::sort(v.begin(), v.end(), std::greater<int>());
    printVector(v, "   After sort (descending)");
    std::cout << "\n";
  }

  // std::stable_sort - Maintains relative order of equal elements
  {
    std::cout << "2. std::stable_sort\n";
    struct Person {
      std::string name;
      int age;
    };

    std::vector<Person> people = {
        {"Alice", 30}, {"Bob", 25}, {"Charlie", 30}, {"David", 25}};

    std::stable_sort(
        people.begin(), people.end(),
        [](const Person &a, const Person &b) { return a.age < b.age; });

    std::cout << "   Sorted by age (stable):\n";
    for (const auto &p : people) {
      std::cout << "     " << p.name << ": " << p.age << "\n";
    }
    std::cout << "\n";
  }

  // std::partial_sort - Sort first N elements
  {
    std::cout << "3. std::partial_sort\n";
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    printVector(v, "   Before");

    std::partial_sort(v.begin(), v.begin() + 3, v.end());
    printVector(v, "   After partial_sort (first 3)");
    std::cout << "   (First 3 are sorted and smallest)\n\n";
  }

  // std::nth_element - Partition around nth element
  {
    std::cout << "4. std::nth_element\n";
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    printVector(v, "   Before");

    std::nth_element(v.begin(), v.begin() + 4, v.end());
    printVector(v, "   After nth_element (n=4)");
    std::cout << "   Element at position 4: " << v[4] << " (5th smallest)\n";
    std::cout << "   All before are <=, all after are >=\n\n";
  }

  // std::binary_search - Check if element exists (requires sorted)
  {
    std::cout << "5. std::binary_search\n";
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    bool found = std::binary_search(v.begin(), v.end(), 5);
    std::cout << "   Is 5 in vector? " << (found ? "Yes" : "No") << "\n";

    found = std::binary_search(v.begin(), v.end(), 99);
    std::cout << "   Is 99 in vector? " << (found ? "Yes" : "No") << "\n";
    std::cout << "\n";
  }

  // std::lower_bound - First element >= value
  {
    std::cout << "6. std::lower_bound\n";
    std::vector<int> v = {1, 2, 2, 2, 3, 4, 5};

    auto it = std::lower_bound(v.begin(), v.end(), 2);
    std::cout << "   First element >= 2 at position: " << (it - v.begin())
              << "\n";
    std::cout << "   Value: " << *it << "\n\n";
  }

  // std::upper_bound - First element > value
  {
    std::cout << "7. std::upper_bound\n";
    std::vector<int> v = {1, 2, 2, 2, 3, 4, 5};

    auto it = std::upper_bound(v.begin(), v.end(), 2);
    std::cout << "   First element > 2 at position: " << (it - v.begin())
              << "\n";
    std::cout << "   Value: " << *it << "\n\n";
  }

  // std::equal_range - Range of elements equal to value
  {
    std::cout << "8. std::equal_range\n";
    std::vector<int> v = {1, 2, 2, 2, 3, 4, 5};

    auto range = std::equal_range(v.begin(), v.end(), 2);
    std::cout << "   Range of 2: [" << (range.first - v.begin()) << ", "
              << (range.second - v.begin()) << ")\n";
    std::cout << "   Count: " << (range.second - range.first) << "\n\n";
  }
}

// ============================================================================
// NUMERIC ALGORITHMS
// ============================================================================

void numericAlgorithms() {
  std::cout << "=== NUMERIC ALGORITHMS ===\n\n";

  // std::accumulate - Sum or fold
  {
    std::cout << "1. std::accumulate\n";
    std::vector<int> v = {1, 2, 3, 4, 5};

    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "   Sum: " << sum << "\n";

    int product = std::accumulate(v.begin(), v.end(), 1,
                                  [](int acc, int x) { return acc * x; });
    std::cout << "   Product: " << product << "\n\n";
  }

  // std::iota - Fill with incrementing values
  {
    std::cout << "2. std::iota\n";
    std::vector<int> v(10);
    std::iota(v.begin(), v.end(), 1);
    printVector(v, "   Filled with 1..10");
    std::cout << "\n";
  }

  // std::inner_product - Dot product
  {
    std::cout << "3. std::inner_product\n";
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2 = {2, 2, 2, 2, 2};

    int dot = std::inner_product(v1.begin(), v1.end(), v2.begin(), 0);
    std::cout << "   Dot product: " << dot << "\n";
    std::cout << "   (1*2 + 2*2 + 3*2 + 4*2 + 5*2 = 30)\n\n";
  }

  // std::partial_sum - Running sum
  {
    std::cout << "4. std::partial_sum\n";
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::vector<int> result(5);

    std::partial_sum(v.begin(), v.end(), result.begin());
    printVector(result, "   Partial sums");
    std::cout << "\n";
  }

  // std::adjacent_difference
  {
    std::cout << "5. std::adjacent_difference\n";
    std::vector<int> v = {1, 3, 6, 10, 15};
    std::vector<int> result(5);

    std::adjacent_difference(v.begin(), v.end(), result.begin());
    printVector(result, "   Adjacent differences");
    std::cout << "\n";
  }
}

// ============================================================================
// HEAP ALGORITHMS
// ============================================================================

void heapAlgorithms() {
  std::cout << "=== HEAP ALGORITHMS ===\n\n";

  std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
  printVector(v, "Original");

  // std::make_heap - Create max heap
  {
    std::cout << "\n1. std::make_heap\n";
    std::make_heap(v.begin(), v.end());
    printVector(v, "   After make_heap (max-heap)");
    std::cout << "   Top element: " << v.front() << "\n";
  }

  // std::push_heap - Add element to heap
  {
    std::cout << "\n2. std::push_heap\n";
    v.push_back(10);
    std::push_heap(v.begin(), v.end());
    printVector(v, "   After push_heap(10)");
    std::cout << "   Top element: " << v.front() << "\n";
  }

  // std::pop_heap - Remove top element
  {
    std::cout << "\n3. std::pop_heap\n";
    std::pop_heap(v.begin(), v.end());
    int top = v.back();
    v.pop_back();
    std::cout << "   Popped: " << top << "\n";
    printVector(v, "   After pop_heap");
    std::cout << "   New top: " << v.front() << "\n";
  }

  // std::sort_heap - Sort heap (destroys heap property)
  {
    std::cout << "\n4. std::sort_heap\n";
    std::sort_heap(v.begin(), v.end());
    printVector(v, "   After sort_heap (sorted ascending)");
    std::cout << "   (No longer a heap!)\n";
  }

  // Min-heap example
  {
    std::cout << "\n5. Min-heap (using std::greater)\n";
    std::vector<int> v2 = {3, 1, 4, 1, 5, 9, 2, 6};
    std::make_heap(v2.begin(), v2.end(), std::greater<int>());
    printVector(v2, "   Min-heap");
    std::cout << "   Top (minimum): " << v2.front() << "\n";
  }

  std::cout << "\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
  std::cout << "STL Algorithms - Complete Examples\n";
  std::cout << "===================================\n\n";

  nonModifyingAlgorithms();
  modifyingAlgorithms();
  sortingSearchingAlgorithms();
  numericAlgorithms();
  heapAlgorithms();

  std::cout << "All algorithm examples completed!\n";

  return 0;
}
