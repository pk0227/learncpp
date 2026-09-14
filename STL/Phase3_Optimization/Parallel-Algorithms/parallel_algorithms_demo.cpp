/**
 * Parallel Algorithms & Execution Policies (C++17 / C++20)
 *
 * Demonstrates:
 * 1. Sequential vs Parallel Sorting (std::sort with std::execution::par)
 * 2. Sequential Accumulation (std::accumulate) vs Parallel Reduction (std::reduce)
 * 3. Parallel Map-Reduce with std::transform_reduce
 * 4. Execution policy comparison: seq vs par vs par_unseq
 *
 * Compile:
 *   g++ -std=c++20 -O2 parallel_algorithms_demo.cpp -o parallel_demo -ltbb
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <execution>
#include <chrono>
#include <random>
#include <string>

template<typename Func>
long long measure(const std::string& name, Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << name << ": " << duration << " ms\n";
    return duration;
}

int main() {
    std::cout << "=== C++17/C++20 Parallel Algorithms Demo ===\n\n";

    constexpr size_t N = 5'000'000;
    std::cout << "Generating dataset of " << N << " random 64-bit integers...\n";

    std::vector<int64_t> original(N);
    std::mt19937_64 rng(42);
    std::uniform_int_distribution<int64_t> dist(1, 1'000'000);
    for (size_t i = 0; i < N; ++i) {
        original[i] = dist(rng);
    }

    // ========================================================================
    // 1. SORTING: Sequential vs Parallel
    // ========================================================================
    std::cout << "\n--- 1. Sorting Benchmark (Introsort) ---\n";

    std::vector<int64_t> data_seq = original;
    std::vector<int64_t> data_par = original;

    auto t_seq_sort = measure("Sequential sort (std::execution::seq)", [&]() {
        std::sort(std::execution::seq, data_seq.begin(), data_seq.end());
    });

    auto t_par_sort = measure("Parallel sort   (std::execution::par)", [&]() {
        std::sort(std::execution::par, data_par.begin(), data_par.end());
    });

    if (t_par_sort > 0) {
        std::cout << "Parallel Speedup: " << (double)t_seq_sort / t_par_sort << "x\n";
    }

    // ========================================================================
    // 2. REDUCTION: std::accumulate vs std::reduce
    // ========================================================================
    std::cout << "\n--- 2. Reduction Benchmark (Summation) ---\n";

    int64_t sum_acc = 0;
    auto t_acc = measure("std::accumulate (strictly sequential left-fold)", [&]() {
        sum_acc = std::accumulate(original.begin(), original.end(), int64_t{0});
    });

    int64_t sum_red = 0;
    auto t_red = measure("std::reduce     (parallel out-of-order sum)", [&]() {
        sum_red = std::reduce(std::execution::par, original.begin(), original.end(), int64_t{0});
    });

    std::cout << "Sum Match: " << (sum_acc == sum_red ? "YES (Verified)" : "NO") << "\n";
    if (t_red > 0) {
        std::cout << "Reduction Speedup: " << (double)t_acc / t_red << "x\n";
    }

    // ========================================================================
    // 3. MAP-REDUCE: std::transform_reduce
    // ========================================================================
    std::cout << "\n--- 3. Parallel Map-Reduce (std::transform_reduce) ---\n";

    std::vector<std::string> words = {
        "Standard", "Template", "Library", "Modern", "Concurrency",
        "Parallelism", "Algorithms", "Optimization", "HighPerformance"
    };

    // Calculate total character count across words in parallel
    size_t total_length = std::transform_reduce(
        std::execution::par,
        words.begin(), words.end(),
        size_t{0},
        std::plus<size_t>{},
        [](const std::string& s) { return s.length(); }
    );

    std::cout << "Total characters across all words: " << total_length << "\n";
    std::cout << "\nParallel algorithm execution completed successfully.\n";

    return 0;
}
