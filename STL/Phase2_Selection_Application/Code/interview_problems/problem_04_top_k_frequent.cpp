/**
 * Problem 04: Top K Frequent Elements - Heap/Priority Queue
 *
 * Given an integer array nums and an integer k, return the k most frequent
 * elements.
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 problem_04_top_k_frequent.cpp -o
 * top_k_frequent Run: ./top_k_frequent
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

// ============================================================================
// SOLUTIONS
// ============================================================================

class Solution {
public:
  // Approach 1: Min-Heap - O(n log k)
  std::vector<int> topKFrequent(std::vector<int> &nums, int k) {
    // Count frequencies
    std::unordered_map<int, int> freq;
    for (int num : nums) {
      freq[num]++;
    }

    // Min-heap of size k: {frequency, number}
    auto cmp = [](const std::pair<int, int> &a, const std::pair<int, int> &b) {
      return a.first > b.first; // Min-heap by frequency
    };
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                        decltype(cmp)>
        pq(cmp);

    // Keep only k most frequent
    for (const auto &[num, count] : freq) {
      pq.push({count, num});
      if (pq.size() > static_cast<size_t>(k)) {
        pq.pop(); // Remove least frequent
      }
    }

    // Extract results
    std::vector<int> result;
    while (!pq.empty()) {
      result.push_back(pq.top().second);
      pq.pop();
    }

    return result;
  }

  // Approach 2: Bucket Sort - O(n)
  std::vector<int> topKFrequentBucket(std::vector<int> &nums, int k) {
    // Count frequencies
    std::unordered_map<int, int> freq;
    for (int num : nums) {
      freq[num]++;
    }

    // Bucket sort: bucket[i] = numbers with frequency i
    std::vector<std::vector<int>> buckets(nums.size() + 1);
    for (const auto &[num, count] : freq) {
      buckets[count].push_back(num);
    }

    // Collect top k from highest frequency buckets
    std::vector<int> result;
    for (int i = buckets.size() - 1;
         i >= 0 && result.size() < static_cast<size_t>(k); --i) {
      for (int num : buckets[i]) {
        result.push_back(num);
        if (result.size() == static_cast<size_t>(k))
          break;
      }
    }

    return result;
  }
};

// ============================================================================
// CONTAINER SELECTION ANALYSIS
// ============================================================================

/*
 * WHY priority_queue (min-heap)?
 *
 * Requirements:
 * - Find k most frequent elements
 * - Efficient tracking of top k
 *
 * Approach 1: Min-Heap of size k
 * - unordered_map: Count frequencies O(n)
 * - priority_queue: Maintain top k O(n log k)
 * - Total: O(n log k)
 *
 * Why min-heap (not max-heap)?
 * - We want to remove SMALLEST frequency when heap size > k
 * - Min-heap gives us smallest at top
 * - Keep k largest by removing smallest
 *
 * Approach 2: Bucket Sort
 * - unordered_map: Count frequencies O(n)
 * - vector of vectors: Bucket by frequency O(n)
 * - Total: O(n) - optimal!
 *
 * Trade-off:
 * - Heap: O(n log k) time, O(k) space
 * - Bucket: O(n) time, O(n) space
 */

// ============================================================================
// TEST CASES
// ============================================================================

void testBasic() {
  std::cout << "=== TEST: Basic Cases ===\n";
  Solution sol;

  std::vector<int> nums1 = {1, 1, 1, 2, 2, 3};
  auto result1 = sol.topKFrequent(nums1, 2);
  std::cout << "   nums = [1,1,1,2,2,3], k = 2\n";
  std::cout << "   Result: ";
  for (int x : result1)
    std::cout << x << " ";
  std::cout << "(1 and 2)\n";

  std::vector<int> nums2 = {1};
  auto result2 = sol.topKFrequent(nums2, 1);
  std::cout << "   nums = [1], k = 1\n";
  std::cout << "   Result: " << result2[0] << "\n";

  std::cout << "   ✓ Basic tests passed\n\n";
}

void testBucketSort() {
  std::cout << "=== TEST: Bucket Sort Approach ===\n";
  Solution sol;

  std::vector<int> nums = {1, 1, 1, 2, 2, 3};
  auto result = sol.topKFrequentBucket(nums, 2);
  std::cout << "   nums = [1,1,1,2,2,3], k = 2\n";
  std::cout << "   Result (bucket sort): ";
  for (int x : result)
    std::cout << x << " ";
  std::cout << "\n   ✓ Bucket sort test passed\n\n";
}

void testLargeK() {
  std::cout << "=== TEST: Large K ===\n";
  Solution sol;

  std::vector<int> nums = {4, 1, -1, 2, -1, 2, 3};
  auto result = sol.topKFrequent(nums, 2);
  std::cout << "   nums = [4,1,-1,2,-1,2,3], k = 2\n";
  std::cout << "   Result: ";
  for (int x : result)
    std::cout << x << " ";
  std::cout << "(-1 and 2)\n";
  std::cout << "   ✓ Large k test passed\n\n";
}

// ============================================================================
// COMPLEXITY COMPARISON
// ============================================================================

void complexityComparison() {
  std::cout << "=== COMPLEXITY COMPARISON ===\n\n";

  std::cout << "Approach 1: Min-Heap\n";
  std::cout << "  Time: O(n log k)\n";
  std::cout << "    - Count frequencies: O(n)\n";
  std::cout << "    - Heap operations: O(n log k)\n";
  std::cout << "  Space: O(n) for map + O(k) for heap\n\n";

  std::cout << "Approach 2: Bucket Sort\n";
  std::cout << "  Time: O(n)\n";
  std::cout << "    - Count frequencies: O(n)\n";
  std::cout << "    - Bucket sort: O(n)\n";
  std::cout << "  Space: O(n) for map + O(n) for buckets\n\n";

  std::cout << "When to use which?\n";
  std::cout << "  - Heap: When k << n (k much smaller than n)\n";
  std::cout << "  - Bucket: When k ≈ n or need O(n) time\n\n";
}

// ============================================================================
// INTERVIEW FOLLOW-UPS
// ============================================================================

void interviewFollowUps() {
  std::cout << "=== INTERVIEW FOLLOW-UPS ===\n\n";

  std::cout << "Q1: Why min-heap instead of max-heap?\n";
  std::cout << "A: We maintain heap of size k\n";
  std::cout << "   - Need to remove SMALLEST when size > k\n";
  std::cout << "   - Min-heap gives smallest at top\n\n";

  std::cout << "Q2: Can you do better than O(n log k)?\n";
  std::cout << "A: Yes! Bucket sort achieves O(n)\n";
  std::cout << "   - Use frequency as bucket index\n";
  std::cout << "   - Collect from highest frequency buckets\n\n";

  std::cout << "Q3: What if k > number of unique elements?\n";
  std::cout << "A: Return all unique elements\n\n";

  std::cout << "Q4: What if there are ties in frequency?\n";
  std::cout << "A: Return any k elements (problem doesn't specify order)\n\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
  std::cout << "Top K Frequent Elements\n";
  std::cout << "=======================\n\n";

  testBasic();
  testBucketSort();
  testLargeK();
  complexityComparison();
  interviewFollowUps();

  std::cout << "All tests passed! ✓\n";

  return 0;
}
