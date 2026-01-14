/**
 * std::unordered_set and std::unordered_map Examples - Hash-Based Containers
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 unordered_examples.cpp -o
 * unordered_examples Run: ./unordered_examples
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main() {
  std::cout << "=== std::unordered_set and std::unordered_map Examples ===\n\n";

  // ========== std::unordered_set ==========
  std::cout << "========== std::unordered_set ==========\n\n";

  // 1. Basic operations
  {
    std::cout << "1. Basic operations\n";
    std::unordered_set<int> us = {3, 1, 4, 1, 5, 9, 2, 6};

    std::cout << "   Set (unordered, unique): ";
    for (int x : us)
      std::cout << x << " ";
    std::cout << " (order undefined!)\n\n";
  }

  // 2. Insert and find (O(1) average)
  {
    std::cout << "2. Insert and find (O(1) average)\n";
    std::unordered_set<int> us;

    us.insert(42);
    us.insert(100);

    auto it = us.find(42);
    if (it != us.end()) {
      std::cout << "   Found 42 in O(1) average time\n";
    }

    bool exists = us.count(100) > 0;
    std::cout << "   100 exists: " << (exists ? "yes" : "no") << "\n\n";
  }

  // 3. Hash table properties
  {
    std::cout << "3. Hash table properties\n";
    std::unordered_set<int> us = {1, 2, 3, 4, 5};

    std::cout << "   size() = " << us.size() << "\n";
    std::cout << "   bucket_count() = " << us.bucket_count() << "\n";
    std::cout << "   load_factor() = " << us.load_factor() << "\n";
    std::cout << "   max_load_factor() = " << us.max_load_factor() << "\n\n";
  }

  // 4. Reserve to prevent rehashing
  {
    std::cout << "4. Reserve to prevent rehashing\n";
    std::unordered_set<int> us;
    us.reserve(1000); // Pre-allocate for 1000 elements

    std::cout << "   After reserve(1000):\n";
    std::cout << "   bucket_count() = " << us.bucket_count() << "\n";
    std::cout << "   (No rehashing until 1000 elements)\n\n";
  }

  // ========== std::unordered_map ==========
  std::cout << "========== std::unordered_map ==========\n\n";

  // 5. Basic map operations
  {
    std::cout << "5. Basic map operations\n";
    std::unordered_map<std::string, int> ages;

    ages["Alice"] = 30;
    ages["Bob"] = 25;
    ages.insert({"Charlie", 35});
    ages.emplace("David", 28);

    std::cout << "   Ages (unordered):\n";
    for (const auto &[name, age] : ages) {
      std::cout << "     " << name << ": " << age << "\n";
    }
    std::cout << "\n";
  }

  // 6. Interview Use Case: Two Sum
  {
    std::cout << "6. Interview Use Case: Two Sum (O(n) solution)\n";
    std::vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    std::unordered_map<int, int> seen; // value -> index

    for (size_t i = 0; i < nums.size(); ++i) {
      int complement = target - nums[i];
      if (seen.find(complement) != seen.end()) {
        std::cout << "   Found: indices " << seen[complement] << " and " << i
                  << "\n";
        std::cout << "   Values: " << complement << " + " << nums[i] << " = "
                  << target << "\n";
        break;
      }
      seen[nums[i]] = i;
    }
    std::cout << "\n";
  }

  // 7. Interview Use Case: Word frequency (fast counting)
  {
    std::cout << "7. Interview Use Case: Word frequency (O(n) counting)\n";
    std::vector<std::string> words = {"apple",  "banana", "apple",
                                      "cherry", "banana", "apple"};
    std::unordered_map<std::string, int> freq;

    for (const auto &word : words) {
      freq[word]++; // O(1) average
    }

    std::cout << "   Frequencies:\n";
    for (const auto &[word, count] : freq) {
      std::cout << "     " << word << ": " << count << "\n";
    }
    std::cout << "\n";
  }

  // 8. Custom hash function
  {
    std::cout << "8. Custom hash function\n";

    struct Point {
      int x, y;
      bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
      }
    };

    struct PointHash {
      size_t operator()(const Point &p) const {
        return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
      }
    };

    std::unordered_set<Point, PointHash> points;
    points.insert({1, 2});
    points.insert({3, 4});

    std::cout << "   Created unordered_set with custom hash\n";
    std::cout << "   Size: " << points.size() << "\n\n";
  }

  // 9. Rehashing demonstration
  {
    std::cout << "9. Rehashing demonstration\n";
    std::unordered_set<int> us;

    std::cout << "   Initial bucket_count: " << us.bucket_count() << "\n";

    for (int i = 0; i < 100; ++i) {
      us.insert(i);
    }

    std::cout << "   After 100 insertions:\n";
    std::cout << "   size = " << us.size() << "\n";
    std::cout << "   bucket_count = " << us.bucket_count() << "\n";
    std::cout << "   load_factor = " << us.load_factor() << "\n";
    std::cout << "   (Rehashing occurred automatically)\n\n";
  }

  // 10. Comparison: map vs unordered_map
  {
    std::cout << "10. Comparison: map vs unordered_map\n";
    std::cout << "   map:\n";
    std::cout << "   - O(log n) operations\n";
    std::cout << "   - Sorted iteration\n";
    std::cout << "   - Range queries possible\n";
    std::cout << "   - Predictable performance\n";
    std::cout << "\n   unordered_map:\n";
    std::cout << "   - O(1) average operations\n";
    std::cout << "   - Unordered iteration\n";
    std::cout << "   - No range queries\n";
    std::cout << "   - Worst case O(n) (hash collisions)\n\n";
  }

  std::cout << "Key Points:\n";
  std::cout << "- unordered_set/map: O(1) average operations\n";
  std::cout << "- Hash table implementation\n";
  std::cout << "- No ordering guarantees\n";
  std::cout << "- Reserve capacity to prevent rehashing\n";
  std::cout << "- Use when you only need lookup/insert/delete\n";
  std::cout << "- Faster than map for large datasets\n";

  return 0;
}
