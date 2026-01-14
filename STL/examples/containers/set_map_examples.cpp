/**
 * std::set and std::map Examples - Ordered Associative Containers
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 set_map_examples.cpp -o
 * set_map_examples Run: ./set_map_examples
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

int main() {
  std::cout << "=== std::set and std::map Examples ===\n\n";

  // ========== std::set ==========
  std::cout << "========== std::set ==========\n\n";

  // 1. Basic set operations
  {
    std::cout << "1. Basic set operations\n";
    std::set<int> s = {3, 1, 4, 1, 5, 9, 2, 6}; // Duplicates removed, sorted

    std::cout << "   Set (sorted, unique): ";
    for (int x : s)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 2. Insert and find
  {
    std::cout << "2. Insert and find\n";
    std::set<int> s;

    auto [it1, inserted1] = s.insert(42);
    std::cout << "   Inserted 42: " << (inserted1 ? "success" : "failed")
              << "\n";

    auto [it2, inserted2] = s.insert(42);
    std::cout << "   Inserted 42 again: " << (inserted2 ? "success" : "failed")
              << "\n";

    auto it = s.find(42);
    if (it != s.end()) {
      std::cout << "   Found: " << *it << "\n";
    }
    std::cout << "\n";
  }

  // 3. Range queries (lower_bound, upper_bound)
  {
    std::cout << "3. Range queries (KEY FEATURE!)\n";
    std::set<int> s = {1, 3, 5, 7, 9, 11, 13};

    auto lower = s.lower_bound(5); // First >= 5
    auto upper = s.upper_bound(9); // First > 9

    std::cout << "   Elements in range [5, 9]:\n   ";
    for (auto it = lower; it != upper; ++it) {
      std::cout << *it << " ";
    }
    std::cout << "\n\n";
  }

  // 4. Custom comparator (descending order)
  {
    std::cout << "4. Custom comparator (descending)\n";
    std::set<int, std::greater<int>> s = {3, 1, 4, 1, 5, 9};

    std::cout << "   Descending: ";
    for (int x : s)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // ========== std::map ==========
  std::cout << "========== std::map ==========\n\n";

  // 5. Basic map operations
  {
    std::cout << "5. Basic map operations\n";
    std::map<std::string, int> ages;

    ages["Alice"] = 30;
    ages["Bob"] = 25;
    ages.insert({"Charlie", 35});
    ages.emplace("David", 28);

    std::cout << "   Ages:\n";
    for (const auto &[name, age] : ages) {
      std::cout << "     " << name << ": " << age << "\n";
    }
    std::cout << "\n";
  }

  // 6. operator[] vs at() vs find()
  {
    std::cout << "6. operator[] vs at() vs find()\n";
    std::map<std::string, int> m = {{"Alice", 30}};

    int age1 = m["Alice"]; // OK
    std::cout << "   m[\"Alice\"] = " << age1 << "\n";

    int age2 = m["Bob"]; // Inserts {"Bob", 0}!
    std::cout << "   m[\"Bob\"] = " << age2 << " (inserted!)\n";

    try {
      int age3 = m.at("Charlie"); // Throws
    } catch (const std::out_of_range &) {
      std::cout << "   m.at(\"Charlie\") threw exception\n";
    }

    auto it = m.find("David");
    if (it == m.end()) {
      std::cout << "   m.find(\"David\") not found (safe)\n";
    }
    std::cout << "\n";
  }

  // 7. Interview Use Case: Word frequency counter
  {
    std::cout << "7. Interview Use Case: Word frequency (sorted output)\n";
    std::vector<std::string> words = {"apple",  "banana", "apple",
                                      "cherry", "banana", "apple"};
    std::map<std::string, int> freq;

    for (const auto &word : words) {
      freq[word]++;
    }

    std::cout << "   Frequencies (alphabetical):\n";
    for (const auto &[word, count] : freq) {
      std::cout << "     " << word << ": " << count << "\n";
    }
    std::cout << "\n";
  }

  // 8. Interview Use Case: Range sum query
  {
    std::cout << "8. Interview Use Case: Range sum query\n";
    std::map<int, int> prefix_sum;
    std::vector<int> arr = {1, 2, 3, 4, 5};

    int sum = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
      sum += arr[i];
      prefix_sum[i] = sum;
    }

    // Query sum in range [1, 3]
    int range_sum = prefix_sum[3] - (1 > 0 ? prefix_sum[0] : 0);
    std::cout << "   Sum of arr[1..3] = " << range_sum << "\n\n";
  }

  // 9. multiset - Allows duplicates
  {
    std::cout << "9. std::multiset - Allows duplicates\n";
    std::multiset<int> ms = {3, 1, 4, 1, 5, 1, 9};

    std::cout << "   Multiset: ";
    for (int x : ms)
      std::cout << x << " ";

    int count = ms.count(1);
    std::cout << "\n   Count of 1: " << count << "\n\n";
  }

  // 10. multimap - Multiple values per key
  {
    std::cout << "10. std::multimap - Multiple values per key\n";
    std::multimap<std::string, int> scores;

    scores.insert({"Alice", 90});
    scores.insert({"Alice", 85});
    scores.insert({"Bob", 95});

    std::cout << "   Scores:\n";
    for (const auto &[name, score] : scores) {
      std::cout << "     " << name << ": " << score << "\n";
    }

    auto range = scores.equal_range("Alice");
    std::cout << "   Alice's scores: ";
    for (auto it = range.first; it != range.second; ++it) {
      std::cout << it->second << " ";
    }
    std::cout << "\n\n";
  }

  std::cout << "Key Points:\n";
  std::cout << "- set/map: O(log n) operations, sorted order\n";
  std::cout << "- Red-Black Tree implementation\n";
  std::cout << "- Range queries with lower_bound/upper_bound\n";
  std::cout << "- Iterator stability (except erased elements)\n";
  std::cout << "- Use when you need ordering or range queries\n";

  return 0;
}
