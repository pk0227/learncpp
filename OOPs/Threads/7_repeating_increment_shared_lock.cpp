#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <chrono>

using uli = unsigned long int;

// ============================================================================
// SHARED DATA STRUCTURE
// ============================================================================
// Global shared value that the increment thread will modify and readers will read
uli g_count{};

// Shared mutex allows multiple readers to read simultaneously (shared_lock)
// but only one writer to write exclusively (unique_lock)
std::shared_mutex g_shared_mutex{};

// Regular mutex for synchronous (sequential) readers
std::mutex g_sync_read_mutex{};

// Mutex for condition variables
std::mutex g_cv_mutex{};

/* 
 * CONDITION VARIABLE 1: Incrementer → Readers communication
 * 
 * Purpose: Signal readers that new data is ready to be read
 * Who signals: Incrementer thread (after incrementing g_count)
 * Who waits: All reader threads (sync and async)
 * 
 * How it works:
 * 1. Readers call wait() - this atomically:
 *    a) Releases the mutex (g_cv_mutex)
 *    b) Puts the thread to sleep
 * 2. Incrementer calls notify_all() - this:
 *    a) Wakes up ALL sleeping reader threads
 * 3. Each woken reader:
 *    a) Re-acquires the mutex (g_cv_mutex)
 *    b) Checks the predicate (is g_increment_done true AND is it my cycle?)
 *    c) If predicate is false: release mutex, go back to sleep
 *    d) If predicate is true: keep the mutex, continue execution
 */
std::condition_variable g_cv_increment_done{};

/* 
 * CONDITION VARIABLE 2: Readers → Incrementer communication
 * 
 * Purpose: Signal incrementer that ALL readers have finished reading
 * Who signals: The LAST reader to finish (when count reaches g_total_readers)
 * Who waits: Incrementer thread (after notifying readers)
 * 
 * How it works:
 * 1. Incrementer calls wait() with predicate checking reader count
 * 2. Each reader increments g_readers_finished_count after reading
 * 3. Last reader (count == g_total_readers) calls notify_one()
 * 4. Incrementer wakes up, checks predicate, and continues to next cycle
 * 
 * This creates a BARRIER - incrementer waits for ALL readers before proceeding
 */
std::condition_variable g_cv_all_reads_done{};

// Flags and counters
bool g_increment_done{ false };           // True when incrementer finishes a cycle
int g_readers_finished_count{ 0 };        // How many readers finished reading current cycle
int g_total_readers{ 0 };                 // Total number of readers
int g_current_cycle{ 0 };                 // Current increment cycle number


