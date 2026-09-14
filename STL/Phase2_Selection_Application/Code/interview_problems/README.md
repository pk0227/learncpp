# 🎯 Interview Problems - Complete Catalog

> **Compilable C++20 implementations of high-frequency senior interview problems**

---

## 📑 Table of Contents

1. [Problems List](#problems-list)
   - [Problem 01: LRU Cache](#problem-01-lru-cache)
   - [Problem 02: Two Sum](#problem-02-two-sum)
   - [Problem 03: Valid Parentheses](#problem-03-valid-parentheses)
   - [Problem 04: Top K Frequent Elements](#problem-04-top-k-frequent-elements)
   - [Problem 05: Group Anagrams](#problem-05-group-anagrams)
   - [Problem 06: In-Memory File System](#problem-06-in-memory-file-system)
   - [Problem 07: Design Twitter](#problem-07-design-twitter)
   - [Problem 08: Merge Intervals](#problem-08-merge-intervals)
2. [Coverage Summary](#coverage-summary)
3. [Compilation and Execution](#compilation-and-execution)
4. [Study Guide](#study-guide)
5. [Key Takeaways](#key-takeaways)
6. [More Problems to Practice](#more-problems-to-practice)

---

## Problems List

### Problem 01: LRU Cache ⭐⭐⭐
**File:** [`problem_01_lru_cache.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_01_lru_cache.cpp)  
**Difficulty:** Medium-Hard  
**Containers:** `std::unordered_map` + `std::list`  
**Complexity:** O(1) get and put

**Key Concepts:**
- Hash table for O(1) lookup
- Doubly linked list for O(1) insertion/deletion
- Iterator stability
- Splice operation (`items.splice`)

**Interview Focus:**
- Container selection justification
- Why list over vector?
- Why unordered_map over map?

---

### Problem 02: Two Sum ⭐⭐⭐
**File:** [`problem_02_two_sum.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_02_two_sum.cpp)  
**Difficulty:** Easy  
**Container:** `std::unordered_map`  
**Complexity:** O(n) time, O(n) space

**Key Concepts:**
- Hash table for complement lookup
- Single pass solution
- Space-time tradeoff

---

### Problem 03: Valid Parentheses ⭐⭐⭐
**File:** [`problem_03_valid_parentheses.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_03_valid_parentheses.cpp)  
**Difficulty:** Easy  
**Container:** `std::stack`  
**Complexity:** O(n) time, O(n) space

**Key Concepts:**
- LIFO stack matching
- Early termination on mismatched brackets

---

### Problem 04: Top K Frequent Elements ⭐⭐⭐
**File:** [`problem_04_top_k_frequent.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_04_top_k_frequent.cpp)  
**Difficulty:** Medium  
**Containers:** `std::unordered_map` + `std::priority_queue` (min-heap)  
**Complexity:** O(n log k) time, O(n) space

**Key Concepts:**
- Frequency counting with hash table
- Size-bounded min-heap of size K
- Custom comparator for pairs

---

### Problem 05: Group Anagrams ⭐⭐
**File:** [`problem_05_group_anagrams.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_05_group_anagrams.cpp)  
**Difficulty:** Medium  
**Container:** `std::unordered_map<std::string, std::vector<std::string>>`  
**Complexity:** O(n * k log k) sort, O(n * k) count

**Key Concepts:**
- Canonical representation as map key
- Character counting array as key optimization

---

### Problem 06: In-Memory File System ⭐⭐⭐
**File:** [`problem_06_file_system.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_06_file_system.cpp)  
**Difficulty:** Hard  
**Containers:** `std::map<std::string, Node*>` (ordered trie) + `std::string`  
**Complexity:** O(depth * log(breadth)) for path navigation

**Key Concepts:**
- Hierarchical tree structure where nodes store sorted children
- Path tokenization with `std::stringstream`
- Clean memory cleanup and RAII in recursive destructor

---

### Problem 07: Design Twitter ⭐⭐⭐
**File:** [`problem_07_design_twitter.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_07_design_twitter.cpp)  
**Difficulty:** Hard  
**Containers:** `std::unordered_map`, `std::unordered_set`, `std::priority_queue`  
**Complexity:** O(K log N) news feed retrieval using max-heap merge

**Key Concepts:**
- Follower graph modeling with `std::unordered_map<int, std::unordered_set<int>>`
- Global logical timestamp for temporal ordering
- Merging K sorted tweet lists using a bounded heap

### Problem 08: Merge Intervals ⭐⭐
**File:** [`problem_08_merge_intervals.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_08_merge_intervals.cpp)  
**Difficulty:** Medium  
**Containers:** `std::vector` + `std::sort` (batch) & `std::map` (streaming)  
**Complexity:** O(n log n) batch, O(log n) streaming insert  

**Key Concepts:**
- Sorting intervals by start time using custom comparator
- Sequential range overlap merging
- Streaming interval updates using `std::map::lower_bound`

---

## Coverage Summary

### By Container Type

| Container | Problems |
|---|---|
| **`std::unordered_map`** | Two Sum, Top K, Group Anagrams, LRU Cache, Design Twitter |
| **`std::list`** | LRU Cache |
| **`std::stack`** | Valid Parentheses |
| **`std::priority_queue`** | Top K Frequent, Design Twitter |
| **`std::map`** | File System (ordered directory traversal), Merge Intervals (streaming) |
| **`std::unordered_set`** | Design Twitter (following list) |
| **`std::vector`** | All (for storage and result returns) |

### By Difficulty

| Difficulty | Count | Problems |
|---|---|---|
| **Easy** | 2 | Two Sum, Valid Parentheses |
| **Medium** | 3 | Top K Frequent, Group Anagrams, Merge Intervals |
| **Hard / System Design** | 3 | LRU Cache, File System, Design Twitter |

---

## Compilation and Execution

All examples use standard C++20 and compile with:

```bash
cd /home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems

# Compile all at once
for file in problem_*.cpp; do
    g++ -std=c++20 -Wall -Wextra -O2 "$file" -o "${file%.cpp}"
done

# Run examples
./problem_01_lru_cache
./problem_02_two_sum
./problem_03_valid_parentheses
./problem_04_top_k_frequent
./problem_05_group_anagrams
./problem_06_file_system
./problem_07_design_twitter
```

---

## Study Guide

### Week 1: Easy Problems
1. **Two Sum** - Master hash table lookup technique
2. **Valid Parentheses** - Understand stack LIFO mechanics

### Week 2: Medium Problems
3. **Top K Frequent** - Learn size-constrained min-heap technique
4. **Group Anagrams** - Practice hash map with complex keys

### Week 3: Hard & Architecture Problems
5. **LRU Cache** - Combine hash map and linked list with `splice()`
6. **File System** - Multi-container trie navigation
7. **Design Twitter** - Multi-user graph and heap-based feed aggregation

---

## Key Takeaways

### Container Selection Patterns
1. **Need O(1) lookup by key?** $\to$ `std::unordered_map`
2. **Need LIFO behavior?** $\to$ `std::stack`
3. **Need top K elements?** $\to$ `std::priority_queue` (min-heap of size K)
4. **Need O(1) insert/delete with iterator stability?** $\to$ `std::list`
5. **Need sorted directory contents?** $\to$ `std::map`
6. **Need fast member set checks?** $\to$ `std::unordered_set`

---

## More Problems to Practice

See [06_Interview_Problems.md](../../Theory/06_Interview_Problems.md) for:
- Sliding Window Maximum (monotonic deque technique)
- Merge Intervals (interval sorting)
- Find Median from Data Stream (two-heaps pattern)
- LFU Cache (frequency map + list of keys)
- Iterator Safety Debugging Exercises

---

**Total:** 7 compilable problems covering all major container types!
