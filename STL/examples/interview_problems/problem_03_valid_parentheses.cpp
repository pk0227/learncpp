/**
 * Problem 03: Valid Parentheses - Stack
 *
 * Given a string containing just the characters '(', ')', '{', '}', '[' and
 * ']', determine if the input string is valid.
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 problem_03_valid_parentheses.cpp -o
 * valid_parentheses Run: ./valid_parentheses
 */

#include <cassert>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

// ============================================================================
// SOLUTION
// ============================================================================

class Solution {
public:
  // Approach 1: Stack - O(n)
  bool isValid(const std::string &s) {
    std::stack<char> st;

    for (char c : s) {
      // Opening brackets - push to stack
      if (c == '(' || c == '[' || c == '{') {
        st.push(c);
      }
      // Closing brackets - check matching
      else {
        if (st.empty())
          return false; // No matching opening

        char top = st.top();
        st.pop();

        if ((c == ')' && top != '(') || (c == ']' && top != '[') ||
            (c == '}' && top != '{')) {
          return false; // Mismatch
        }
      }
    }

    return st.empty(); // All brackets matched
  }

  // Approach 2: Stack with map (cleaner)
  bool isValidMap(const std::string &s) {
    std::stack<char> st;
    std::unordered_map<char, char> pairs = {{')', '('}, {']', '['}, {'}', '{'}};

    for (char c : s) {
      // Closing bracket
      if (pairs.count(c)) {
        if (st.empty() || st.top() != pairs[c]) {
          return false;
        }
        st.pop();
      }
      // Opening bracket
      else {
        st.push(c);
      }
    }

    return st.empty();
  }
};

// ============================================================================
// CONTAINER SELECTION ANALYSIS
// ============================================================================

/*
 * WHY stack?
 *
 * Requirements:
 * - Match most recent opening bracket
 * - LIFO (Last In, First Out) behavior
 * - O(1) push/pop operations
 *
 * Options:
 *
 * 1. vector:
 *    - Can use push_back/pop_back
 *    - Works, but stack is more semantic
 *    ✓ Possible but not ideal
 *
 * 2. deque:
 *    - Can use push_back/pop_back
 *    - Works, but stack is more semantic
 *    ✓ Possible but not ideal
 *
 * 3. stack: ✓✓✓
 *    - LIFO semantics match problem perfectly
 *    - O(1) push/pop
 *    - Clear intent
 *    ✅ Perfect choice!
 *
 * KEY INSIGHT: Stack naturally models the bracket matching problem.
 * Most recent opening must match first closing.
 */

// ============================================================================
// TEST CASES
// ============================================================================

void testBasicCases() {
  std::cout << "=== TEST: Basic Cases ===\n";
  Solution sol;

  assert(sol.isValid("()") == true);
  std::cout << "   \"()\" -> true ✓\n";

  assert(sol.isValid("()[]{}") == true);
  std::cout << "   \"()[]{}\" -> true ✓\n";

  assert(sol.isValid("(]") == false);
  std::cout << "   \"(]\" -> false ✓\n";

  assert(sol.isValid("([)]") == false);
  std::cout << "   \"([)]\" -> false ✓\n";

  assert(sol.isValid("{[]}") == true);
  std::cout << "   \"{[]}\" -> true ✓\n";

  std::cout << "   ✓ All basic tests passed\n\n";
}

void testEdgeCases() {
  std::cout << "=== TEST: Edge Cases ===\n";
  Solution sol;

  assert(sol.isValid("") == true);
  std::cout << "   \"\" (empty) -> true ✓\n";

  assert(sol.isValid("(") == false);
  std::cout << "   \"(\" (unclosed) -> false ✓\n";

  assert(sol.isValid(")") == false);
  std::cout << "   \")\" (no opening) -> false ✓\n";

  assert(sol.isValid("((") == false);
  std::cout << "   \"((\" (multiple unclosed) -> false ✓\n";

  assert(sol.isValid("))") == false);
  std::cout << "   \"))\" (multiple unopened) -> false ✓\n";

  std::cout << "   ✓ All edge cases passed\n\n";
}

