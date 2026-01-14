# 🔁 Iterators Deep Dive - The Glue Layer

> **Mastering Iterators for Senior C++ Interviews**

---

## 🎯 What Are Iterators?

**Iterators are generalized pointers** that provide a uniform interface for traversing different container types.

### The Core Concept

```cpp
// Pointer to array
int arr[] = {1, 2, 3, 4, 5};
int* p = arr;
*p;      // Dereference: access element
++p;     // Advance: move to next
p[2];    // Random access: jump to position

// Iterator to vector (same interface!)
std::vector<int> v = {1, 2, 3, 4, 5};
auto it = v.begin();
*it;     // Dereference: access element
++it;    // Advance: move to next
it[2];   // Random access: jump to position (if random-access iterator)
```

**Key Insight:** Iterators abstract away the underlying data structure, allowing algorithms to work uniformly.

---

## 📊 Iterator Categories - The Hierarchy

```
                    Input Iterator ──┐
                                     │
                                     ├──► Forward Iterator
                                     │         │
                   Output Iterator ──┘         │
                                               ▼
                                    Bidirectional Iterator
                                               │
                                               ▼
                                    Random Access Iterator
                                               │
                                               ▼
                                    Contiguous Iterator (C++20)
```

### Category Capabilities Matrix

| Category | Read | Write | Multi-pass | Forward | Backward | Jump | Contiguous Memory |
|----------|------|-------|------------|---------|----------|------|-------------------|
| **Input** | ✅ | ❌ | ❌ | ✅ | ❌ | ❌ | ❌ |
| **Output** | ❌ | ✅ | ❌ | ✅ | ❌ | ❌ | ❌ |
| **Forward** | ✅ | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ |
| **Bidirectional** | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ | ❌ |
| **Random Access** | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ |
| **Contiguous** | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |

---

## 1️⃣ Input Iterator

**Purpose:** Read elements **once** in a forward direction.

### Operations
```cpp
*it;        // Read element
++it;       // Advance (pre-increment)
it++;       // Advance (post-increment)
it1 == it2; // Compare
it1 != it2; // Compare
```

### Example: Reading from Input Stream
```cpp
#include <iostream>
#include <iterator>
#include <algorithm>

std::istream_iterator<int> input(std::cin);  // Read from stdin
std::istream_iterator<int> eof;              // End-of-stream

// Read until EOF
while (input != eof) {
    std::cout << *input << " ";
    ++input;
}

// Or use algorithm
std::for_each(std::istream_iterator<int>(std::cin),
              std::istream_iterator<int>(),
              [](int n) { std::cout << n << " "; });
```

### Limitations
- **Single-pass only** - Can't go back or re-read
- **Read-only** - Can't modify elements
- **No random access** - Can't jump to arbitrary position

### Containers: None (used with streams)

---

## 2️⃣ Output Iterator

**Purpose:** Write elements **once** in a forward direction.

### Operations
```cpp
*it = value;  // Write element
++it;         // Advance
it++;         // Advance
```

### Example: Writing to Output Stream
```cpp
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

std::vector<int> v = {1, 2, 3, 4, 5};

// Copy to stdout
std::copy(v.begin(), v.end(),
          std::ostream_iterator<int>(std::cout, " "));
// Output: 1 2 3 4 5

// Insert into another container
std::vector<int> dest;
std::copy(v.begin(), v.end(),
          std::back_inserter(dest));  // back_inserter is an output iterator
```

### Special Output Iterators

#### `std::back_inserter` - Inserts at end
```cpp
std::vector<int> v;
auto it = std::back_inserter(v);
*it = 10;  // v.push_back(10)
*it = 20;  // v.push_back(20)
```

#### `std::front_inserter` - Inserts at front
```cpp
std::deque<int> d;
auto it = std::front_inserter(d);
*it = 10;  // d.push_front(10)
*it = 20;  // d.push_front(20)
```

#### `std::inserter` - Inserts at specific position
```cpp
std::vector<int> v = {1, 2, 5};
auto it = std::inserter(v, v.begin() + 2);
*it = 3;  // Insert 3 at position 2
*it = 4;  // Insert 4 at position 3
// v is now {1, 2, 3, 4, 5}
```

### Containers: None (used with streams and inserters)

---

