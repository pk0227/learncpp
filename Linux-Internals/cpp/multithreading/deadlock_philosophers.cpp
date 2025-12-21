/*
 * DINING PHILOSOPHERS - CLASSIC DEADLOCK PROBLEM
 * 
 * Famous concurrency problem demonstrating resource deadlock.
 * 
 * PROBLEM SETUP:
 * - 5 philosophers sit at round table
 * - 1 fork between each pair (5 forks total)
 * - Each philosopher needs 2 forks to eat
 * - Each philosopher: think → pick up forks → eat → put down forks → repeat
 * 
 * DEADLOCK SCENARIO:
 * - All philosophers pick up LEFT fork simultaneously
 * - All wait for RIGHT fork
 * - Everyone holds one resource, waiting for another
 * - Circular wait → DEADLOCK!
 * 
 * CODE REPRESENTATION:
 * - std::scoped_lock(first_bin, second_bin) acquires BOTH locks
 * - If order inconsistent between threads → deadlock possible
 * - This example uses consistent order (always both locks) → NO deadlock
 * - (Name might be misleading - this version is actually deadlock-free!)
 * 
 * FOUR DEADLOCK CONDITIONS (All must hold):
 * 1. Mutual Exclusion: Resources non-shareable (forks)
 * 2. Hold and Wait: Hold one resource while waiting for another
 * 3. No Preemption: Can't forcibly take resources
 * 4. Circular Wait: Circular chain of waiting
 * 
 * SOLUTIONS:
 * 1. Resource ordering: Always acquire lowest-numbered fork first
 * 2. std::scoped_lock: Acquires all locks atomically (THIS CODE)
 * 3. Limit diners: Only N-1 philosophers can pick up forks
 * 4. Timeout: Drop forks if can't get both (risk: livelock)
 * 5. Arbitrator: Central authority grants fork pairs
 * 
 * INTERVIEW FAVORITE:
 * - Explain the problem
 * - Identify deadlock conditions
 * - Propose multiple solutions
 * - Compare solution trade-offs
 * - Relate to real systems (database locks, file locks)
 */

#include <thread>
#include <mutex>
#include <cstdio>

int item_count = 5000;

void worker(std::mutex &first_bin, std::mutex &second_bin) {
    while (item_count > 0) {
        std::scoped_lock lock(first_bin, second_bin);
        if (item_count > 0) {
            item_count--;
        }
    }
}

int main() {
    std::mutex bin_a, bin_b;
    std::thread alice(worker, std::ref(bin_a), std::ref(bin_b));
    std::thread bob(worker, std::ref(bin_b), std::ref(bin_a));
    alice.join();
    bob.join();
    printf("The workers are done picking items.\n");
    return 0;
}
