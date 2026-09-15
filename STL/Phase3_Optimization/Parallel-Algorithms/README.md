# ⚡ Parallel Algorithms & Execution Policies (C++17 / C++20)

> **Senior Dev Context:**  
> *"Why write manual thread pool loops for embarrassingly parallel operations when modern C++ provides multi-threaded, SIMD-vectorized STL algorithms out-of-the-box?"*  
> Starting in C++17, standard STL algorithms can be executed concurrently and vectorized with a single policy argument (`<execution>`).

---

## 📑 Table of Contents

1. [🚀 What are Execution Policies?](#-what-are-execution-policies)
2. [📊 The Four Standard Execution Policies](#-the-four-standard-execution-policies)
3. [⚡ Parallel Algorithms Overview](#-parallel-algorithms-overview)
   - [Sorting in Parallel (`std::sort`)](#sorting-in-parallel-stdsort)
   - [Parallel Associative Reduction (`std::reduce`)](#parallel-associative-reduction-stdreduce)
   - [Map-Reduce in Parallel (`std::transform_reduce`)](#map-reduce-in-parallel-stdtransform_reduce)
4. [⚠️ Critical Rules & Pitfalls](#️-critical-rules--pitfalls)
   - [1. Exception Handling (`std::terminate`)](#1-exception-handling-stdterminate)
   - [2. The Vectorization Safety Trap (`par_unseq` / `unseq`)](#2-the-vectorization-safety-trap-par_unseq--unseq)
   - [3. Associativity & Commutativity Requirement](#3-associativity--commutativity-requirement)
5. [🎓 Senior Interview Cheat Sheet & Q&A](#-senior-interview-cheat-sheet--qa)
6. [📁 Code Examples](#-code-examples)

---

## 🚀 What are Execution Policies?

Before C++17, algorithms like `std::sort` and `std::transform` were strictly single-threaded and sequential. To parallelize work across multiple CPU cores, developers had to manually slice data and distribute jobs into `std::thread`, OpenMP, or external thread pools.

C++17 introduced **Execution Policies** in header `<execution>`:
```cpp
#include <algorithm>
#include <execution>
#include <vector>

std::vector<int> data = /* ... 10,000,000 elements ... */;

// Runs Introsort across all CPU cores in parallel!
std::sort(std::execution::par, data.begin(), data.end());
```

---

## 📊 The Four Standard Execution Policies

Defined in `namespace std::execution`:

| Policy | Standard | Execution Guarantee | SIMD Vectorized? | Multi-threaded? |
|---|---|---|---|---|
| `std::execution::seq` | C++17 | Strictly sequential on calling thread | ❌ No | ❌ No |
| `std::execution::par` | C++17 | Multi-threaded parallel execution | ❌ No | ✅ Yes (Thread pool) |
| `std::execution::par_unseq` | C++17 | Multi-threaded AND vectorized | ✅ Yes (SIMD instructions) | ✅ Yes (Multi-threaded) |
| `std::execution::unseq` | C++20 | Single-threaded vectorized | ✅ Yes (SIMD instructions) | ❌ No (Single thread) |

### Understanding "Unsequenced" Execution:
- **`seq`:** Step $N$ finishes before step $N+1$ begins on the same thread.
- **`par`:** Steps can run concurrently on multiple worker threads, but each thread executes its assigned steps sequentially.
- **`par_unseq` / `unseq`:** Steps on a single thread can be interleaved arbitrarily (e.g., SIMD vector registers processing 4, 8, or 16 elements in parallel).

---

## ⚡ Parallel Algorithms Overview

Over 60 standard algorithms in `<algorithm>` and `<numeric>` accept an execution policy as their first parameter.

### Sorting in Parallel (`std::sort`)

```cpp
#include <algorithm>
#include <execution>
#include <vector>

std::vector<double> sensor_readings(10'000'000);
// Parallel introsort utilizing all CPU cores:
std::sort(std::execution::par, sensor_readings.begin(), sensor_readings.end());
```

### Parallel Associative Reduction (`std::reduce`)

Why is `std::accumulate` NOT parallelizable?
- `std::accumulate` is defined by the C++ standard to evaluate strictly from left to right:
  $$\text{result} = (((init + x_0) + x_1) + x_2) \dots$$
- Because addition order is strictly sequential, the compiler cannot reorder or parallelize additions across threads.

**`std::reduce` (C++17)** removes the strict left-to-right requirement:
- Assumes the binary operation is **associative** and **commutative**.
- Slices the container across multiple threads, accumulates partial sums independently, and combines the results:

```cpp
#include <numeric>
#include <execution>
#include <vector>

std::vector<long long> values(50'000'000, 1);

// Runs parallel multi-threaded reduction across CPU cores:
long long sum = std::reduce(std::execution::par, values.begin(), values.end(), 0LL);
```

### Map-Reduce in Parallel (`std::transform_reduce`)

Executes a transformation function on each element and then reduces the transformed values:

```cpp
#include <numeric>
#include <execution>
#include <vector>
#include <string>

std::vector<std::string> words = {"apple", "banana", "cherry", "date"};

// Count total characters across all words in parallel
size_t total_chars = std::transform_reduce(
    std::execution::par,
    words.begin(), words.end(),
    size_t{0},
    std::plus<size_t>{},                 // Reduction operator
    [](const std::string& s) { return s.length(); } // Transformation
);
```

---

## ⚠️ Critical Rules & Pitfalls

### 1. Exception Handling (`std::terminate`)

> [!WARNING]
> **Unhandled exceptions terminate the entire process!**  
> If an algorithm invoked with an execution policy throws an uncaught exception from any thread, `std::terminate` is invoked immediately. Exceptions do **not** propagate back to the caller.

### 2. The Vectorization Safety Trap (`par_unseq` / `unseq`)

When using `par_unseq` or `unseq`, your lambda/functor must be **vectorization-safe**:
- ❌ **Do NOT acquire mutexes or locks:** If a thread is interrupted in the middle of a SIMD instruction while holding a lock, a deadlock occurs.
- ❌ **Do NOT allocate dynamic memory (`new` / `malloc`):** Dynamic allocators take internal locks.
- ❌ **Do NOT invoke non-reentrant system functions.**
- ✅ Only perform pure mathematical, bitwise, or memory calculations.

### 3. Associativity & Commutativity Requirement

Algorithms like `std::reduce` partition work into chunks that are summed in arbitrary tree order.
- If your binary operation is **not associative** (e.g., floating-point addition where $(a + b) + c \ne a + (b + c)$ due to rounding errors), `std::reduce` may produce slightly different results between runs depending on thread scheduling.

---

## 🎓 Senior Interview Cheat Sheet & Q&A

### Q1: Why can't `std::accumulate` take an execution policy like `std::execution::par`?
**A:** `std::accumulate` is standardized with strict left-to-right sequential evaluation semantics ($((init + a) + b) + c$). To parallelize reduction across multiple threads, the algorithm must be allowed to partition elements into arbitrary chunks and evaluate partial sums in arbitrary order. C++17 introduced `std::reduce` and `std::transform_reduce` specifically to allow parallel, out-of-order associative reduction.

### Q2: When does `std::execution::par` actually slow down performance?
**A:** Parallel execution incurs thread scheduling, work distribution, and thread-pool coordination overhead. If the dataset is small (e.g., $< 10,000$ integers) or if each element's processing cost is negligible, the multi-threading overhead exceeds the computation time, making parallel execution slower than plain sequential execution.

---

## 📁 Code Examples

- [`parallel_algorithms_demo.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase3_Optimization/Parallel-Algorithms/parallel_algorithms_demo.cpp): Compilable micro-benchmark comparing serial `std::sort` vs parallel `std::sort(std::execution::par)` and `std::accumulate` vs parallel `std::reduce`.
