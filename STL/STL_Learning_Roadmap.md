# 🗺️ STL Mastery Roadmap: Zero to Architect

## 📑 Table of Contents

1. [📅 The Strategy](#-the-strategy)
2. [🟢 Phase 1: Verified Fundamentals (The "What" & "How")](#-phase-1-verified-fundamentals-the-what--how)
   - [Step 1: Theoretical Foundation](#step-1-theoretical-foundation-read-first)
   - [Step 2: Sequence Containers](#step-2-sequence-containers-arrays-lists--views)
   - [Step 3: Associative Containers](#step-3-associative-containers-trees--hash-maps)
   - [Step 4: Adaptors](#step-4-adaptors-restricted-interfaces)
   - [Step 5: Algorithms & Utilities](#step-5-algorithms--utilities)
3. [🟡 Phase 2: Intelligent Selection & Application (The "Wisdom")](#-phase-2-intelligent-selection--application-the-wisdom)
   - [Step 1: The Decision Framework](#step-1-the-decision-framework)
   - [Step 2: Interview Patterns (Application)](#step-2-interview-patterns-application)
4. [🔴 Phase 3: Architect-Level Optimization (The "Wizardry")](#-phase-3-architect-level-optimization-the-wizardry)
   - [Step 1: Concurrency (Thread Safety)](#step-1-concurrency-thread-safety)
   - [Step 2: Memory Optimization (Low Latency with PMR)](#step-2-memory-optimization-low-latency-with-pmr)
   - [Step 3: Modern C++20 Ranges](#step-3-modern-c20-ranges)
   - [Step 4: Parallel Algorithms & Execution Policies (C++17/20)](#step-4-parallel-algorithms--execution-policies-c1720)
5. [🎓 Graduation Criteria](#-graduation-criteria)

---

## 📅 The Strategy

This roadmap follows a strict **"Crawl, Walk, Run"** progression:
1. **Phase 1**: Knowledge (*What is it? How do I use it? What are the complexity guarantees?*)
2. **Phase 2**: Wisdom (*Which one should I choose? What are the trade-offs and invalidation traps?*)
3. **Phase 3**: Optimization (*How do I make it multi-threaded, memory-efficient, cache-friendly, and modern?*)

---

## 🟢 Phase 1: Verified Fundamentals (The "What" & "How")

*Goal: Understand every container type, iterator capabilities, algorithm principles, and basic syntax.*

### Step 1: Theoretical Foundation (Read First)
* [**01_STL_Overview.md**](Phase1_Fundamentals/Theory/1_General/01_STL_Overview.md) $\rightarrow$ The Big Picture, four pillars, C++11–C++23 evolution.
* [**02_Design_Philosophy.md**](Phase1_Fundamentals/Theory/1_General/02_Design_Philosophy.md) $\rightarrow$ Why STL exists, generic programming, zero-overhead principle.
* [**07_Quick_Reference.md**](Phase1_Fundamentals/Theory/1_General/07_Quick_Reference.md) $\rightarrow$ Big-O complexity tables, iterator category matrix, cheat sheet.

### Step 2: Sequence Containers (Arrays, Lists & Views)
* **Theory**: [`Phase1_Fundamentals/Theory/2_Containers/sequence_containers.md`](Phase1_Fundamentals/Theory/2_Containers/sequence_containers.md) (Vector vs Deque vs List vs Forward List vs Array)
* **Practice**:
  * [`vector_examples.cpp`](Phase1_Fundamentals/Code/containers/vector_examples.cpp) (**Master this: geometric growth, capacity vs size, reallocation**)
  * [`list_examples.cpp`](Phase1_Fundamentals/Code/containers/list_examples.cpp) (Doubly linked lists, node splicing, iterator stability)
  * [`deque_examples.cpp`](Phase1_Fundamentals/Code/containers/deque_examples.cpp) (Segmented chunks, sliding window, front/back efficiency)
  * [`array_examples.cpp`](Phase1_Fundamentals/Code/containers/array_examples.cpp) (Fixed stack arrays, zero overhead)
  * [`forward_list_examples.cpp`](Phase1_Fundamentals/Code/containers/forward_list_examples.cpp) (Singly linked minimal nodes, `insert_after`)
  * [`span_examples.cpp`](Phase1_Fundamentals/Code/containers/span_examples.cpp) (C++20 `std::span` contiguous views, C++17 `std::string_view`)

### Step 3: Associative Containers (Trees & Hash Maps)
* **Theory**:
  * [`Phase1_Fundamentals/Theory/2_Containers/associative_containers.md`](Phase1_Fundamentals/Theory/2_Containers/associative_containers.md) (Sets/Maps - Red-Black Trees, C++17 Node Handles, C++23 Flat Containers)
  * [`Phase1_Fundamentals/Theory/2_Containers/unordered_containers.md`](Phase1_Fundamentals/Theory/2_Containers/unordered_containers.md) (Hash Tables, separate chaining, buckets, rehashing)
* **Practice**:
  * [`set_map_examples.cpp`](Phase1_Fundamentals/Code/containers/set_map_examples.cpp) (Ordered lookup, custom comparators)
  * [`unordered_examples.cpp`](Phase1_Fundamentals/Code/containers/unordered_examples.cpp) (Hash bucketing, load factors, rehashing)

### Step 4: Adaptors (Restricted Interfaces)
* **Theory**: [`Phase1_Fundamentals/Theory/2_Containers/container_adaptors.md`](Phase1_Fundamentals/Theory/2_Containers/container_adaptors.md)
* **Practice**: [`adaptors_examples.cpp`](Phase1_Fundamentals/Code/containers/adaptors_examples.cpp) (Stack LIFO, Queue FIFO, Priority Queue Binary Heap)

### Step 5: Algorithms & Utilities
* **Theory**:
  * [`Phase1_Fundamentals/Theory/3_Iterators/03_Iterators_Deep_Dive.md`](Phase1_Fundamentals/Theory/3_Iterators/03_Iterators_Deep_Dive.md) (Iterator categories, traits, contiguous iterators)
  * [`Phase1_Fundamentals/Theory/4_Algorithms/algorithms_overview.md`](Phase1_Fundamentals/Theory/4_Algorithms/algorithms_overview.md) (Introsort, partitioning, binary search, erase-remove idiom)
  * [`Phase1_Fundamentals/Theory/5_Utilities/functors_lambdas.md`](Phase1_Fundamentals/Theory/5_Utilities/functors_lambdas.md) (Strict weak ordering, lambdas, transparent lookup)
  * [`Phase1_Fundamentals/Theory/5_Utilities/utility_types.md`](Phase1_Fundamentals/Theory/5_Utilities/utility_types.md) (Pairs, tuples, optional, variant, span, bitset)
* **Practice**:
  * [`algorithm_examples.cpp`](Phase1_Fundamentals/Code/algorithms/algorithm_examples.cpp) (Sorting, searching, transforms, heaps)
  * [`erase_remove_idiom.cpp`](Phase1_Fundamentals/Code/algorithms/erase_remove_idiom.cpp) (C++98 erase-remove vs C++20 `std::erase_if`)
  * [`custom_comparators.cpp`](Phase1_Fundamentals/Code/algorithms/custom_comparators.cpp) (Strict weak ordering, priority queue min-heaps, transparent `std::less<>`)

---

## 🟡 Phase 2: Intelligent Selection & Application (The "Wisdom")

*Goal: Apply your knowledge to solve real-world system and interview problems efficiently.*

### Step 1: The Decision Framework
* [**04_Container_Selection_Guide.md**](Phase2_Selection_Application/Theory/04_Container_Selection_Guide.md) $\rightarrow$ **Crucial flowchart** and head-to-head comparisons.
* [**05_Iterator_Invalidation.md**](Phase2_Selection_Application/Theory/05_Iterator_Invalidation.md) $\rightarrow$ The "Gotchas" (Reference vs iterator validity, deque nuances, reallocation crashes).

### Step 2: Interview Patterns (Application)
* [**06_Interview_Problems.md**](Phase2_Selection_Application/Theory/06_Interview_Problems.md) $\rightarrow$ Top 10 system and algorithmic patterns.
* **Hands-on Coding**:
  * [`problem_01_lru_cache.cpp`](Phase2_Selection_Application/Code/interview_problems/problem_01_lru_cache.cpp) (`unordered_map` + `list` with $O(1)$ `splice`)
  * [`problem_02_two_sum.cpp`](Phase2_Selection_Application/Code/interview_problems/problem_02_two_sum.cpp) (Hash map single-pass lookup)
  * [`problem_03_valid_parentheses.cpp`](Phase2_Selection_Application/Code/interview_problems/problem_03_valid_parentheses.cpp) (Stack LIFO bracket matching)
  * [`problem_04_top_k_frequent.cpp`](Phase2_Selection_Application/Code/interview_problems/problem_04_top_k_frequent.cpp) (Priority Queue min-heap vs Bucket sort)
  * [`problem_05_group_anagrams.cpp`](Phase2_Selection_Application/Code/interview_problems/problem_05_group_anagrams.cpp) (Sorted string key hash map)
  * [`problem_06_file_system.cpp`](Phase2_Selection_Application/Code/interview_problems/problem_06_file_system.cpp) (Hierarchical tree/map file system)
  * [`problem_07_design_twitter.cpp`](Phase2_Selection_Application/Code/interview_problems/problem_07_design_twitter.cpp) (Social feed design merging heaps and maps)
  * [`problem_08_merge_intervals.cpp`](Phase2_Selection_Application/Code/interview_problems/problem_08_merge_intervals.cpp) (Interval merging with batch vector sort and streaming map)

---

## 🔴 Phase 3: Architect-Level Optimization (The "Wizardry")

*Goal: Write thread-safe, memory-efficient, SIMD-vectorized, and modern C++ code.*

### Step 1: Concurrency (Thread Safety)
* **Theory**: [`Phase3_Optimization/Thread-Safe/README.md`](Phase3_Optimization/Thread-Safe/README.md) (ISO thread-safety rules, Reference Trap, lock striping)
* **Code**:
  * [`thread_safe_queue.cpp`](Phase3_Optimization/Thread-Safe/thread_safe_queue.cpp) (Producer-Consumer with condition variables)
  * [`thread_safe_map_rwlock.cpp`](Phase3_Optimization/Thread-Safe/thread_safe_map_rwlock.cpp) (Read-Heavy workloads with `std::shared_mutex`)
  * [`thread_safe_vector.cpp`](Phase3_Optimization/Thread-Safe/thread_safe_vector.cpp) (Handling reallocations and returning by value)
  * [`thread_safe_lru_cache.cpp`](Phase3_Optimization/Thread-Safe/thread_safe_lru_cache.cpp) (Synchronized cache)

### Step 2: Memory Optimization (Low Latency with PMR)
* **Theory**: [`Phase3_Optimization/Memory-Optimization/README.md`](Phase3_Optimization/Memory-Optimization/README.md) (Custom allocators, polymorphic memory resources)
* **Code**: [`pmr_benchmark.cpp`](Phase3_Optimization/Memory-Optimization/pmr_benchmark.cpp) (Stack-backed monotonic buffer resources yielding 10x+ speedups)

### Step 3: Modern C++20 Ranges
* **Theory**: [`Phase3_Optimization/Ranges/README.md`](Phase3_Optimization/Ranges/README.md) (Pipe syntax, views, projections, borrowed ranges)
* **Code**: [`ranges_demo.cpp`](Phase3_Optimization/Ranges/ranges_demo.cpp) (Declarative zero-allocation data processing)

### Step 4: Parallel Algorithms & Execution Policies (C++17/20)
* **Theory**: [`Phase3_Optimization/Parallel-Algorithms/README.md`](Phase3_Optimization/Parallel-Algorithms/README.md) (Execution policies: `seq`, `par`, `par_unseq`, `unseq`, vectorization safety)
* **Code**: [`parallel_algorithms_demo.cpp`](Phase3_Optimization/Parallel-Algorithms/parallel_algorithms_demo.cpp) (Parallel sort, parallel reduction with `std::reduce`, map-reduce)

---

## 🎓 Graduation Criteria

You are ready for your Senior Interview when:
1. You can explain **Internal Memory Layout & Invalidation** of Vector vs Deque vs List in depth. (Phase 1)
2. You can justify container choices for any interview scenario with Big-O and cache locality trade-offs. (Phase 2)
3. You can explain how to make containers **Thread-Safe**, how to eliminate allocations using **`std::pmr`**, and how to accelerate throughput with **Parallel Algorithms**. (Phase 3)

**Good Luck! 🚀**
