# ⚡ STL Quick Reference - Cheat Sheet

> **Last-Minute Review for Interviews**

---

## 📦 Container Complexity Table

### Sequence Containers

| Container | Access | Insert (end) | Insert (front) | Insert (middle) | Find | Erase | Memory |
|-----------|--------|--------------|----------------|-----------------|------|-------|--------|
| `array` | O(1) | N/A | N/A | N/A | O(n) | N/A | Minimal |
| `vector` | O(1) | O(1)* | O(n) | O(n) | O(n) | O(n) | Low |
| `deque` | O(1) | O(1) | O(1) | O(n) | O(n) | O(n) | Medium |
| `list` | O(n) | O(1) | O(1) | O(1)† | O(n) | O(1)† | High |
| `forward_list` | O(n) | O(1)‡ | O(1) | O(1)† | O(n) | O(1)† | Medium |

\* Amortized  
† If you have iterator  
‡ No direct end access

### Associative Containers

| Container | Insert | Find | Erase | Iterate (sorted) | Memory |
|-----------|--------|------|-------|------------------|--------|
| `set/map` | O(log n) | O(log n) | O(log n) | O(n) | High |
| `multiset/multimap` | O(log n) | O(log n) | O(log n) | O(n) | High |
| `unordered_set/map` | O(1)** | O(1)** | O(1)** | O(n log n) | High |
| `unordered_multiset/multimap` | O(1)** | O(1)** | O(1)** | O(n log n) | High |

** Average case; worst case O(n)

---

## 🔁 Iterator Categories Matrix

| Container | Iterator Category | Invalidation Risk |
|-----------|------------------|-------------------|
| `array` | Random Access, Contiguous | Never |
| `vector` | Random Access, Contiguous | High (realloc) |
| `deque` | Random Access | Medium |
| `list` | Bidirectional | Low |
| `forward_list` | Forward | Low |
| `set/map` | Bidirectional | Low |
| `unordered_*` | Forward | Medium (rehash) |

### Iterator Capabilities

```
Input ──┐
        ├──► Forward ──► Bidirectional ──► Random Access ──► Contiguous
Output ─┘
```

| Category | Read | Write | Multi-pass | ++/-- | +/-/[] | Contiguous |
|----------|------|-------|------------|-------|--------|------------|
| Input | ✅ | ❌ | ❌ | ++ | ❌ | ❌ |
| Output | ❌ | ✅ | ❌ | ++ | ❌ | ❌ |
| Forward | ✅ | ✅ | ✅ | ++ | ❌ | ❌ |
| Bidirectional | ✅ | ✅ | ✅ | ++/-- | ❌ | ❌ |
| Random Access | ✅ | ✅ | ✅ | ++/-- | ✅ | ❌ |
| Contiguous | ✅ | ✅ | ✅ | ++/-- | ✅ | ✅ |

---

## ⚠️ Iterator Invalidation Rules

| Container | Insert | Erase | Notes |
|-----------|--------|-------|-------|
| **vector** | All (if realloc), else from insert point → end | From erase point → end | Check `capacity()` |
| **deque** | All (middle), none (ends) | All (middle), only erased (ends) | Safe only at ends |
| **list** | None | Only erased | Most stable |
| **forward_list** | None | Only erased | Most stable |
| **set/map** | None | Only erased | Stable |
| **unordered_*** | All (if rehash) | Only erased | Check `load_factor()` |

---

## 🎯 Container Selection Flowchart

```
Need key-value? ──YES──► Ordered? ──YES──► Duplicates? ──YES──► multimap
                │                   │                    └─NO───► map
                │                   └─NO───► Duplicates? ──YES──► unordered_multimap
                │                                        └─NO───► unordered_map
                │
                └─NO───► Unique elements? ──YES──► Ordered? ──YES──► set
                         │                                  └─NO───► unordered_set
                         │
                         └─NO───► Access pattern?
                                  ├─ Random access ──► Fixed size? ──YES──► array
                                  │                                └─NO───► vector
                                  ├─ Front/back ──────────────────────────► deque
                                  ├─ Middle insert/delete ────────────────► list
                                  ├─ LIFO ────────────────────────────────► stack
                                  ├─ FIFO ────────────────────────────────► queue
                                  └─ Priority ────────────────────────────► priority_queue
```

