/*
 * ATOMIC OPERATIONS - LOCK-FREE SYNCHRONIZATION
 * 
 * Demonstrates std::atomic for lock-free thread-safe operations.
 * 
 * ATOMIC OPERATIONS:
 * - Indivisible: Executes as single uninterruptible unit
 * - No race conditions: Other threads see before or after, never during
 * - Lock-free: No OS mutex, uses CPU atomic instructions
 * - Much faster than mutex for simple operations
 * 
 * HOW CPU IMPLEMENTS ATOMICS:
 * - x86: LOCK prefix (lock INCL memory location)
 * - ARM: LDREX/STREX (load-exclusive/store-exclusive)
 * - Hardware ensures atomicity at cache/memory level
 * - May use cache coherence protocols (MESI, MOESI)
 * 
 * ATOMIC++ OPERATIONS:
 * - std::atomic<T>::operator++: fetch_add(1)
 * - Atomic read-modify-write operation
 * - Other threads can't see intermediate state
 * - Memory ordering guarantees (default: sequential consistency)
 * 
 * MEMORY ORDERING:
 * - memory_order_seq_cst: Default, strongest (sequential consistency)
 * - memory_order_acquire/release: Lighter, synchronization
 * - memory_order_relaxed: Lightest, no synchronization guarantees
 * - Advanced topic: affects compiler/CPU reordering
 * 
 * WHEN TO USE ATOMIC:
 * - Simple operations: increment, decrement, load, store
 * - Flags and counters
 * - Lock-free data structures (advanced)
 * - High-contention scenarios (faster than mutex)
 * 
 * WHEN NOT TO USE:
 * - Complex operations requiring multiple steps
 * - Need to protect multiple variables together
 * - Compound operations (e.g., if-then-else)
 * - Use mutex for complex critical sections
 * 
 * ATOMIC vs MUTEX:
 * - Atomic: ~10ns, lock-free, simple operations only
 * - Mutex: ~100ns-1us, can deadlock, any critical section
 * - Atomic: Can't wait or sleep
 * - Mutex: Can condition-wait
 * 
 * SUPPORTED TYPES:
 * - std::atomic<int>, std::atomic<long>, etc.
 * - std::atomic<T*> (pointers)
 * - std::atomic<bool>
 * - Custom types if trivially copyable and small enough
 * 
 * INTERVIEW MUST-KNOW:
 * - What makes operation atomic?
 * - Atomic vs mutex tradeoffs
 * - Memory ordering models (basic understanding)
 * - ABA problem in lock-free programming
 * - When lock-free isn't actually faster (cache ping-pong)
 */

#include <thread>
#include <atomic>

std::atomic<unsigned int> task_count(0);

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
    printf("We completed %u tasks.\n", task_count.load());

    return 0;
}
