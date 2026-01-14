# 🚀 STL Interview Preparation - Master Index

> **For Senior C++ Developers (7+ Years Experience)**  
> Complete guide to mastering STL for technical interviews

---

## 📖 How to Use This Guide

This material is designed for **progressive learning**:
1. Start with **Core Concepts** if you need a refresher
2. Deep dive into **Containers** and **Algorithms** 
3. Study **Advanced Topics** for senior-level questions
4. Practice with **Interview Problems**
5. Use **Quick Reference** during final preparation

---

## 🎯 Core Concepts

### Essential Reading (Start Here)
1. [**STL Overview**](file:///home/prashanth/learncpp_workspace/STL/01_STL_Overview.md)  
   High-level architecture, components, and how they work together

2. [**Design Philosophy**](file:///home/prashanth/learncpp_workspace/STL/02_Design_Philosophy.md)  
   Why STL is designed this way - critical for senior interviews

3. [**Iterators Deep Dive**](file:///home/prashanth/learncpp_workspace/STL/03_Iterators_Deep_Dive.md)  
   Complete guide to iterator categories, traits, and mechanics

---

## 📦 Containers

### Sequence Containers
[**Sequence Containers Guide**](file:///home/prashanth/learncpp_workspace/STL/containers/sequence_containers.md)
- `std::array` - Fixed-size, zero-overhead
- `std::vector` - Dynamic array, most used
- `std::deque` - Double-ended queue
- `std::list` - Doubly linked list
- `std::forward_list` - Singly linked list

### Associative Containers (Ordered)
[**Associative Containers Guide**](file:///home/prashanth/learncpp_workspace/STL/containers/associative_containers.md)
- `std::set` / `std::multiset` - Red-Black Tree
- `std::map` / `std::multimap` - Key-value pairs

### Unordered Containers (Hash-based)
[**Unordered Containers Guide**](file:///home/prashanth/learncpp_workspace/STL/containers/unordered_containers.md)
- `std::unordered_set` / `std::unordered_multiset`
- `std::unordered_map` / `std::unordered_multimap`

### Container Adaptors
[**Container Adaptors Guide**](file:///home/prashanth/learncpp_workspace/STL/containers/container_adaptors.md)
- `std::stack` - LIFO
- `std::queue` - FIFO
- `std::priority_queue` - Heap-based

---

## 🧮 Algorithms

1. [**Algorithms Overview**](file:///home/prashanth/learncpp_workspace/STL/algorithms/algorithms_overview.md)
2. [**Non-Modifying Algorithms**](file:///home/prashanth/learncpp_workspace/STL/algorithms/non_modifying_algorithms.md)  
   `find`, `count`, `all_of`, `any_of`, `none_of`
3. [**Modifying Algorithms**](file:///home/prashanth/learncpp_workspace/STL/algorithms/modifying_algorithms.md)  
   `copy`, `move`, `transform`, `remove`, **erase-remove idiom**
4. [**Sorting & Searching**](file:///home/prashanth/learncpp_workspace/STL/algorithms/sorting_searching.md)  
   `sort`, `binary_search`, `lower_bound`, `upper_bound`
5. [**Numeric Algorithms**](file:///home/prashanth/learncpp_workspace/STL/algorithms/numeric_algorithms.md)  
   `accumulate`, `iota`, `inner_product`
6. [**Heap Algorithms**](file:///home/prashanth/learncpp_workspace/STL/algorithms/heap_algorithms.md)  
   `make_heap`, `push_heap`, `pop_heap`, `sort_heap`

---

## 🎓 Advanced Topics (Senior Level)

### Critical for Interviews
1. [**Container Selection Guide**](file:///home/prashanth/learncpp_workspace/STL/04_Container_Selection_Guide.md) ⭐  
   Decision-making framework - **most asked in senior interviews**

2. [**Iterator Invalidation & Pitfalls**](file:///home/prashanth/learncpp_workspace/STL/05_Iterator_Invalidation.md) ⭐  
   Common bugs and safe patterns

### Utilities
1. [**Functors & Lambdas**](file:///home/prashanth/learncpp_workspace/STL/utilities/functors_lambdas.md)  
   Function objects, predicates, custom comparators

2. [**Utility Types**](file:///home/prashanth/learncpp_workspace/STL/utilities/utility_types.md)  
   `pair`, `tuple`, `optional`, `variant`, `any`

3. [**Memory & Allocators**](file:///home/prashanth/learncpp_workspace/STL/utilities/memory_allocators.md)  
   Allocator concepts, RAII principles

---

## 💻 Practical Examples

### Container Examples (Compilable Code)
- [array_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/examples/containers/array_examples.cpp)
- [vector_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/examples/containers/vector_examples.cpp)
- [deque_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/examples/containers/deque_examples.cpp)
- [list_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/examples/containers/list_examples.cpp)
- [forward_list_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/examples/containers/forward_list_examples.cpp)
- [set_map_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/examples/containers/set_map_examples.cpp)
- [unordered_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/examples/containers/unordered_examples.cpp)
- [adaptors_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/examples/containers/adaptors_examples.cpp)

### Algorithm Examples
- [algorithm_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/examples/algorithms/algorithm_examples.cpp)
- [erase_remove_idiom.cpp](file:///home/prashanth/learncpp_workspace/STL/examples/algorithms/erase_remove_idiom.cpp)
- [custom_comparators.cpp](file:///home/prashanth/learncpp_workspace/STL/examples/algorithms/custom_comparators.cpp)

---

## 🏆 Interview Problems

[**Real Interview Problems with Solutions**](file:///home/prashanth/learncpp_workspace/STL/06_Interview_Problems.md) ⭐⭐⭐

### Problem Categories
- **LRU Cache** - Classic design problem
- **Sliding Window** - Array/deque techniques
- **Interval Problems** - Sorting + merging
- **Top K Elements** - Heap/priority_queue
- **Custom Data Structures** - Combining containers
- **Iterator Safety** - Advanced debugging

Each problem includes:
- Problem statement
- Multiple solution approaches
- Time/space complexity analysis
- STL container justification
- Common pitfalls

---

## ⚡ Quick Reference

[**STL Quick Reference Cheat Sheet**](file:///home/prashanth/learncpp_workspace/STL/07_Quick_Reference.md)

Perfect for last-minute review:
- Complexity tables
- Iterator category matrix
- Common idioms
- Container selection flowchart

---

## 🎯 Interview Preparation Strategy

### Week 1-2: Foundations
- [ ] Read STL Overview & Design Philosophy
- [ ] Master Iterators Deep Dive
- [ ] Study all Container guides
- [ ] Run all container examples

### Week 3: Algorithms & Patterns
- [ ] Study all Algorithm guides
- [ ] Practice erase-remove idiom
- [ ] Master custom comparators
- [ ] Run all algorithm examples

### Week 4: Advanced Topics
- [ ] Master Container Selection Guide
- [ ] Study Iterator Invalidation thoroughly
- [ ] Review Utility types
- [ ] Practice decision-making scenarios

### Week 5+: Problem Solving
- [ ] Solve all Interview Problems
- [ ] Time yourself on each problem
- [ ] Explain your container choices
- [ ] Review Quick Reference daily

---

## 📊 What Interviewers Expect at Senior Level

### ✅ EXPECTED
- Internal data structure knowledge (not implementation)
- Complexity guarantees (time/space)
- Iterator behavior and categories
- Correct container selection with justification
- Iterator invalidation awareness
- STL idioms (erase-remove, etc.)

### ❌ NOT EXPECTED
- Implementing vector/map from scratch
- Writing tree rotations
- Coding hash tables
- Low-level memory management details

---

## 🔥 Most Important Topics for Interviews

1. **Container Selection** (asked in 90% of senior interviews)
2. **Iterator Invalidation** (common debugging questions)
3. **Complexity Analysis** (must know for every operation)
4. **Erase-Remove Idiom** (very common gotcha)
5. **Custom Comparators** (frequently needed in problems)
6. **Unordered vs Ordered** (trade-off questions)

---

## 📝 Notes

- All code examples are C++17/C++20 compatible
- Compile with: `g++ -std=c++20 -Wall -Wextra -O2 filename.cpp`
- Each document is self-contained but cross-referenced
- Focus on **understanding WHY** over memorization

---

**Good luck with your interviews! 🚀**

*Remember: Senior interviews focus on decision-making and trade-offs, not just syntax.*
