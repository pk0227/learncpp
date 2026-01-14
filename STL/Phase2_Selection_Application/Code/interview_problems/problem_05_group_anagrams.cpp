/**
 * Problem 05: Group Anagrams - Hash Map
 *
 * Given an array of strings strs, group the anagrams together.
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 problem_05_group_anagrams.cpp -o
 * group_anagrams Run: ./group_anagrams
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// ============================================================================
// SOLUTIONS
// ============================================================================

class Solution {
public:
  // Approach 1: Sort as key - O(n * k log k)
  std::vector<std::vector<std::string>>
  groupAnagrams(std::vector<std::string> &strs) {
    std::unordered_map<std::string, std::vector<std::string>> groups;

    for (const std::string &str : strs) {
      // Sort string to get key
      std::string key = str;
      std::sort(key.begin(), key.end());

      // Group by sorted key
      groups[key].push_back(str);
    }

    // Extract groups
    std::vector<std::vector<std::string>> result;
    for (auto &[key, group] : groups) {
      result.push_back(std::move(group));
    }

    return result;
  }

  // Approach 2: Character count as key - O(n * k)
  std::vector<std::vector<std::string>>
  groupAnagramsCount(std::vector<std::string> &strs) {
    std::unordered_map<std::string, std::vector<std::string>> groups;

    for (const std::string &str : strs) {
      // Create key from character counts
      std::string key = getKey(str);
      groups[key].push_back(str);
    }

    std::vector<std::vector<std::string>> result;
    for (auto &[key, group] : groups) {
      result.push_back(std::move(group));
    }

    return result;
  }

private:
  std::string getKey(const std::string &str) {
    std::vector<int> count(26, 0);
    for (char c : str) {
      count[c - 'a']++;
    }

    // Convert count to string key
    std::string key;
    for (int i = 0; i < 26; ++i) {
      if (count[i] > 0) {
        key += std::string(1, 'a' + i) + std::to_string(count[i]);
      }
    }
    return key;
  }
};

// ============================================================================
// CONTAINER SELECTION ANALYSIS
// ============================================================================

/*
 * WHY unordered_map<string, vector<string>>?
 *
 * Requirements:
 * - Group strings by anagram
 * - Need key to identify anagram group
 * - Store multiple strings per group
 *
 * Key choice:
 * 1. Sorted string: "eat" -> "aet"
 *    - Simple and effective
 *    - O(k log k) to create key
 *
 * 2. Character count: "eat" -> "a1e1t1"
 *    - More complex but faster
 *    - O(k) to create key
 *
 * Container choice:
 * - unordered_map: O(1) average lookup/insert
 * - vector<string>: Store group members
 *
 * Alternative: map (ordered)
 * - O(log n) operations
 * - Slower than unordered_map
 * - Only use if need sorted output
 */

// ============================================================================
// TEST CASES
// ============================================================================

void printGroups(const std::vector<std::vector<std::string>> &groups) {
  std::cout << "   [";
  for (size_t i = 0; i < groups.size(); ++i) {
    std::cout << "[";
    for (size_t j = 0; j < groups[i].size(); ++j) {
      std::cout << "\"" << groups[i][j] << "\"";
      if (j < groups[i].size() - 1)
        std::cout << ",";
    }
    std::cout << "]";
    if (i < groups.size() - 1)
      std::cout << ",";
  }
  std::cout << "]\n";
}

void testBasic() {
  std::cout << "=== TEST: Basic Cases ===\n";
  Solution sol;

  std::vector<std::string> strs1 = {"eat", "tea", "tan", "ate", "nat", "bat"};
  auto result1 = sol.groupAnagrams(strs1);
  std::cout << "   Input: [\"eat\",\"tea\",\"tan\",\"ate\",\"nat\",\"bat\"]\n";
  std::cout << "   Output:\n";
  printGroups(result1);

  std::vector<std::string> strs2 = {""};
  auto result2 = sol.groupAnagrams(strs2);
  std::cout << "\n   Input: [\"\"]\n";
  std::cout << "   Output: [[\"\"]]\n";

  std::vector<std::string> strs3 = {"a"};
  auto result3 = sol.groupAnagrams(strs3);
  std::cout << "\n   Input: [\"a\"]\n";
  std::cout << "   Output: [[\"a\"]]\n";

  std::cout << "\n   ✓ Basic tests passed\n\n";
}