void testNestedCases() {
  std::cout << "=== TEST: Nested Cases ===\n";
  Solution sol;

  assert(sol.isValid("((()))") == true);
  std::cout << "   \"((()))\" -> true ✓\n";

  assert(sol.isValid("{[()]}") == true);
  std::cout << "   \"{[()]}\" -> true ✓\n";

  assert(sol.isValid("{{{{}}}}") == true);
  std::cout << "   \"{{{{}}}}\" -> true ✓\n";

  assert(sol.isValid("{[(])}") == false);
  std::cout << "   \"{[(])}\" (interleaved) -> false ✓\n";

  std::cout << "   ✓ All nested cases passed\n\n";
}

void testLongString() {
  std::cout << "=== TEST: Long String ===\n";
  Solution sol;

  std::string s = "";
  for (int i = 0; i < 1000; ++i) {
    s += "({[";
  }
  for (int i = 0; i < 1000; ++i) {
    s += "]})";
  }

  assert(sol.isValid(s) == true);
  std::cout << "   3000 character string -> true ✓\n";
  std::cout << "   ✓ Long string test passed\n\n";
}

// ============================================================================
// STEP-BY-STEP EXAMPLE
// ============================================================================

void stepByStepExample() {
  std::cout << "=== STEP-BY-STEP EXAMPLE ===\n";
  std::cout << "Input: \"{[()]}\"\n\n";

  std::string s = "{[()]}";
  std::stack<char> st;

  for (size_t i = 0; i < s.length(); ++i) {
    char c = s[i];
    std::cout << "Step " << (i + 1) << ": char = '" << c << "'\n";

    if (c == '(' || c == '[' || c == '{') {
      st.push(c);
      std::cout << "  Action: Push '" << c << "'\n";
    } else {
      if (!st.empty()) {
        char top = st.top();
        st.pop();
        std::cout << "  Action: Pop '" << top << "', match with '" << c
                  << "'\n";
      }
    }

    std::cout << "  Stack: ";
    std::stack<char> temp = st;
    std::string stack_str;
    while (!temp.empty()) {
      stack_str = temp.top() + stack_str;
      temp.pop();
    }
    std::cout << (stack_str.empty() ? "empty" : stack_str) << "\n\n";
  }

  std::cout << "Final: Stack is empty -> Valid!\n\n";
}

// ============================================================================
// COMPLEXITY ANALYSIS
// ============================================================================

void complexityAnalysis() {
  std::cout << "=== COMPLEXITY ANALYSIS ===\n\n";

  std::cout << "Time Complexity: O(n)\n";
  std::cout << "  - Single pass through string\n";
  std::cout << "  - Each character processed once\n";
  std::cout << "  - Push/pop are O(1)\n\n";

  std::cout << "Space Complexity: O(n)\n";
  std::cout << "  - Worst case: all opening brackets\n";
  std::cout << "  - Example: \"((((((\" requires stack of size n\n\n";
}

// ============================================================================
// INTERVIEW FOLLOW-UPS
// ============================================================================

void interviewFollowUps() {
  std::cout << "=== INTERVIEW FOLLOW-UPS ===\n\n";

  std::cout << "Q1: Can you solve without extra space?\n";
  std::cout << "A: No - need stack to track opening brackets\n";
  std::cout << "   Space complexity is inherent to problem\n\n";

  std::cout << "Q2: What if string contains other characters?\n";
  std::cout << "A: Ignore non-bracket characters\n";
  std::cout << "   Only process (){}[]\n\n";

  std::cout << "Q3: How to handle different bracket types?\n";
  std::cout << "A: Use map to store matching pairs (shown in solution)\n\n";

  std::cout << "Q4: Can you return the position of first error?\n";
  std::cout << "A: Yes - track index when mismatch found\n\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
  std::cout << "Valid Parentheses - Stack Solution\n";
  std::cout << "===================================\n\n";

  testBasicCases();
  testEdgeCases();
  testNestedCases();
  testLongString();
  stepByStepExample();
  complexityAnalysis();
  interviewFollowUps();

  std::cout << "All tests passed! ✓\n";

  return 0;
}