## 3️⃣ Forward Iterator

**Purpose:** Read/write elements in a **forward** direction, **multiple passes** allowed.

### Operations
```cpp
*it;        // Read element
*it = val;  // Write element (if not const)
++it;       // Advance
it++;       // Advance
it1 == it2; // Compare
it1 != it2; // Compare
```

### Key Difference from Input/Output
- **Multi-pass** - Can iterate multiple times
- **Copyable** - Can save iterator and use later

### Example
```cpp
std::forward_list<int> fl = {1, 2, 3, 4, 5};

auto it1 = fl.begin();
auto it2 = it1;  // Copy iterator

++it1;
// it1 points to 2
// it2 still points to 1 (multi-pass!)

// Can iterate multiple times
for (auto it = fl.begin(); it != fl.end(); ++it) { /* pass 1 */ }
for (auto it = fl.begin(); it != fl.end(); ++it) { /* pass 2 */ }
```

### Containers
- `std::forward_list`
- `std::unordered_set`, `std::unordered_map`
- `std::unordered_multiset`, `std::unordered_multimap`

---

## 4️⃣ Bidirectional Iterator

**Purpose:** Read/write elements in **both directions**.

### Operations
All forward iterator operations, plus:
```cpp
--it;  // Move backward (pre-decrement)
it--;  // Move backward (post-decrement)
```

### Example
```cpp
std::list<int> l = {1, 2, 3, 4, 5};

auto it = l.begin();
++it;  // Points to 2
++it;  // Points to 3
--it;  // Points to 2 (can go backward!)

// Reverse iteration
for (auto it = l.end(); it != l.begin(); ) {
    --it;  // Move backward first
    std::cout << *it << " ";
}
// Output: 5 4 3 2 1
```

### Why `list::sort()` Exists
```cpp
std::list<int> l = {3, 1, 4, 1, 5};

// ❌ Won't compile - std::sort needs random-access iterators
// std::sort(l.begin(), l.end());

// ✅ Use list's member function (uses merge sort)
l.sort();  // Works with bidirectional iterators
```

### Containers
- `std::list`
- `std::set`, `std::multiset`
- `std::map`, `std::multimap`

---

## 5️⃣ Random Access Iterator

**Purpose:** Jump to any position in **O(1)** time.

### Operations
All bidirectional iterator operations, plus:
```cpp
it + n;     // Jump forward n positions
it - n;     // Jump backward n positions
it += n;    // Advance n positions
it -= n;    // Move back n positions
it[n];      // Access nth element from current position
it1 - it2;  // Distance between iterators
it1 < it2;  // Comparison
it1 > it2;
it1 <= it2;
it1 >= it2;
```

### Example
```cpp
std::vector<int> v = {10, 20, 30, 40, 50};

auto it = v.begin();
it += 2;        // Jump to index 2 (value 30)
std::cout << *it << "\n";  // 30

std::cout << it[1] << "\n";  // Access relative position (40)

auto it2 = v.begin() + 4;
std::cout << (it2 - it) << "\n";  // Distance: 2

// Binary search requires random access
auto pos = std::lower_bound(v.begin(), v.end(), 35);
```

### Why `std::sort()` Requires Random Access

```cpp
// Introsort (std::sort) needs to:
// 1. Pick pivot at middle: O(1) with random access
auto mid = first + (last - first) / 2;

// 2. Partition around pivot: needs bidirectional movement
// 3. Recursively sort sub-ranges: needs arithmetic

// With bidirectional iterator, finding middle is O(n)!
auto mid = first;
std::advance(mid, std::distance(first, last) / 2);  // O(n)
```

### Containers
- `std::vector`
- `std::deque`
- `std::array`
- C-style arrays

---

## 6️⃣ Contiguous Iterator (C++20)

**Purpose:** Random access iterator where elements are **contiguous in memory**.

### Additional Guarantee
- Elements are stored in **consecutive memory addresses**
- `&*(it + n) == &*it + n` (pointer arithmetic works)

### Why This Matters
```cpp
std::vector<int> v = {1, 2, 3, 4, 5};

// Can safely pass to C API
int* ptr = &v[0];  // or v.data()
legacy_c_function(ptr, v.size());

// Enables optimizations
// Compiler can use SIMD, memcpy, etc.
```