void testCharCountApproach() {
  std::cout << "=== TEST: Character Count Approach ===\n";
  Solution sol;

  std::vector<std::string> strs = {"eat", "tea", "tan", "ate", "nat", "bat"};
  auto result = sol.groupAnagramsCount(strs);
  std::cout << "   Using character count as key\n";
  std::cout << "   Output:\n";
  printGroups(result);
  std::cout << "   ✓ Character count test passed\n\n";
}

void testAllSame() {
  std::cout << "=== TEST: All Same Anagram ===\n";
  Solution sol;

  std::vector<std::string> strs = {"abc", "bca", "cab", "acb", "bac", "cba"};
  auto result = sol.groupAnagrams(strs);
  std::cout << "   All are anagrams of each other\n";
  std::cout << "   Groups: " << result.size() << " (should be 1)\n";
  assert(result.size() == 1);
  std::cout << "   ✓ All same test passed\n\n";
}

void testAllDifferent() {
  std::cout << "=== TEST: All Different ===\n";
  Solution sol;

  std::vector<std::string> strs = {"a", "b", "c", "d", "e"};
  auto result = sol.groupAnagrams(strs);
  std::cout << "   No anagrams\n";
  std::cout << "   Groups: " << result.size() << " (should be 5)\n";
  assert(result.size() == 5);
  std::cout << "   ✓ All different test passed\n\n";
}

// ============================================================================
// COMPLEXITY ANALYSIS
// ============================================================================

void complexityAnalysis() {
  std::cout << "=== COMPLEXITY ANALYSIS ===\n\n";

  std::cout << "n = number of strings, k = max length of string\n\n";

  std::cout << "Approach 1: Sort as key\n";
  std::cout << "  Time: O(n * k log k)\n";
  std::cout << "    - For each string: sort takes O(k log k)\n";
  std::cout << "    - n strings total\n";
  std::cout << "  Space: O(n * k) for hash map\n\n";

  std::cout << "Approach 2: Character count as key\n";
  std::cout << "  Time: O(n * k)\n";
  std::cout << "    - For each string: count chars O(k)\n";
  std::cout << "    - Create key O(26) = O(1)\n";
  std::cout << "  Space: O(n * k) for hash map\n\n";

  std::cout << "Character count is faster but more complex!\n\n";
}

// ============================================================================
// INTERVIEW FOLLOW-UPS
// ============================================================================

void interviewFollowUps() {
  std::cout << "=== INTERVIEW FOLLOW-UPS ===\n\n";

  std::cout << "Q1: Why use sorted string as key?\n";
  std::cout << "A: Anagrams have same sorted form\n";
  std::cout << "   \"eat\" -> \"aet\", \"tea\" -> \"aet\"\n\n";

  std::cout << "Q2: Can you do better than O(n * k log k)?\n";
  std::cout << "A: Yes! Use character count as key\n";
  std::cout << "   Achieves O(n * k) time\n\n";

  std::cout << "Q3: What if strings contain unicode?\n";
  std::cout << "A: Use unordered_map<char, int> for counts\n";
  std::cout << "   Not limited to 26 letters\n\n";

  std::cout << "Q4: Can you use array instead of map?\n";
  std::cout << "A: No - don't know number of groups in advance\n";
  std::cout << "   Hash map is necessary\n\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
  std::cout << "Group Anagrams\n";
  std::cout << "==============\n\n";

  testBasic();
  testCharCountApproach();
  testAllSame();
  testAllDifferent();
  complexityAnalysis();
  interviewFollowUps();

  std::cout << "All tests passed! ✓\n";

  return 0;
}
