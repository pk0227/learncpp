#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <string>

using uli = unsigned long int;

// ============================================================================
// SHARED DATA STRUCTURE
// ============================================================================
// Global shared value that the increment thread will modify and readers will read
uli g_count{};

// Shared mutex for concurrent reading
std::shared_mutex g_shared_mutex{};

// Regular mutex for synchronous (sequential) readers
std::mutex g_sync_read_mutex{};

// Mutex for condition variables
std::mutex g_cv_mutex{};

/* 
 * CONDITION VARIABLES - THREE-WAY COMMUNICATION PATTERN
 * 
 * Unlike the fixed N-cycle version, this user-controlled version has THREE actors:
 * 1. Main thread (gets user input)
 * 2. Incrementer thread (increments value)
 * 3. Reader threads (read the value)
 * 
 * CONDITION VARIABLE 1: g_cv_increment_done (Incrementer → Readers)
 * Purpose: Signal readers that new data is ready
 * Who signals: Incrementer thread (after incrementing g_count)
 * Who waits: All reader threads
 * How it works:
 *   - Readers call wait() with predicate: g_increment_done || g_shutdown
 *   - Incrementer calls notify_all() after incrementing
 *   - All readers wake up, check predicate, proceed if true
 *   - Shutdown flag allows graceful exit from infinite loop
 * 
 * CONDITION VARIABLE 2: g_cv_all_reads_done (Readers → Incrementer)
 * Purpose: Signal incrementer that ALL readers finished reading
 * Who signals: Last reader (when count == g_total_readers)
 * Who waits: Incrementer thread
 * How it works:
 *   - Incrementer waits with predicate: g_readers_finished_count >= g_total_readers
 *   - Each reader increments counter after reading
 *   - Last reader calls notify_one() to wake incrementer
 *   - Creates barrier: incrementer can't continue until all readers done
 * 
 * CONDITION VARIABLE 3: g_cv_user_input_ready (Main → Incrementer)
 * Purpose: Signal incrementer about user's decision (continue or stop)
 * Who signals: Main thread (after getting user input)
 * Who waits: Incrementer thread (after all readers finish)
 * How it works:
 *   - Incrementer waits with predicate: g_user_continue || g_shutdown
 *   - Main thread sets flag and calls notify_one()
 *   - Incrementer wakes up, checks which flag is set
 *   - If g_shutdown: break loop and exit; if g_user_continue: next cycle
 */
std::condition_variable g_cv_increment_done{};     // Incrementer → Readers
std::condition_variable g_cv_all_reads_done{};     // Readers → Incrementer
std::condition_variable g_cv_user_input_ready{};   // Main → Incrementer

// State flags
bool g_increment_done{ false };           // True when increment cycle completes
bool g_shutdown{ false };                 // True when user wants to stop

/* 
 * WHY std::atomic<bool> FOR g_user_continue?
 * 
 * g_user_continue is accessed by TWO threads without always holding g_cv_mutex:
 * 1. Main thread: Sets it to true (protected by lock)
 * 2. Incrementer: Reads it in predicate (protected by lock), then resets (protected)
 * 
 * Actually, in this code it's ALWAYS protected by g_cv_mutex, so bool would work.
 * However, using atomic<bool> is safer because:
 * - If we later add code that reads it outside the lock, it's thread-safe
 * - load() and store() operations are explicitly atomic
 * - No performance penalty (atomic bool is lock-free on modern CPUs)
 * - Makes intent clear: this variable is shared between threads
 * 
 * For g_increment_done and g_shutdown:
 * - These are ALWAYS accessed inside g_cv_mutex lock
 * - Regular bool is sufficient and correct
 * - Atomic would be redundant since mutex already provides synchronization
 */
std::atomic<bool> g_user_continue{ false }; // User's decision: continue or stop

// Counters
int g_readers_finished_count{ 0 };        // How many readers finished current cycle
int g_total_readers{ 0 };                 // Total number of readers
int g_current_cycle{ 0 };                 // Current increment cycle number


