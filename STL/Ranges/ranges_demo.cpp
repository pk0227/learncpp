/**
 * Modern C++20 Ranges & Views
 *
 * WHY THIS MATTERS FOR SENIOR ROLES:
 * C++20 Ranges completely changes how we write algorithms.
 * - No more `begin(), end()`.
 * - Readable "Pipe Syntax" (like Shell/Python/Linq).
 * - "Lazy Evaluation" (Views) for performance.
 * - "Projections" simplify sorting/searching structs.
 *
 * Compile: g++ -std=c++20 -O2 -Wall -Wextra ranges_demo.cpp -o ranges_demo
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <ranges> // C++20
#include <string>
#include <vector>

// ============================================================================
// 1. THE OLD WAY vs THE NEW WAY (Pipe Syntax)
// ============================================================================

void demo_pipes() {
  std::cout << "=== 1. Pipe Syntax (Filter -> Transform -> Take) ===\n";
  std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  // GOAL: Get square of first 3 even numbers

  // OLD WAY (Verbose, hard to read, allocates temps)
  std::vector<int> temp;
  for (int n : nums) {
    if (n % 2 == 0)
      temp.push_back(n * n);
  }
  std::cout << "Old Way: ";
  for (size_t i = 0; i < std::min(temp.size(), size_t(3)); ++i) {
    std::cout << temp[i] << " ";
  }
  std::cout << "\n";

  // NEW WAY (Ranges)
  // 1. Filter evens
  // 2. Transform (Square)
  // 3. Take 3
  auto result = nums | std::views::filter([](int n) { return n % 2 == 0; }) |
                std::views::transform([](int n) { return n * n; }) |
                std::views::take(3);

  std::cout << "New Way: ";
  for (int n : result) {
    std::cout << n << " ";
  }
  std::cout << " (Zero allocations!)\n\n";
}

// ============================================================================
// 2. PROJECTIONS (Sorting/Searching Structs)
// ============================================================================

struct User {
  int id;
  std::string name;
  int score;
};

void demo_projections() {
  std::cout << "=== 2. Projections (Sorting Structs made easy) ===\n";
  std::vector<User> users = {{101, "Alice", 50},
                             {102, "Bob", 30},
                             {103, "Charlie", 90},
                             {104, "Dave", 30}};

  // OLD WAY: Boilerplate lambda
  // std::sort(users.begin(), users.end(), [](const User& a, const User& b) {
  //     return a.score < b.score;
  // });

  // NEW WAY: Pass the MEMBER directly!
  std::ranges::sort(users, {},
                    &User::score); // Projection: Use 'score' to compare

  std::cout << "Sorted by Score:\n";
  for (const auto &u : users) {
    std::cout << "  " << u.name << " (" << u.score << ")\n";
  }

  // BREAK TIES: Stable Sort isn't default, but let's try finding min
  auto min_user = std::ranges::min_element(users, {}, &User::score);
  std::cout << "Lowest Score: " << min_user->name << "\n\n";
}

// ============================================================================
// 3. LAZY EVALUATION (Infinite Sequences)
// ============================================================================

void demo_lazy() {
  std::cout << "=== 3. Lazy Evaluation (Infinite Views) ===\n";

  // std::views::iota(0) generates 0, 1, 2, 3 ... forever
  auto infinite_seq = std::views::iota(0);

  // Compute on demand: "Get me the first 5 numbers > 1000 that are div by 7"
  auto result = infinite_seq |
                std::views::filter([](int i) { return i % 7 == 0; }) |
                std::views::drop_while(
                    [](int i) { return i <= 1000; }) // Skip until > 1000
                | std::views::take(5);

  std::cout << "First 5 multiples of 7 after 1000: ";
  for (int n : result) {
    std::cout << n << " ";
  }
  std::cout << "\n\n";
}

// ============================================================================
// 4. MAP KEYS/VALUES (Views)
// ============================================================================

void demo_map_views() {
  std::cout << "=== 4. Map Keys/Values Views ===\n";
  std::map<std::string, int> scores = {
      {"Alice", 95}, {"Bob", 85}, {"Charlie", 90}};

  std::cout << "Keys only:   ";
  for (const auto &name : std::views::keys(scores)) {
    std::cout << name << " ";
  }
  std::cout << "\n";

  std::cout << "Values only: ";
  for (int score : std::views::values(scores)) {
    std::cout << score << " ";
  }
  std::cout << "\n\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
  std::cout << "C++20 Ranges & Views Demo\n";
  std::cout << "=========================\n\n";

  demo_pipes();
  demo_projections();
  demo_lazy();
  demo_map_views();

  return 0;
}