---

## 🧮 Common Algorithm Complexities

| Algorithm | Complexity | Iterator Requirement |
|-----------|-----------|---------------------|
| `find` | O(n) | Input |
| `count` | O(n) | Input |
| `sort` | O(n log n) | Random Access |
| `stable_sort` | O(n log n) | Random Access |
| `binary_search` | O(log n) | Forward (but O(n) without random access) |
| `lower_bound` | O(log n) | Forward (but O(n) without random access) |
| `upper_bound` | O(log n) | Forward (but O(n) without random access) |
| `copy` | O(n) | Input (source), Output (dest) |
| `transform` | O(n) | Input (source), Output (dest) |
| `remove` | O(n) | Forward |
| `reverse` | O(n) | Bidirectional |
| `accumulate` | O(n) | Input |

---

## 💡 Essential Idioms

### 1. Erase-Remove Idiom

```cpp
// Remove all elements equal to value
v.erase(std::remove(v.begin(), v.end(), value), v.end());

// Remove all elements satisfying predicate
v.erase(std::remove_if(v.begin(), v.end(), 
    [](int x) { return x % 2 == 0; }), v.end());
```

### 2. Safe Erase While Iterating

```cpp
// For vector, deque, list, set, map
for (auto it = container.begin(); it != container.end(); ) {
    if (should_erase(*it)) {
        it = container.erase(it);  // Returns next iterator
    } else {
        ++it;
    }
}
```

### 3. Swap Idiom (Shrink to Fit)

```cpp
// Guaranteed to free memory
std::vector<int>().swap(v);

// Or modern C++
v.shrink_to_fit();  // Request (not guaranteed)
```

### 4. Reserve to Prevent Reallocation

```cpp
std::vector<int> v;
v.reserve(1000);  // Pre-allocate capacity

// Now push_back won't reallocate until 1000 elements
```

### 5. Emplace vs Insert/Push

```cpp
// Construct in-place (more efficient)
v.emplace_back(arg1, arg2);  // Calls constructor directly

// Constructs temporary, then moves
v.push_back(MyClass(arg1, arg2));
```

---

## 🔥 Common Interview Questions

### Q: `vector` vs `list`?
**A:** `vector` for random access and cache efficiency. `list` only if frequent middle insert/delete on large containers.

### Q: `map` vs `unordered_map`?
**A:** `map` for ordering or range queries. `unordered_map` for pure lookup speed.

### Q: Why can't `std::sort` work on `list`?
**A:** `std::sort` needs random-access iterators. `list` has bidirectional. Use `list::sort()` instead (merge sort).

### Q: What's the erase-remove idiom?
**A:** `v.erase(std::remove(...), v.end())` - `remove` moves elements to end, `erase` actually removes them.

### Q: When does `vector` reallocate?
**A:** When `size() == capacity()` and you insert. Typically grows by 1.5× or 2×.

### Q: How to avoid iterator invalidation?
**A:** 
- Use indices instead of iterators when modifying
- Use erase return value
- Reserve capacity for `vector`/`unordered_map`
- Use `list`/`set`/`map` for stable iterators

---

## 📊 Memory Overhead Comparison

| Container | Overhead per Element | Notes |
|-----------|---------------------|-------|
| `array` | 0 bytes | Fixed size, stack allocated |
| `vector` | ~0 bytes | Contiguous, may have unused capacity |
| `deque` | ~8 bytes | Pointers to chunks |
| `list` | ~16 bytes | 2 pointers (prev/next) |
| `forward_list` | ~8 bytes | 1 pointer (next) |
| `set/map` | ~24-32 bytes | Red-Black Tree node (3 pointers + color) |
| `unordered_set/map` | ~8-16 bytes | Bucket pointer + hash |

