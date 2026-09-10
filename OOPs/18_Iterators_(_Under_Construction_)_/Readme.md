# Chapter 18: Iterators

An **iterator** is an object designed to traverse through a container (such as the values in an array, or the characters in a string), providing uniform sequential or random access to each element along the way without exposing the container's internal data structures.

---

## Table of Contents
1. [1 — Introduction to iterators](#1--introduction-to-iterators)
   - [What is an Iterator?](#what-is-an-iterator)
   - [Types of Iterators in C++](#types-of-iterators-in-c)
   - [Pointers as an Iterator](#pointers-as-an-iterator)
   - [Standard Library Iterators](#standard-library-iterators)
   - [Container Iterator Functions](#container-iterator-functions)
   - [Non-Member Iterator Functions (std::begin and std::end)](#non-member-iterator-functions-stdbegin-and-stdend)
   - [operator< vs operator!= for Iterators](#operator-vs-operator-for-iterators)
   - [How Range-Based For Loops Work Behind the Scenes](#how-range-based-for-loops-work-behind-the-scenes)
   - [Iterator Invalidation (Dangling Iterators)](#iterator-invalidation-dangling-iterators)
   - [📁 Code Examples](#-code-examples)

---

## 1 — Introduction to iterators

### What is an Iterator?
- An **iterator** is an object designed to traverse through a container (e.g. the values in an array, or the characters in a string), providing access to each element along the way.
- Once the appropriate type of iterator is created, we can use the interface provided by the iterator to traverse and access elements **without worrying about the underlying traversal mechanics or how data is stored in the container**.
- Because all C++ iterators share the same basic interface (`operator++` for stepping forward and `operator*` for dereferencing), you can iterate over many completely different container types (arrays, vectors, linked lists, sets, maps) in an identical, uniform way.

### Types of Iterators in C++
C++ categorizes iterators based on the operations and traversal directions they support:

```
┌────────────────────────────────────────────────────────────────────────┐
│                        Iterator Hierarchy                              │
│                                                                        │
│   Input Iterator  ──┐                                                  │
│                     ├─► Forward Iterator ─► Bidirectional ─► Random ─► Contiguous │
│   Output Iterator ──┘                                        Access    (C++20)   │
└────────────────────────────────────────────────────────────────────────┘
```

| Iterator Category | Capabilities | Supported Operations | Typical Containers / Adapters |
|---|---|---|---|
| **1. Input Iterator** | Read elements sequentially (single-pass only) | `*it` (read-only), `++it`, `it++`, `==`, `!=` | `std::istream_iterator`, single-pass algorithms |
| **2. Output Iterator** | Write elements sequentially (single-pass only) | `*it = val` (write-only), `++it`, `it++` | `std::ostream_iterator`, `std::back_inserter` |
| **3. Forward Iterator** | Read and write, multi-pass traversal forward | `*it`, `++it`, `it++`, `==`, `!=` | `std::forward_list`, `std::unordered_set`, `std::unordered_map` |
| **4. Bidirectional Iterator** | Move both forward and backward | `*it`, `++it`, `--it`, `==`, `!=` | `std::list`, `std::set`, `std::map`, `std::multiset`, `std::multimap` |
| **5. Random Access Iterator** | Jump directly to any element in $O(1)$ time | `*it`, `++`, `--`, `+`, `-`, `+=`, `-=`, `[]`, `<`, `>`, `<=`, `>=` | `std::vector`, `std::deque`, `std::array` |
| **6. Contiguous Iterator (C++20)** | Random access where elements are stored in adjacent memory locations | All random access operations plus pointer arithmetic guarantees: `&(*(it + n)) == (&(*it) + n)` | `std::vector`, `std::array`, `std::string`, `std::span`, raw pointers |

### Pointers as an Iterator
The simplest kind of iterator is a raw pointer. Pointers naturally function as contiguous random-access iterators for sequentially stored data using pointer arithmetic:

```cpp
#include <iostream>
#include <array>

int main() {
    std::array arr{ 100, 200, 300, 400, 500 };

    auto begin{ &arr[0] };
    auto end{ &arr[0] + std::size(arr) }; // One-past-the-end pointer

    for (auto it{ begin }; it != end; ++it) {
        std::cout << *it << ' ';
    }
    std::cout << '\n';
}
```

### Standard Library Iterators
- All standard library containers offer direct, first-class support for iteration.
- Containers provide member functions like `begin()` and `end()` that return iterators representing the start and end points of the sequence:

```cpp
std::array arr{ 100, 200, 300, 400, 500 };

for (auto it{ arr.begin() }; it != arr.end(); ++it) {
    std::cout << *it << ' ';
}
```

### Container Iterator Functions
STL containers provide member functions that return iterators pointing to the beginning and one-past-the-end of the container:

| Method | Return Iterator Type | Description |
|---|---|---|
| `begin()` | `iterator` | Points to the first element of the container. |
| `end()` | `iterator` | Points to the theoretical element **just after the last element** of the container. |
| `cbegin()` | `const_iterator` | Points to the first element; elements cannot be modified through this iterator. |
| `cend()` | `const_iterator` | Points to the theoretical element just after the last element; read-only. |
| `rbegin()` | `reverse_iterator` | Points to the **last valid element** of the container, beginning a backward traversal. |
| `rend()` | `reverse_iterator` | Points to the theoretical element **preceding the first element** of the container. |
| `crbegin()` | `const_reverse_iterator` | Points to the last valid element; backward and read-only. |
| `crend()` | `const_reverse_iterator` | Points to the theoretical element preceding the first element; backward and read-only. |

> [!WARNING]
> The `end()` iterator does **NOT** point to the last element of the container—it points to the theoretical **one-past-the-end** position. Dereferencing `end()` is **undefined behavior**.

```cpp
std::vector vec{ 1, 2, 3, 4, 5 };

// Reverse iteration: ++it moves backward!
for (auto it{ vec.rbegin() }; it != vec.rend(); ++it) {
    std::cout << *it << ' '; // Prints: 5 4 3 2 1
}
```

### Non-Member Iterator Functions (`std::begin` and `std::end`)
- The `<iterator>` header provides generic, non-member functions: `std::begin()`, `std::end()`, `std::cbegin()`, `std::cend()`, `std::rbegin()`, `std::rend()`, `std::crbegin()`, `std::crend()`.
- `std::begin` and `std::end` for C-style arrays are defined in the `<iterator>` header.
- `std::begin` and `std::end` for standard containers are also defined in the container headers (e.g. `<array>`, `<vector>`).
- Using non-member `std::begin()` and `std::end()` is more generic because it works identically for both STL containers and built-in C-style arrays:

```cpp
#include <iostream>
#include <iterator>

int arr[]{ 10, 20, 30 };

for (auto it{ std::begin(arr) }; it != std::end(arr); ++it) {
    std::cout << *it << ' ';
}
```

> [!NOTE]
> **C++20 Constrained Range Access**: C++20 introduced `std::ranges::begin()` and `std::ranges::end()`, which prevent dangling iterators when called on temporary rvalue containers.

### `operator<` vs `operator!=` for Iterators
- When writing explicit iterator loops, you must test whether the iterator has reached the end:
  ```cpp
  for (it = arr.begin(); it != arr.end(); ++it) // Preferred: operator!=
  // for (it = arr.begin(); it < arr.end(); ++it) // Avoid: operator<
  ```

> [!IMPORTANT]
> **Always prefer `operator!=` over `operator<` when comparing iterators.**  
> Relational comparisons (`<`, `<=`) require **random access iterators** (such as `std::vector` or `std::array`). Many standard containers—such as linked lists (`std::list`), sets (`std::set`), and maps (`std::map`)—provide bidirectional or forward iterators that **do not support `operator<`**.  
> `operator!=` works across **all iterator categories**, ensuring consistent and generic code.

### How Range-Based For Loops Work Behind the Scenes
Behind the scenes, range-based `for` loops expand conceptually into standard iterator loops:

```cpp
// Your range-based for loop:
for (const auto& elem : container) {
    std::cout << elem << ' ';
}

// What the compiler translates it to:
auto&& __range = container;
auto __begin = std::begin(__range);
auto __end   = std::end(__range);
for (; __begin != __end; ++__begin) {
    const auto& elem = *__begin;
    std::cout << elem << ' ';
}
```

#### Why Fixed C-Style Arrays Work, but Decayed Pointers Fail:
- `std::array` and `std::vector` have member `begin()` and `end()` functions, so range-for loops work effortlessly.
- Fixed-size C-style arrays (e.g. `int arr[5]`) work with range-based `for` loops because `std::begin(arr)` and `std::end(arr)` deduce the array bounds from the array type information.
- **Decayed C-style arrays (pointers) fail to compile**: When an array decays into an `int*` (such as in a function parameter), the type loses all size information. `std::end` cannot be evaluated for an `int*`, so range-based `for` loops **cannot** iterate over decayed pointers.

### Iterator Invalidation (Dangling Iterators)
Much like pointers and references, iterators can be left **"dangling"** if the elements they point to are moved, reallocated in memory, or destroyed. When this happens, we say the iterator has been **invalidated**. Accessing or advancing an invalidated iterator produces **undefined behavior**.

#### Common Causes of Iterator Invalidation:
1. **Capacity Reallocation in `std::vector`**:
   Adding elements with `push_back()` or `emplace_back()` when `size() == capacity()` triggers dynamic reallocation. All existing iterators, pointers, and references to elements of that vector become dangling.
2. **Modifying Containers During a Range-Based For Loop**:
   Since range-based `for` loops cache `begin` and `end` iterators at the start of the loop, modifying the container size inside the loop causes undefined behavior:
   ```cpp
   std::vector v{ 10, 20, 30, 40, 50 };

   // ❌ DANGEROUS: Undefined Behavior!
   for (auto num : v) {
       if (num % 2 == 0)
           v.push_back(num + 1); // Reallocation invalidates cached loop iterators!
   }
   ```
3. **Element Erasing (`erase`)**:
   `v.erase(it)` destroys the element at `it` and shifts subsequent elements to fill the gap. Consequently, `it` and all subsequent iterators are invalidated:
   ```cpp
   // ❌ Undefined behavior:
   v.erase(it);
   ++it; // Accessing invalidated iterator!
   ```

#### How to Revalidate Iterators:
An invalidated iterator can be revalidated by assigning a new valid iterator to it.  
The member function `erase()` returns an iterator to the element immediately following the erased element (or `end()` if the last element was removed).

```cpp
// ✅ Correct idiom: reassign the returned iterator
it = v.erase(it); // 'it' is now valid and points to the next element
```

### 📁 Code Examples
- [`1_pointers_as_an_iterators.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/18_Iterators_(_Under_Construction_)_/1_pointers_as_an_iterators.cpp): Demonstrates raw pointers acting as iterators with `begin` and `end` addresses and pointer arithmetic.
- [`2_standard_library_iterators.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/18_Iterators_(_Under_Construction_)_/2_standard_library_iterators.cpp): Demonstrates standard library container member functions `.begin()` and `.end()`, and explains why `operator!=` is preferred over `operator<`.
- [`3_global_iterators.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/18_Iterators_(_Under_Construction_)_/3_global_iterators.cpp): Demonstrates generic non-member iterator functions `std::begin()` and `std::end()`.
- [`4_range_based_loop_for_std_array.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/18_Iterators_(_Under_Construction_)_/4_range_based_loop_for_std_array.cpp): Demonstrates range-based `for` loops utilizing member iterators under the hood for `std::array`.
- [`5_range_based_loop_for_C-style_array.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/18_Iterators_(_Under_Construction_)_/5_range_based_loop_for_C-style_array.cpp): Demonstrates range-based `for` loops and non-member `std::begin`/`std::end` operating on fixed C-style arrays.
- [`6_iterator_invalidation.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/18_Iterators_(_Under_Construction_)_/6_iterator_invalidation.cpp): Demonstrates the pitfall of modifying a `std::vector` inside a range-based `for` loop causing iterator invalidation.
- [`7_iterator_invalidation.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/18_Iterators_(_Under_Construction_)_/7_iterator_invalidation.cpp): Demonstrates iterator invalidation upon calling `erase()` and the canonical revalidation fix using `it = v.erase(it)`.
- [`8_const_and_reverse_iterators.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/18_Iterators_(_Under_Construction_)_/8_const_and_reverse_iterators.cpp): Demonstrates const iterators (`cbegin`/`cend`) for read-only access, reverse iterators (`rbegin`/`rend`) for backward traversal, and non-member `std::crbegin`/`std::crend`.