// ============================================================================
// INCREMENTER THREAD FUNCTION (N TIMES REPEATING WITH BARRIER)
// ============================================================================
// This thread increments, waits for ALL readers to finish, then repeats N times
void Incrementer(int increment_value, int num_cycles)
{
    std::thread::id tid{ std::this_thread::get_id() };
    std::cout << "[Incrementer " << tid << "] Started, will perform " 
              << num_cycles << " increment cycles\n";

    for (int cycle = 1; cycle <= num_cycles; ++cycle)
    {
        std::cout << "\n[Incrementer " << tid << "] === CYCLE " << cycle 
                  << " of " << num_cycles << " ===\n";

        // Simulate some work before incrementing
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // STEP 1: PERFORM INCREMENT
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
        {
            std::lock_guard<std::mutex> lg{ g_cv_mutex };
            g_current_cycle = cycle;
            g_increment_done = true;
            g_readers_finished_count = 0; // Reset counter for this cycle
            
            std::cout << "[Incrementer " << tid << "] Notifying all readers...\n";
            
            /* 
             * notify_all() wakes up ALL waiting threads
             * 
             * What happens:
             * 1. All readers are sleeping in g_cv_increment_done.wait()
             * 2. notify_all() is called
             * 3. ALL reader threads wake up (spurious wakeup protection active)
             * 4. Each reader tries to re-acquire g_cv_mutex
             * 5. One-by-one, they acquire the lock, check predicate
             * 6. If predicate true (g_increment_done && correct cycle), proceed
             * 7. If predicate false, release lock and go back to sleep
             * 
             * Note: notify_all() vs notify_one()
             * - notify_all(): Wake ALL waiting threads (used here for readers)
             * - notify_one(): Wake ONE waiting thread (used for incrementer)
             */
            g_cv_increment_done.notify_all();
        }

        // STEP 3: WAIT FOR ALL READERS TO FINISH READING
        {
            /* 
             * INCREMENTER WAITS FOR READERS (BARRIER SYNCHRONIZATION)
             * 
             * The incrementer BLOCKS here until ALL readers finish reading
             * This creates a synchronization barrier between cycles
             * 
             * wait() with predicate works the same way as in readers:
             * while (!predicate()) {
             *     ul.unlock();        // Release mutex
             *     <sleep>             // Wait for notify
             *     <woken by last reader's notify_one()>
             *     ul.lock();          // Re-acquire mutex
             * }
             * 
             * Predicate: g_readers_finished_count >= g_total_readers
             * - True when ALL readers have finished reading
             * - Last reader increments count to g_total_readers
             * - Last reader calls notify_one() to wake this incrementer
             */
            std::unique_lock<std::mutex> ul{ g_cv_mutex };
            
            std::cout << "[Incrementer " << tid << "] Waiting for all " 
                      << g_total_readers << " readers to finish...\n";
            
            g_cv_all_reads_done.wait(ul, [] { 
                return g_readers_finished_count >= g_total_readers; 
            });
            
            // When we reach here, all readers have finished
            std::cout << "[Incrementer " << tid << "] All readers finished reading! "
                      << "Moving to next cycle...\n";
            
            // Reset flag for next cycle
            g_increment_done = false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "\n[Incrementer " << tid << "] All " << num_cycles 
              << " cycles completed! Exiting.\n";
}


// ============================================================================
// READER THREAD FUNCTION (SYNCHRONOUS - READS ALL N CYCLES)
// ============================================================================
// Synchronous reader: waits for increment, reads sequentially, notifies when done
void ReaderSynchronous(int reader_id, int expected_cycles)
{
    std::thread::id tid{ std::this_thread::get_id() };
    std::cout << "[SyncReader-" << reader_id << " " << tid 
              << "] Started, will read all " << expected_cycles << " cycles\n";

    for (int cycle = 1; cycle <= expected_cycles; ++cycle)
    {
        // STEP 1: WAIT FOR INCREMENT TO COMPLETE
        {
            /* 
             * CONDITION VARIABLE WAIT MECHANISM:
             * 
             * unique_lock<mutex> ul{g_cv_mutex} - Lock the mutex first
             * 
             * g_cv_increment_done.wait(ul, predicate) does:
             * 
             * while (!predicate()) {  // Loop to handle spurious wakeups
             *     ul.unlock();        // Release the mutex
             *     <sleep/wait>        // Put thread to sleep (waiting for notify)
             *     <woken up>          // notify_all() was called by incrementer
             *     ul.lock();          // Re-acquire the mutex
             * }
             * // Predicate is true, mutex is locked, continue execution
             * 
             * The predicate: [cycle] { return g_increment_done && g_current_cycle == cycle; }
             * - Checks if increment is done for THIS specific cycle
             * - Prevents reading stale data from previous cycles
             * - Protects against spurious wakeups (random wakeups without notify)
             */
            std::unique_lock<std::mutex> ul{ g_cv_mutex };
            
            g_cv_increment_done.wait(ul, [cycle] { 
                return g_increment_done && g_current_cycle == cycle; 
            });
            
            // When we reach here:
            // - g_increment_done is true
            // - g_current_cycle == cycle (our cycle)
            // - We hold the lock on g_cv_mutex
            // - Lock is automatically released when ul goes out of scope
        }

        // STEP 2: READ SEQUENTIALLY (ONE AT A TIME)
        {
            std::lock_guard<std::mutex> lg{ g_sync_read_mutex };
            
            std::cout << "[SyncReader-" << reader_id << " " << tid 
                      << "] [Cycle " << cycle << "] Reading SEQUENTIALLY: " 
                      << g_count << "\n";
            
            // Simulate reading work
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        // STEP 3: MARK THIS READER AS FINISHED FOR THIS CYCLE
        {
            /* 
             * READER COUNTER & LAST READER NOTIFICATION:
             * 
             * Each reader increments g_readers_finished_count atomically
             * (protected by g_cv_mutex lock)
             * 
             * When count reaches g_total_readers, the LAST reader calls
             * notify_one() to wake up the incrementer who is waiting in
             * g_cv_all_reads_done.wait()
             * 
             * This implements a BARRIER pattern:
             * - All readers must finish reading before incrementer continues
             * - Incrementer cannot proceed to next cycle until all readers done
             * - This ensures readers always see consistent data
             */
            std::lock_guard<std::mutex> lg{ g_cv_mutex };
            g_readers_finished_count++;
            
            std::cout << "[SyncReader-" << reader_id << " " << tid 
                      << "] Finished reading. (" << g_readers_finished_count 
                      << "/" << g_total_readers << " readers done)\n";
            
            // If this is the last reader, notify the incrementer
            if (g_readers_finished_count >= g_total_readers)
            {
                std::cout << "[SyncReader-" << reader_id << " " << tid 
                          << "] I'm the last reader! Notifying incrementer...\n";
                
                /* 
                 * notify_one() wakes up ONE waiting thread (the incrementer)
                 * 
                 * What happens:
                 * 1. Incrementer is sleeping in g_cv_all_reads_done.wait()
                 * 2. notify_one() is called
                 * 3. Incrementer wakes up, tries to re-acquire g_cv_mutex
                 * 4. When it gets the lock, checks predicate (count >= total)
                 * 5. Predicate is true, so continues execution (next cycle)
                 */
                g_cv_all_reads_done.notify_one();
            }
        }
    }

    std::cout << "[SyncReader-" << reader_id << " " << tid 
              << "] Completed all " << expected_cycles << " cycles. Exiting.\n";
}


// ============================================================================
// READER THREAD FUNCTION (ASYNCHRONOUS - READS ALL N CYCLES)
// ============================================================================
// Asynchronous reader: waits for increment, reads concurrently, notifies when done
void ReaderAsynchronous(int reader_id, int expected_cycles)
{
    std::thread::id tid{ std::this_thread::get_id() };
    std::cout << "[AsyncReader-" << reader_id << " " << tid 
              << "] Started, will read all " << expected_cycles << " cycles\n";

    for (int cycle = 1; cycle <= expected_cycles; ++cycle)
    {
        // STEP 1: WAIT FOR INCREMENT TO COMPLETE
        {
            /* 
             * Same wait mechanism as synchronous readers
             * Both sync and async readers use the SAME condition variable
             * and wait for the SAME signal from incrementer
             * 
             * The difference is in STEP 2 (reading) - not in waiting
             */
            std::unique_lock<std::mutex> ul{ g_cv_mutex };
            
            g_cv_increment_done.wait(ul, [cycle] { 
                return g_increment_done && g_current_cycle == cycle; 
            });
        }

        // STEP 2: READ CONCURRENTLY (MULTIPLE READERS CAN READ SIMULTANEOUSLY)
        {
            std::shared_lock<std::shared_mutex> sl{ g_shared_mutex };
            
            std::cout << "[AsyncReader-" << reader_id << " " << tid 
                      << "] [Cycle " << cycle << "] Reading CONCURRENTLY: " 
                      << g_count << "\n";
            
            // Simulate reading work
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        // STEP 3: MARK THIS READER AS FINISHED FOR THIS CYCLE
        {
            /* Same notification mechanism as synchronous readers */
            std::lock_guard<std::mutex> lg{ g_cv_mutex };
            g_readers_finished_count++;
            
            std::cout << "[AsyncReader-" << reader_id << " " << tid 
                      << "] Finished reading. (" << g_readers_finished_count 
                      << "/" << g_total_readers << " readers done)\n";
            
            // If this is the last reader, notify the incrementer
            if (g_readers_finished_count >= g_total_readers)
            {
                std::cout << "[AsyncReader-" << reader_id << " " << tid 
                          << "] I'm the last reader! Notifying incrementer...\n";
                g_cv_all_reads_done.notify_one();
            }
        }
    }

    std::cout << "[AsyncReader-" << reader_id << " " << tid 
              << "] Completed all " << expected_cycles << " cycles. Exiting.\n";
}


// ============================================================================
// MAIN FUNCTION - DEMONSTRATION OF THREE CASES
// ============================================================================
int main()
{
    // Configuration
    const int NUM_CYCLES = 3;           // Number of increment cycles
    const int INCREMENT_VALUE = 10;     // How much to increment each cycle

    // ========================================================================
    // CASE 1: SYNCHRONOUS READERS ONLY (Sequential Reading)
    // ========================================================================
    std::cout << "============================================================================\n";
    std::cout << "CASE 1: SYNCHRONOUS READERS ONLY (Sequential Reading)\n";
    std::cout << "============================================================================\n";
    std::cout << "Behavior: Readers read ONE AT A TIME using std::mutex\n";
    std::cout << "Configuration:\n";
    std::cout << "  - Increment cycles: " << NUM_CYCLES << "\n";
    std::cout << "  - Increment per cycle: " << INCREMENT_VALUE << "\n";
    std::cout << "  - Synchronous readers: 4\n";
    std::cout << "============================================================================\n\n";

    {
        // Initialize global state
        g_count = 0;
        g_current_cycle = 0;
        g_increment_done = false;
        g_readers_finished_count = 0;
        g_total_readers = 4;

        std::vector<std::thread> threads{};

        // Launch synchronous readers
        std::cout << "[Main] Launching 4 synchronous readers...\n";
        for (int i{ 0 }; i < 4; i++)
        {
            threads.push_back(std::thread(ReaderSynchronous, i + 1, NUM_CYCLES));
        }

        // Small delay to ensure all readers are waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Launch the incrementer thread
        std::cout << "[Main] Launching incrementer thread...\n\n";
        threads.push_back(std::thread(Incrementer, INCREMENT_VALUE, NUM_CYCLES));

        // Wait for all threads to complete
        for (auto& th : threads)
        {
            if (th.joinable())
                th.join();
        }

        // Results
        std::cout << "\n[Main] CASE 1 COMPLETED - Final value: " << g_count;
        std::cout << " (Expected: " << (NUM_CYCLES * INCREMENT_VALUE) << ")\n";
        std::cout << "[Main] Notice: Readers read SEQUENTIALLY (one after another)\n";
        std::cout << "============================================================================\n\n";
    }

    // ========================================================================
    // CASE 2: ASYNCHRONOUS READERS ONLY (Concurrent Reading)
    // ========================================================================
    std::cout << "============================================================================\n";
    std::cout << "CASE 2: ASYNCHRONOUS READERS ONLY (Concurrent Reading)\n";
    std::cout << "============================================================================\n";
    std::cout << "Behavior: Readers read SIMULTANEOUSLY using std::shared_lock\n";
    std::cout << "Configuration:\n";
    std::cout << "  - Increment cycles: " << NUM_CYCLES << "\n";
    std::cout << "  - Increment per cycle: " << INCREMENT_VALUE << "\n";
    std::cout << "  - Asynchronous readers: 4\n";
    std::cout << "============================================================================\n\n";

    {
        // Initialize global state
        g_count = 0;
        g_current_cycle = 0;
        g_increment_done = false;
        g_readers_finished_count = 0;
        g_total_readers = 4;

        std::vector<std::thread> threads{};

        // Launch asynchronous readers
        std::cout << "[Main] Launching 4 asynchronous readers...\n";
        for (int i{ 0 }; i < 4; i++)
        {
            threads.push_back(std::thread(ReaderAsynchronous, i + 1, NUM_CYCLES));
        }

        // Small delay to ensure all readers are waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Launch the incrementer thread
        std::cout << "[Main] Launching incrementer thread...\n\n";
        threads.push_back(std::thread(Incrementer, INCREMENT_VALUE, NUM_CYCLES));

        // Wait for all threads to complete
        for (auto& th : threads)
        {
            if (th.joinable())
                th.join();
        }

        // Results
        std::cout << "\n[Main] CASE 2 COMPLETED - Final value: " << g_count;
        std::cout << " (Expected: " << (NUM_CYCLES * INCREMENT_VALUE) << ")\n";
        std::cout << "[Main] Notice: Readers read CONCURRENTLY (all at the same time)\n";
        std::cout << "============================================================================\n\n";
    }

    // ========================================================================
    // CASE 3: MIXED (Both Synchronous and Asynchronous Readers)
    // ========================================================================
    std::cout << "============================================================================\n";
    std::cout << "CASE 3: MIXED MODE (Synchronous + Asynchronous Readers)\n";
    std::cout << "============================================================================\n";
    std::cout << "Behavior: Sync readers read sequentially, Async readers read concurrently\n";
    std::cout << "Configuration:\n";
    std::cout << "  - Increment cycles: " << NUM_CYCLES << "\n";
    std::cout << "  - Increment per cycle: " << INCREMENT_VALUE << "\n";
    std::cout << "  - Synchronous readers: 2\n";
    std::cout << "  - Asynchronous readers: 2\n";
    std::cout << "============================================================================\n\n";

    {
        // Initialize global state
        g_count = 0;
        g_current_cycle = 0;
        g_increment_done = false;
        g_readers_finished_count = 0;
        g_total_readers = 4;

        std::vector<std::thread> threads{};

        // Launch synchronous readers
        std::cout << "[Main] Launching 2 synchronous readers...\n";
        for (int i{ 0 }; i < 2; i++)
        {
            threads.push_back(std::thread(ReaderSynchronous, i + 1, NUM_CYCLES));
        }

        // Launch asynchronous readers
        std::cout << "[Main] Launching 2 asynchronous readers...\n";
        for (int i{ 0 }; i < 2; i++)
        {
            threads.push_back(std::thread(ReaderAsynchronous, i + 1, NUM_CYCLES));
        }

        // Small delay to ensure all readers are waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Launch the incrementer thread
        std::cout << "[Main] Launching incrementer thread...\n\n";
        threads.push_back(std::thread(Incrementer, INCREMENT_VALUE, NUM_CYCLES));

        // Wait for all threads to complete
        for (auto& th : threads)
        {
            if (th.joinable())
                th.join();
        }

        // Results
        std::cout << "\n[Main] CASE 3 COMPLETED - Final value: " << g_count;
        std::cout << " (Expected: " << (NUM_CYCLES * INCREMENT_VALUE) << ")\n";
        std::cout << "[Main] Notice: Async readers overlap, sync readers do not\n";
        std::cout << "============================================================================\n\n";
    }

    // Summary
    std::cout << R"(
============================================================================================
KEY CONCEPTS IN TWO-WAY BARRIER SYNCHRONIZATION:
============================================================================================

1. CONDITION VARIABLES - wait() and notify():
   - wait(lock, predicate): Atomically releases lock, sleeps, wakes up, re-acquires lock
   - notify_all(): Wakes ALL waiting threads (used for readers)
   - notify_one(): Wakes ONE waiting thread (used for incrementer)
   - Predicate prevents spurious wakeups and ensures correct state

2. TWO-WAY BARRIER PATTERN:
   - Incrementer → notifies readers → WAITS for all readers
   - Readers → wake up → read → last reader notifies incrementer
   - Creates lock-step synchronization between producer and consumers

3. SYNCHRONOUS vs ASYNCHRONOUS READING:
   - Sync: std::mutex - only ONE reader at a time (sequential)
   - Async: std::shared_lock - MULTIPLE readers simultaneously (concurrent)
   - Both wait for increment the same way, differ only in reading mechanism

4. WHY g_shared_mutex IS ESSENTIAL:
   - Condition variables signal WHEN to access data
   - Mutexes provide SAFE ACCESS to the actual data
   - Without it: DATA RACE on g_count (undefined behavior!)

============================================================================================
)" << std::endl;

    return 0;
}
