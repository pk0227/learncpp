# 🎯 C++20 Ranges: The Modern Era

> **Senior Dev Context:**
> "I use Ranges because they make intent clearer (`filter | transform`) and prevent bugs (no off-by-one errors with `begin()/end()`). Plus, they compose beautifully."

---

## 🔑 Core Concepts

### 1. Ranges vs Views
*   **Range**: Anything you can iterate over (owns the data). E.g., `std::vector`, `std::list`.
*   **View**: A lightweight wrapper that *looks* at the data (lazy evaluation). Does **not** own data.
    *   *Analogy*: SQL Query plan. It doesn't run until you iterate.
    *   `std::views::filter`, `std::views::transform`.

### 2. Pipe Syntax (`|`)
Allows chaining operations left-to-right (Unix style).
```cpp
auto res = nums | filter(even) | transform(square) | take(5);
```
*   **Readable**: Reads like a sentence.
*   **Efficient**: No temporary vectors created between steps.

### 3. Projections
The killer feature for algorithms.
Instead of writing a custom lambda comparator:
```cpp
// Old
sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.size() < b.size(); });
```
Just point to the member or function you want to compare:
```cpp
// New
ranges::sort(v, {}, &string::size);
```

### 4. Dangers (Dangling Iterators)
*   **Rule**: Never return a View to a local container.
*   `auto v = std::views::all(std::vector{1,2,3});` <- ⚠️ DANGER. Vector dies, view dangles.
*   C++20 concepts (`std::ranges::borrowed_range`) try to catch this at compile time!

---

## 🎓 Interview Cheat Sheet

**Q: What is the difference between `std::sort` and `std::ranges::sort`?**
A: `std::ranges::sort` accepts a container directly (no `begin/end`), supports **Projections** (sort by member), and is constrained by Concepts (better error messages).

**Q: What is Lazy Evaluation in Ranges?**
A: Operations like `filter` or `transform` don't happen when you define them. They happen element-by-element *as you iterate*. This allows working with "infinite" sequences or large data without allocating intermediate memory.
