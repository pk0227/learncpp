/*
 * MUTEX - MUTUAL EXCLUSION FOR DATA RACE PREVENTION
 * 
 * Shows proper synchronization of shared data using std::mutex.
 * 
 * MUTEX CONCEPT:
 * - Mutual Exclusion: Only ONE thread can hold mutex at a time
 * - lock(): Acquires mutex (blocks if already locked)
 * - unlock(): Releases mutex (allows others to acquire)
 * - Critical section: Code between lock() and unlock()
 * 
 * HOW IT FIXES DATA RACE:
 * - task_count++ protected by mutex
 * - Only one thread can increment at a time
 * - Other thread waits for mutex release
 * - No interleaving of read-modify-write operations
 * 
 * CRITICAL SECTION:
 * - Smallest amount of code that accesses shared data
 * - Keep it short (holds lock = blocks others)
 * - Don't do I/O or sleep inside critical section
 * 
 * EXECUTION TIMELINE:
 * Time 1: Alice locks mutex, increments, unlocks
 * Time 2: Bob locks mutex (waits if Alice still holds)
 * Time 3: Bob increments, unlocks
 * Result: Both increments counted, no lost updates
 * 
 * MUTEX vs ATOMIC:
 * - Mutex: Heavier, can protect complex operations
 * - Atomic: Lighter, only for simple operations (++, load, store)
 * - Mutex: OS kernel may be involved (context switch possible)
 * - Atomic: Usually implemented with CPU atomic instructions
 * 
 * COMMON MISTAKES:
 * - Forgetting to unlock (DEADLOCK!)
 * - Locking different mutexes for same data
 * - Accessing data outside critical section
 * 
 * BEST PRACTICE:
 * - Use RAII: std::lock_guard or std::scoped_lock
 * - Automatic unlock when scope exits
 * - Exception-safe (see updated examples)
 * 
 * PERFORMANCE:
 * - Mutex has overhead (few microseconds per lock/unlock)
 * - Contention: Multiple threads waiting = serialization
 * - For simple counters, atomic is faster
 * 
 * INTERVIEW POINTS:
 * - What is critical section?
 * - Why use RAII for mutexes?
 * - Mutex vs spinlock tradeoffs
 * - Can you have nested locks? (recursive_mutex)
 */

#include <thread>
#include <mutex>
#include <chrono>

unsigned int task_count = 0;
std::mutex counter_mutex;

void worker() {
    for (int i = 0; i < 5; i++) {
        printf("Worker %d is planning...\n", std::this_thread::get_id());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        counter_mutex.lock();
        task_count++;
        counter_mutex.unlock();
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
