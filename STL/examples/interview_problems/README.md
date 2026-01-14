# 🎯 Interview Problems - Complete Catalog

## Overview

This directory contains popular interview problems with complete solutions, container selection analysis, and test cases.

---

## 📋 Problems List

### Problem 01: LRU Cache ⭐⭐⭐
**File:** `problem_01_lru_cache.cpp`  
**Difficulty:** Medium-Hard  
**Containers:** `unordered_map` + `list`  
**Complexity:** O(1) get and put

**Key Concepts:**
- Hash table for O(1) lookup
- Doubly linked list for O(1) insertion/deletion
- Iterator stability
- Splice operation

**Interview Focus:**
- Container selection justification
- Why list over vector?
- Why unordered_map over map?

---

### Problem 02: Two Sum ⭐⭐⭐
**File:** `problem_02_two_sum.cpp`  
**Difficulty:** Easy  
**Container:** `unordered_map`  
**Complexity:** O(n) time, O(n) space

**Key Concepts:**
- Hash table for complement lookup
- Single pass solution
- Space-time tradeoff

**Interview Focus:**
- Why hash table?
- Can you do better than O(n²)?
- What if array is sorted?

---

### Problem 03: Valid Parentheses ⭐⭐⭐
**File:** `problem_03_valid_parentheses.cpp`  
**Difficulty:** Easy  
**Container:** `stack`  
**Complexity:** O(n) time, O(n) space

**Key Concepts:**
- LIFO behavior for matching
- Stack naturally models the problem
- Step-by-step demonstration

**Interview Focus:**
- Why stack?
- Can you solve without extra space?
- How to handle different bracket types?

---

### Problem 04: Top K Frequent Elements ⭐⭐⭐
**File:** `problem_04_top_k_frequent.cpp`  
**Difficulty:** Medium  
**Containers:** `unordered_map` + `priority_queue`  
**Complexity:** O(n log k) heap, O(n) bucket sort

**Key Concepts:**
- Min-heap of size k
- Why min-heap not max-heap?
- Bucket sort optimization

**Interview Focus:**
- Heap vs bucket sort trade-off
- Why keep min-heap of size k?
- Can you do better than O(n log k)?

---

### Problem 05: Group Anagrams ⭐⭐
**File:** `problem_05_group_anagrams.cpp`  
**Difficulty:** Medium  
**Container:** `unordered_map<string, vector<string>>`  
**Complexity:** O(n * k log k) sort, O(n * k) count

**Key Concepts:**
- Sorted string as key
- Character count as key (faster)
- Grouping with hash map

**Interview Focus:**
- Why sorted string works as key?
- Can you optimize the key generation?
- Character count vs sorting trade-off

---

## 🎯 Coverage Summary

### By Container Type

| Container | Problems |
|-----------|----------|
| **unordered_map** | Two Sum, Top K, Group Anagrams, LRU Cache |
| **list** | LRU Cache |
| **stack** | Valid Parentheses |
| **priority_queue** | Top K Frequent |
| **vector** | All (for storage/results) |

### By Algorithm Type

| Algorithm | Problems |
|-----------|----------|
| **Hash Table** | Two Sum, Group Anagrams, LRU Cache |
| **Heap** | Top K Frequent |
| **Stack** | Valid Parentheses |
| **Linked List** | LRU Cache |
| **Sorting** | Group Anagrams |

### By Difficulty

| Difficulty | Count | Problems |
|------------|-------|----------|
| **Easy** | 2 | Two Sum, Valid Parentheses |
| **Medium** | 2 | Top K Frequent, Group Anagrams |
| **Medium-Hard** | 1 | LRU Cache |

---

## 🚀 Compilation

```bash
cd /home/prashanth/learncpp_workspace/STL/examples/interview_problems

# Compile all
g++ -std=c++20 -Wall -Wextra -O2 problem_01_lru_cache.cpp -o lru_cache
g++ -std=c++20 -Wall -Wextra -O2 problem_02_two_sum.cpp -o two_sum
g++ -std=c++20 -Wall -Wextra -O2 problem_03_valid_parentheses.cpp -o valid_parentheses
g++ -std=c++20 -Wall -Wextra -O2 problem_04_top_k_frequent.cpp -o top_k_frequent
g++ -std=c++20 -Wall -Wextra -O2 problem_05_group_anagrams.cpp -o group_anagrams

# Run examples
./lru_cache
./two_sum
./valid_parentheses
./top_k_frequent
./group_anagrams
```

---

## 📚 Study Guide

### Week 1: Easy Problems
1. **Two Sum** - Master hash table technique
2. **Valid Parentheses** - Understand stack usage

### Week 2: Medium Problems
3. **Top K Frequent** - Learn heap techniques
4. **Group Anagrams** - Practice hash map with complex keys

### Week 3: Hard Problems
5. **LRU Cache** - Combine multiple containers

---

## 🎓 Key Takeaways

### Container Selection Patterns

1. **Need O(1) lookup by key?** → `unordered_map`
2. **Need LIFO behavior?** → `stack`
3. **Need top K elements?** → `priority_queue` (min-heap of size k)
4. **Need O(1) insert/delete with iterator?** → `list`
5. **Need to group items?** → `unordered_map<key, vector<items>>`

### Common Interview Patterns

1. **Two pointers** - Sorted array problems
2. **Hash table** - O(1) lookup, complement finding
3. **Stack** - Matching, parsing, backtracking
4. **Heap** - Top K, merge K, priority-based
5. **Multiple containers** - Complex data structures (LRU)

---

## 💡 Interview Tips

1. **Always explain container choice** - Why this container?
2. **Discuss trade-offs** - Time vs space, simplicity vs performance
3. **Mention alternatives** - What else could work?
4. **Analyze complexity** - Time and space for each approach
5. **Test edge cases** - Empty input, single element, duplicates

---

## 🔥 More Problems to Practice

See [06_Interview_Problems.md](file:///home/prashanth/learncpp_workspace/STL/06_Interview_Problems.md) for:
- Sliding Window Maximum
- Merge Intervals
- Find Median from Data Stream
- Design Twitter
- And 5 more problems!

---

**All problems include:**
- ✅ Complete working solutions
- ✅ Container selection analysis
- ✅ Complexity analysis
- ✅ Test cases
- ✅ Interview follow-up questions

**Total:** 5 compilable problems covering all major container types!
