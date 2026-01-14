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
1. [**STL Overview**](01_STL_Overview.md)  
   High-level architecture, components, and how they work together

2. [**Design Philosophy**](02_Design_Philosophy.md)  
   Why STL is designed this way - critical for senior interviews

3. [**Iterators Deep Dive**](../3_Iterators/03_Iterators_Deep_Dive.md)  
   Complete guide to iterator categories, traits, and mechanics

---

## 📦 Containers

### Sequence Containers
[**Sequence Containers Guide**](../2_Containers/sequence_containers.md)
- `std::array` - Fixed-size, zero-overhead
- `std::vector` - Dynamic array, most used
- `std::deque` - Double-ended queue
- `std::list` - Doubly linked list
- `std::forward_list` - Singly linked list

### Associative Containers (Ordered)
[**Associative Containers Guide**](../2_Containers/associative_containers.md)
- `std::set` / `std::multiset` - Red-Black Tree
- `std::map` / `std::multimap` - Key-value pairs

### Unordered Containers (Hash-based)
[**Unordered Containers Guide**](../2_Containers/unordered_containers.md)
- `std::unordered_set` / `std::unordered_multiset`
- `std::unordered_map` / `std::unordered_multimap`

### Container Adaptors
[**Container Adaptors Guide**](../2_Containers/container_adaptors.md)
- `std::stack` - LIFO
- `std::queue` - FIFO
- `std::priority_queue` - Heap-based

---

## 🧮 Algorithms

1. [**Algorithms Overview**](../4_Algorithms/algorithms_overview.md)
2. [**Algorithm Examples Code**](../../Code/algorithms/algorithm_examples.cpp)

---

## 🎓 Advanced Topics (Senior Level)

### Critical for Interviews
1. [**Container Selection Guide**](../../../Phase2_Selection_Application/Theory/04_Container_Selection_Guide.md) ⭐  
   Decision-making framework - **most asked in senior interviews**

2. [**Iterator Invalidation & Pitfalls**](../../../Phase2_Selection_Application/Theory/05_Iterator_Invalidation.md) ⭐  
   Common bugs and safe patterns

### Utilities
1. [**Functors & Lambdas**](../5_Utilities/functors_lambdas.md)  
   Function objects, predicates, custom comparators

2. [**Utility Types**](../5_Utilities/utility_types.md)  
   `pair`, `tuple`, `optional`, `variant`, `any`

3. [**Memory & Allocators**](../../../Phase3_Optimization/Memory-Optimization/README.md)  
   Allocator concepts, RAII principles

---

## 💻 Practical Examples

### Container Examples (Compilable Code)
- [array_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/Phase1_Fundamentals/Code/containers/array_examples.cpp)
- [vector_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/Phase1_Fundamentals/Code/containers/vector_examples.cpp)
- [deque_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/Phase1_Fundamentals/Code/containers/deque_examples.cpp)
- [list_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/Phase1_Fundamentals/Code/containers/list_examples.cpp)
- [forward_list_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/Phase1_Fundamentals/Code/containers/forward_list_examples.cpp)
- [set_map_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/Phase1_Fundamentals/Code/containers/set_map_examples.cpp)
- [unordered_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/Phase1_Fundamentals/Code/containers/unordered_examples.cpp)
- [adaptors_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/Phase1_Fundamentals/Code/containers/adaptors_examples.cpp)

### Algorithm Examples
- [algorithm_examples.cpp](file:///home/prashanth/learncpp_workspace/STL/Phase1_Fundamentals/Code/algorithms/algorithm_examples.cpp)
- [erase_remove_idiom.cpp](file:///home/prashanth/learncpp_workspace/STL/Phase1_Fundamentals/Code/algorithms/erase_remove_idiom.cpp)
- [custom_comparators.cpp](file:///home/prashanth/learncpp_workspace/STL/Phase1_Fundamentals/Code/algorithms/custom_comparators.cpp)

---

## 🏆 Interview Problems

[**Real Interview Problems with Solutions**](file:///home/prashanth/learncpp_workspace/STL/Phase2_Selection_Application/Theory/06_Interview_Problems.md) ⭐⭐⭐

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

[**STL Quick Reference Cheat Sheet**](file:///home/prashanth/learncpp_workspace/STL/Phase1_Fundamentals/Theory/07_Quick_Reference.md)

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
