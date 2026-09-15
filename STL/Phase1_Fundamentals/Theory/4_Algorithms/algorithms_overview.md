# 🧮 STL Algorithms Overview & Architecture Guide

> **Comprehensive Senior-Level Reference for C++ Standard Template Library Algorithms**

---

## 📑 Table of Contents

1. [Architectural Overview](#architectural-overview)
2. [Algorithm Categories](#algorithm-categories)
   - [1. Non-Modifying Sequence Operations](#1-non-modifying-sequence-operations)
   - [2. Modifying Sequence Operations](#2-modifying-sequence-operations)
   - [3. Sorting and Partitioning Algorithms](#3-sorting-and-partitioning-algorithms)
   - [4. Binary Search Operations](#4-binary-search-operations)
   - [5. Numeric Algorithms](#5-numeric-algorithms)
   - [6. Binary Heap Operations](#6-binary-heap-operations)
3. [Deep Dive: Introsort Architecture in std::sort](#deep-dive-introsort-architecture-in-stdsort)
4. [The Erase-Remove Idiom vs C++20 std::erase](#the-erase-remove-idiom-vs-c20-stderase)
5. [C++20 Ranges Algorithms and Projections](#c20-ranges-algorithms-and-projections)
6. [C++17 Parallel Execution Policies](#c17-parallel-execution-policies)
7. [Common Interview Bug Patterns](#common-interview-bug-patterns)
8. [Code Examples](#code-examples)
9. [Key Takeaways](#key-takeaways)

---

## Architectural Overview

The STL algorithm library (`<algorithm>`, `<numeric>`, `<ranges>`) embodies Alexander Stepanov's core insight: **Algorithms are decoupled from containers through the abstraction of iterators**.

```
┌──────────────────┐               ┌──────────────────┐
│    CONTAINER     │               │    ALGORITHM     │
│ (owns & manages  │               │ (processes data  │
│  physical RAM)   │               │  via iterators)  │
└─────────┬────────┘               └────────▲─────────┘
          │                                 │
          │   .begin()           .end()     │
          └───► [Iterator] ─────► [Iterator]┘
```

### Core Design Rules
1. **Iterators, Not Containers**: Algorithms take iterator pairs `[first, last)`. They have zero awareness of the underlying container's type, size, capacity, or memory allocation strategy.
2. **Algorithms Never Resize Containers**: Functions like `std::remove`, `std::unique`, and `std::reverse` only read, write, or swap element values within the valid range `[first, last)`. They cannot alter container capacity or invoke `erase()`.
3. **Complexity Guarantees**: Every algorithm in the standard comes with strict mathematical upper bounds on comparisons, assignments, or predicate evaluations.

---

## Algorithm Categories

### 1. Non-Modifying Sequence Operations
These algorithms inspect elements without modifying order or value:

- **`std::find(first, last, val)`**: Linear search returning iterator to first match, or `last` if not found. $O(N)$ comparisons.
- **`std::find_if(first, last, pred)`**: Returns first element where unary predicate `pred(*it)` evaluates to `true`.
- **`std::count(first, last, val)` / `std::count_if(first, last, pred)`**: Returns occurrences of target value or condition.
- **`std::all_of` / `std::any_of` / `std::none_of`**: Short-circuiting boolean checks on range `[first, last)`.
- **`std::for_each(first, last, func)`**: Applies `func(*it)` to every element sequentially.
- **`std::adjacent_find(first, last)`**: Finds two consecutive identical elements.

### 2. Modifying Sequence Operations
These algorithms reorder, copy, or mutate element values:

- **`std::copy(first, last, d_first)`**: Copies elements to destination buffer.
- **`std::move(first, last, d_first)`**: Moves elements using `std::move` value semantics.
- **`std::transform(first, last, d_first, unary_op)`**: Functional map operation applying `unary_op` and writing output to `d_first`.
- **`std::fill(first, last, val)` / `std::generate(first, last, gen)`**: Populates range with constant or generator output.
- **`std::replace(first, last, old_val, new_val)`**: Replaces matching values in-place.
- **`std::unique(first, last)`**: Compacts adjacent duplicate elements into the front of the range and returns past-the-end iterator of unique elements. (Range must be sorted first!).
- **`std::reverse(first, last)`**: Inverts order in-place using `std::iter_swap`.
- **`std::rotate(first, middle, last)`**: Rotates elements such that `middle` becomes the new `first`.

### 3. Sorting and Partitioning Algorithms

| Algorithm | Complexity | Internal Implementation | Stability | Iterator Requirement |
|---|---|---|:---:|:---:|
| **`std::sort`** | $O(N \log N)$ worst | **Introsort** (Quicksort + Heapsort + Insertion Sort) | ❌ No | RandomAccessIterator |
| **`std::stable_sort`** | $O(N \log N)$ | **Adaptive Merge Sort** | ✅ Yes | RandomAccessIterator |
| **`std::partial_sort`** | $O(N \log K)$ | **Heapsort** on top $K$ elements | ❌ No | RandomAccessIterator |
| **`std::nth_element`** | $O(N)$ average | **Introselect** (Quickselect with median fallback) | ❌ No | RandomAccessIterator |
| **`std::partition`** | $O(N)$ | Two-pointer swap | ❌ No | ForwardIterator |
| **`std::stable_partition`** | $O(N \log N)$ or $O(N)$ with memory | Buffer-backed partition | ✅ Yes | BidirectionalIterator |

### 4. Binary Search Operations
All binary search operations require the underlying range `[first, last)` to be **already sorted** (or partitioned with respect to the query value):

- **`std::lower_bound(first, last, val)`**: Returns iterator to the first element **$\ge val$** (the earliest position where `val` could be inserted without violating sorted order).
- **`std::upper_bound(first, last, val)`**: Returns iterator to the first element **$> val$** (the latest position where `val` could be inserted).
- **`std::equal_range(first, last, val)`**: Returns `std::pair<It, It>` spanning `[lower_bound, upper_bound)`. In a container with duplicates, this gives the exact range of elements equal to `val`.
- **`std::binary_search(first, last, val)`**: Returns `bool` (`true` if element exists, `false` otherwise). Implemented internally as `std::lower_bound` check.

### 5. Numeric Algorithms
Located in `<numeric>`:

- **`std::accumulate(first, last, init)`**: Strictly sequential left-to-right fold.
- **`std::iota(first, last, start_val)`**: Fills range with consecutively incrementing values (`val`, `val+1`, `val+2`, ...).
- **`std::inner_product`**: Computes dot product of two ranges.
- **`std::reduce` (C++17)**: Out-of-order parallelizable reduction. Requires binary operator to be **associative and commutative**!
- **`std::transform_reduce` (C++17)**: Fused map-reduce operation executing map transform and parallel reduction in a single memory pass.

### 6. Binary Heap Operations
All heap functions model a **max-heap** by default inside a random-access range `[first, last)`:
- **`std::make_heap(first, last)`**: Converts range into a binary heap in $O(N)$ time.
- **`std::push_heap(first, last)`**: Re-heaps after an element was appended at `last - 1`. $O(\log N)$ time.
- **`std::pop_heap(first, last)`**: Swaps `first` with `last - 1` and reheaps `[first, last - 1)`. $O(\log N)$ time.
- **`std::sort_heap(first, last)`**: Converts a heap into a fully sorted array in $O(N \log N)$ time (destroys heap property).

---

## Deep Dive: Introsort Architecture in `std::sort`

A common interview question is: *"What algorithm does `std::sort` use?"*
Calling it simply "Quicksort" is incorrect. Standard C++ requires $O(N \log N)$ worst-case time complexity. Pure Quicksort degrades to $O(N^2)$ on adversarial pivot inputs (e.g. sorted arrays or Dutch national flag patterns).

Modern STL implementations use **Introsort** (Introspective Sort):
1. **Phase 1 (Quicksort)**: Starts with median-of-three Quicksort. It partitions the array and tracks the recursion depth limit:
   $$MaxDepth = 2 \times \lfloor \log_2(N) \rfloor$$
2. **Phase 2 (Heapsort Fallback)**: If the recursion depth exceeds $MaxDepth$, Introsort immediately switches the active subpartition to **Heapsort**. This guarantees the worst-case never exceeds $O(N \log N)$.
3. **Phase 3 (Insertion Sort Finish)**: For small subpartitions (typically $\le 16$ elements), the overhead of recursive calls and pivot selection is slower than simple quadratic algorithms. Introsort leaves small subarrays unsorted and finishes the entire container with a single **Insertion Sort** pass ($O(N)$ for nearly sorted data).

---

## The Erase-Remove Idiom vs C++20 `std::erase`

### The Classic Problem
Why does `std::remove` not erase elements?
```cpp
std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};

// INCORRECT:
std::remove(v.begin(), v.end(), 2);
// v.size() is still 7! Memory was NOT freed!
```
Because `std::remove` takes only iterators, it cannot call member methods like `v.erase()` or `v.resize()`. It simply shifts non-matching elements forward and returns a "logical end" iterator:

```
Before: [ 1, 2, 3, 2, 4, 2, 5 ]
After:  [ 1, 3, 4, 5, ?, ?, ? ]
                      ▲
                      └─ Returned Iterator (new_end)
```

### The C++98 / C++11 Idiom
```cpp
// Erase-remove idiom:
v.erase(std::remove(v.begin(), v.end(), 2), v.end());
```

### The Modern C++20 Solution: Uniform Container Erasure
C++20 introduced non-member `std::erase` and `std::erase_if` functions:
```cpp
#include <vector>

std::vector<int> v = {1, 2, 3, 4, 5, 6};

// Erase by value:
std::erase(v, 2);

// Erase by predicate:
std::erase_if(v, [](int x) { return x % 2 == 0; });
```

---

## C++20 Ranges Algorithms and Projections

C++20 modernized the algorithm library through `<ranges>`:
1. **Container Passing**: No need to write `.begin(), .end()`. Pass the container directly:
   ```cpp
   std::ranges::sort(v);
   ```
2. **Projections**: A projection is a callable applied to elements before evaluation. Eliminates complex custom comparators:
   ```cpp
   struct Employee {
       int id;
       std::string name;
   };
   std::vector<Employee> staff = {{2, "Bob"}, {1, "Alice"}};

   // Sort by ID directly without custom lambda comparator:
   std::ranges::sort(staff, {}, &Employee::id);
   ```
3. **Concept Constraints**: Compiler generates clean, readable compile errors when types violate iterator or comparison concepts.

---

## C++17 Parallel Execution Policies

Standard C++17 added execution policies to over 60 STL algorithms:
```cpp
#include <algorithm>
#include <execution>

std::vector<int> data(10'000'000);

// Sequential (default):
std::sort(std::execution::seq, data.begin(), data.end());

// Parallel (multi-threaded work-stealing across CPU cores):
std::sort(std::execution::par, data.begin(), data.end());

// Parallel and Vectorized (multi-threaded + SIMD lanes):
std::sort(std::execution::par_unseq, data.begin(), data.end());
```

> [!WARNING]
> **Parallel Execution Policy Rules**:
> 1. With `std::execution::par`, user lambdas must not introduce **data races** across threads.
> 2. With `std::execution::par_unseq`, code executes across vectorized SIMD lanes on the same thread: you **must not allocate heap memory, acquire mutexes, or call non-thread-safe functions** inside the element callable!
> 3. If an uncaught exception escapes a parallel algorithm policy, `std::terminate()` is invoked immediately.

---

## Common Interview Bug Patterns

| Anti-Pattern | Manifestation | Correct Fix |
|---|---|---|
| Calling `std::sort` on `std::list` | Compiler error: `list` iterators do not satisfy `random_access_iterator`. | Use member function `l.sort()` (bottom-up merge sort). |
| Using `<=` instead of `<` in comparator | Infinite loop or segfault in `std::sort` due to violating strict weak ordering. | Use strict `<` (`comp(x, x)` must return `false`). |
| Forgetting `v.erase()` after `std::remove` | Container size unchanged, duplicate garbage left at tail. | Use C++20 `std::erase(v, val)` or classic erase-remove idiom. |
| Binary search on unsorted range | Wrong answer, element reported missing when present. | Sort range first using identical comparator. |
| Calling `std::accumulate` with parallel policy | Does not support parallel policies; strictly left-to-right. | Use `std::reduce` or `std::transform_reduce`. |

---

## Code Examples

- [`algorithm_examples.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase1_Fundamentals/Code/algorithms/algorithm_examples.cpp): 30+ core algorithms covering sorting, searching, transforms, and heap operations.
- [`erase_remove_idiom.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase1_Fundamentals/Code/algorithms/erase_remove_idiom.cpp): Detailed walkthrough of `std::remove`, `std::unique`, and C++20 `std::erase_if`.
- [`custom_comparators.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase1_Fundamentals/Code/algorithms/custom_comparators.cpp): Strict weak ordering, functors, lambdas, and priority queue ordering.

---

## Key Takeaways

1. **Introsort** guarantees $O(N \log N)$ worst-case for `std::sort` via Quicksort, Heapsort fallback, and Insertion sort.
2. **`std::remove`** shifts values; only container member `erase()` alters size.
3. Use **`std::lower_bound`** ($\ge$) and **`std::upper_bound`** ($>$) for binary search range queries.
4. In C++20, prefer **`std::erase_if`** and **`std::ranges::sort`** with projections.
5. In C++17, use **`std::reduce`** for parallel aggregation, requiring associative and commutative operations.
