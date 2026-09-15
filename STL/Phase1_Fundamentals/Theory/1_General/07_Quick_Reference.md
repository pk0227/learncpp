# ⚡ STL Quick Reference - Cheat Sheet

> **Last-Minute Review for Senior C++ Interviews**

---

## 📑 Table of Contents

1. [📦 Container Complexity Table](#-container-complexity-table)
2. [🔁 Iterator Categories Matrix](#-iterator-categories-matrix)
3. [⚠️ Iterator Invalidation Rules](#️-iterator-invalidation-rules)
4. [🎯 Container Selection Flowchart](#-container-selection-flowchart)
5. [🧮 Common Algorithm Complexities](#-common-algorithm-complexities)
6. [💡 Essential Idioms](#-essential-idioms)
7. [🔥 Common Interview Questions](#-common-interview-questions)
8. [📊 Memory Overhead Comparison](#-memory-overhead-comparison)
9. [🎯 Container Selection Decision Matrix](#-container-selection-decision-matrix)
10. [🧰 Useful Utility Types](#-useful-utility-types)
11. [🔧 Custom Comparators](#-custom-comparators)
12. [⚡ Performance Tips](#-performance-tips)
13. [🎓 One-Liners for Interviews](#-one-liners-for-interviews)
14. [📁 Code Examples](#-code-examples)
15. [📚 See Also](#-see-also)

---

## 📦 Container Complexity Table

### Sequence Containers

| Container | Access | Insert (End) | Insert (Front) | Insert (Middle) | Find | Erase | Memory Overhead |
|---|---|---|---|---|---|---|---|
| `std::array` | $O(1)$ | N/A | N/A | N/A | $O(N)$ | N/A | None (0 bytes overhead) |
| `std::vector` | $O(1)$ | Amortized $O(1)^*$ | $O(N)$ | $O(N)$ | $O(N)$ | $O(N)$ | Low (3 pointers: 24 bytes) |
| `std::deque` | $O(1)$ | Amortized $O(1)$ | Amortized $O(1)$ | $O(N)$ | $O(N)$ | $O(N)$ | Medium (map of fixed chunks) |
| `std::list` | $O(N)$ | $O(1)$ | $O(1)$ | $O(1)^\dagger$ | $O(N)$ | $O(1)^\dagger$ | High (2 pointers per node: 16 bytes) |
| `std::forward_list` | $O(N)$ | $O(1)^\ddagger$ | $O(1)$ | $O(1)^\dagger$ | $O(N)$ | $O(1)^\dagger$ | Low/Medium (1 pointer per node: 8 bytes) |

$^*$ Amortized due to capacity doubling when full.  
$^\dagger$ Constant time $O(1)$ only when iterator to the insertion/deletion position is already held.  
$^\ddagger$ Requires maintaining an iterator/pointer to the last node.

### Associative & Unordered Containers

| Container | Insert | Find / Count | Erase | Sorted Iteration | Underlying Structure |
|---|---|---|---|---|---|
| `std::set` / `std::map` | $O(\log N)$ | $O(\log N)$ | $O(\log N)$ | $O(N)$ (in-order traversal) | Red-Black Tree (balanced BST) |
| `std::multiset` / `std::multimap` | $O(\log N)$ | $O(\log N)$ | $O(\log N)$ | $O(N)$ | Red-Black Tree |
| `std::unordered_set` / `std::unordered_map` | Avg $O(1)$, Worst $O(N)$ | Avg $O(1)$, Worst $O(N)$ | Avg $O(1)$, Worst $O(N)$ | $O(N \log N)$ (requires sort) | Hash Table (separate chaining) |
| `std::unordered_multiset` / `std::unordered_multimap` | Avg $O(1)$, Worst $O(N)$ | Avg $O(1)$, Worst $O(N)$ | Avg $O(1)$, Worst $O(N)$ | $O(N \log N)$ (requires sort) | Hash Table (separate chaining) |
| `std::flat_map` / `std::flat_set` (C++23) | $O(N)$ | $O(\log N)$ | $O(N)$ | $O(N)$ (contiguous scan) | Sorted contiguous vectors |

---

## 🔁 Iterator Categories Matrix

```
Input Iterator ──┐
                 ├──► Forward Iterator ──► Bidirectional Iterator ──► Random Access Iterator ──► Contiguous Iterator (C++20)
Output Iterator ─┘
```

| Container | Iterator Category | Invalidation Risk |
|---|---|---|
| `std::array` | Random Access, Contiguous | Never (fixed size) |
| `std::vector` | Random Access, Contiguous | High (reallocation invalidates all; insertion/erasure invalidates downstream) |
| `std::deque` | Random Access | Medium (insertion at ends invalidates all iterators; middle operations invalidate all) |
| `std::list` | Bidirectional | Low (only erased node invalidated; iterators never move) |
| `std::forward_list` | Forward | Low (only erased node invalidated) |
| `std::set` / `std::map` | Bidirectional | Low (only erased node invalidated) |
| `std::unordered_*` | Forward | Medium (rehash invalidates all iterators; element pointers/references remain stable) |

### Iterator Capabilities Comparison

| Capability | Input | Output | Forward | Bidirectional | Random Access | Contiguous (C++20) |
|---|---|---|---|---|---|---|
| Read (`*it`) | ✅ | ❌ | ✅ | ✅ | ✅ | ✅ |
| Write (`*it = val`) | ❌ | ✅ | ✅ | ✅ | ✅ | ✅ |
| Multi-pass traversal | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ |
| Pre/Post-increment (`++it`) | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| Pre/Post-decrement (`--it`) | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ |
| Random offset (`it + n`, `it - n`, `it[n]`) | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ |
| Relational comparisons (`<`, `>`, `<=`, `>=`) | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ |
| Contiguous memory math (`*(it + n) == *(&*it + n)`) | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ |

---

## ⚠️ Iterator Invalidation Rules

| Container | Operation | Iterator Validity | Reference / Pointer Validity |
|---|---|---|---|
| **`std::vector`** | `push_back` / `insert` with realloc (`size == capacity`) | ❌ **All invalidated** | ❌ **All invalidated** |
| | `insert` without realloc | ❌ Invalidation from insertion point to end | ❌ Invalidation from insertion point to end |
| | `erase` | ❌ Invalidation from erase point to end | ❌ Invalidation from erase point to end |
| **`std::deque`** | `push_front` / `push_back` / `insert` at ends | ❌ **All iterators invalidated** | ✅ **All references remain valid** |
| | `insert` in middle | ❌ **All iterators invalidated** | ❌ **All references invalidated** |
| | `pop_front` / `pop_back` at ends | ❌ Only erased element & `end()` iterator | ❌ Only erased element |
| | `erase` in middle | ❌ **All iterators invalidated** | ❌ **All references invalidated** |
| **`std::list`** | `insert` / `push_*` anywhere | ✅ **All valid** | ✅ **All valid** |
| | `erase` / `pop_*` | ❌ Only erased element invalidated | ❌ Only erased element invalidated |
| **`std::set` / `std::map`** | `insert` / `emplace` | ✅ **All valid** | ✅ **All valid** |
| | `erase` | ❌ Only erased element invalidated | ❌ Only erased element invalidated |
| **`std::unordered_*`** | `insert` / `rehash` triggering rehash | ❌ **All iterators invalidated** | ✅ **All references/pointers remain valid** |
| | `insert` without rehash | ✅ **All valid** | ✅ **All valid** |
| | `erase` | ❌ Only erased element invalidated | ❌ Only erased element invalidated |

> [!WARNING]
> **Classic Interview Pitfall: Deque iterator invalidation vs reference validity!**
> Inserting at either end of a `std::deque` invalidates **all iterators**, but **references and pointers to elements remain completely valid**. Conversely, inserting in the middle invalidates both.

---

## 🎯 Container Selection Flowchart

```
Need key-value? ──YES──► Ordered? ──YES──► Duplicates? ──YES──► std::multimap
                │                   │                    └─NO───► std::map
                │                   └─NO───► Duplicates? ──YES──► std::unordered_multimap
                │                                        └─NO───► std::unordered_map
                │
                └─NO───► Unique elements? ──YES──► Ordered? ──YES──► std::set
                         │                                  └─NO───► std::unordered_set
                         │
                         └─NO───► Access pattern?
                                  ├─ Random access ──► Fixed size? ──YES──► std::array
                                  │                                └─NO───► std::vector
                                  ├─ Front/back push & pop ───────────────► std::deque
                                  ├─ Frequent middle insert/delete ───────► std::list
                                  ├─ LIFO (Stack) ────────────────────────► std::stack
                                  ├─ FIFO (Queue) ────────────────────────► std::queue
                                  └─ Priority / Top element ──────────────► std::priority_queue
```

---

## 🧮 Common Algorithm Complexities

| Algorithm | Header | Time Complexity | Required Iterator Category | Notes |
|---|---|---|---|---|
| `std::find`, `std::count` | `<algorithm>` | $O(N)$ | Input Iterator | Linear scan |
| `std::sort` | `<algorithm>` | $O(N \log N)$ worst-case | Random Access | Introsort (Quicksort + Heapsort + Insertion Sort) |
| `std::stable_sort` | `<algorithm>` | $O(N \log N)$ | Random Access | Merge Sort (preserves equivalent order) |
| `std::partial_sort` | `<algorithm>` | $O(N \log K)$ | Random Access | Heapsort on top $K$ elements |
| `std::nth_element` | `<algorithm>` | $O(N)$ average | Random Access | Quickselect (partitions $n$-th element) |
| `std::lower_bound` | `<algorithm>` | $O(\log N)$ random, $O(N)$ forward | Forward Iterator | First element $\ge$ value |
| `std::upper_bound` | `<algorithm>` | $O(\log N)$ random, $O(N)$ forward | Forward Iterator | First element $>$ value |
| `std::binary_search` | `<algorithm>` | $O(\log N)$ random, $O(N)$ forward | Forward Iterator | Returns `bool` existence |
| `std::reverse` | `<algorithm>` | $O(N)$ | Bidirectional | In-place swap |
| `std::accumulate` | `<numeric>` | $O(N)$ | Input Iterator | Sequential left fold |
| `std::reduce` | `<numeric>` | $O(N)$ | Forward Iterator (C++17) | Out-of-order parallel associative reduction |

---

## 💡 Essential Idioms

### 1. Erase-Remove Idiom (Pre-C++20 vs Modern C++20)

```cpp
#include <vector>
#include <algorithm>

std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};

// Pre-C++20: Two-step erase-remove idiom
v.erase(std::remove(v.begin(), v.end(), 2), v.end());

// Pre-C++20 with predicate:
v.erase(std::remove_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }), v.end());

// Modern C++20: Uniform container erasure (clean and bug-free)
std::erase(v, 2);
std::erase_if(v, [](int x) { return x % 2 == 0; });
```

### 2. Safe Erase While Iterating (Single-Pass)

```cpp
// Works uniformly on std::vector, std::deque, std::list, std::set, std::map
for (auto it = container.begin(); it != container.end(); ) {
    if (should_erase(*it)) {
        it = container.erase(it);  // erase returns next valid iterator!
    } else {
        ++it;
    }
}
```

### 3. Vector Deallocation (Swap Trick vs shrink_to_fit)

```cpp
std::vector<int> v(1000000);
v.clear(); // size becomes 0, but capacity remains 1,000,000!

// C++98/03: Swap with empty temporary to forcefully free heap memory
std::vector<int>().swap(v); // capacity is now 0

// C++11+: Non-binding request to shrink capacity to size
v.shrink_to_fit();
```

### 4. Emplace vs Insert/Push

```cpp
struct Widget {
    Widget(int id, std::string name);
};

std::vector<Widget> w;
// push_back: constructs temporary Widget, moves/copies into vector, destructs temporary
w.push_back(Widget(1, "Alpha"));

// emplace_back: forwards constructor arguments directly into uninitialized storage in-place
w.emplace_back(1, "Alpha");
```

---

## 🔥 Common Interview Questions

### Q: `std::vector` vs `std::list`?
**A:** `std::vector` is the default choice because contiguous memory provides superior hardware cache line utilization and hardware prefetching. Even with $O(N)$ shifts, `vector` often beats `list` for middle insertions up to tens of thousands of elements. Choose `std::list` only when **iterator stability** across insertions/deletions is strictly required, or when large nodes must be spliced ($O(1)$ node relinking) between lists without copying.

### Q: `std::map` vs `std::unordered_map`?
**A:** `std::map` provides ordered traversal, range queries (`lower_bound`, `upper_bound`), and guaranteed $O(\log N)$ worst-case time using Red-Black Trees. `std::unordered_map` provides average $O(1)$ lookup using a hash table, but has worse worst-case complexity ($O(N)$ under hash collisions or rehash) and requires defining a custom hash function for user-defined keys.

### Q: Why can't `std::sort` work on `std::list`?
**A:** `std::sort` requires random-access iterators for Introsort partitioning. `std::list` only provides bidirectional iterators. Use `list::sort()` which executes an in-place $O(N \log N)$ merge sort.

### Q: What is Node Extraction (C++17 Node Handles)?
**A:** C++17 introduced `.extract()`, allowing a node to be extracted from an associative or unordered container without copying or allocating memory. The node handle can then be modified and inserted into another compatible container (`.merge()` or `.insert(std::move(node))`).

---

## 📊 Memory Overhead Comparison

| Container | Typical Overhead per Element | Memory Layout |
|---|---|---|
| `std::array<T, N>` | 0 bytes | Pure contiguous array on stack/data segment |
| `std::vector<T>` | ~0 bytes (plus unused capacity buffer) | 24 bytes stack overhead (3 pointers: begin, end, end-of-storage) |
| `std::deque<T>` | ~8–16 bytes per chunk pointer | Map of pointers pointing to fixed-size array chunks (typically 512 bytes) |
| `std::list<T>` | 16 bytes (64-bit: 2 pointers `prev` and `next`) | Dispersed individual heap node allocations |
| `std::forward_list<T>` | 8 bytes (64-bit: 1 pointer `next`) | Dispersed individual heap node allocations |
| `std::set<T>` / `std::map<K, V>` | 24–32 bytes (3 pointers: `parent`, `left`, `right` + color bit + alignment padding) | Dispersed Red-Black Tree heap nodes |
| `std::unordered_set<T>` / `map<K, V>` | 8–16 bytes (bucket array pointer + node pointer + cached hash) | Dispersed heap nodes chained from bucket vector |

---

## 🎯 Container Selection Decision Matrix

| Concrete Requirement | Optimal Container Choice | Rationale |
|---|---|---|
| Compile-time fixed size, maximum cache locality | `std::array` | Zero heap overhead, stack allocated, contiguous memory |
| General dynamic sequence, random access | `std::vector` | Amortized $O(1)$ push_back, minimum memory overhead, cache-friendly |
| Frequent push/pop at both front and back | `std::deque` | Amortized $O(1)$ at both ends without reallocating existing elements |
| Stable iterators with frequent middle insert/delete | `std::list` | $O(1)$ node insertion/removal with existing iterator; no element shifting |
| Sorted unique keys with range queries | `std::set` | In-order traversal, logarithmic operations via Red-Black Tree |
| Fast unique key lookup without ordering | `std::unordered_set` | Average $O(1)$ lookup via hash bucketing |
| Sorted key-value association | `std::map` | Logarithmic lookup, structured range searching |
| Maximum speed key-value lookup | `std::unordered_map` | Average $O(1)$ hash table lookup |
| LIFO discipline | `std::stack` | Restricted container adaptor (defaults to `deque`) |
| FIFO discipline | `std::queue` | Restricted container adaptor (defaults to `deque`) |
| Min/Max extraction priority | `std::priority_queue` | Binary heap adaptor over `vector` |

---

## 🧰 Useful Utility Types

### `std::string_view` (C++17) & `std::span` (C++20) - Non-owning Views
```cpp
#include <string_view>
#include <span>
#include <vector>
#include <iostream>

// Zero-copy string inspection (no heap allocation)
void print_string(std::string_view sv) {
    std::cout << sv << "\n";
}

// Zero-copy contiguous sequence view over array, vector, or raw pointer
void print_span(std::span<const int> s) {
    for (int x : s) std::cout << x << " ";
}
```

### `std::pair` & `std::tuple` (Structured Bindings C++17)
```cpp
#include <tuple>
#include <string>

std::tuple<int, std::string, double> record{42, "Alice", 99.5};
auto [id, name, score] = record;  // Structured binding unpacks in-place
```

### `std::optional` (C++17)
```cpp
#include <optional>

std::optional<int> try_parse(std::string_view s) {
    if (s.empty()) return std::nullopt;
    return std::stoi(std::string(s));
}
```

### `std::variant` & `std::visit` (C++17)
```cpp
#include <variant>
#include <iostream>

std::variant<int, std::string> data = "Hello";
std::visit([](const auto& val) { std::cout << val << "\n"; }, data);
```

---

## 🔧 Custom Comparators

### For Sorting Algorithms
```cpp
#include <vector>
#include <algorithm>

// 1. Lambda
std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });

// 2. Standard functor
std::sort(v.begin(), v.end(), std::greater<int>{});

// 3. Custom functor struct
struct Person { std::string name; int age; };
struct AgeCmp {
    bool operator()(const Person& a, const Person& b) const {
        return a.age < b.age; // Strict weak ordering: must use <, never <=
    }
};
std::sort(people.begin(), people.end(), AgeCmp{});
```

### For Ordered Containers & Heaps
```cpp
#include <set>
#include <queue>

// set with descending order
std::set<int, std::greater<int>> desc_set;

// priority_queue default is max-heap; min-heap requires underlying container + comparator:
std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
```

---

## ⚡ Performance Tips

1. **Always `reserve()` for `std::vector`** when estimated element count is known to prevent redundant heap reallocations.
2. **Prefer `emplace_back` over `push_back`** to construct objects in-place and bypass temporary copy/move operations.
3. **Use `std::string_view` (C++17) and `std::span` (C++20)** as read-only function parameters instead of `const std::string&` and `const std::vector<T>&`.
4. **Use `std::erase` / `std::erase_if` (C++20)** instead of the verbose pre-C++20 erase-remove idiom.
5. **Prefer pre-increment (`++it`) over post-increment (`it++`)** to eliminate redundant iterator copies.
6. **Prefer `std::vector` over `std::list`** even when insertions occur in the middle, unless element size is large or iterator stability is mandatory.
7. **Reserve buckets for `std::unordered_map`** via `.reserve(N)` to avoid expensive rehashing.
8. **Never use `<=` in comparators** — strict weak ordering requires `comp(x, x) == false`. Using `<=` causes undefined behavior, buffer overflows, and segmentation faults in `std::sort`.

---

## 🎓 One-Liners for Interviews

- **STL:** A generic C++ framework consisting of containers, iterators, algorithms, and functors designed with templates for zero-overhead abstraction.
- **Iterators:** Generalized pointer-like abstractions that decouple containers from algorithms, achieving $O(N + M)$ library design.
- **Erase-Remove Idiom:** Moves unwanted elements to the back and shortens the container via `erase` (superseded in C++20 by `std::erase`).
- **Iterator Invalidation:** Occurs when container modifications cause existing iterators to refer to relocated, repurposed, or deleted memory.
- **Vector vs Deque:** Vector guarantees single contiguous memory with amortized $O(1)$ push_back; Deque uses segmented chunks allowing $O(1)$ push/pop at both front and back without full reallocation.
- **Map vs Unordered Map:** Map uses Red-Black Trees guaranteeing $O(\log N)$ sorted order; Unordered Map uses hash bucketing providing average $O(1)$ search.

---

## 📁 Code Examples

- [`Phase1_Fundamentals/Code/containers/vector_examples.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase1_Fundamentals/Code/containers/vector_examples.cpp): Vector capacity growth, shrink_to_fit, and memory management.
- [`Phase1_Fundamentals/Code/containers/deque_examples.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase1_Fundamentals/Code/containers/deque_examples.cpp): Deque operations, chunk indexing, and front/back operations.
- [`Phase1_Fundamentals/Code/containers/list_examples.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase1_Fundamentals/Code/containers/list_examples.cpp): Doubly linked list splicing and node pointer operations.
- [`Phase1_Fundamentals/Code/containers/set_map_examples.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase1_Fundamentals/Code/containers/set_map_examples.cpp): Ordered map and set operations, lower_bound/upper_bound.
- [`Phase1_Fundamentals/Code/containers/unordered_examples.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase1_Fundamentals/Code/containers/unordered_examples.cpp): Hash table bucketing, load factors, and custom hash functions.
- [`Phase1_Fundamentals/Code/algorithms/algorithm_examples.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase1_Fundamentals/Code/algorithms/algorithm_examples.cpp): Sorting, partition, binary search, and numeric algorithms.
- [`Phase1_Fundamentals/Code/algorithms/erase_remove_idiom.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase1_Fundamentals/Code/algorithms/erase_remove_idiom.cpp): Safe element removal in sequence containers.

---

## 📚 See Also

- [STL Overview](01_STL_Overview.md)
- [Container Selection Guide](../../../Phase2_Selection_Application/Theory/04_Container_Selection_Guide.md)
- [Iterator Invalidation](../../../Phase2_Selection_Application/Theory/05_Iterator_Invalidation.md)
- [Interview Problems](../../../Phase2_Selection_Application/Theory/06_Interview_Problems.md)
