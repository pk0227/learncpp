/**
 * Memory Optimization using C++17 std::pmr (Polymorphic Memory Resources)
 *
 * THE PROBLEM:
 * Standard `std::vector` and `std::string` use `new` (heap allocation) for
 * every object.
 * - Thousands of small objects = Thousands of `malloc/free` calls.
 * - Result: Heap fragmentation + Cache misses + Slow performance.
 *
 * THE SOLUTION (Senior Level):
 * Use `std::pmr::monotonic_buffer_resource`.
 * - Pre-allocate a big chunk of memory (on stack or heap).
 * - "Allocating" is just moving a pointer forward (0 cycles).
 * - "Deallocating" is a no-op (memory reused when resource is destroyed).
 *
 * INTERVIEW TAKEAWAY:
 * "For short-lived containers or many small allocations, I use `std::pmr` with
 * a local buffer to achieve stack-like performance with dynamic containers."
 *
 * Compile: g++ -std=c++17 -O3 -Wall -Wextra pmr_benchmark.cpp -o pmr_benchmark
 */

#include <array>
#include <chrono>
#include <cstddef> // for std::byte
#include <iostream>
#include <list>
#include <memory_resource> // C++17
#include <string>
#include <vector>

// ============================================================================
// BENCHMARK UTILS
// ============================================================================

template <typename Func> long long measure(Func func, const std::string &name) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();
  std::cout << name << ": " << duration << " us\n";
  return duration;
}

// ============================================================================
// SCENARIO: Create 10,000 temporary lists
// ============================================================================

const int ITERATIONS = 10000;
const int LIST_SIZE = 1000;

void benchmark_standard_allocator() {
  for (int i = 0; i < ITERATIONS; ++i) {
    // Standard list: Allocates node on Heap for EVERY element
    std::list<int> l;
    for (int j = 0; j < LIST_SIZE; ++j) {
      l.push_back(j);
    }
  }
}

void benchmark_pmr_allocator() {
  // 1. Create a buffer on stack (FAST!)
  // 4MB to be safe for lists overhead
  std::array<std::byte, 4 * 1024 * 1024> buffer;

  // 2. Create a monotonic resource using that buffer
  // "Monotonic" means it only grows, never frees individual items.
  // Very fast for "build up and tear down" patterns.
  std::pmr::monotonic_buffer_resource pool{(void *)buffer.data(), buffer.size(),
                                           std::pmr::null_memory_resource()};

  for (int i = 0; i < ITERATIONS; ++i) {
    // 3. Pass the resource to the list
    // pmr::list will allocate nodes from the monotonic buffer (simple ptr bump)
    std::pmr::list<int> l(&pool);

    for (int j = 0; j < LIST_SIZE; ++j) {
      l.push_back(j);
    }

    // IMPORTANT: Reset the pool to reuse the same stack buffer for next
    // iteration!
    pool.release();
  }
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
  std::cout << "=== Memory Optimization: Heap vs PMR (Stack Buffer) ===\n";
  std::cout << "Scenario: Creating " << ITERATIONS << " std::list, each with "
            << LIST_SIZE << " ints.\n";
  std::cout << "(Lists require 1 allocation PER element - PMR should win huge "
               "here)\n\n";

  auto t1 = measure(benchmark_standard_allocator,
                    "Standard Allocator (Heap via new)");
  auto t2 =
      measure(benchmark_pmr_allocator, "std::pmr Allocator (Stack Buffer)");

  std::cout << "\nResults:\n";
  std::cout << "PMR was " << (double)t1 / t2 << "x faster!\n";

  return 0;
}
