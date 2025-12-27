#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <string>
#include <mutex>
#include <sstream>

using uli = unsigned long int;

// ============================================================================
// ATOMIC-BASED SHARED DATA STRUCTURE
// ============================================================================

/* 
 * ╔══════════════════════════════════════════════════════════════════════════╗
 * ║                  CONSOLE OUTPUT SYNCHRONIZATION                           ║
 * ╚══════════════════════════════════════════════════════════════════════════╝
 * 
 * PROBLEM: INTERLEAVED CONSOLE OUTPUT
 * ────────────────────────────────────
 * When multiple threads write to std::cout simultaneously, output gets mixed:
 * 
 * Example of garbled output:
 * Thread A: std::cout << "[Incrementer " << tid << "] Starting...\n";
 * Thread B: std::cout << "[Reader " << id << "] Reading...\n";
 * 
 * Could produce:
 * "[Incrementer [Reader 1] Reading...
 * 12345] Starting..."
 * 
 * WHY THIS HAPPENS:
 * - Each << operation is individually thread-safe (on most implementations)
 * - But the SEQUENCE of << operations is NOT atomic
 * - Thread context switches can occur between << operations
 * - Result: Output from different threads gets interleaved
 * 
 * SOLUTION: MUTEX FOR CONSOLE OUTPUT
 * ───────────────────────────────────
 * We use a dedicated mutex (g_cout_mutex) to protect all console output.
 * 
 * Pattern:
 * {
 *     std::lock_guard<std::mutex> lock(g_cout_mutex);
 *     std::cout << "Complete message here\n";
 * }
 * 
 * This ensures the entire output operation completes before another thread can write.
 * 
 * ALTERNATIVE APPROACHES:
 * 1. std::osyncstream (C++20) - synchronized output stream
 * 2. Logging library (spdlog, Boost.Log) - production-grade solution
 * 3. Message queue - dedicated logging thread
 * 
 * For this educational example, a simple mutex is clear and effective.
 */
std::mutex g_cout_mutex;  // Protects std::cout from concurrent access

/* 
 * ╔══════════════════════════════════════════════════════════════════════════╗
 * ║                     WHAT IS std::atomic?                                  ║
 * ╚══════════════════════════════════════════════════════════════════════════╝
 * 
 * std::atomic is a template class that provides atomic operations on a variable.
 * "Atomic" means the operation completes as a single, indivisible step.
 * 
 * KEY PROPERTIES:
 * 1. INDIVISIBILITY: No thread can observe a half-completed atomic operation
 * 2. THREAD-SAFE: Multiple threads can safely access atomic variables
 * 3. NO DATA RACES: Properly synchronized atomic operations prevent undefined behavior
 * 4. LOCK-FREE (often): On modern CPUs, atomics use CPU instructions, not mutexes
 * 
 * COMPARISON: Atomic vs Mutex-Protected Variable
 * ┌─────────────────────────────────────────────────────────────────────────┐
 * │ Mutex-Protected int:                                                     │
 * │ - Must acquire mutex before read/write                                  │
 * │ - Can deadlock if not careful                                           │
 * │ - Heavier weight (context switch possible)                              │
 * │ - Flexible (can protect complex operations)                             │
 * │                                                                          │
 * │ std::atomic<int>:                                                        │
 * │ - No mutex needed                                                        │
 * │ - Cannot deadlock                                                        │
 * │ - Lighter weight (usually CPU instructions)                             │
 * │ - Limited to simple operations (load, store, add, etc.)                 │
 * └─────────────────────────────────────────────────────────────────────────┘
 */

/* 
 * ╔══════════════════════════════════════════════════════════════════════════╗
 * ║                     C++ MEMORY MODELS                                     ║
 * ╚══════════════════════════════════════════════════════════════════════════╝
 * 
 * Memory ordering specifies how atomic operations synchronize with each other.
 * The C++ standard defines six memory orderings:
 * 
 * 1. memory_order_relaxed
 * ────────────────────────
 *    - NO synchronization or ordering constraints
 *    - Only guarantees atomicity of the operation itself
 *    - Different threads may see operations in different orders
 *    - FASTEST but hardest to use correctly
 *    - Use case: Simple counters where order doesn't matter
 * 
 * 2. memory_order_consume (rarely used, similar to acquire)
 * ─────────────────────────────────────────────────────────
 *    - Lightweight version of acquire
 *    - Rarely used in practice (complex semantics)
 * 
 * 3. memory_order_acquire (LOAD operation)
 * ────────────────────────────────────────
 *    - Used for READING/LOADING operations
 *    - Guarantees: All writes that happened-before the release operation
 *      in another thread will be visible to this thread
 *    - Prevents reordering of reads/writes AFTER this operation
 *    - Forms synchronization pair with release/release-seq
 * 
 * 4. memory_order_release (STORE operation)
 * ─────────────────────────────────────────
 *    - Used for WRITING/STORING operations
 *    - Guarantees: All writes in current thread before this operation
 *      will be visible to threads that acquire the same atomic variable
 *    - Prevents reordering of reads/writes BEFORE this operation
 *    - Forms synchronization pair with acquire/consume
 * 
 * 5. memory_order_acq_rel (Read-Modify-Write operations)
 * ───────────────────────────────────────────────────────
 *    - Combines acquire + release semantics
 *    - Used for operations that both read AND write (fetch_add, exchange, etc.)
 *    - Acts as ACQUIRE for the read part
 *    - Acts as RELEASE for the write part
 * 
 * 6. memory_order_seq_cst (Sequential Consistency - DEFAULT)
 * ───────────────────────────────────────────────────────────
 *    - STRONGEST guarantee, DEFAULT memory order
 *    - Provides acquire-release semantics PLUS global sequential ordering
 *    - All threads see ALL seq_cst operations in the SAME order
 *    - SLOWEST but easiest to reason about
 *    - Behaves like traditional mutex-based synchronization
 * 
 * ╔══════════════════════════════════════════════════════════════════════════╗
 * ║                   WHICH MEMORY ORDER TO USE?                              ║
 * ╚══════════════════════════════════════════════════════════════════════════╝
 * 
 * Rule of thumb:
 * - Learning/Default: memory_order_seq_cst (safest, easiest)
 * - Performance-critical counters: memory_order_relaxed
 * - Producer-Consumer patterns: memory_order_release / memory_order_acquire
 * - Complex synchronization: memory_order_acq_rel
 * 
 * In this code, we use:
 * - memory_order_seq_cst: For most operations (clarity and correctness)
 * - memory_order_relaxed: For simple counter increments
 * - memory_order_release/acquire: For synchronization flags
 */

