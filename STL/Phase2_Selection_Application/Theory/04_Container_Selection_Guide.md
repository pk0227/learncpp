# 🎯 Container Selection Guide - Decision-Making Framework

> **The Most Important Topic for Senior C++ Interviews**

---

## 📑 Table of Contents

1. [🚨 Why This Matters](#-why-this-matters)
2. [🗺️ Container Selection Flowchart](#️-container-selection-flowchart)
3. [📊 Complete Comparison Matrix](#-complete-comparison-matrix)
4. [🥊 Head-to-Head Comparisons](#-head-to-head-comparisons)
   - [1. vector vs list](#1-vector-vs-list)
   - [2. map vs unordered_map](#2-map-vs-unordered_map)
   - [3. deque vs vector](#3-deque-vs-vector)
   - [4. set vs priority_queue](#4-set-vs-priority_queue)
   - [5. array vs vector](#5-array-vs-vector)
   - [6. std::map vs std::flat_map (C++23)](#6-stdmap-vs-stdflat_map-c23)
5. [🎯 Decision-Making Framework](#-decision-making-framework)
6. [🔥 Real Interview System Design Problems](#-real-interview-system-design-problems)
   - [Q1: LRU Cache (unordered_map + list)](#q1-design-a-cache-with-o1-lookup-insert-and-eviction-lru-cache)
   - [Q2: Median in a Data Stream (Two Heaps)](#q2-find-median-in-a-stream-of-integers)
   - [Q3: Range Sum Queries (Prefix Sums vs Trees)](#q3-implement-a-data-structure-for-range-sum-queries)
7. [🎓 Key Takeaways](#-key-takeaways)
8. [📁 Code Examples](#-code-examples)
9. [📚 Next Steps](#-next-steps)

---

## 🚨 Why This Matters

> **90% of senior C++ interviews include container selection questions.**

Interviewers care MORE about your **decision-making process** than your ability to memorize syntax. They want to see you:
- Understand hardware cache lines and memory layout trade-offs
- Justify container choices with Big-O and empirical benchmark reality
- Consider iterator and reference invalidation safety
- Factor in modern C++ features (`std::span`, PMR, flat containers)

---

## 🗺️ Container Selection Flowchart

```
START: What do you need to store?
│
├─► Need key-value pairs?
│   │
│   ├─► YES → Need ordering?
│   │   │
│   │   ├─► YES → Need duplicates?
│   │   │   ├─► YES → std::multimap
│   │   │   └─► NO  → std::map (or std::flat_map in C++23 if lookups dominate)
│   │   │
│   │   └─► NO → Need duplicates?
│   │       ├─► YES → std::unordered_multimap
│   │       └─► NO  → std::unordered_map
│   │
│   └─► NO → Need unique elements?
│       │
│       ├─► YES → Need ordering?
│       │   │
│       │   ├─► YES → std::set (or std::flat_set in C++23)
│       │   └─► NO  → std::unordered_set
│       │
│       └─► NO → What access pattern?
│           │
│           ├─► Random access → Size known at compile time?
│           │   ├─► YES (fixed) → std::array
│           │   └─► NO (dynamic) → std::vector
│           │
│           ├─► Front/back push & pop only → std::deque
│           │
│           ├─► Frequent middle insert/delete with iterator stability → std::list
│           │
│           ├─► LIFO (stack) → std::stack
│           │
│           ├─► FIFO (queue) → std::queue
│           │
│           └─► Priority-based (extremum access) → std::priority_queue
```

---

## 📊 Complete Comparison Matrix

### Performance Characteristics

| Container | Access | Insert (End) | Insert (Front) | Insert (Middle) | Find | Delete | Memory Overhead |
|---|---|---|---|---|---|---|---|
| **`std::array`** | $O(1)$ | N/A | N/A | N/A | $O(N)$ | N/A | 0 bytes |
| **`std::vector`** | $O(1)$ | Amortized $O(1)^*$ | $O(N)$ | $O(N)$ | $O(N)$ | $O(N)$ | Low (3 pointers) |
| **`std::deque`** | $O(1)$ | Amortized $O(1)$ | Amortized $O(1)$ | $O(N)$ | $O(N)$ | $O(N)$ | Medium (map of chunks) |
| **`std::list`** | $O(N)$ | $O(1)$ | $O(1)$ | $O(1)^\dagger$ | $O(N)$ | $O(1)^\dagger$ | High (16 bytes/node) |
| **`std::forward_list`** | $O(N)$ | $O(1)^\ddagger$ | $O(1)$ | $O(1)^\dagger$ | $O(N)$ | $O(1)^\dagger$ | Medium (8 bytes/node) |
| **`std::set` / `std::map`** | N/A | $O(\log N)$ | $O(\log N)$ | $O(\log N)$ | $O(\log N)$ | $O(\log N)$ | High (24–32 bytes/node) |
| **`std::unordered_set/map`** | N/A | Avg $O(1)$, Worst $O(N)$ | Avg $O(1)$, Worst $O(N)$ | Avg $O(1)$, Worst $O(N)$ | Avg $O(1)$, Worst $O(N)$ | Avg $O(1)$, Worst $O(N)$ | High (buckets + nodes) |
| **`std::flat_map` (C++23)** | $O(\log N)$ | $O(N)$ | $O(N)$ | $O(N)$ | $O(\log N)$ | $O(N)$ | Very Low (contiguous vectors) |

$^*$ Amortized due to geometric capacity doubling.  
$^\dagger$ Constant time $O(1)$ only when iterator to the insertion position is already held.  
$^\ddagger$ Requires keeping an iterator/pointer to the last node.

### Iterator Categories & Stability

| Container | Iterator Category | Invalidation Risk |
|---|---|---|
| **`std::array`** | Random Access, Contiguous | Never (fixed compile-time size) |
| **`std::vector`** | Random Access, Contiguous | High (reallocation invalidates all; insertion/erase invalidates downstream) |
| **`std::deque`** | Random Access | Medium (insertion at ends invalidates all iterators; middle ops invalidate all) |
| **`std::list`** | Bidirectional | Low (only erased node invalidated; iterators never relocate) |
| **`std::forward_list`** | Forward | Low (only erased node invalidated) |
| **`std::set` / `std::map`** | Bidirectional | Low (only erased node invalidated) |
| **`std::unordered_set/map`** | Forward | Medium (rehash invalidates all iterators; element references remain valid) |

---

## 🥊 Head-to-Head Comparisons

### 1. `vector` vs `list`

**The Classic Interview Question!**

#### Use `vector` when:
- ✅ Random access is needed (`v[i]`)
- ✅ Cache-friendly traversal is critical (modern CPUs fetch 64-byte cache lines)
- ✅ Elements are predominantly appended to the end
- ✅ Insertions/deletions in the middle are infrequent or container size is small ($<10,000$)
- ✅ Memory overhead must be minimized

#### Use `list` when:
- ✅ Frequent insertions/deletions occur in the middle AND you already hold an iterator
- ✅ Strict **iterator stability** is required (iterators must never invalidate on insertion)
- ✅ Constant-time $O(1)$ **splicing** between lists is needed
- ✅ Elements are very expensive to move/copy and cannot be stored via pointers

#### Example Scenario

```cpp
#include <vector>
#include <list>
#include <algorithm>

struct Task { int id; int priority; };

// ❌ ANTI-PATTERN: std::list for simple sorted iteration
// LinkedList causes pointer chasing and cache misses on every node access!

// ✅ PREFERRED: std::vector + std::sort + std::lower_bound
std::vector<Task> tasks;
tasks.push_back({101, 2});
std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
    return a.priority < b.priority;
});
// Binary search in contiguous memory is blisteringly fast
```

#### Benchmark Reality Check
```
Insert 1,000 small elements in middle:
- std::vector: ~50 μs (O(N) memory shift, but prefetcher keeps memory in L1/L2 cache)
- std::list:   ~200 μs (O(1) pointer relink, but every node allocation incurs malloc lock & cache miss)

Lesson: Big-O notation ignores constant factors and hardware cache hierarchies!
```

> [!TIP]
> **Interview Answer Template:**  
> *"I choose `std::vector` as my default container because contiguous memory maximizes CPU cache line utilization and hardware prefetching. I only switch to `std::list` if I have frequent middle insertions/deletions on large objects where iterator stability is mandatory or where I must splice ranges in $O(1)$ without copying."*

---

### 2. `map` vs `unordered_map`

**Another Top Interview Question!**

#### Use `std::map` when:
- ✅ Sorted in-order iteration is required
- ✅ Range queries are needed (`std::lower_bound`, `std::upper_bound`)
- ✅ Strict $O(\log N)$ worst-case performance guarantee is mandatory (no hash collision spikes)
- ✅ Key type does not have a readily available or efficient hash function

#### Use `std::unordered_map` when:
- ✅ Only point lookups (`find`), insertions, and deletions are needed (no ordering)
- ✅ Large datasets where average $O(1)$ beats $O(\log N)$
- ✅ An efficient, well-distributed hash function is available

#### Example Scenarios

```cpp
#include <map>
#include <unordered_map>
#include <string>
#include <iostream>

// Scenario 1: Word frequency counter (no ordering needed)
// ✅ GOOD: unordered_map for O(1) average update
std::unordered_map<std::string, int> word_count;
word_count["apple"]++;

// Scenario 2: Range query - find all accounts with balance between $1,000 and $5,000
// ✅ GOOD: map (ordered tree allows logarithmic lower_bound and upper_bound)
std::map<int, std::string> account_balances;
auto it_start = account_balances.lower_bound(1000);
auto it_end   = account_balances.upper_bound(5000);
for (auto it = it_start; it != it_end; ++it) {
    // Range query processed in O(log N + K) where K is number of matched accounts
}
```

#### Performance Comparison

| Operation | `std::map` | `std::unordered_map` |
|---|---|---|
| Average Lookup | $O(\log N)$ | Average $O(1)$, Worst $O(N)$ |
| Worst-case Lookup | Guaranteed $O(\log N)$ | $O(N)$ (hash collision attack) |
| Memory Overhead | 3 pointers + color bit per node | Bucket vector + node pointers |
| Iteration Order | Strictly sorted by key | Unspecified bucket order |
| Range Queries | $O(\log N + K)$ | $O(N)$ (must inspect every element) |

---

### 3. `deque` vs `vector`

#### Use `std::vector` when:
- ✅ Elements are only appended at the end
- ✅ Memory must be contiguous (e.g., passing `vec.data()` to C APIs or GPU buffers)
- ✅ Maximum cache locality is needed

#### Use `std::deque` when:
- ✅ You require fast $O(1)$ insertion and removal at **both front and back**
- ✅ You are allocating millions of elements and want to **avoid massive contiguous reallocation failures** (deque allocates fixed chunks)
- ✅ You require pointer/reference validity across front/back insertions

#### Example Scenario: Sliding Window

```cpp
#include <deque>
#include <vector>

// ✅ GOOD: deque for sliding window (efficient pop_front)
std::deque<int> window;
window.push_back(42);
if (window.size() > 5) {
    window.pop_front();  // O(1) operation!
}

// ❌ BAD: vector for sliding window
// window.erase(window.begin()) is O(N) because it shifts every remaining element!
```

---

### 4. `set` vs `priority_queue`

#### Use `std::set` when:
- ✅ You need to **search**, **iterate**, and **remove arbitrary elements**
- ✅ Elements must be strictly unique
- ✅ Range queries (`lower_bound`) are needed

#### Use `std::priority_queue` when:
- ✅ You only need access to the **extremum** (maximum or minimum element via `.top()`)
- ✅ You do not need to iterate through all elements
- ✅ You want minimum memory overhead (flat heap stored inside a contiguous `std::vector`)

```cpp
#include <queue>
#include <set>

// Dijkstra's shortest path: only needs the smallest tentative distance
// ✅ GOOD: priority_queue (cache friendly, low memory)
std::priority_queue<std::pair<int, int>, 
                    std::vector<std::pair<int, int>>, 
                    std::greater<>> pq;
```

---

### 5. `array` vs `vector`

#### Use `std::array` when:
- ✅ Element count is fixed and known at **compile time**
- ✅ You want pure **stack allocation** (zero heap fragmentation)
- ✅ Zero runtime overhead (no capacity or dynamic pointer tracking)

#### Use `std::vector` when:
- ✅ Element count is determined at **runtime**
- ✅ The collection must dynamically grow or shrink

---

### 6. `std::map` vs `std::flat_map` (C++23)

In C++23, `std::flat_map` stores keys and values in two parallel contiguous `std::vector` containers kept in sorted order.

| Metric | `std::map` (Node-based) | `std::flat_map` (Contiguous) |
|---|---|---|
| Storage Layout | Heap nodes scattered across memory | Two contiguous vectors (`vector<Key>`, `vector<Value>`) |
| Lookup (`find`) | $O(\log N)$ pointer dereferencing | $O(\log N)$ binary search in contiguous cache line |
| Insertion | $O(\log N)$ node allocation | $O(N)$ shifting contiguous vector elements |
| Memory Overhead | 24–32 bytes per element | Minimal vector capacity overhead |
| Ideal Use Case | Dynamic frequent insertions and lookups | Build-once, lookup-heavy read workloads |

---

## 🎯 Decision-Making Framework

### Step 1: What Is Your Access Pattern?

| Access Pattern | Primary Recommendation |
|---|---|
| Random access by integer index | `std::vector`, `std::array`, `std::deque` |
| Key-based associative lookup | `std::unordered_map` (speed) or `std::map` (ordering) |
| Front and back push/pop | `std::deque` |
| Extremum (min/max) only | `std::priority_queue` |
| Sequential iteration only | `std::vector` |

### Step 2: What Are Your Constraints?

| Primary Constraint | Container Recommendation |
|---|---|
| **Low Latency / Cache Critical** | `std::vector`, `std::array`, `std::flat_map` |
| **Iterator Stability Required** | `std::list`, `std::set`, `std::map` |
| **Zero Heap Allocations** | `std::array` |
| **Guaranteed Worst-Case Time** | `std::map`, `std::set` (avoid `unordered_*`) |
| **Non-owning Function Parameter** | `std::span` (C++20) or `std::string_view` (C++17) |

---

## 🔥 Real Interview System Design Problems

### Q1: Design a Cache with O(1) Lookup, Insert, and Eviction (LRU Cache)

**Container Selection:** `std::unordered_map` + `std::list`
- `std::list` maintains access order from MRU (front) to LRU (back).
- `std::unordered_map` maps `Key -> list::iterator`.
- **Why this combination?** `std::list::splice` moves nodes in $O(1)$ time without invalidating existing iterators stored in the map.

```cpp
#include <list>
#include <unordered_map>
#include <utility>

class LRUCache {
    int capacity;
    std::list<std::pair<int, int>> items; // {key, value}
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> cache;

public:
    LRUCache(int cap) : capacity(cap) {}

    int get(int key) {
        auto it = cache.find(key);
        if (it == cache.end()) return -1;

        // Splice moves node to front in O(1) without reallocating
        items.splice(items.begin(), items, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = cache.find(key);
        if (it != cache.end()) {
            it->second->second = value;
            items.splice(items.begin(), items, it->second);
            return;
        }

        if (cache.size() == capacity) {
            auto lru = items.back();
            cache.erase(lru.first);
            items.pop_back();
        }

        items.push_front({key, value});
        cache[key] = items.begin();
    }
};
```

---

### Q2: Find Median in a Stream of Integers

**Container Selection:** Two Heaps (`std::priority_queue`)
- Max-heap stores smaller half of stream.
- Min-heap stores larger half of stream.
- **Why two heaps?** Provides $O(\log N)$ insertion and instantaneous $O(1)$ median retrieval.

```cpp
#include <queue>
#include <vector>

class MedianFinder {
    std::priority_queue<int> max_heap; // Lower half
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap; // Upper half

public:
    void addNum(int num) {
        max_heap.push(num);
        min_heap.push(max_heap.top());
        max_heap.pop();

        if (max_heap.size() < min_heap.size()) {
            max_heap.push(min_heap.top());
            min_heap.pop();
        }
    }

    double findMedian() const {
        if (max_heap.size() > min_heap.size()) {
            return max_heap.top();
        }
        return (max_heap.top() + min_heap.top()) / 2.0;
    }
};
```

---

### Q3: Implement a Data Structure for Range Sum Queries

**Container Selection:** Prefix Sums inside `std::vector` (if read-heavy), or Binary Indexed Tree (Fenwick) / Segment Tree (if updates are frequent).

```cpp
#include <vector>

class NumArray {
    std::vector<int> prefix;
public:
    NumArray(const std::vector<int>& nums) : prefix(nums.size() + 1, 0) {
        for (size_t i = 0; i < nums.size(); ++i) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
    }

    int sumRange(int left, int right) const {
        return prefix[right + 1] - prefix[left]; // O(1) query!
    }
};
```

---

## 🎓 Key Takeaways

1. **Default to `std::vector`** for dynamic collections; hardware cache lines favor contiguous memory.
2. **`std::map` vs `std::unordered_map`** is a trade-off between strict ordering/predictability ($O(\log N)$) and raw point lookup speed (average $O(1)$).
3. **`std::list` is rarely the right choice** unless node splicing or unconditional iterator stability across modifications is strictly needed.
4. **Compose containers** (`unordered_map` + `list`) to combine point lookup with stable ordering.
5. **Modern C++23 Flat Containers** (`std::flat_map`) provide vector cache performance with map lookup semantics for read-heavy workloads.

---

## 📁 Code Examples

- [`Phase2_Selection_Application/Code/interview_problems/problem_01_lru_cache.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_01_lru_cache.cpp): Complete LRU Cache implementation with `unordered_map` + `list`.
- [`Phase2_Selection_Application/Code/interview_problems/problem_04_top_k_frequent.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_04_top_k_frequent.cpp): Top-K frequent elements using min-heap `priority_queue`.
- [`Phase2_Selection_Application/Code/interview_problems/problem_05_group_anagrams.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_05_group_anagrams.cpp): Anagram grouping using hash map bucketing.
- [`Phase2_Selection_Application/Code/interview_problems/problem_07_design_twitter.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase2_Selection_Application/Code/interview_problems/problem_07_design_twitter.cpp): Social graph design composing vectors, maps, and priority queues.

---

## 📚 Next Steps

1. [**Iterator Invalidation**](05_Iterator_Invalidation.md) - Understand invalidation rules and pitfalls
2. [**Interview Problems**](06_Interview_Problems.md) - Practice container selection scenarios
3. [**Sequence Containers Guide**](../../Phase1_Fundamentals/Theory/2_Containers/sequence_containers.md) - Deep dive into vector, deque, and list
4. [**Associative Containers Guide**](../../Phase1_Fundamentals/Theory/2_Containers/associative_containers.md) - Deep dive into map and set
