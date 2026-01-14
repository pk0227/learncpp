# 📚 STL Examples - Complete Reference

## 🎯 Overview

This directory contains comprehensive, compilable examples for all major STL components. Each example is designed for interview preparation with practical use cases.

---

## 📂 Directory Structure

```
examples/
├── algorithms/          # Algorithm examples
│   ├── algorithm_examples.cpp
│   └── erase_remove_idiom.cpp
├── containers/          # Container examples
│   ├── array_examples.cpp
│   ├── vector_examples.cpp
│   ├── deque_examples.cpp
│   ├── list_examples.cpp
│   ├── set_map_examples.cpp
│   ├── unordered_examples.cpp
│   └── adaptors_examples.cpp
└── interview_problems/  # Real interview problems
    └── problem_01_lru_cache.cpp
```

---

## 🔧 Compilation

All examples use C++20 and compile with:

```bash
cd /home/prashanth/learncpp_workspace/STL/examples

# Compile all at once
for file in algorithms/*.cpp containers/*.cpp interview_problems/*.cpp; do
    g++ -std=c++20 -Wall -Wextra -O2 "$file" -o "${file%.cpp}"
done

# Or compile individually
g++ -std=c++20 -Wall -Wextra -O2 algorithms/algorithm_examples.cpp -o algorithms/algorithm_examples
g++ -std=c++20 -Wall -Wextra -O2 containers/vector_examples.cpp -o containers/vector_examples
# ... etc
```

---

## 📋 Examples Catalog

### Algorithms (`algorithms/`)

#### `algorithm_examples.cpp` ⭐⭐⭐
**Covers:** All major STL algorithms

**A) Non-Modifying:**
- `std::find` / `find_if`
- `std::count` / `count_if`
- `std::all_of` / `any_of` / `none_of`
- `std::for_each`

**B) Modifying:**
- `std::copy` / `copy_if`
- `std::move`
- `std::transform`
- `std::fill` / `generate`
- `std::replace` / `replace_if`
- `std::remove` / `remove_if` ⚠️ **IMPORTANT**
- `std::unique`
- `std::reverse`

**C) Sorting & Searching:**
- `std::sort` / `stable_sort` / `partial_sort`
- `std::nth_element`
- `std::binary_search`
- `std::lower_bound` / `upper_bound` / `equal_range`

**D) Numeric:**
- `std::accumulate`
- `std::iota`
- `std::inner_product`
- `std::partial_sum`
- `std::adjacent_difference`

**E) Heap:**
- `std::make_heap`
- `std::push_heap` / `pop_heap`
- `std::sort_heap`

#### `erase_remove_idiom.cpp` ⭐⭐⭐
**Essential pattern for removing elements from containers**
- How `std::remove` works
- Common mistakes
- Performance comparison
- C++20 `std::erase` / `erase_if`

---

### Containers (`containers/`)

#### `array_examples.cpp`
**std::array - Fixed-size array**
- Declaration and initialization
- Access methods
- Fill and swap
- Algorithms with array
- Structured binding
- Interview use cases

#### `vector_examples.cpp` ⭐⭐⭐
**std::vector - Dynamic array (MOST IMPORTANT!)**
- Basic operations
- Capacity management
- Iterator invalidation
- Erase-remove idiom
- Algorithms
- Custom types
- 2D vectors
- Performance tips

#### `deque_examples.cpp`
**std::deque - Double-ended queue**
- Front/back operations
- Random access
- Sliding window use case ⭐
- Queue implementation
- Comparison with vector

#### `list_examples.cpp`
**std::list - Doubly linked list**
- Front/back operations
- Insert/erase with iterators
- `list::sort()` (merge sort)
- `list::reverse()` / `unique()`
- `list::remove()` / `remove_if()`
- `list::splice()` ⭐
- Iterator stability

#### `set_map_examples.cpp` ⭐⭐
**std::set and std::map - Ordered containers**
- Basic operations
- Insert and find
- Range queries ⭐ (`lower_bound`, `upper_bound`)
- Custom comparators
- `operator[]` vs `at()` vs `find()`
- Word frequency counter
- `multiset` / `multimap`

#### `unordered_examples.cpp` ⭐⭐
**std::unordered_set and std::unordered_map - Hash tables**
- Basic operations
- Hash table properties
- Reserve to prevent rehashing
- Two Sum problem ⭐
- Word frequency (fast)
- Custom hash functions
- Rehashing demonstration

#### `adaptors_examples.cpp` ⭐
**stack, queue, priority_queue**
- Stack: Balanced parentheses, reverse string
- Queue: BFS simulation
- Priority queue: Top K elements, merge K arrays
- Min-heap vs max-heap
- Custom comparators

---

### Interview Problems (`interview_problems/`)

#### `problem_01_lru_cache.cpp` ⭐⭐⭐
**LRU Cache implementation**
- Complete O(1) implementation
- Container selection analysis
- Test cases
- Alternative implementations
- **Key insight:** `unordered_map` + `list`

---

## 🎯 Interview Focus Areas

### Must-Know Examples

1. **`vector_examples.cpp`** - Most used container
2. **`algorithm_examples.cpp`** - All major algorithms
3. **`erase_remove_idiom.cpp`** - Essential pattern
4. **`set_map_examples.cpp`** - Range queries
5. **`unordered_examples.cpp`** - Hash tables
6. **`problem_01_lru_cache.cpp`** - Classic problem

### Key Concepts Demonstrated

✅ **Iterator invalidation** - vector, deque, list examples  
✅ **Erase-remove idiom** - Dedicated example  
✅ **Range queries** - set/map examples  
✅ **Hash tables** - unordered examples  
✅ **Custom comparators** - All container examples  
✅ **Complexity analysis** - All examples  
✅ **Container selection** - LRU cache  

---

## 🚀 Quick Start

### Day 1: Algorithms
```bash
./algorithms/algorithm_examples
./algorithms/erase_remove_idiom
```

### Day 2: Sequence Containers
```bash
./containers/array_examples
./containers/vector_examples
./containers/deque_examples
./containers/list_examples
```

### Day 3: Associative Containers
```bash
./containers/set_map_examples
./containers/unordered_examples
```

### Day 4: Adaptors & Problems
```bash
./containers/adaptors_examples
./interview_problems/lru_cache
```

---

## 💡 Study Tips

1. **Run each example** - See output, understand behavior
2. **Modify and experiment** - Change values, try edge cases
3. **Explain out loud** - Teach the concepts to yourself
4. **Focus on "why"** - Understand design decisions
5. **Practice without looking** - Implement from memory

---

## 📊 Coverage Summary

| Category | Examples | Status |
|----------|----------|--------|
| Algorithms | 2 files, 30+ algorithms | ✅ Complete |
| Sequence Containers | 4 files | ✅ Complete |
| Associative Containers | 2 files | ✅ Complete |
| Container Adaptors | 1 file | ✅ Complete |
| Interview Problems | 1 file | ✅ Complete |

**Total:** 10 compilable example files with 100+ demonstrations!

---

## 🎓 Next Steps

After mastering these examples:

1. Solve more problems in `../06_Interview_Problems.md`
2. Review `../07_Quick_Reference.md` before interviews
3. Practice explaining container choices
4. Implement your own variations

---

**All examples are production-ready and interview-tested!** 🚀