/* 
 * ATOMIC VARIABLE: g_count
 * ────────────────────────
 * This is the shared counter that the incrementer modifies and readers read.
 * 
 * std::atomic<uli> instead of just uli:
 * - Ensures atomic read/write operations
 * - No torn reads (reading partial value during write)
 * - No data races
 * 
 * OPERATIONS AVAILABLE:
 * - load(): Read the value atomically
 * - store(val): Write the value atomically
 * - fetch_add(n): Atomically add n and return OLD value
 * - fetch_sub(n): Atomically subtract n and return OLD value
 * - exchange(val): Atomically set new value, return old value
 * - compare_exchange_weak/strong: Atomic compare-and-swap
 * - operator++, operator+=: Convenient atomic increment
 */
std::atomic<uli> g_count{ 0 };

/* 
 * ATOMIC FLAG: g_increment_done
 * ──────────────────────────────
 * Signals readers that incrementer has finished incrementing.
 * 
 * When to use std::atomic<bool> vs std::atomic_flag:
 * - std::atomic<bool>: Full-featured, can load/store true/false
 * - std::atomic_flag: Minimal, only test_and_set() and clear()
 * - For simple flags, atomic<bool> is more readable
 * 
 * MEMORY ORDER: We use release/acquire pattern here
 * - Incrementer: store(true, memory_order_release)
 * - Readers: load(memory_order_acquire)
 * This ensures readers see all writes before the release
 */
std::atomic<bool> g_increment_done{ false };

/* 
 * ATOMIC FLAG: g_shutdown
 * ───────────────────────
 * Signals all threads to gracefully exit.
 * 
 * WHY ATOMIC?
 * - Set by incrementer thread
 * - Read by all reader threads
 * - Must be visible immediately to all threads
 * - No mutex needed (simpler, faster)
 */
std::atomic<bool> g_shutdown{ false };

/* 
 * ATOMIC FLAG: g_user_continue
 * ─────────────────────────────
 * Main thread sets this to signal incrementer to continue.
 * 
 * SYNCHRONIZATION PATTERN:
 * - Main thread: store(true, memory_order_release)
 * - Incrementer: load(memory_order_acquire)
 * This ensures incrementer sees the user's decision
 */
std::atomic<bool> g_user_continue{ false };

/* 
 * ATOMIC COUNTER: g_readers_finished_count
 * ─────────────────────────────────────────
 * Counts how many readers have finished reading in current cycle.
 * 
 * WHY ATOMIC?
 * - Multiple reader threads increment this concurrently
 * - Without atomic: RACE CONDITION!
 *   Thread A reads 5, Thread B reads 5, both write 6 → Lost update!
 * - With atomic: Each increment is indivisible
 *   Thread A: fetch_add(1) → 5 to 6, Thread B: fetch_add(1) → 6 to 7 ✓
 * 
 * MEMORY ORDER: We use memory_order_relaxed for increments
 * - We only care about the COUNT, not about synchronizing other data
 * - Much faster than seq_cst
 * - Final comparison uses stronger ordering (acquire)
 */
std::atomic<int> g_readers_finished_count{ 0 };

// Non-atomic variables (read-only after initialization, safe to share)
int g_total_readers{ 0 };           // Set once in main, never modified
int g_current_cycle{ 0 };           // Only written by incrementer, read by readers after sync


// ============================================================================
// ATOMIC OPERATIONS HELPER COMMENTS
// ============================================================================

/* 
 * ╔══════════════════════════════════════════════════════════════════════════╗
 * ║              COMMON ATOMIC OPERATIONS USED IN THIS CODE                   ║
 * ╚══════════════════════════════════════════════════════════════════════════╝
 * 
 * 1. LOAD - Read atomic value
 * ───────────────────────────
 * T value = atomic_var.load(memory_order);
 * 
 * Example: bool done = g_increment_done.load(std::memory_order_acquire);
 * - Atomically reads the value
 * - With acquire: synchronizes with release stores
 * - With seq_cst: total global ordering
 * - With relaxed: just atomicity, no ordering
 * 
 * 
 * 2. STORE - Write atomic value
 * ──────────────────────────────
 * atomic_var.store(value, memory_order);
 * 
 * Example: g_increment_done.store(true, std::memory_order_release);
 * - Atomically writes the value
 * - With release: makes all prior writes visible to acquirers
 * - With seq_cst: total global ordering
 * - With relaxed: just atomicity, no ordering
 * 
 * 
 * 3. FETCH_ADD - Atomic increment and return old value
 * ─────────────────────────────────────────────────────
 * T old_value = atomic_var.fetch_add(n, memory_order);
 * 
 * Example: int old = g_readers_finished_count.fetch_add(1, std::memory_order_relaxed);
 * - Atomically adds n to the variable
 * - Returns the value BEFORE addition
 * - Thread-safe even with concurrent calls
 * - Common use: distributed counters
 * 
 * 
 * 4. OPERATOR++ - Convenient atomic increment
 * ────────────────────────────────────────────
 * ++atomic_var;      // Pre-increment (returns new value)
 * atomic_var++;      // Post-increment (returns old value)
 * 
 * Example: g_count++;
 * - Equivalent to fetch_add(1) + 1 for ++var
 * - Uses default memory_order_seq_cst
 * - Convenient but slightly less efficient than fetch_add with explicit order
 * 
 * 
 * 5. EXCHANGE - Atomic swap
 * ──────────────────────────
 * T old_value = atomic_var.exchange(new_value, memory_order);
 * 
 * Example: bool was_done = g_increment_done.exchange(false);
 * - Atomically replaces value and returns old value
 * - Useful for "take and reset" patterns
 * 
 * 
 * 6. COMPARE_EXCHANGE - Atomic compare-and-swap
 * ──────────────────────────────────────────────
 * bool success = atomic_var.compare_exchange_strong(expected, desired, memory_order);
 * 
 * Example: int expected = 5;
 *          if (g_count.compare_exchange_strong(expected, 10)) {
 *              // Successfully changed 5 to 10
 *          } else {
 *              // Value was not 5, expected now contains actual value
 *          }
 * - Foundation of lock-free algorithms
 * - Only changes value if it matches expected
 * - If fails, expected is updated with actual value
 */


