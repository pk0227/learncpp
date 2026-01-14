# STL Algorithms Overview

This directory contains comprehensive documentation on STL algorithms for interview preparation.

## 📚 Algorithm Categories

### 1. Non-Modifying Algorithms
Algorithms that don't modify the container.

**File:** See `../examples/algorithms/algorithm_examples.cpp`

- `std::find` / `find_if` - Find element
- `std::count` / `count_if` - Count occurrences
- `std::all_of` / `any_of` / `none_of` - Check conditions
- `std::for_each` - Apply function to each element
- `std::search` - Find subsequence
- `std::adjacent_find` - Find adjacent duplicates

### 2. Modifying Algorithms
Algorithms that modify the container.

**File:** See `../examples/algorithms/algorithm_examples.cpp`

- `std::copy` / `copy_if` - Copy elements
- `std::move` - Move elements
- `std::transform` - Apply function and store result
- `std::fill` / `generate` - Fill with values
- `std::replace` / `replace_if` - Replace values
- `std::remove` / `remove_if` - Remove elements (use with erase!)
- `std::unique` - Remove consecutive duplicates
- `std::reverse` - Reverse order
- `std::rotate` - Rotate elements

### 3. Sorting & Searching
Algorithms for sorting and binary search.

**File:** See `../examples/algorithms/algorithm_examples.cpp`

- `std::sort` - Quick sort (O(n log n))
- `std::stable_sort` - Stable sort (preserves order)
- `std::partial_sort` - Sort first N elements
- `std::nth_element` - Partition around nth element
- `std::binary_search` - Check if element exists
- `std::lower_bound` - First element >= value
- `std::upper_bound` - First element > value
- `std::equal_range` - Range of equal elements

### 4. Numeric Algorithms
Mathematical operations.

**File:** See `../examples/algorithms/algorithm_examples.cpp`

- `std::accumulate` - Sum or fold
- `std::iota` - Fill with incrementing values
- `std::inner_product` - Dot product
- `std::partial_sum` - Running sum
- `std::adjacent_difference` - Differences

### 5. Heap Algorithms
Binary heap operations.

**File:** See `../examples/algorithms/algorithm_examples.cpp`

- `std::make_heap` - Create heap
- `std::push_heap` - Add to heap
- `std::pop_heap` - Remove from heap
- `std::sort_heap` - Sort heap (destroys heap property)

## 🎯 Interview Focus

### Most Important Algorithms (Must Know)

1. **`std::sort`** - Default sorting
2. **`std::binary_search`** / **`lower_bound`** / **`upper_bound`** - Binary search
3. **`std::find`** / **`find_if`** - Linear search
4. **`std::count`** / **`count_if`** - Counting
5. **`std::transform`** - Map operation
6. **`std::accumulate`** - Reduce operation
7. **`std::remove`** / **`remove_if`** - With erase-remove idiom

### Common Interview Patterns

1. **Erase-Remove Idiom** ⭐⭐⭐
   ```cpp
   v.erase(std::remove_if(v.begin(), v.end(), predicate), v.end());
   ```

2. **Binary Search on Sorted** ⭐⭐⭐
   ```cpp
   auto it = std::lower_bound(v.begin(), v.end(), target);
   ```

3. **Transform-Accumulate** ⭐⭐
   ```cpp
   std::transform(v.begin(), v.end(), result.begin(), func);
   int sum = std::accumulate(result.begin(), result.end(), 0);
   ```

4. **Partition** ⭐⭐
   ```cpp
   auto mid = std::partition(v.begin(), v.end(), predicate);
   ```

## 📖 See Also

- [Algorithm Examples](file:///home/prashanth/learncpp_workspace/STL/examples/algorithms/algorithm_examples.cpp) - All algorithms with examples
- [Erase-Remove Idiom](file:///home/prashanth/learncpp_workspace/STL/examples/algorithms/erase_remove_idiom.cpp) - Essential pattern
- [Quick Reference](file:///home/prashanth/learncpp_workspace/STL/07_Quick_Reference.md) - Cheat sheet

## 🎓 Key Takeaways

1. Most algorithms work with **iterators**, not containers
2. Algorithms don't change container **size** (except container methods)
3. Use **`std::remove`** with **`erase()`** to actually remove elements
4. **Binary search** requires sorted range
5. **Complexity** matters - know Big-O for each algorithm