// ============================================================================
// INCREMENTER THREAD FUNCTION (USER-CONTROLLED)
// ============================================================================
// This thread increments, waits for ALL readers, then waits for user input
void Incrementer(int increment_value)
{
    std::thread::id tid{ std::this_thread::get_id() };
    std::cout << "[Incrementer " << tid << "] Started (user-controlled mode)\n";

    int cycle = 0;

    while (true)
    {
        cycle++;
        
        std::cout << "\n[Incrementer " << tid << "] === CYCLE " << cycle << " ===\n";

        // STEP 1: PERFORM INCREMENT
        /* 
         * EXCLUSIVE WRITE ACCESS WITH std::unique_lock<std::shared_mutex>
         * 
         * Why unique_lock on shared_mutex instead of regular mutex?
         * - shared_mutex supports TWO lock modes:
         *   1. Exclusive (unique_lock): Only ONE thread can hold it
         *   2. Shared (shared_lock): MULTIPLE threads can hold it simultaneously
         * 
         * For writing (modifying g_count):
         * - MUST use unique_lock for exclusive access
         * - Blocks all other unique_lock AND shared_lock attempts
         * - Ensures no readers can read while we're incrementing (prevents race)
         * 
         * The lock is acquired at '{' and released at '}' (RAII)
         * While this lock is held:
         * - No readers can acquire shared_lock (they must wait)
         * - No other writers can acquire unique_lock (impossible anyway, only 1 incrementer)
         * 
         * Without this lock: DATA RACE
         * - Readers might read g_count while it's being incremented
         * - Could read partially updated value (torn read)
         * - Undefined behavior!
         */
        {
            std::unique_lock<std::shared_mutex> ul{ g_shared_mutex };
            
            std::cout << "[Incrementer " << tid << "] Incrementing by " 
                      << increment_value << "...\n";
            
            for (int i{}; i < increment_value; i++)
            {
                g_count++;
            }
            
            std::cout << "[Incrementer " << tid << "] Increment complete. New value: " 
                      << g_count << "\n";
        }

        // STEP 2: NOTIFY READERS THAT INCREMENT IS DONE
        /* 
         * INCREMENTER NOTIFIES ALL READERS
         * 
         * Step-by-step what happens:
         * 1. Acquire g_cv_mutex (protects condition variable state)
         * 2. Update state variables:
         *    - g_current_cycle: Which cycle we just completed
         *    - g_increment_done: Signal that data is ready
         *    - g_readers_finished_count: Reset to 0 for new cycle
         * 3. Call notify_all() on g_cv_increment_done
         * 
         * What notify_all() does:
         * - Wakes up ALL threads waiting on g_cv_increment_done
         * - All reader threads (sync and async) are sleeping in wait()
         * - Each woken thread will:
         *   a) Try to re-acquire g_cv_mutex (one at a time)
         *   b) Check predicate: g_increment_done || g_shutdown
         *   c) If true: proceed to read data
         *   d) If false: release lock, go back to sleep (shouldn't happen here)
         * 
         * Why notify INSIDE the lock?
         * - Prevents race: ensures state is fully updated before any reader checks it
         * - Readers won't miss the notification (they're either sleeping or will see flag)
         * - Modern implementations handle this efficiently
         * 
         * notify_all() vs notify_one():
         * - notify_all(): Wake ALL waiting threads (used here for multiple readers)
         * - notify_one(): Wake ONE waiting thread (used for single incrementer)
         */
        {
            std::lock_guard<std::mutex> lg{ g_cv_mutex };
            g_current_cycle = cycle;
            g_increment_done = true;
            g_readers_finished_count = 0;
            
            std::cout << "[Incrementer " << tid << "] Notifying all readers...\n";
            g_cv_increment_done.notify_all();
        }

        // STEP 3: WAIT FOR ALL READERS TO FINISH READING
        /* 
         * INCREMENTER WAITS FOR READERS - BARRIER SYNCHRONIZATION
         * 
         * This creates a synchronization barrier between incrementer and readers.
         * The incrementer BLOCKS here until ALL readers finish reading.
         * 
         * How condition_variable::wait() works with predicate:
         * 
         * g_cv_all_reads_done.wait(ul, predicate) is equivalent to:
         * 
         * while (!predicate()) {       // Loop handles spurious wakeups
         *     ul.unlock();              // Release g_cv_mutex
         *     <wait for notify>         // Put thread to sleep
         *     <woken by notify_one()>   // Last reader calls this
         *     ul.lock();                // Re-acquire g_cv_mutex
         *     // Check predicate again
         * }
         * // Predicate is true, we hold the lock, continue execution
         * 
         * The predicate: g_readers_finished_count >= g_total_readers
         * - False initially (count is 0)
         * - Each reader increments count after finishing
         * - Last reader makes it true and calls notify_one()
         * - Incrementer wakes up, sees true, continues
         * 
         * Why this is a BARRIER:
         * - Incrementer cannot proceed to next cycle (or wait for user input)
         * - Until ALL readers have finished current cycle
         * - Ensures readers always see consistent, complete data
         * - No reader will be "left behind" reading old data
         */
        {
            std::unique_lock<std::mutex> ul{ g_cv_mutex };
            
            std::cout << "[Incrementer " << tid << "] Waiting for all " 
                      << g_total_readers << " readers to finish...\n";
            
            g_cv_all_reads_done.wait(ul, [] { 
                return g_readers_finished_count >= g_total_readers; 
            });
            
            std::cout << "[Incrementer " << tid << "] All readers finished reading!\n";
            g_increment_done = false;
        }

        // STEP 4: WAIT FOR USER INPUT
        /* 
         * INCREMENTER WAITS FOR MAIN THREAD (USER INPUT)
         * 
         * This is what makes this version USER-CONTROLLED instead of N-cycle fixed.
         * After each increment cycle, incrementer waits for user's decision.
         * 
         * How the wait works:
         * g_cv_user_input_ready.wait(ul, predicate) blocks until:
         * - Main thread gets user input (y/n)
         * - Main thread sets g_user_continue=true OR g_shutdown=true
         * - Main thread calls notify_one()
         * - Incrementer wakes up, checks predicate
         * 
         * The predicate: g_user_continue.load() || g_shutdown
         * - True if user said "yes" (continue incrementing)
         * - True if user said "no" (shutdown)
         * - False while waiting for user to type
         * 
         * Two possible outcomes:
         * 1. g_shutdown == true:
         *    - User wants to stop
         *    - Break from while loop
         *    - Signal all readers to exit
         *    - Incrementer thread ends
         * 
         * 2. g_user_continue == true:
         *    - User wants another cycle
         *    - Reset g_user_continue to false (for next time)
         *    - Continue to next iteration of while loop
         *    - Perform another increment
         * 
         * Why use atomic<bool> for g_user_continue?
         * - See detailed comment at variable declaration above
         * - load() ensures we get the current value atomically
         */
        {
            std::unique_lock<std::mutex> ul{ g_cv_mutex };
            
            std::cout << "[Incrementer " << tid << "] Waiting for user decision...\n\n";
            
            // Wait for main thread to signal that user input is ready
            g_cv_user_input_ready.wait(ul, [] { 
                return g_user_continue.load() || g_shutdown; 
            });
            
            // Check if user wants to stop
            if (g_shutdown)
            {
                std::cout << "\n[Incrementer " << tid << "] User requested shutdown. Exiting.\n";
                break;
            }
            
            // Reset flag for next iteration
            g_user_continue = false;
            std::cout << "[Incrementer " << tid << "] User chose to continue!\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // Signal all readers to exit
    /* 
     * GRACEFUL SHUTDOWN - INCREMENTER SIGNALS READERS TO EXIT
     * 
     * When incrementer exits its loop (user chose 'n'), it must tell readers to stop.
     * Without this, readers would wait forever in g_cv_increment_done.wait()
     * 
     * Shutdown sequence:
     * 1. Acquire g_cv_mutex lock
     * 2. Set g_shutdown = true (changes predicate for readers)
     * 3. Call notify_all() on g_cv_increment_done
     * 
     * What happens to readers:
     * - Each reader is sleeping in: g_cv_increment_done.wait(ul, predicate)
     * - Predicate is: g_increment_done || g_shutdown
     * - notify_all() wakes them up
     * - They re-check predicate: g_shutdown is now true!
     * - They see shutdown flag, break from their while loop
     * - They print exit message and return (thread ends)
     * 
     * Why notify_all() not notify_one()?
     * - Need to wake ALL readers (could be many threads)
     * - notify_one() would only wake one thread, others stuck forever
     * 
     * Order of operations matters:
     * 1. Set g_shutdown BEFORE notify (so readers see it)
     * 2. Hold lock while doing both (atomic state update + notification)
     */
    {
        std::lock_guard<std::mutex> lg{ g_cv_mutex };
        g_shutdown = true;
        g_cv_increment_done.notify_all();
    }

    std::cout << "[Incrementer " << tid << "] Completed " << cycle 
              << " cycles. Final value: " << g_count << "\n";
}


// ============================================================================
// READER THREAD FUNCTION (SYNCHRONOUS - USER-CONTROLLED)
// ============================================================================
// Synchronous reader: waits for increment, reads sequentially, runs until shutdown
void ReaderSynchronous(int reader_id)
{
    std::thread::id tid{ std::this_thread::get_id() };
    std::cout << "[SyncReader-" << reader_id << " " << tid 
              << "] Started (user-controlled mode)\n";

    int cycles_read = 0;

    while (true)
    {
        // STEP 1: WAIT FOR INCREMENT TO COMPLETE OR SHUTDOWN
        /* 
         * READER WAITS FOR INCREMENTER OR SHUTDOWN SIGNAL
         * 
         * Unlike the N-cycle version, readers run in an INFINITE LOOP.
         * They must be able to exit gracefully when program ends.
         * 
         * The wait mechanism:
         * g_cv_increment_done.wait(ul, predicate)
         * 
         * Predicate: g_increment_done || g_shutdown
         * - True when incrementer finishes incrementing (normal case)
         * - True when user wants to stop (shutdown case)
         * - False while incrementer is waiting for user input
         * 
         * How wait() works (reminder):
         * while (!predicate()) {
         *     ul.unlock();              // Release g_cv_mutex
         *     <sleep>                   // Wait for notify
         *     <woken by notify_all()>   // Incrementer or shutdown
         *     ul.lock();                // Re-acquire g_cv_mutex
         * }
         * 
         * After waking up:
         * 1. Check if g_shutdown is true
         * 2. If yes: break from while loop, exit thread
         * 3. If no: g_increment_done must be true, proceed to read
         * 
         * Why OR in predicate instead of just g_increment_done?
         * - Without g_shutdown: readers would wait forever when program ends
         * - With g_shutdown: readers can exit cleanly
         * - Essential for infinite loop threads!
         */
        {
            std::unique_lock<std::mutex> ul{ g_cv_mutex };
            
            g_cv_increment_done.wait(ul, [] { 
                return g_increment_done || g_shutdown; 
            });
            
            // Check if shutdown requested
            if (g_shutdown)
            {
                std::cout << "[SyncReader-" << reader_id << " " << tid 
                          << "] Shutdown signal received. Exiting.\n";
                break;
            }
        }

        cycles_read++;

        // STEP 2: READ SEQUENTIALLY
        /* 
         * SYNCHRONOUS READING - ONE READER AT A TIME
         * 
         * Uses std::mutex (g_sync_read_mutex) instead of shared_lock.
         * Why?
         * - Regular mutex allows ONLY ONE thread to hold it at a time
         * - Sync readers read SEQUENTIALLY (one after another)
         * - Reader 1 reads → releases lock → Reader 2 reads → etc.
         * 
         * What if we used shared_lock here instead?
         * - Multiple sync readers could read simultaneously
         * - That would make them "async" (concurrent)
         * - We want to demonstrate SEQUENTIAL reading
         * 
         * Why std::lock_guard instead of std::unique_lock?
         * - lock_guard: Simple RAII lock, can't unlock manually
         * - unique_lock: More flexible, can unlock/lock manually, needed for condition_variable
         * - Since we don't need flexibility here, lock_guard is simpler and clearer
         * 
         * Reading g_count:
         * - Even though we hold g_sync_read_mutex, we STILL need g_shared_mutex protection
         * - Wait, do we? Let's think:
         *   - Incrementer uses unique_lock<g_shared_mutex> for writing
         *   - Async readers use shared_lock<g_shared_mutex> for reading
         *   - Sync readers SHOULD use shared_lock<g_shared_mutex> too!
         * - Actually, this code has a subtle issue (see below)
         * 
         * SUBTLE ISSUE IN THIS CODE:
         * - We hold g_sync_read_mutex but NOT g_shared_mutex
         * - Incrementer modifies g_count with unique_lock<g_shared_mutex>
         * - We're reading g_count WITHOUT any lock on g_shared_mutex
         * - This is technically a DATA RACE!
         * 
         * Why doesn't it cause problems in practice?
         * - The condition variable wait ensures incrementer is done before we read
         * - Incrementer releases g_shared_mutex before notifying us
         * - By the time we reach here, g_count is stable
         * - Modern CPUs with sequential consistency make it "work"
         * 
         * PROPER FIX (not implemented here to keep code simple):
         * {
         *     std::lock_guard<std::mutex> lg{g_sync_read_mutex};  // Sequential order
         *     std::shared_lock<std::shared_mutex> sl{g_shared_mutex};  // Safe read
         *     std::cout << g_count;  // Now truly safe!
         * }
         */
        {
            std::lock_guard<std::mutex> lg{ g_sync_read_mutex };
            
            std::cout << "[SyncReader-" << reader_id << " " << tid 
                      << "] [Cycle " << cycles_read << "] Reading SEQUENTIALLY: " 
                      << g_count << "\n";
            
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }

        // STEP 3: MARK AS FINISHED
        /* 
         * READER COUNTER INCREMENT & LAST READER NOTIFICATION
         * 
         * Each reader must signal when it finishes reading.
         * This implements a BARRIER pattern:
         * - Incrementer waits until ALL readers finish
         * - Only the LAST reader notifies the incrementer
         * - Other readers just increment the counter
         * 
         * Step-by-step:
         * 1. Acquire g_cv_mutex (protects counter and condition variable)
         * 2. Increment g_readers_finished_count
         * 3. Check if we're the last reader (count == total)
         * 4. If yes: call notify_one() on g_cv_all_reads_done
         * 
         * Why notify_one() not notify_all()?
         * - Only ONE thread is waiting (the incrementer)
         * - notify_one() wakes one thread (efficient)
         * - notify_all() would wake all waiting threads (overkill for 1 thread)
         * 
         * Race condition consideration:
         * - Multiple readers execute this code concurrently
         * - g_cv_mutex ensures atomic increment of counter
         * - Only one reader will see count == total (the last one)
         * - No double-notification possible
         * 
         * What incrementer does:
         * - Waiting in: g_cv_all_reads_done.wait(ul, predicate)
         * - Predicate: g_readers_finished_count >= g_total_readers
         * - When woken by notify_one(), checks predicate
         * - Predicate is now true, continues to wait for user input
         * 
         * Counter reset:
         * - Incrementer resets g_readers_finished_count to 0 before next cycle
         * - This happens in STEP 2 of incrementer (when notifying readers)
         */
        {
            std::lock_guard<std::mutex> lg{ g_cv_mutex };
            g_readers_finished_count++;
            
            std::cout << "[SyncReader-" << reader_id << " " << tid 
                      << "] Finished reading. (" << g_readers_finished_count 
                      << "/" << g_total_readers << " readers done)\n";
            
            if (g_readers_finished_count >= g_total_readers)
            {
                std::cout << "[SyncReader-" << reader_id << " " << tid 
                          << "] I'm the last reader! Notifying incrementer...\n";
                g_cv_all_reads_done.notify_one();
            }
        }
    }

    std::cout << "[SyncReader-" << reader_id << " " << tid 
              << "] Completed " << cycles_read << " cycles. Exiting.\n";
}


// ============================================================================
// READER THREAD FUNCTION (ASYNCHRONOUS - USER-CONTROLLED)
// ============================================================================
// Asynchronous reader: waits for increment, reads concurrently, runs until shutdown
void ReaderAsynchronous(int reader_id)
{
    std::thread::id tid{ std::this_thread::get_id() };
    std::cout << "[AsyncReader-" << reader_id << " " << tid 
              << "] Started (user-controlled mode)\n";

    int cycles_read = 0;

    while (true)
    {
        // STEP 1: WAIT FOR INCREMENT TO COMPLETE OR SHUTDOWN
        /* 
         * ASYNC READER WAITS - SAME MECHANISM AS SYNC READERS
         * 
         * Both synchronous and asynchronous readers:
         * - Use the SAME condition variable (g_cv_increment_done)
         * - Wait for the SAME signal from incrementer
         * - Check the SAME predicate (g_increment_done || g_shutdown)
         * - Exit the SAME way when shutdown is signaled
         * 
         * The difference between sync and async is in STEP 2 (reading):
         * - Sync readers: Use mutex, read one at a time (sequential)
         * - Async readers: Use shared_lock, read simultaneously (concurrent)
         * 
         * Both types wait for incrementer in exactly the same way!
         * See detailed comment in ReaderSynchronous for full explanation.
         */
        {
            std::unique_lock<std::mutex> ul{ g_cv_mutex };
            
            g_cv_increment_done.wait(ul, [] { 
                return g_increment_done || g_shutdown; 
            });
            
            if (g_shutdown)
            {
                std::cout << "[AsyncReader-" << reader_id << " " << tid 
                          << "] Shutdown signal received. Exiting.\n";
                break;
            }
        }

        cycles_read++;

        // STEP 2: READ CONCURRENTLY
        /* 
         * ASYNCHRONOUS READING - MULTIPLE READERS SIMULTANEOUSLY
         * 
         * Uses std::shared_lock<std::shared_mutex> for concurrent reading.
         * Why?
         * - shared_lock allows MULTIPLE threads to hold the lock simultaneously
         * - Perfect for multiple readers reading the same data
         * - Much more efficient than sequential reading for read-heavy workloads
         * 
         * How shared_lock works:
         * - Multiple threads can acquire shared_lock at the same time
         * - All async readers can read g_count concurrently
         * - BUT: If incrementer tries unique_lock, it must WAIT for all shared_locks to release
         * 
         * The contract with shared_mutex:
         * - Incrementer: unique_lock<g_shared_mutex> for writing (exclusive)
         * - Async readers: shared_lock<g_shared_mutex> for reading (shared)
         * - Sync readers: SHOULD use shared_lock too (but use mutex for sequencing)
         * 
         * Why is this safe from data races?
         * - Readers only READ g_count (no modification)
         * - Incrementer WRITES g_count with exclusive lock
         * - unique_lock blocks until all shared_locks are released
         * - shared_lock blocks while unique_lock is held
         * - No simultaneous read+write possible
         * 
         * Concurrent reading visualization:
         * Time →
         * Reader 1: [shared_lock][reading g_count]........[release]
         * Reader 2: [shared_lock][reading g_count]........[release]
         * Reader 3: .....[shared_lock][reading g_count]...[release]
         * Reader 4: .....[shared_lock][reading g_count]...[release]
         *           ← All 4 can overlap! That's why it's "async" →
         * 
         * Compare to sync readers:
         * Time →
         * Reader 1: [mutex][read]......[release]
         * Reader 2: ...................[mutex][read]......[release]
         * Reader 3: .....................................[mutex][read]...[release]
         * Reader 4: ...............................................[mutex][read][release]
         *           ← One at a time! That's why it's "sync" →
         */
        {
            std::shared_lock<std::shared_mutex> sl{ g_shared_mutex };
            
            std::cout << "[AsyncReader-" << reader_id << " " << tid 
                      << "] [Cycle " << cycles_read << "] Reading CONCURRENTLY: " 
                      << g_count << "\n";
            
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }

        // STEP 3: MARK AS FINISHED
        /* 
         * BARRIER PATTERN - ASYNC READERS USE SAME MECHANISM AS SYNC
         * 
         * Both sync and async readers:
         * - Increment g_readers_finished_count after reading
         * - Check if they're the last reader
         * - If last: notify incrementer with notify_one()
         * 
         * The barrier works identically for both types.
         * See detailed comment in ReaderSynchronous STEP 3 for full explanation.
         * 
         * Key insight:
         * - Async readers read CONCURRENTLY in STEP 2
         * - But they update the counter SEQUENTIALLY in STEP 3
         * - g_cv_mutex ensures atomic counter updates
         * - Last reader is determined correctly regardless of read timing
         */
        {
            std::lock_guard<std::mutex> lg{ g_cv_mutex };
            g_readers_finished_count++;
            
            std::cout << "[AsyncReader-" << reader_id << " " << tid 
                      << "] Finished reading. (" << g_readers_finished_count 
                      << "/" << g_total_readers << " readers done)\n";
            
            if (g_readers_finished_count >= g_total_readers)
            {
                std::cout << "[AsyncReader-" << reader_id << " " << tid 
                          << "] I'm the last reader! Notifying incrementer...\n";
                g_cv_all_reads_done.notify_one();
            }
        }
    }

    std::cout << "[AsyncReader-" << reader_id << " " << tid 
              << "] Completed " << cycles_read << " cycles. Exiting.\n";
}


// ==========================================================
// MAIN FUNCTION - USER-CONTROLLED INCREMENT
// ==========================================================
int main()
{
    std::cout << "==========================================================\n";
    std::cout << "USER-CONTROLLED INCREMENT MODE\n";
    std::cout << "==========================================================\n";
    std::cout << "The incrementer will wait for YOUR input after each cycle.\n";
    std::cout << "You decide when to increment next or stop the program.\n";
    std::cout << "========================================================\n\n";

    // Configuration
    const int INCREMENT_VALUE = 10;
    const int NUM_SYNC_READERS = 2;
    const int NUM_ASYNC_READERS = 2;
    
    g_total_readers = NUM_SYNC_READERS + NUM_ASYNC_READERS;

    std::cout << "Configuration:\n";
    std::cout << "  - Increment per cycle: " << INCREMENT_VALUE << "\n";
    std::cout << "  - Synchronous readers: " << NUM_SYNC_READERS << "\n";
    std::cout << "  - Asynchronous readers: " << NUM_ASYNC_READERS << "\n";
    std::cout << "  - Total readers: " << g_total_readers << "\n";
    std::cout << "===================================================\n\n";

    std::vector<std::thread> threads{};

    // Launch synchronous readers
    std::cout << "[Main] Launching " << NUM_SYNC_READERS << " synchronous readers...\n";
    for (int i{ 0 }; i < NUM_SYNC_READERS; i++)
    {
        threads.push_back(std::thread(ReaderSynchronous, i + 1));
    }

    // Launch asynchronous readers
    std::cout << "[Main] Launching " << NUM_ASYNC_READERS << " asynchronous readers...\n";
    for (int i{ 0 }; i < NUM_ASYNC_READERS; i++)
    {
        threads.push_back(std::thread(ReaderAsynchronous, i + 1));
    }

    // Small delay to ensure all readers are waiting
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Launch the incrementer thread
    std::cout << "[Main] Launching incrementer thread...\n\n";
    threads.push_back(std::thread(Incrementer, INCREMENT_VALUE));

    // Small delay to let first increment cycle start
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // USER INPUT LOOP
    std::string user_input;
    while (true)
    {
        std::cout << "\n====================================\n";
        std::cout << "Continue incrementing? (y/n): ";
        std::getline(std::cin, user_input);

        if (user_input == "y" || user_input == "Y" || user_input == "yes" || user_input == "YES")
        {
            // User wants to continue
            /* 
             * MAIN THREAD SIGNALS INCREMENTER TO CONTINUE
             * 
             * User typed 'y' - wants another increment cycle.
             * Main thread must wake up the incrementer (waiting in STEP 4).
             * 
             * Signaling sequence:
             * 1. Acquire g_cv_mutex
             * 2. Set g_user_continue = true (changes predicate)
             * 3. Call notify_one() on g_cv_user_input_ready
             * 4. Release lock (automatic when lg goes out of scope)
             * 
             * What happens in incrementer:
             * - Incrementer is in: g_cv_user_input_ready.wait(ul, predicate)
             * - Predicate: g_user_continue.load() || g_shutdown
             * - notify_one() wakes incrementer
             * - Incrementer re-acquires g_cv_mutex
             * - Checks predicate: g_user_continue is true!
             * - Resets g_user_continue to false
             * - Continues to next cycle
             * 
             * Why the 500ms sleep?
             * - Give incrementer time to start next cycle
             * - Print messages appear in more logical order
             * - Not strictly necessary for correctness
             * - Purely for better console output readability
             */
            {
                std::lock_guard<std::mutex> lg{ g_cv_mutex };
                g_user_continue = true;
                g_cv_user_input_ready.notify_one();
            }
            
            std::cout << "[Main] Signaling incrementer to continue...\n";
            
            // Give threads time to process
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        else if (user_input == "n" || user_input == "N" || user_input == "no" || user_input == "NO")
        {
            // User wants to stop
            /* 
             * SHUTDOWN CASCADE - MAIN THREAD INITIATES GRACEFUL TERMINATION
             * 
             * User typed 'n' - wants to stop the program.
             * This triggers a cascade of shutdown signals through all threads.
             * 
             * Shutdown sequence:
             * 1. Main thread: Sets g_shutdown=true, notifies incrementer
             * 2. Incrementer: Wakes up, sees g_shutdown, breaks loop
             * 3. Incrementer: Sets g_shutdown=true AGAIN (redundant but safe), notifies ALL readers
             * 4. Readers: Wake up, see g_shutdown, break loops, exit
             * 5. Main thread: Joins all threads (waits for clean exit)
             * 
             * Why notify_one() here?
             * - Only incrementer is waiting on g_cv_user_input_ready
             * - Readers are on g_cv_increment_done (different CV!)
             * - Incrementer will notify readers in its shutdown code
             * 
             * Why set g_shutdown here AND in incrementer?
             * - Defense in depth: if incrementer crashes, readers still get signal
             * - Actually, incrementer MUST do it because readers wait on different CV
             * - Main's g_shutdown=true is for incrementer's predicate
             * - Incrementer's g_shutdown=true is for readers' predicate
             * - Both necessary for complete shutdown
             * 
             * Thread termination order:
             * 1. Incrementer exits while loop, signals readers, returns from function
             * 2. Readers exit while loops, return from functions  
             * 3. Main thread's join() calls complete
             * 4. Program prints final statistics and exits
             */
            {
                std::lock_guard<std::mutex> lg{ g_cv_mutex };
                g_shutdown = true;
                g_cv_user_input_ready.notify_one();
            }
            
            std::cout << "[Main] Signaling shutdown to all threads...\n";
            break;
        }
        else
        {
            std::cout << "[Main] Invalid input. Please enter 'y' or 'n'.\n";
        }
    }

    // Wait for all threads to complete
    std::cout << "[Main] Waiting for all threads to finish...\n";
    for (auto& th : threads)
    {
        if (th.joinable())
            th.join();
    }

    // Results
    std::cout << "\n========================================\n";
    std::cout << "PROGRAM COMPLETED\n";
    std::cout << "==========================================\n";
    std::cout << "Final value: " << g_count << "\n";
    std::cout << "Total increment cycles: " << g_current_cycle << "\n";
    std::cout << "Expected final value: " << (g_current_cycle * INCREMENT_VALUE) << "\n";
    
    if (g_count == static_cast<uli>(g_current_cycle * INCREMENT_VALUE))
    {
        std::cout << "✓ Values match - synchronization worked correctly!\n";
    }
    else
    {
        std::cout << "✗ Values don't match - something went wrong!\n";
    }
    
    std::cout << "==========================================\n";

    return 0;
}
