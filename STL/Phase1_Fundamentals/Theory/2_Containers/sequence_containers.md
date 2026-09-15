# 📦 Sequence Containers - Complete Guide

> **`array`, `vector`, `deque`, `list`, `forward_list`**

---

## 📑 Table of Contents

1. [Overview](#overview)
2. [1. std::array - Fixed-Size Array](#1-stdarray---fixed-size-array)
3. [2. std::vector - Dynamic Array](#2-stdvector---dynamic-array)
   - [Growth Strategy & Factor Analysis](#growth-strategy)
   - [Iterator Invalidation](#iterator-invalidation)
   - [C++20 Uniform Erasure](#c20-uniform-container-erasure)
   - [Interview Deep Dives](#interview-points-1)
4. [3. std::deque - Double-Ended Queue](#3-stddeque---double-ended-queue)
   - [Internal Structure](#internal-structure)
   - [Iterator Invalidation Rules](#iterator-invalidation-rules-critical-interview-topic)
5. [4. std::list - Doubly Linked List](#4-stdlist---doubly-linked-list)
   - [The splice() Operation](#basic-operations-3)
6. [5. std::forward_list - Singly Linked List](#5-stdforward_list---singly-linked-list)
7. [Comparison Summary](#comparison-summary)
8. [Next Steps](#next-steps)

---

## Overview

Sequence containers store elements in **linear order** with positional access.

| Container | Internal Structure | Iterator | Best For |
|-----------|-------------------|----------|----------|
| `array` | Fixed-size array | Random Access, Contiguous | Compile-time fixed size |
| `vector` | Dynamic array | Random Access, Contiguous | General-purpose dynamic array |
| `deque` | Segmented array | Random Access | Front/back operations |
| `list` | Doubly linked list | Bidirectional | Frequent middle insert/delete |
| `forward_list` | Singly linked list | Forward | Memory-constrained linked list |

---

## 1️⃣ `std::array` - Fixed-Size Array

### Characteristics
- **Fixed size** determined at compile time
- **Stack allocated** (no heap allocation)
- **Zero overhead** over C arrays
- **Contiguous memory**
- **Random access** O(1)

### When to Use
✅ Size known at compile time  
✅ Want stack allocation  
✅ Need zero overhead  
✅ Interfacing with C APIs  

### Basic Operations

```cpp
#include <array>

// Declaration
std::array<int, 5> arr = {1, 2, 3, 4, 5};
std::array<int, 5> arr2{};  // All zeros

// Access
arr[0] = 10;           // No bounds checking
arr.at(0) = 10;        // Bounds checking (throws)
int first = arr.front();
int last = arr.back();

// Size
size_t size = arr.size();       // Always 5
bool empty = arr.empty();       // Always false (unless size is 0)

// Iterators
for (auto it = arr.begin(); it != arr.end(); ++it) { /* ... */ }
for (int& x : arr) { /* ... */ }  // Range-based for

// Comparison
std::array<int, 5> arr3 = {1, 2, 3, 4, 5};
bool equal = (arr == arr3);  // Lexicographical comparison

// Fill
arr.fill(42);  // All elements = 42

// Swap
arr.swap(arr2);  // O(n) - swaps each element
```

### Complexity

| Operation | Complexity |
|-----------|-----------|
| Access `[]` / `at()` | O(1) |
| `front()` / `back()` | O(1) |
| Iteration | O(n) |
| `fill()` | O(n) |
| `swap()` | O(n) |

### Interview Points

**Q: `array` vs C-style array?**
```cpp
// C-style array
int c_arr[5] = {1, 2, 3, 4, 5};
// ❌ No size() method
// ❌ Decays to pointer easily
// ❌ No bounds checking

// std::array
std::array<int, 5> arr = {1, 2, 3, 4, 5};
// ✅ Has size() method
// ✅ Doesn't decay to pointer
// ✅ Has at() with bounds checking
// ✅ Works with STL algorithms
```

**Q: `array` vs `vector`?**
- `array`: Fixed size, stack allocated, zero overhead
- `vector`: Dynamic size, heap allocated, small overhead (capacity tracking)

---

## 2️⃣ `std::vector` - Dynamic Array

### Characteristics
- **Dynamic size** (grows/shrinks)
- **Contiguous memory** (cache-friendly)
- **Random access** O(1)
- **Amortized O(1)** append
- **Most commonly used** container

### When to Use
✅ Default choice for dynamic arrays  
✅ Need random access  
✅ Mostly append operations  
✅ Cache performance critical  

### Basic Operations

```cpp
#include <vector>

// Declaration
std::vector<int> v;                    // Empty
std::vector<int> v(10);                // 10 elements (default-initialized)
std::vector<int> v(10, 42);            // 10 elements, all 42
std::vector<int> v = {1, 2, 3, 4, 5};  // Initializer list
std::vector<int> v2(v);                // Copy constructor
std::vector<int> v3(std::move(v));     // Move constructor

// Capacity
size_t size = v.size();        // Number of elements
size_t cap = v.capacity();     // Allocated space
bool empty = v.empty();
v.reserve(100);                // Pre-allocate capacity
v.shrink_to_fit();             // Request to reduce capacity

// Access
v[0] = 10;                     // No bounds checking
v.at(0) = 10;                  // Bounds checking (throws)
int first = v.front();
int last = v.back();
int* data = v.data();          // Pointer to underlying array

// Modifiers
v.push_back(6);                // Append (may reallocate)
v.emplace_back(7);             // Construct in-place
v.pop_back();                  // Remove last
v.insert(v.begin() + 2, 99);   // Insert at position
v.erase(v.begin() + 2);        // Erase at position
v.erase(v.begin(), v.begin() + 3);  // Erase range
v.clear();                     // Remove all elements (size = 0, capacity unchanged)
v.resize(20);                  // Change size
v.resize(20, 42);              // Change size, fill with 42

// Swap
v.swap(v2);                    // O(1) - swaps pointers
```

### Complexity

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Access `[]` / `at()` | O(1) | |
| `push_back()` | O(1)* | Amortized |
| `pop_back()` | O(1) | |
| `insert()` (middle) | O(n) | Shifts elements |
| `erase()` (middle) | O(n) | Shifts elements |
| `find()` | O(n) | Linear search |

\* Amortized O(1) - occasionally O(n) due to reallocation

### Growth Strategy

```cpp
std::vector<int> v;
std::cout << "Size: " << v.size() << ", Capacity: " << v.capacity() << "\n";

for (int i = 0; i < 10; ++i) {
    v.push_back(i);
    std::cout << "Size: " << v.size() << ", Capacity: " << v.capacity() << "\n";
}

// Typical output (growth factor ~1.5 or 2):
// Size: 0, Capacity: 0
// Size: 1, Capacity: 1
// Size: 2, Capacity: 2
// Size: 3, Capacity: 4
// Size: 4, Capacity: 4
// Size: 5, Capacity: 8
// ...
```

### Iterator Invalidation

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
auto it = v.begin() + 2;

// Reallocation invalidates ALL iterators
if (v.size() == v.capacity()) {
    v.push_back(6);  // ⚠️ it is now invalid!
}

// Insertion invalidates from insertion point to end
v.insert(v.begin() + 1, 99);  // ⚠️ it may be invalid

// Erase invalidates from erase point to end
v.erase(v.begin() + 2);  // ⚠️ it is invalid
```

### Interview Points

**Q: Why does `vector` grow geometrically (1.5x or 2x)? Why not arithmetically (+100 elements)?**  
**A:** If a vector grew arithmetically by a fixed constant $K$, appending $N$ elements would require $N/K$ reallocations, resulting in $O(N^2)$ total work (an average of $O(N)$ per `push_back`). Geometric growth guarantees **amortized $O(1)$** insertion time:
- **GCC / Clang (`libstdc++`, `libc++`)**: Uses a **$2\times$** factor (doubling). Simple and fast bit shifts, but never reuses memory blocks from earlier allocations in standard memory managers.
- **MSVC (`msvcstl`)**: Uses a **$1.5\times$** factor. A factor below the golden ratio ($\phi \approx 1.618$) mathematically permits the memory allocator to reuse memory previously allocated and freed by earlier reallocations of the same vector once sufficient steps have passed.

### C++20 Uniform Container Erasure

In C++20, the verbose Erase-Remove idiom has been superseded by non-member functions in `<vector>`:
```cpp
std::vector<int> v = {1, 2, 3, 4, 5, 6};

// C++20: Erase all occurrences of value 3
std::erase(v, 3);

// C++20: Erase all matching a predicate (even numbers)
std::erase_if(v, [](int x) { return x % 2 == 0; });
```

**Q: `vector<bool>` is special?**  
**A:** Yes! It is a **space-optimized template specialization** that packs 8 boolean flags per byte instead of using a full byte per bool. Consequence:
- ❌ `operator[]` cannot return `bool&` (a hardware address cannot point to an individual bit). It returns a temporary proxy object `std::vector<bool>::reference`.
- ❌ Code like `auto& ref = v[0];` fails to compile!
- ❌ Not thread-safe for concurrent writes to distinct indices on the same byte!
- ✅ Use `std::vector<char>` or `std::deque<bool>` if real `bool&` references or concurrent writes are required.

---

## 3️⃣ `std::deque` - Double-Ended Queue

### Characteristics
- **Segmented storage** (chunks of contiguous memory)
- **Random access** O(1) (slightly slower than `vector`)
- **Fast insert/delete** at both ends O(1)
- **No element relocation on growth**: Chunks are allocated independently; pointers and references to existing elements remain valid upon `push_front`/`push_back`, though iterators are invalidated.

### When to Use
✅ Need fast front AND back insertion  
✅ Don't need contiguous memory  
✅ Implementing queue or sliding window  

### Internal Structure

```
deque:  [chunk1] [chunk2] [chunk3] [chunk4]
         ↑                            ↑
         front                        back
         
Each chunk is contiguous, but chunks are not adjacent in memory
```

### Basic Operations

```cpp
#include <deque>

// Declaration
std::deque<int> d;
std::deque<int> d = {1, 2, 3, 4, 5};

// Access (same as vector)
d[0] = 10;
d.at(0) = 10;
int first = d.front();
int last = d.back();

// Modifiers
d.push_back(6);      // O(1) - append at end
d.push_front(0);     // O(1) - prepend at front
d.pop_back();        // O(1) - remove from end
d.pop_front();       // O(1) - remove from front
d.insert(d.begin() + 2, 99);  // O(n) - middle insertion
d.erase(d.begin() + 2);       // O(n) - middle deletion

// No reserve() or capacity() (no reallocation concept)
```

### Complexity

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Access `[]` / `at()` | O(1) | Slightly slower than `vector` |
| `push_back()` / `push_front()` | O(1) | |
| `pop_back()` / `pop_front()` | O(1) | |
| `insert()` (middle) | O(n) | |
| `erase()` (middle) | O(n) | |

### Iterator Invalidation Rules (Critical Interview Topic)

> [!WARNING]
> **Common Misconception Alert:**
> Many developers mistakenly believe that inserting at the ends of a `std::deque` preserves iterators.
> Under the ISO C++ Standard (`[deque.modifiers]`):
> - **Insertion at either end (`push_front`, `push_back`, `emplace_front`, `emplace_back`)**: **INVALIDATES ALL ITERATORS** to the deque, but **REFERENCES AND POINTERS REMAIN VALID**!
> - **Insertion in the middle**: **INVALIDATES ALL iterators, references, and pointers**.
> - **Erasure at either end (`pop_front`, `pop_back`)**: Invalidates **only** iterators, references, and pointers to the erased element.
> - **Erasure in the middle**: **INVALIDATES ALL iterators, references, and pointers**.

```cpp
std::deque<int> d = {1, 2, 3, 4, 5};
int& ref = d[2];         // Reference to element '3'
auto it = d.begin() + 2; // Iterator to element '3'

d.push_back(6);   // ⚠️ 'it' is INVALIDATED! (Undetected UB if dereferenced)
                  // ✅ 'ref' remains 100% VALID! (Memory chunk wasn't moved)

std::cout << ref << "\n"; // Safe: prints 3
// *it;                   // Undefined Behavior: iterator was invalidated!

// Middle operations invalidate EVERYTHING (iterators AND references):
d.insert(d.begin() + 2, 99); // ⚠️ BOTH 'it' and 'ref' are completely invalid!
```

### Interview Points

**Q: `deque` vs `vector`?**
- `vector`: Contiguous memory, faster random access, only fast at back
- `deque`: Segmented memory, fast at both ends, slightly slower random access

**Q: When to use `deque`?**  
**A:** Sliding window, queue implementation, need fast front insertion.

```cpp
// Sliding window example
std::deque<int> window;
for (int x : data) {
    window.push_back(x);
    if (window.size() > k) {
        window.pop_front();  // O(1) - vector would be O(n)!
    }
    // Process window...
}
```

---

## 4️⃣ `std::list` - Doubly Linked List

### Characteristics
- **Doubly linked list** (each node has prev/next pointers)
- **Bidirectional iterators** (can go forward/backward)
- **No random access** (must traverse)
- **Stable iterators** (insertion/deletion doesn't invalidate others)
- **High memory overhead** (~16 bytes per element)

### When to Use
✅ Frequent middle insert/delete on large containers  
✅ Need iterator stability  
✅ Splicing operations  
❌ Rarely the best choice (cache performance usually beats Big-O)

### Basic Operations

```cpp
#include <list>

// Declaration
std::list<int> l;
std::list<int> l = {1, 2, 3, 4, 5};

// Access (NO operator[] or at()!)
int first = l.front();
int last = l.back();

// Modifiers
l.push_back(6);       // O(1)
l.push_front(0);      // O(1)
l.pop_back();         // O(1)
l.pop_front();        // O(1)
auto it = l.begin();
++it;
l.insert(it, 99);     // O(1) - insert before iterator
l.erase(it);          // O(1) - erase at iterator

// List-specific operations
l.sort();             // O(n log n) - merge sort
l.reverse();          // O(n)
l.unique();           // O(n) - remove consecutive duplicates
l.remove(42);         // O(n) - remove all elements equal to 42
l.remove_if([](int x) { return x % 2 == 0; });  // Remove if predicate

// Splicing (move elements from another list)
std::list<int> l2 = {10, 20, 30};
l.splice(l.begin(), l2);  // Move all of l2 to beginning of l
// l is now {10, 20, 30, 1, 2, 3, 4, 5}
// l2 is now empty
```

### Complexity

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Access (traverse) | O(n) | No random access |
| `push_back()` / `push_front()` | O(1) | |
| `pop_back()` / `pop_front()` | O(1) | |
| `insert()` | O(1) | If you have iterator |
| `erase()` | O(1) | If you have iterator |
| `sort()` | O(n log n) | Merge sort |
| `splice()` | O(1) or O(n) | Depends on operation |

### Iterator Invalidation

```cpp
std::list<int> l = {1, 2, 3, 4, 5};
auto it1 = l.begin();
auto it2 = ++l.begin();

// Insertion doesn't invalidate any iterators
l.insert(it2, 99);  // ✅ it1 and it2 still valid

// Erase only invalidates erased iterator
l.erase(it2);  // ⚠️ it2 invalid, ✅ it1 still valid
```

### Interview Points

**Q: Why does `list` have its own `sort()`?**  
**A:** `std::sort` requires random-access iterators. `list` only has bidirectional iterators. `list::sort()` uses merge sort, which works with bidirectional iterators.

**Q: When is `list` actually better than `vector`?**  
**A:** Rarely! Modern CPUs love contiguous memory. `list` is only better when:
1. Container is very large (>10,000 elements)
2. Frequent middle insertions/deletions
3. Iterator stability is critical

Even then, benchmark first!

---

## 5️⃣ `std::forward_list` - Singly Linked List

### Characteristics
- **Singly linked list** (only next pointer, no prev)
- **Forward iterators** (can only go forward)
- **No `size()`** method (would be O(n))
- **Minimal memory overhead** (~8 bytes per element)
- **Most memory-efficient** linked list

### When to Use
✅ Memory constrained  
✅ Only need forward traversal  
✅ Don't need `size()`  

### Basic Operations

```cpp
#include <forward_list>

// Declaration
std::forward_list<int> fl;
std::forward_list<int> fl = {1, 2, 3, 4, 5};

// Access
int first = fl.front();
// ❌ No back() - would be O(n)

// Modifiers
fl.push_front(0);     // O(1)
fl.pop_front();       // O(1)
// ❌ No push_back() - would be O(n)

// Insert/erase AFTER position
auto it = fl.begin();
fl.insert_after(it, 99);   // Insert after it
fl.erase_after(it);        // Erase element after it

// List-specific operations
fl.sort();            // O(n log n) - merge sort
fl.reverse();         // O(n)
fl.unique();          // O(n)
fl.remove(42);        // O(n)

// No size() method!
// Use std::distance(fl.begin(), fl.end()) if needed (O(n))
```

### Complexity

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Access (traverse) | O(n) | |
| `push_front()` | O(1) | |
| `pop_front()` | O(1) | |
| `insert_after()` | O(1) | |
| `erase_after()` | O(1) | |
| `sort()` | O(n log n) | |

### Interview Points

**Q: `forward_list` vs `list`?**
- `forward_list`: Smaller memory, forward-only, no `size()`
- `list`: Bidirectional, has `size()`, more features

**Q: Why no `size()`?**  
**A:** To maintain O(1) space overhead. Storing size would require extra memory in every node or a separate counter.

---

## 🎯 Comparison Summary

| Feature | array | vector | deque | list | forward_list |
|---------|-------|--------|-------|------|--------------|
| **Size** | Fixed | Dynamic | Dynamic | Dynamic | Dynamic |
| **Memory** | Stack | Heap | Heap | Heap | Heap |
| **Random Access** | ✅ O(1) | ✅ O(1) | ✅ O(1) | ❌ O(n) | ❌ O(n) |
| **Contiguous** | ✅ | ✅ | ❌ | ❌ | ❌ |
| **Insert Front** | ❌ | O(n) | ✅ O(1) | ✅ O(1) | ✅ O(1) |
| **Insert Back** | ❌ | ✅ O(1)* | ✅ O(1) | ✅ O(1) | ❌ O(n) |
| **Insert Middle** | ❌ | O(n) | O(n) | ✅ O(1)† | ✅ O(1)† |
| **Iterator Stability** | ✅ | ❌ | Medium | ✅ | ✅ |
| **Memory Overhead** | None | Low | Medium | High | Medium |
| **Cache Performance** | Excellent | Excellent | Good | Poor | Poor |

\* Amortized  
† If you have iterator

---

## 📚 Next Steps

- [Associative Containers](associative_containers.md)
- [Unordered Containers](unordered_containers.md)
- [Container Examples](../../Code/containers/)

---

**Remember:** `vector` is the default choice. Use others only when you have a specific reason!