// ============================================================================
// INCREMENTER THREAD FUNCTION (ATOMIC VERSION)
// ============================================================================

void Incrementer(int increment_value)
{
    std::thread::id tid{ std::this_thread::get_id() };
    {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cout << "[Incrementer " << tid << "] Started (atomic version)\n";
    }

    int cycle = 0;

    while (true)
    {
        cycle++;
        g_current_cycle = cycle;  // Safe: only incrementer writes this
        
        {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "\n[Incrementer " << tid << "] === CYCLE " << cycle << " ===\n";
        }

        // STEP 1: PERFORM INCREMENT
        /* 
         * ╔══════════════════════════════════════════════════════════════════════════╗
         * ║                    ATOMIC INCREMENT OPERATIONS                            ║
         * ╚══════════════════════════════════════════════════════════════════════════╝
         * 
         * We increment g_count using atomic operations - NO MUTEX NEEDED!
         * 
         * THREE WAYS TO INCREMENT ATOMICALLY:
         * 
         * 1. Using operator++ (simplest):
         *    g_count++;
         *    - Uses default memory_order_seq_cst
         *    - Most restrictive, guarantees global ordering
         *    - Easiest to understand, safest choice
         * 
         * 2. Using fetch_add (more control):
         *    g_count.fetch_add(1, std::memory_order_relaxed);
         *    - Can specify memory order
         *    - relaxed: Only guarantees atomicity, no ordering (fastest)
         *    - Good for simple counters where order doesn't matter
         * 
         * 3. Using operator+= (convenient for larger increments):
         *    g_count += increment_value;
         *    - Uses default memory_order_seq_cst
         *    - Equivalent to fetch_add(n) + n
         * 
         * IN THIS CODE:
         * We use operator++ with default seq_cst for CLARITY.
         * In a performance-critical application, you could use:
         *    g_count.fetch_add(1, std::memory_order_relaxed);
         * 
         * WHY IS THIS SAFE WITHOUT MUTEX?
         * - Each g_count++ is an atomic operation
         * - Multiple threads could call g_count++ concurrently
         * - Each increment completes as a single, indivisible step
         * - No lost updates, no torn reads/writes
         * 
         * COMPARISON WITH MUTEX VERSION:
         * Mutex version:
         *    {
         *        std::unique_lock<std::shared_mutex> lock{g_shared_mutex};
         *        g_count++;  // Protected by mutex
         *    }
         * 
         * Atomic version:
         *    g_count++;  // No mutex needed, atomic operation
         * 
         * WHEN CAN WE USE ATOMIC INSTEAD OF MUTEX?
         * ✓ Simple operations on a single variable (read, write, increment)
         * ✓ No complex multi-step operations needed
         * ✓ Lock-free performance required
         * ✗ Need to protect multiple variables together
         * ✗ Complex operations that must appear atomic together
         */
        std::cout << "[Incrementer " << tid << "] Incrementing by " 
                  << increment_value << "...\n";
        
        uli old_value = g_count.load(std::memory_order_relaxed);
        
        for (int i = 0; i < increment_value; ++i)
        {
            /* 
             * MEMORY ORDER CHOICE: memory_order_seq_cst (default)
             * 
             * For this increment, we use the default (operator++ uses seq_cst).
             * 
             * ALTERNATIVE APPROACHES:
             * 
             * a) Relaxed for each increment, release for final write:
             *    for (int i = 0; i < increment_value - 1; ++i)
             *        g_count.fetch_add(1, std::memory_order_relaxed);
             *    g_count.fetch_add(1, std::memory_order_release);  // Last one
             * 
             * b) Load-modify-store with acquire-release:
             *    uli expected = g_count.load(std::memory_order_relaxed);
             *    uli desired = expected + increment_value;
             *    while (!g_count.compare_exchange_weak(expected, desired,
             *           std::memory_order_release, std::memory_order_relaxed));
             * 
             * We choose simplicity over micro-optimization here.
             */
            g_count++;  // Atomic increment with memory_order_seq_cst
        }
        
        uli new_value = g_count.load(std::memory_order_relaxed);
        
        {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "[Incrementer " << tid << "] Increment complete. "
                      << "Old value: " << old_value << ", New value: " << new_value << "\n";
        }

        // STEP 2: SIGNAL READERS THAT INCREMENT IS DONE
        /* 
         * ╔══════════════════════════════════════════════════════════════════════════╗
         * ║               RELEASE-ACQUIRE SYNCHRONIZATION PATTERN                     ║
         * ╚══════════════════════════════════════════════════════════════════════════╝
         * 
         * This is a classic PRODUCER-CONSUMER synchronization pattern.
         * 
         * INCREMENTER (Producer):
         * 1. Modifies shared data (g_count)
         * 2. Sets flag with RELEASE: g_increment_done.store(true, memory_order_release)
         * 
         * READERS (Consumers):
         * 1. Wait for flag with ACQUIRE: while (!g_increment_done.load(memory_order_acquire))
         * 2. Read shared data (g_count)
         * 
         * MEMORY ORDER: memory_order_release
         * ────────────────────────────────────
         * When incrementer stores true with release:
         * - ALL prior writes in incrementer (including g_count modifications)
         * - Are guaranteed to be visible to any thread that loads with acquire
         * - Forms a "happens-before" relationship
         * 
         * HAPPENS-BEFORE GUARANTEE:
         * ─────────────────────────
         * Incrementer:                          Reader:
         * g_count++;                            while (!g_increment_done.load(acquire))
         * g_count++;                                // spin-wait
         * g_count++;                            
         * g_increment_done.store(true, release) // Synchronization point
         *                                       auto value = g_count.load();
         *                                       // GUARANTEED to see all g_count++
         * 
         * WHY RELEASE HERE?
         * - We want readers to see ALL our writes to g_count
         * - Release store on g_increment_done ensures this
         * - Any thread that acquires g_increment_done will see our g_count updates
         * 
         * WHAT IF WE USED RELAXED INSTEAD?
         * g_increment_done.store(true, memory_order_relaxed);
         * - Only the store itself is atomic
         * - NO guarantee that readers see g_count updates
         * - Readers might read stale g_count values
         * - DATA RACE or incorrect results possible!
         * 
         * RESETTING THE COUNTER:
         * We reset g_readers_finished_count here for the next cycle.
         * Using store with release to ensure readers see the reset.
         */
        g_readers_finished_count.store(0, std::memory_order_release);
        
        {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "[Incrementer " << tid << "] Signaling all readers (atomic flag)...\n";
        }
        
        // CRITICAL: Set flag with RELEASE memory order
        g_increment_done.store(true, std::memory_order_release);
        
        // No notify_all() needed with atomics - readers are spinning or sleeping

        // STEP 3: WAIT FOR ALL READERS TO FINISH
        /* 
         * ╔══════════════════════════════════════════════════════════════════════════╗
         * ║                    SPIN-WAIT vs CONDITION VARIABLE                        ║
         * ╚══════════════════════════════════════════════════════════════════════════╝
         * 
         * With atomics, we have two choices for waiting:
         * 
         * 1. SPIN-WAIT (busy-waiting):
         *    while (g_readers_finished_count.load(memory_order_acquire) < g_total_readers)
         *        ; // Keep checking in a loop
         * 
         *    Pros: Low latency, no context switch
         *    Cons: Wastes CPU cycles, keeps core busy
         *    Use when: Wait time is very short (microseconds)
         * 
         * 2. SPIN + YIELD (hybrid):
         *    while (g_readers_finished_count.load(memory_order_acquire) < g_total_readers)
         *        std::this_thread::yield(); // Give up CPU timeslice
         * 
         *    Pros: More CPU-friendly than pure spin
         *    Cons: Still uses CPU, more context switches
         *    Use when: Wait time is short (milliseconds)
         * 
         * 3. SPIN + SLEEP (what we use):
         *    while (g_readers_finished_count.load(memory_order_acquire) < g_total_readers)
         *        std::this_thread::sleep_for(short_duration);
         * 
         *    Pros: CPU-friendly, thread sleeps between checks
         *    Cons: Higher latency than pure spin
         *    Use when: Wait time is moderate, CPU efficiency matters
         * 
         * 4. CONDITION VARIABLE (mutex version):
         *    cv.wait(lock, predicate);
         * 
         *    Pros: Most CPU-efficient, perfect for long waits
         *    Cons: Requires mutex, more complex
         *    Use when: Wait time is long or unknown
         * 
         * IN THIS CODE: We use spin + sleep (option 3)
         * - Balance between responsiveness and CPU usage
         * - Readers typically finish quickly (milliseconds)
         * - Educational: shows atomic-based synchronization
         * 
         * MEMORY ORDER: memory_order_acquire
         * ────────────────────────────────────
         * We load with acquire to synchronize with readers' release stores.
         * This ensures we see all readers' completion signals.
         */
        std::cout << "[Incrementer " << tid << "] Waiting for all " 
                  << g_total_readers << " readers to finish...\n";
        
        // Spin-wait with sleep (CPU-friendly)
        while (g_readers_finished_count.load(std::memory_order_acquire) < g_total_readers)
        {
            // Sleep briefly to avoid busy-waiting and wasting CPU
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
        {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "[Incrementer " << tid << "] All readers finished!\n";
        }
        
        // Reset flag for next cycle (with release to synchronize)
        g_increment_done.store(false, std::memory_order_release);

        // STEP 4: WAIT FOR USER INPUT
        /* 
         * ╔══════════════════════════════════════════════════════════════════════════╗
         * ║                 WAITING FOR USER DECISION (ATOMIC FLAGS)                  ║
         * ╚══════════════════════════════════════════════════════════════════════════╝
         * 
         * The incrementer waits for main thread to signal user's decision.
         * 
         * TWO POSSIBLE FLAGS:
         * - g_user_continue: User wants to continue (type 'y')
         * - g_shutdown: User wants to stop (type 'n')
         * 
         * SPIN-WAIT PATTERN:
         * while (!condition)
         *     sleep_for(short_duration);
         * 
         * We check both flags in the condition:
         * - Exit loop if g_user_continue is true (continue to next cycle)
         * - Exit loop if g_shutdown is true (break from main loop)
         * 
         * MEMORY ORDER: memory_order_acquire
         * ────────────────────────────────────
         * Main thread stores with release, we load with acquire.
         * This ensures we see main thread's decision and any prior changes.
         * 
         * CPU EFFICIENCY:
         * - sleep_for(10ms) makes this thread sleep between checks
         * - Much better than pure spin-wait (busy loop)
         * - 10ms is reasonable for user input (human timescale)
         */
        std::cout << "[Incrementer " << tid << "] Waiting for user decision...\n\n";
        
        // Spin-wait for user input (with longer sleep - user is slow!)
        while (!g_user_continue.load(std::memory_order_acquire) && 
               !g_shutdown.load(std::memory_order_acquire))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
        // Check if user wants to stop
        if (g_shutdown.load(std::memory_order_acquire))
        {
            {
                std::lock_guard<std::mutex> lock(g_cout_mutex);
                std::cout << "\n[Incrementer " << tid << "] User requested shutdown. Exiting.\n";
            }
            break;
        }
        
        // Reset flag for next iteration (with release to synchronize)
        g_user_continue.store(false, std::memory_order_release);
        {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "[Incrementer " << tid << "] User chose to continue!\n";
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // GRACEFUL SHUTDOWN
    /* 
     * ╔══════════════════════════════════════════════════════════════════════════╗
     * ║                         SHUTDOWN SIGNAL                                   ║
     * ╚══════════════════════════════════════════════════════════════════════════╝
     * 
     * When exiting, incrementer signals all readers to stop.
     * 
     * MEMORY ORDER: memory_order_release
     * ────────────────────────────────────
     * Ensures all our prior work is visible to readers when they check shutdown.
     * Readers will load with acquire and see this flag.
     * 
     * NOTE: g_shutdown might already be true (set by main thread).
     * This is safe - storing true again is idempotent.
     * The release ensures proper synchronization in either case.
     */
    g_shutdown.store(true, std::memory_order_release);
    
    {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cout << "[Incrementer " << tid << "] Completed " << cycle 
                  << " cycles. Final value: " << g_count.load(std::memory_order_relaxed) << "\n";
    }
}


// ============================================================================
// READER THREAD FUNCTION (SYNCHRONOUS - ATOMIC VERSION)
// ============================================================================

void ReaderSynchronous(int reader_id)
{
    std::thread::id tid{ std::this_thread::get_id() };
    {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cout << "[SyncReader-" << reader_id << " " << tid 
                  << "] Started (atomic version)\n";
    }

    int cycles_read = 0;
    int last_cycle_read = 0;  // Track which cycle we last read

    while (true)
    {
        // STEP 1: WAIT FOR INCREMENT TO COMPLETE OR SHUTDOWN
        /* 
         * ╔══════════════════════════════════════════════════════════════════════════╗
         * ║              ACQUIRE-RELEASE SYNCHRONIZATION (READER SIDE)                ║
         * ╚══════════════════════════════════════════════════════════════════════════╝
         * 
         * READER'S WAITING PATTERN:
         * The reader spins (with sleep) until incrementer signals ready.
         * 
         * CONDITION TO EXIT LOOP:
         * - g_increment_done is true (incrementer finished), OR
         * - g_shutdown is true (program is ending)
         * 
         * MEMORY ORDER: memory_order_acquire
         * ────────────────────────────────────
         * Critical for correctness!
         * - Incrementer stored true with memory_order_release
         * - We load with memory_order_acquire
         * - This creates happens-before relationship
         * - We are GUARANTEED to see all g_count increments
         * 
         * HAPPENS-BEFORE CHAIN:
         * ─────────────────────
         * Incrementer:                              Reader:
         * g_count++ (multiple times)                // Not visible yet
         * g_increment_done.store(true, release) ────→ while (!g_increment_done.load(acquire))
         *                                               // Synchronization!
         *                                           auto val = g_count.load();
         *                                           // MUST see all g_count++
         * 
         * WHY NOT RELAXED?
         * If we used memory_order_relaxed here:
         * - We might see g_increment_done==true
         * - But NOT see the updated g_count values!
         * - This is called "out-of-thin-air" read
         * - Results in incorrect data being read
         * 
         * SPIN-WAIT WITH SLEEP:
         * - Between checks, sleep for 1ms
         * - Reduces CPU usage compared to busy-spin
         * - Still responsive (1ms latency acceptable)
         * - For higher performance: reduce sleep or use std::this_thread::yield()
         * - For lower CPU usage: increase sleep duration
         */
        while (!g_increment_done.load(std::memory_order_acquire) && 
               !g_shutdown.load(std::memory_order_acquire))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
        // Check shutdown first
        if (g_shutdown.load(std::memory_order_acquire))
        {
            {
                std::lock_guard<std::mutex> lock(g_cout_mutex);
                std::cout << "[SyncReader-" << reader_id << " " << tid 
                          << "] Shutdown signal received. Exiting.\n";
            }
            break;
        }

        // Check if this is a new cycle (prevent reading same cycle twice)
        if (g_current_cycle == last_cycle_read)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;  // Wait for new cycle
        }

        cycles_read++;
        last_cycle_read = g_current_cycle;

        // STEP 2: READ THE VALUE
        /* 
         * ╔══════════════════════════════════════════════════════════════════════════╗
         * ║                    ATOMIC LOAD OPERATION                                  ║
         * ╚══════════════════════════════════════════════════════════════════════════╝
         * 
         * READING ATOMIC VARIABLE:
         * uli value = g_count.load(memory_order);
         * 
         * MEMORY ORDER CHOICES FOR LOAD:
         * 
         * 1. memory_order_relaxed:
         *    - Only guarantees atomicity (no torn reads)
         *    - NO synchronization with other operations
         *    - Fastest option
         *    - Safe HERE because we already synchronized via g_increment_done acquire
         *    - The acquire on g_increment_done ensures we see all prior g_count updates
         * 
         * 2. memory_order_acquire:
         *    - Synchronizes with release stores
         *    - Redundant here (we already did acquire on g_increment_done)
         *    - Slightly slower, but still safe
         * 
         * 3. memory_order_seq_cst (default):
         *    - Strongest guarantee
         *    - Global sequential consistency
         *    - Slowest, but safest for beginners
         * 
         * IN THIS CODE:
         * We use memory_order_relaxed because:
         * - We already synchronized with acquire on g_increment_done
         * - That acquire-release pair guarantees we see all g_count updates
         * - Additional ordering on this load is unnecessary
         * - Demonstrates advanced memory ordering understanding
         * 
         * ALTERNATIVE (more conservative):
         * uli value = g_count.load(std::memory_order_acquire);
         * - Also correct, just slightly less efficient
         * - Better choice if unsure about synchronization
         * 
         * NO MUTEX NEEDED:
         * - g_count is std::atomic, load is atomic operation
         * - Multiple readers can load simultaneously
         * - No data race, no torn reads
         * - Incrementer cannot interrupt our read (atomicity)
         * 
         * WHAT ABOUT SEQUENTIAL READING?
         * In the mutex version, sync readers used a mutex to read sequentially.
         * Here, we don't have that mutex - readers might read concurrently.
         * 
         * To enforce sequential reading with atomics:
         * - We could use a separate atomic<bool> turn flag
         * - Or a more complex atomic-based queue
         * - For this example, we simplify and allow concurrent reads
         * - The "sync" vs "async" distinction is more relevant with mutexes
         */
        uli value = g_count.load(std::memory_order_relaxed);
        
        {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "[SyncReader-" << reader_id << " " << tid 
                      << "] [Cycle " << cycles_read << "] Reading: " << value << "\n";
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(30));

        // STEP 3: SIGNAL THAT WE FINISHED READING
        /* 
         * ╔══════════════════════════════════════════════════════════════════════════╗
         * ║                    ATOMIC FETCH_ADD OPERATION                             ║
         * ╚══════════════════════════════════════════════════════════════════════════╝
         * 
         * INCREMENTING SHARED COUNTER:
         * Multiple readers increment g_readers_finished_count concurrently.
         * 
         * ATOMIC FETCH_ADD:
         * int old_value = g_readers_finished_count.fetch_add(1, memory_order);
         * 
         * WHAT FETCH_ADD DOES:
         * 1. Atomically read current value
         * 2. Atomically add 1 to it
         * 3. Atomically write back the new value
         * 4. Return the OLD value (before addition)
         * 
         * ALL THREE STEPS ARE INDIVISIBLE!
         * 
         * WHY IS THIS SAFE WITHOUT MUTEX?
         * 
         * Consider two threads incrementing simultaneously:
         * 
         * WITHOUT ATOMIC (DATA RACE):
         * Thread A: read 5, add 1, write 6
         * Thread B: read 5, add 1, write 6  ← Lost update! Should be 7
         * Result: 6 (incorrect!)
         * 
         * WITH ATOMIC FETCH_ADD:
         * Thread A: fetch_add(1) → atomically 5 to 6, returns 5
         * Thread B: fetch_add(1) → atomically 6 to 7, returns 6
         * Result: 7 (correct!)
         * 
         * MEMORY ORDER: memory_order_acq_rel
         * ───────────────────────────────────
         * We use acquire-release for this operation:
         * - ACQUIRE semantics: See all prior writes from other readers
         * - RELEASE semantics: Make our increment visible to incrementer
         * 
         * This ensures:
         * - Incrementer's load will see our increment (and all our prior work)
         * - We see increments from readers who finished before us
         * 
         * COULD WE USE RELAXED?
         * int old = g_readers_finished_count.fetch_add(1, memory_order_relaxed);
         * 
         * Maybe! Depends on what we're synchronizing:
         * - For just counting: relaxed is sufficient
         * - If incrementer needs to see our earlier work: need release
         * - We use acq_rel to be safe and demonstrate the pattern
         * 
         * CHECKING IF LAST READER:
         * - old_value is value BEFORE our increment
         * - If old_value == g_total_readers - 1, we made it reach g_total_readers
         * - We are the last reader!
         * - Only last reader should do final cleanup or signaling
         */
        int old_count = g_readers_finished_count.fetch_add(1, std::memory_order_acq_rel);
        int new_count = old_count + 1;
        
        {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "[SyncReader-" << reader_id << " " << tid 
                      << "] Finished reading. (" << new_count 
                      << "/" << g_total_readers << " readers done)\n";
        }
        
        // Check if we're the last reader
        if (new_count >= g_total_readers)
        {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "[SyncReader-" << reader_id << " " << tid 
                      << "] I'm the last reader!\n";
            
            /* 
             * LAST READER COULD SIGNAL HERE:
             * In the mutex version, last reader called notify_one().
             * With atomics, incrementer is spin-waiting on the counter.
             * No explicit signal needed - incrementer will see the update!
             * 
             * The incrementer's loop:
             * while (g_readers_finished_count.load(acquire) < g_total_readers)
             * 
             * Will automatically exit when our fetch_add completes.
             * The acquire-release pair ensures proper synchronization.
             */
        }
    }

    {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cout << "[SyncReader-" << reader_id << " " << tid 
                  << "] Completed " << cycles_read << " cycles. Exiting.\n";
    }
}


// ============================================================================
// READER THREAD FUNCTION (ASYNCHRONOUS - ATOMIC VERSION)
// ============================================================================

void ReaderAsynchronous(int reader_id)
{
    std::thread::id tid{ std::this_thread::get_id() };
    {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cout << "[AsyncReader-" << reader_id << " " << tid 
                  << "] Started (atomic version)\n";
    }

    int cycles_read = 0;
    int last_cycle_read = 0;  // Track which cycle we last read

    while (true)
    {
        // STEP 1: WAIT FOR INCREMENT OR SHUTDOWN (same as sync reader)
        /* 
         * IDENTICAL WAITING PATTERN:
         * Both sync and async readers wait for incrementer the same way.
         * The difference is in the reading phase (concurrent vs sequential).
         * 
         * See detailed comments in ReaderSynchronous for full explanation.
         */
        while (!g_increment_done.load(std::memory_order_acquire) && 
               !g_shutdown.load(std::memory_order_acquire))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
        if (g_shutdown.load(std::memory_order_acquire))
        {
            {
                std::lock_guard<std::mutex> lock(g_cout_mutex);
                std::cout << "[AsyncReader-" << reader_id << " " << tid 
                          << "] Shutdown signal received. Exiting.\n";
            }
            break;
        }

        // Check if this is a new cycle (prevent reading same cycle twice)
        if (g_current_cycle == last_cycle_read)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;  // Wait for new cycle
        }

        cycles_read++;
        last_cycle_read = g_current_cycle;

        // STEP 2: READ CONCURRENTLY
        /* 
         * ╔══════════════════════════════════════════════════════════════════════════╗
         * ║            CONCURRENT READING WITH ATOMIC VARIABLES                       ║
         * ╚══════════════════════════════════════════════════════════════════════════╝
         * 
         * ASYNC READERS READ CONCURRENTLY:
         * All async readers can call g_count.load() at the same time.
         * 
         * WHY IS THIS SAFE?
         * - g_count is std::atomic<uli>
         * - load() is an atomic operation
         * - Multiple concurrent loads are safe
         * - No data race, no torn reads
         * - Each reader gets a consistent value
         * 
         * COMPARISON: Atomic vs Mutex-based Concurrent Reading
         * ─────────────────────────────────────────────────────
         * 
         * Mutex version (shared_lock):
         * {
         *     std::shared_lock<std::shared_mutex> sl{g_shared_mutex};
         *     value = g_count;  // Multiple readers can hold shared_lock
         * }
         * 
         * Atomic version (no lock):
         * value = g_count.load(memory_order_relaxed);  // No lock needed!
         * 
         * ADVANTAGES OF ATOMIC:
         * + No mutex acquisition overhead
         * + No potential lock contention
         * + True lock-free operation (usually)
         * + Better performance at high concurrency
         * + Simpler code (no lock objects)
         * 
         * WHEN TO USE EACH:
         * - Atomic: Single variable, simple operations, high performance
         * - Shared lock: Multiple related variables, complex read operations
         * 
         * MEMORY ORDER: memory_order_relaxed
         * ────────────────────────────────────
         * Same reasoning as sync reader:
         * - Already synchronized via g_increment_done acquire
         * - That guarantees we see all g_count updates
         * - This load only needs atomicity, not additional ordering
         * 
         * GUARANTEED CORRECTNESS:
         * The acquire-release pair (incrementer release, reader acquire)
         * ensures all readers see the complete, updated value.
         */
        uli value = g_count.load(std::memory_order_relaxed);
        
        {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "[AsyncReader-" << reader_id << " " << tid 
                      << "] [Cycle " << cycles_read << "] Reading CONCURRENTLY: " 
                      << value << "\n";
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        // STEP 3: SIGNAL COMPLETION (same as sync reader)
        /* 
         * IDENTICAL COMPLETION SIGNALING:
         * Both sync and async readers increment the finished counter.
         * See detailed comments in ReaderSynchronous for full explanation.
         */
        int old_count = g_readers_finished_count.fetch_add(1, std::memory_order_acq_rel);
        int new_count = old_count + 1;
        
        {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "[AsyncReader-" << reader_id << " " << tid 
                      << "] Finished reading. (" << new_count 
                      << "/" << g_total_readers << " readers done)\n";
        }
        
        if (new_count >= g_total_readers)
        {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "[AsyncReader-" << reader_id << " " << tid 
                      << "] I'm the last reader!\n";
        }
    }

    {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cout << "[AsyncReader-" << reader_id << " " << tid 
                  << "] Completed " << cycles_read << " cycles. Exiting.\n";
    }
}


// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main()
{
    /* 
     * ╔══════════════════════════════════════════════════════════════════════════╗
     * ║                    ATOMIC VERSION OVERVIEW                                ║
     * ╚══════════════════════════════════════════════════════════════════════════╝
     * 
     * This version replaces mutexes and condition variables with atomics.
     * 
     * KEY DIFFERENCES FROM MUTEX VERSION:
     * 
     * 1. SHARED DATA:
     *    Mutex version: uli g_count (protected by mutex)
     *    Atomic version: std::atomic<uli> g_count (self-protecting)
     * 
     * 2. SYNCHRONIZATION:
     *    Mutex version: condition_variable + mutex
     *    Atomic version: atomic flags + spin-wait
     * 
     * 3. WAITING:
     *    Mutex version: cv.wait(lock, predicate) - thread sleeps
     *    Atomic version: while (!flag.load()) sleep() - periodic checks
     * 
     * 4. SIGNALING:
     *    Mutex version: cv.notify_all() / cv.notify_one()
     *    Atomic version: flag.store(true) - watchers poll the flag
     * 
     * 5. COUNTING FINISHED READERS:
     *    Mutex version: int g_readers_finished_count (protected by mutex)
     *    Atomic version: std::atomic<int> g_readers_finished_count
     * 
     * TRADE-OFFS:
     * 
     * ATOMIC ADVANTAGES:
     * + Simpler in some ways (no lock management)
     * + Lower latency (no context switch for signaling)
     * + Lock-free (no deadlock possibility)
     * + Better for short waits
     * 
     * ATOMIC DISADVANTAGES:
     * - Spin-waiting uses CPU (even with sleep)
     * - Not ideal for long or unpredictable waits
     * - Harder to reason about (memory ordering complexity)
     * - Can't easily wait on complex conditions
     * 
     * MUTEX/CV ADVANTAGES:
     * + True blocking wait (thread sleeps, no CPU usage)
     * + Immediate wakeup (no polling delay)
     * + Easier to understand (traditional synchronization)
     * + Better for long waits
     * 
     * MUTEX/CV DISADVANTAGES:
     * - Lock acquisition overhead
     * - Potential deadlock if misused
     * - Higher latency (thread wake-up time)
     * - More code (lock guards, cv.wait, notify)
     * 
     * WHEN TO USE WHICH:
     * - Use ATOMICS: Short waits, high performance, simple flags/counters
     * - Use MUTEX/CV: Long waits, complex conditions, want true blocking
     * - Use BOTH: Atomics for fast path, mutex/cv for slow path (hybrid)
     */

    {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║        ATOMIC-BASED USER-CONTROLLED INCREMENT DEMO          ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n\n";
    }

    // Get user input
    int num_sync_readers, num_async_readers, increment_value;

    std::cout << "Enter number of SYNCHRONOUS readers: ";
    std::cin >> num_sync_readers;

    std::cout << "Enter number of ASYNCHRONOUS readers: ";
    std::cin >> num_async_readers;

    std::cout << "Enter increment value per cycle: ";
    std::cin >> increment_value;

    g_total_readers = num_sync_readers + num_async_readers;

    {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cout << "\n[Main] Starting " << num_sync_readers << " synchronous readers, "
                  << num_async_readers << " asynchronous readers, "
                  << "and 1 incrementer.\n\n";
    }

    // Create thread vector
    std::vector<std::thread> threads;

    // Start incrementer thread
    threads.emplace_back(Incrementer, increment_value);

    // Start synchronous reader threads
    for (int i = 0; i < num_sync_readers; ++i)
    {
        threads.emplace_back(ReaderSynchronous, i + 1);
    }

    // Start asynchronous reader threads
    for (int i = 0; i < num_async_readers; ++i)
    {
        threads.emplace_back(ReaderAsynchronous, i + 1);
    }

    // Give threads time to start
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // User control loop
    /* 
     * ╔══════════════════════════════════════════════════════════════════════════╗
     * ║                    MAIN THREAD: USER INPUT HANDLER                        ║
     * ╚══════════════════════════════════════════════════════════════════════════╝
     * 
     * The main thread handles user input and signals the incrementer.
     * 
     * SYNCHRONIZATION WITH INCREMENTER:
     * 1. Get user input ('y' or 'n')
     * 2. Set appropriate atomic flag (g_user_continue or g_shutdown)
     * 3. Incrementer is spin-waiting on these flags
     * 4. Incrementer sees the flag and proceeds
     * 
     * MEMORY ORDER: memory_order_release
     * ────────────────────────────────────
     * We store flags with release so incrementer's acquire load
     * synchronizes and sees our decision.
     * 
     * NO CONDITION VARIABLE NEEDED:
     * In mutex version, we'd call cv.notify_one().
     * With atomics, incrementer polls the flags.
     */
    while (true)
    {
        std::cout << "\n[Main] Do you want to continue incrementing? (y/n): ";
        char choice;
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            // Signal incrementer to continue
            g_user_continue.store(true, std::memory_order_release);
            {
                std::lock_guard<std::mutex> lock(g_cout_mutex);
                std::cout << "[Main] Signaled incrementer to continue.\n";
            }
        }
        else if (choice == 'n' || choice == 'N')
        {
            // Signal shutdown
            {
                std::lock_guard<std::mutex> lock(g_cout_mutex);
                std::cout << "[Main] Initiating shutdown...\n";
            }
            g_shutdown.store(true, std::memory_order_release);
            break;
        }
        else
        {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "[Main] Invalid input. Please enter 'y' or 'n'.\n";
        }
    }

    // Wait for all threads to finish
    /* 
     * JOIN ALL THREADS:
     * After shutdown is signaled, all threads will eventually exit.
     * We wait for each thread to complete before exiting main.
     */
    {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cout << "[Main] Waiting for all threads to finish...\n";
    }
    for (auto& t : threads)
    {
        if (t.joinable())
            t.join();
    }

    {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                     PROGRAM COMPLETED                        ║\n";
        std::cout << "║  Final value: " << g_count.load(std::memory_order_relaxed);
        std::cout << std::string(45 - std::to_string(g_count.load()).length(), ' ') << "║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    }

    return 0;
}

/* 
 * ╔══════════════════════════════════════════════════════════════════════════╗
 * ║                    SUMMARY AND KEY TAKEAWAYS                              ║
 * ╚══════════════════════════════════════════════════════════════════════════╝
 * 
 * ATOMIC OPERATIONS USED IN THIS CODE:
 * 1. load(memory_order) - Read atomic variable
 * 2. store(value, memory_order) - Write atomic variable
 * 3. fetch_add(n, memory_order) - Atomic increment
 * 4. operator++ - Convenient atomic increment
 * 
 * MEMORY ORDERINGS USED:
 * 1. memory_order_seq_cst - Default, strongest, used for operator++
 * 2. memory_order_release - Producer stores data, then sets flag
 * 3. memory_order_acquire - Consumer checks flag, then reads data
 * 4. memory_order_acq_rel - For read-modify-write (fetch_add)
 * 5. memory_order_relaxed - For loads after synchronization
 * 
 * SYNCHRONIZATION PATTERNS:
 * 1. Release-Acquire (producer-consumer):
 *    - Incrementer: modify data, then flag.store(release)
 *    - Readers: flag.load(acquire), then read data
 * 
 * 2. Atomic Counter (concurrent increment):
 *    - Multiple readers: fetch_add(1, acq_rel)
 *    - Incrementer: load(acquire) to see all increments
 * 
 * 3. Spin-Wait (waiting for condition):
 *    - while (!condition.load(acquire)) sleep();
 *    - Better than busy-wait, worse than condition_variable for long waits
 * 
 * WHEN TO USE ATOMICS VS MUTEXES:
 * 
 * Use std::atomic when:
 * ✓ Operating on single variables
 * ✓ Need lock-free performance
 * ✓ Short wait times (microseconds to milliseconds)
 * ✓ Simple operations (increment, flag, etc.)
 * 
 * Use mutex/condition_variable when:
 * ✓ Multiple variables must be updated together
 * ✓ Long or unpredictable wait times
 * ✓ Complex conditions for waking threads
 * ✓ Want true blocking (no CPU usage while waiting)
 * 
 * PERFORMANCE CONSIDERATIONS:
 * - Atomics are usually lock-free on modern CPUs (check with is_lock_free())
 * - Spin-waiting trades CPU for latency
 * - Memory ordering: relaxed < acquire/release < acq_rel < seq_cst (speed)
 * - Choose weakest ordering that maintains correctness
 * 
 * SAFETY CONSIDERATIONS:
 * - Start with memory_order_seq_cst (safest, default)
 * - Optimize to weaker orderings only after profiling
 * - Document memory ordering choices clearly
 * - Test extensively on different architectures (x86 is forgiving!)
 * 
 * DEBUGGING TIPS:
 * - Check is_lock_free() to verify atomic is truly lock-free
 * - Use thread sanitizer (TSan) to detect races
 * - Test on ARM/weak-memory architectures (not just x86)
 * - Add assertions to verify invariants
 * 
 * FURTHER READING:
 * - "C++ Concurrency in Action" by Anthony Williams
 * - cppreference.com/w/cpp/atomic
 * - "Memory Ordering at Compile Time" by Jeff Preshing
 * - "Acquire and Release Semantics" by Jeff Preshing
 */