---

## 🎯 Container Selection Decision Matrix

| Requirement | Container |
|-------------|-----------|
| Fixed size, compile-time | `array` |
| Dynamic array, random access | `vector` |
| Fast front/back insert | `deque` |
| Frequent middle insert/delete | `list` |
| Unique, sorted | `set` |
| Unique, fast lookup | `unordered_set` |
| Key-value, sorted | `map` |
| Key-value, fast lookup | `unordered_map` |
| LIFO | `stack` |
| FIFO | `queue` |
| Priority-based | `priority_queue` |

---

## 🧰 Useful Utility Types

### `std::pair`
```cpp
std::pair<int, std::string> p{1, "hello"};
auto [key, value] = p;  // Structured binding (C++17)
```

### `std::tuple`
```cpp
std::tuple<int, std::string, double> t{1, "hello", 3.14};
auto [a, b, c] = t;  // Structured binding
```

### `std::optional` (C++17)
```cpp
std::optional<int> find(const std::vector<int>& v, int target) {
    auto it = std::find(v.begin(), v.end(), target);
    if (it != v.end()) return *it;
    return std::nullopt;
}
```

### `std::variant` (C++17)
```cpp
std::variant<int, std::string> v = 42;
v = "hello";
std::visit([](auto&& arg) { std::cout << arg; }, v);
```

---

## 🔧 Custom Comparators

### For Sorting
```cpp
// Lambda
std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });

// Function object
struct Greater {
    bool operator()(int a, int b) const { return a > b; }
};
std::sort(v.begin(), v.end(), Greater{});

// Function pointer
bool greater(int a, int b) { return a > b; }
std::sort(v.begin(), v.end(), greater);
```

### For Containers
```cpp
// set with custom comparator
std::set<int, std::greater<int>> s;  // Descending order

// map with custom comparator
auto cmp = [](int a, int b) { return a > b; };
std::map<int, std::string, decltype(cmp)> m(cmp);

// priority_queue (max-heap by default)
std::priority_queue<int> max_heap;

// priority_queue (min-heap)
std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
```

---

## ⚡ Performance Tips

1. **Use `reserve()` for `vector`** when you know approximate size
2. **Use `emplace` over `insert`/`push`** for in-place construction
3. **Use `const_iterator`** when not modifying
4. **Use `++it` over `it++`** (avoid temporary copy)
5. **Prefer `vector` over `list`** (cache-friendly)
6. **Use `unordered_map` for large datasets** (O(1) vs O(log n))
7. **Reserve capacity for `unordered_map`** to avoid rehashing
8. **Use `std::move` for large objects** to avoid copies

---

## 🎓 One-Liners for Interviews

**STL:** Generic C++ library with containers, iterators, algorithms, and utilities using templates for zero-overhead abstraction.

**Iterators:** Generalized pointers that decouple containers from algorithms through a uniform traversal interface.

**Erase-Remove Idiom:** `remove` moves unwanted elements to end, `erase` actually removes them.

**Iterator Invalidation:** Iterators become invalid after container modifications (varies by container).

**`vector` vs `list`:** `vector` for random access and cache efficiency; `list` for stable iterators and frequent middle operations.

**`map` vs `unordered_map`:** `map` for ordering/range queries (O(log n)); `unordered_map` for speed (O(1) average).

---

## 📚 See Also

- [STL Overview](01_STL_Overview.md)
- [Container Selection Guide](../../../Phase2_Selection_Application/Theory/04_Container_Selection_Guide.md)
- [Iterator Invalidation](../../../Phase2_Selection_Application/Theory/05_Iterator_Invalidation.md)
- [Interview Problems](../../../Phase2_Selection_Application/Theory/06_Interview_Problems.md)

---

**Print this for quick review before interviews! 🚀**