### Containers
- `std::vector`
- `std::array`
- `std::string`
- C-style arrays

### Not Contiguous
- `std::deque` - Segmented storage (still random access, but not contiguous!)

---

## 🧠 Iterator Traits

**Iterator traits** allow algorithms to query iterator properties at compile time.

### The Traits Class

```cpp
template<typename Iterator>
struct iterator_traits {
    using iterator_category = /* ... */;
    using value_type = /* ... */;
    using difference_type = /* ... */;
    using pointer = /* ... */;
    using reference = /* ... */;
};
```

### Example: Algorithm Using Traits

```cpp
// Efficient distance calculation based on iterator category
template<typename Iterator>
typename iterator_traits<Iterator>::difference_type
distance_impl(Iterator first, Iterator last, std::random_access_iterator_tag) {
    return last - first;  // O(1) for random access
}

template<typename Iterator>
typename iterator_traits<Iterator>::difference_type
distance_impl(Iterator first, Iterator last, std::input_iterator_tag) {
    typename iterator_traits<Iterator>::difference_type n = 0;
    while (first != last) {
        ++first;
        ++n;
    }
    return n;  // O(n) for input/forward/bidirectional
}

template<typename Iterator>
typename iterator_traits<Iterator>::difference_type
distance(Iterator first, Iterator last) {
    return distance_impl(first, last,
        typename iterator_traits<Iterator>::iterator_category{});
}
```

**Key Insight:** Algorithms can **optimize based on iterator capabilities** at compile time!

---

## 🔄 Special Iterators

### 1. `reverse_iterator`

Reverses the direction of iteration.

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};

// Forward iteration
for (auto it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << " ";  // 1 2 3 4 5
}

// Reverse iteration
for (auto it = v.rbegin(); it != v.rend(); ++it) {
    std::cout << *it << " ";  // 5 4 3 2 1
}
```

**How it works:**
```cpp
// reverse_iterator wraps a regular iterator
// ++reverse_it actually does --underlying_it
```

### 2. `const_iterator`

Prevents modification of elements.

```cpp
std::vector<int> v = {1, 2, 3};

std::vector<int>::iterator it = v.begin();
*it = 10;  // ✅ OK

std::vector<int>::const_iterator cit = v.cbegin();
*cit = 10;  // ❌ Error: can't modify through const_iterator
```

**Best Practice:** Use `cbegin()`/`cend()` when you don't need to modify.

```cpp
// Modern C++: auto deduces const_iterator
const std::vector<int> v = {1, 2, 3};
for (auto it = v.begin(); it != v.end(); ++it) {
    // it is const_iterator (v is const)
}
```

### 3. `move_iterator`

Dereferences to rvalue reference (enables moving instead of copying).

```cpp
std::vector<std::string> src = {"hello", "world"};
std::vector<std::string> dest;

// Copy
std::copy(src.begin(), src.end(), std::back_inserter(dest));
// src still has {"hello", "world"}

// Move
std::copy(std::make_move_iterator(src.begin()),
          std::make_move_iterator(src.end()),
          std::back_inserter(dest));
// src now has {"", ""} (moved-from state)
```

---

## 🎯 Iterator Requirements for Algorithms

### Common Algorithms and Their Iterator Requirements

| Algorithm | Minimum Iterator | Why |
|-----------|-----------------|-----|
| `std::find` | Input | Only needs forward traversal |
| `std::copy` | Input (source), Output (dest) | Read source, write dest |
| `std::reverse` | Bidirectional | Needs to move backward |
| `std::sort` | Random Access | Needs O(1) jumps for quicksort |
| `std::binary_search` | Forward | Needs multi-pass, but can work with forward |
| `std::lower_bound` | Forward | Can work with forward, but O(n) instead of O(log n) |

### Why `std::sort` Needs Random Access

```cpp
// Introsort (used by std::sort) needs:
// 1. Partition: swap elements far apart - needs random access
// 2. Pick median-of-three pivot - needs arithmetic (first + (last-first)/2)
// 3. Recursion on sub-ranges - needs iterator arithmetic

// With only bidirectional iterators, this would be O(n²) or worse!
```

### Why `list` Has Its Own `sort()`

```cpp
std::list<int> l = {3, 1, 4, 1, 5};

