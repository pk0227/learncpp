/*
 * DATA RACE DEMONSTRATION - CLASSIC CONCURRENCY BUG
 * 
 * Shows what happens when multiple threads access shared data without synchronization.
 * 
 * DATA RACE DEFINED:
 * - Two or more threads access same memory location
 * - At least one access is a write
 * - No synchronization between accesses
 * - Result: Undefined behavior, lost updates
 * 
 * WHY THIS FAILS:
 * - task_count++ is NOT atomic (actually 3 operations)
 * - 1. Read task_count from memory to register
 * - 2. Increment register value
 * - 3. Write register back to memory
 * - Threads can interleave these steps!
 * 
 * EXAMPLE RACE:
 * Time 1: Alice reads task_count = 100
 * Time 2: Bob reads task_count = 100
 * Time 3: Alice increments to 101, writes back
 * Time 4: Bob increments to 101, writes back
 * Result: task_count = 101 (should be 102, lost one increment!)
 * 
 * EXPECTED vs ACTUAL:
 * - Expected: 2 * 10,000,000 = 20,000,000 tasks
 * - Actual: Usually much less (15-19 million)
 * - Lost updates due to race condition
 * 
 * FIXES:
 * 1. Mutex: Use std::mutex (see simple_mutex.cpp)
 * 2. Atomic: Use std::atomic (see atomic.cpp)
 * 3. Lock-free algorithms (advanced)
 * 
 * DETECTION:
 * - ThreadSanitizer (TSan): Detects data races at runtime
 * - Compile with: -fsanitize=thread
 * - Valgrind helgrind: Another race detector
 * 
 * INTERVIEW GOLD:
 * - Can you identify data races in code?
 * - Why is ++ not atomic?
 * - How to fix this race?
 * - What is "happens-before" relationship?
 * 
 * RUN MULTIPLE TIMES: You'll see different results (non-deterministic)
 */

#include <thread>

unsigned int task_count = 0;

void worker() {
    for (int i = 0; i < 10000000; i++) {
        task_count++;
    }
}

int main() {
    std::thread alice(worker);
    std::thread bob(worker);
    alice.join();
    bob.join();
    printf("We completed %u tasks.\n", task_count);

    return 0;
}
