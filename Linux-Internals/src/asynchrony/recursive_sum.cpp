/*
 * RECURSIVE PARALLEL SUM USING DIVIDE-AND-CONQUER
 * 
 * Demonstrates parallel divide-and-conquer using std::async for CPU-intensive operations.
 * 
 * ALGORITHM:
 * - Recursively splits range into halves
 * - Left half computed asynchronously (potential parallel execution)
 * - Right half computed synchronously on current thread
 * - Results combined when both complete
 * 
 * DEPTH LIMITING:
 * - Stops creating async tasks after depth 3 (2^3 = 8 max concurrent tasks)
 * - Too deep: Overhead of thread creation exceeds benefits
 * - Too shallow: Underutilizes available CPU cores
 * 
 * PERFORMANCE CONSIDERATIONS:
 * - Best when: workload >> thread creation overhead
 * - Granularity: depth threshold prevents creating millions of tiny tasks
 * - Hardware threads: Optimal depth depends on CPU core count
 * 
 * PATTERN BREAKDOWN:
 * - Depth 0-3: Create async tasks (parallel execution)
 * - Depth > 3: Sequential computation (avoid overhead)
 * - This balances parallelism with task overhead
 * 
 * ASYNC BEHAVIOR:
 * - left.get() blocks until left half completes
 * - Right half may complete before left (depends on scheduling)
 * - Final sum waits for both halves
 * 
 * INTERVIEW INSIGHTS:
 * - Why not make both halves async? (Current thread would idle)
 * - How to choose optimal depth threshold?
 * - Comparison with fork-join pattern
 * - When is parallel algorithm slower than sequential?
 * 
 * NOTE: For production, use std::reduce with execution policies (C++17)
 */

#include <cstdio>
#include <future>

unsigned long long calculate_sum(unsigned int lo, unsigned int hi, unsigned int depth = 0) {
    if (depth > 3) { // base case threshold
        unsigned long long sum = 0;
        for (auto i = lo; i < hi; i++) {
            sum += i;
        }
        return sum;
    } else {  // divide and conquer
        auto mid = (hi + lo) / 2; // middle index for splitting
        auto left = std::async(std::launch::async, calculate_sum, lo, mid, depth + 1);
        auto right = calculate_sum(mid, hi, depth + 1);
        return left.get() + right;
    }
}

int main() {
    unsigned long long total = calculate_sum(0, 1000000000);
    printf("Total: %llu\n", total);
    return 0;
}