// ❌ Can't use std::sort (needs random access)
// std::sort(l.begin(), l.end());

// ✅ list::sort() uses merge sort (works with bidirectional)
l.sort();  // O(n log n) with bidirectional iterators
```

---

## ⚠️ Iterator Invalidation - Preview

Different operations invalidate iterators in different ways. This is covered in detail in [Iterator Invalidation & Pitfalls](file:///home/prashanth/learncpp_workspace/STL/05_Iterator_Invalidation.md).

### Quick Reference

| Container | Invalidates on Insert | Invalidates on Erase |
|-----------|----------------------|---------------------|
| `vector` | All (if reallocation) | From erase point to end |
| `deque` | All | All (unless at ends) |
| `list` | None | Only erased element |
| `set/map` | None | Only erased element |
| `unordered_*` | All (if rehash) | Only erased element |

---

## 🔥 Common Interview Questions

### Q1: What's the difference between `iterator` and `const_iterator`?

**A:**
- `iterator` - Can modify elements: `*it = value`
- `const_iterator` - Read-only: `*it` is const

```cpp
std::vector<int> v = {1, 2, 3};
auto it = v.begin();        // iterator
auto cit = v.cbegin();      // const_iterator

*it = 10;   // ✅ OK
*cit = 10;  // ❌ Error
```

---

### Q2: Why can't you use `std::sort()` on a `std::list`?

**A:** `std::sort()` requires **random-access iterators** for O(n log n) performance (introsort needs to jump to arbitrary positions). `std::list` only provides **bidirectional iterators** (linked list can't jump). That's why `list` has its own `list::sort()` using merge sort, which works with bidirectional iterators.

---

### Q3: What's the difference between `++it` and `it++` for iterators?

**A:**
- `++it` (pre-increment) - Increments and returns reference to incremented iterator
- `it++` (post-increment) - Returns copy of old iterator, then increments

```cpp
// ++it is more efficient (no copy)
Iterator& operator++() {
    // advance
    return *this;
}

// it++ creates temporary copy
Iterator operator++(int) {
    Iterator tmp = *this;  // Copy!
    ++(*this);
    return tmp;
}
```

**Best practice:** Use `++it` unless you need the old value.

---

### Q4: Can you convert a `const_iterator` to an `iterator`?

**A:** Not directly (for safety). But you can use `std::next()` with `begin()`:

```cpp
std::vector<int> v = {1, 2, 3};
auto cit = v.cbegin() + 1;

// ❌ Can't convert directly
// auto it = static_cast<std::vector<int>::iterator>(cit);

// ✅ Compute offset and recreate
auto it = v.begin() + std::distance(v.cbegin(), cit);
```

---

### Q5: What's the complexity of `std::distance()`?

**A:** **Depends on iterator category:**
- **Random-access iterators:** O(1) - uses subtraction: `last - first`
- **Other iterators:** O(n) - counts increments from `first` to `last`

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
auto dist = std::distance(v.begin(), v.end());  // O(1)

std::list<int> l = {1, 2, 3, 4, 5};
auto dist = std::distance(l.begin(), l.end());  // O(n)
```

---

## 🎓 Key Takeaways

1. **Iterators are generalized pointers** with different capability levels
2. **Five main categories:** Input, Output, Forward, Bidirectional, Random Access (+ Contiguous in C++20)
3. **Algorithms require specific iterator categories** for efficiency
4. **Iterator traits** enable compile-time optimization
5. **Special iterators:** `reverse_iterator`, `const_iterator`, `move_iterator`
6. **`std::sort` needs random access** - that's why `list::sort()` exists
7. **Use `++it` over `it++`** for efficiency
8. **`const_iterator`** for read-only access

---

## 📚 Next Steps

1. [**Container Selection Guide**](file:///home/prashanth/learncpp_workspace/STL/04_Container_Selection_Guide.md) - Choose containers based on iterator needs
2. [**Iterator Invalidation**](file:///home/prashanth/learncpp_workspace/STL/05_Iterator_Invalidation.md) - Avoid common pitfalls
3. [**Algorithms**](file:///home/prashanth/learncpp_workspace/STL/algorithms/algorithms_overview.md) - See iterators in action

---

**Remember:** Iterators are the **glue** that makes STL's separation of containers and algorithms possible!
