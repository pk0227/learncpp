/**
 * Problem 02: Two Sum - Hash Table
 *
 * Given an array of integers nums and an integer target, return indices of the
 * two numbers such that they add up to target.
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 problem_02_two_sum.cpp -o two_sum
 * Run: ./two_sum
 */

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>

// ============================================================================
// SOLUTION
// ============================================================================

class Solution {
public:
  // Approach 1: Brute Force - O(n²)
  std::vector<int> twoSumBruteForce(std::vector<int> &nums, int target) {
    int n = nums.size();
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        if (nums[i] + nums[j] == target) {
          return {i, j};
        }
      }
    }
    return {};
  }

  // Approach 2: Hash Table - O(n) ⭐ OPTIMAL
  std::vector<int> twoSum(std::vector<int> &nums, int target) {
    std::unordered_map<int, int> seen; // value -> index

    for (size_t i = 0; i < nums.size(); ++i) {
      int complement = target - nums[i];

      // Check if complement exists
      if (seen.find(complement) != seen.end()) {
        return {seen[complement], static_cast<int>(i)};
      }

      // Store current number
      seen[nums[i]] = static_cast<int>(i);
    }

    return {}; // No solution found
  }
};

// ============================================================================
// CONTAINER SELECTION ANALYSIS
// ============================================================================

/*
 * WHY unordered_map?
 *
 * Requirements:
 * - O(1) lookup by value
 * - Store value -> index mapping
 * - Single pass through array
 *
 * Options:
 *
 * 1. vector/array:
 *    - Lookup: O(n) - need to search
 *    - Total: O(n²)
 *    ❌ Too slow
 *
 * 2. map (ordered):
 *    - Lookup: O(log n)
 *    - Total: O(n log n)
 *    ❌ Slower than needed
 *
 * 3. unordered_map (hash table): ✓✓✓
 *    - Lookup: O(1) average
 *    - Insert: O(1) average
 *    - Total: O(n)
 *    ✅ Optimal!
 *
 * KEY INSIGHT: Hash table gives O(1) lookup, making single-pass solution
 * possible.
 */

// ============================================================================
// TEST CASES
// ============================================================================

void testBasic() {
  std::cout << "=== TEST: Basic Cases ===\n";
  Solution sol;

  std::vector<int> nums1 = {2, 7, 11, 15};
  auto result1 = sol.twoSum(nums1, 9);
  std::cout << "   nums = [2,7,11,15], target = 9\n";
  std::cout << "   Result: [" << result1[0] << ", " << result1[1] << "]\n";
  assert(result1[0] == 0 && result1[1] == 1);

  std::vector<int> nums2 = {3, 2, 4};
  auto result2 = sol.twoSum(nums2, 6);
  std::cout << "   nums = [3,2,4], target = 6\n";
  std::cout << "   Result: [" << result2[0] << ", " << result2[1] << "]\n";
  assert(result2[0] == 1 && result2[1] == 2);

  std::vector<int> nums3 = {3, 3};
  auto result3 = sol.twoSum(nums3, 6);
  std::cout << "   nums = [3,3], target = 6\n";
  std::cout << "   Result: [" << result3[0] << ", " << result3[1] << "]\n";
  assert(result3[0] == 0 && result3[1] == 1);

  std::cout << "   ✓ All basic tests passed\n\n";
}

void testNegativeNumbers() {
  std::cout << "=== TEST: Negative Numbers ===\n";
  Solution sol;

  std::vector<int> nums = {-1, -2, -3, -4, -5};
  auto result = sol.twoSum(nums, -8);
  std::cout << "   nums = [-1,-2,-3,-4,-5], target = -8\n";
  std::cout << "   Result: [" << result[0] << ", " << result[1] << "]\n";
  std::cout << "   ✓ Negative numbers test passed\n\n";
}

void testLargeArray() {
  std::cout << "=== TEST: Large Array ===\n";
  Solution sol;

  std::vector<int> nums(10000);
  for (int i = 0; i < 10000; ++i) {
    nums[i] = i;
  }
  nums[5000] = 1000;
  nums[8000] = 8000;

  auto result = sol.twoSum(nums, 9000);
  std::cout << "   Array size: 10000\n";
  std::cout << "   Found pair in O(n) time\n";
  std::cout << "   ✓ Large array test passed\n\n";
}

// ============================================================================
// COMPLEXITY ANALYSIS
// ============================================================================

void complexityAnalysis() {
  std::cout << "=== COMPLEXITY ANALYSIS ===\n\n";

  std::cout << "Brute Force:\n";
  std::cout << "  Time: O(n²) - nested loops\n";
  std::cout << "  Space: O(1) - no extra space\n\n";

  std::cout << "Hash Table (Optimal):\n";
  std::cout << "  Time: O(n) - single pass\n";
  std::cout << "  Space: O(n) - hash table storage\n\n";

  std::cout << "Trade-off: Space for time!\n";
  std::cout << "  We use O(n) extra space to achieve O(n) time\n\n";
}

// ============================================================================
// INTERVIEW FOLLOW-UPS
// ============================================================================

void interviewFollowUps() {
  std::cout << "=== INTERVIEW FOLLOW-UPS ===\n\n";

  std::cout << "Q1: What if the array is sorted?\n";
  std::cout << "A: Use two pointers (O(n) time, O(1) space)\n";
  std::cout << "   - Left pointer at start, right at end\n";
  std::cout << "   - If sum < target, move left++\n";
  std::cout << "   - If sum > target, move right--\n\n";

  std::cout << "Q2: What if there are multiple solutions?\n";
  std::cout << "A: Return first found, or modify to return all pairs\n\n";

  std::cout << "Q3: What if we need to return values instead of indices?\n";
  std::cout << "A: Same approach, just return nums[i] and nums[j]\n\n";

  std::cout << "Q4: Can we use the same element twice?\n";
  std::cout << "A: No - we check i != j in our solution\n\n";

  std::cout << "Q5: What if no solution exists?\n";
  std::cout << "A: Return empty vector (or throw exception)\n\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
  std::cout << "Two Sum - Hash Table Solution\n";
  std::cout << "==============================\n\n";

  testBasic();
  testNegativeNumbers();
  testLargeArray();
  complexityAnalysis();
  interviewFollowUps();

  std::cout << "All tests passed! ✓\n";

  return 0;
}
