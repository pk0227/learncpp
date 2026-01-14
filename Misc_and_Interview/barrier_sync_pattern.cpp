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

// Condition variable: Incrementer notifies readers when increment is done
std::condition_variable g_cv_increment_done{};

// Condition variable: Readers notify incrementer when ALL reads are done
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
            g_cv_increment_done.notify_all();
        }

        // STEP 3: WAIT FOR ALL READERS TO FINISH READING
        {
            std::unique_lock<std::mutex> ul{ g_cv_mutex };
            
            std::cout << "[Incrementer " << tid << "] Waiting for all " 
                      << g_total_readers << " readers to finish...\n";
            
            g_cv_all_reads_done.wait(ul, [] { 
                return g_readers_finished_count >= g_total_readers; 
            });
            
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
            std::unique_lock<std::mutex> ul{ g_cv_mutex };
            
            g_cv_increment_done.wait(ul, [cycle] { 
                return g_increment_done && g_current_cycle == cycle; 
            });
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
// MAIN FUNCTION - DEMONSTRATION
// ============================================================================
int main()
{
    // Configuration
    const int NUM_CYCLES = 5;           // Number of increment cycles
    const int INCREMENT_VALUE = 10;     // How much to increment each cycle
    const int NUM_SYNC_READERS = 3;     // Number of synchronous readers
    const int NUM_ASYNC_READERS = 3;    // Number of asynchronous readers

    std::cout << "============================================================================\n";
    std::cout << "N-TIMES REPEATING INCREMENT WITH TWO-WAY BARRIER SYNCHRONIZATION\n";
    std::cout << "============================================================================\n";
    std::cout << "Configuration:\n";
    std::cout << "  - Increment cycles: " << NUM_CYCLES << "\n";
    std::cout << "  - Increment per cycle: " << INCREMENT_VALUE << "\n";
    std::cout << "  - Synchronous readers: " << NUM_SYNC_READERS << "\n";
    std::cout << "  - Asynchronous readers: " << NUM_ASYNC_READERS << "\n";
    std::cout << "============================================================================\n\n";

    // Initialize global state
    g_count = 0;
    g_current_cycle = 0;
    g_increment_done = false;
    g_readers_finished_count = 0;
    g_total_readers = NUM_SYNC_READERS + NUM_ASYNC_READERS;

    std::vector<std::thread> threads{};

    // Launch synchronous readers
    std::cout << "[Main] Launching " << NUM_SYNC_READERS << " synchronous readers...\n";
    for (int i{ 0 }; i < NUM_SYNC_READERS; i++)
    {
        threads.push_back(std::thread(ReaderSynchronous, i + 1, NUM_CYCLES));
    }

    // Launch asynchronous readers
    std::cout << "[Main] Launching " << NUM_ASYNC_READERS << " asynchronous readers...\n";
    for (int i{ 0 }; i < NUM_ASYNC_READERS; i++)
    {
        threads.push_back(std::thread(ReaderAsynchronous, i + 1, NUM_CYCLES));
    }

    // Small delay to ensure all readers are waiting
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Launch the incrementer thread
    std::cout << "[Main] Launching incrementer thread...\n\n";
    threads.push_back(std::thread(Incrementer, INCREMENT_VALUE, NUM_CYCLES));

    // Wait for all threads to complete
    std::cout << "[Main] Waiting for all threads to complete...\n";
    for (auto& th : threads)
    {
        if (th.joinable())
            th.join();
    }

    // Final results
    std::cout << "\n============================================================================\n";
    std::cout << "ALL THREADS COMPLETED\n";
    std::cout << "============================================================================\n";
    std::cout << "Final value: " << g_count << "\n";
    std::cout << "Expected value: " << (NUM_CYCLES * INCREMENT_VALUE) << "\n";
    std::cout << "Total cycles completed: " << g_current_cycle << "\n";
    
    if (g_count == NUM_CYCLES * INCREMENT_VALUE)
    {
        std::cout << "✓ SUCCESS: Value matches expected result!\n";
    }
    else
    {
        std::cout << "✗ ERROR: Value mismatch!\n";
    }
    
    std::cout << "============================================================================\n\n";

    std::cout << R"(
============================================================================================
KEY CONCEPTS IN TWO-WAY BARRIER SYNCHRONIZATION:
============================================================================================

1. BARRIER SYNCHRONIZATION PATTERN:
   - Incrementer increments → notifies readers → WAITS for all readers
   - Readers wake up → read → last reader notifies incrementer
   - Incrementer wakes up → increments again → repeat N times
   - This creates a lock-step synchronization between producer and consumers

2. TWO CONDITION VARIABLES:
   - g_cv_increment_done: Incrementer signals "new data ready"
   - g_cv_all_reads_done: Last reader signals "everyone finished reading"
   - This enables bidirectional communication between threads

3. READER COUNTER MECHANISM:
   - g_readers_finished_count tracks how many readers completed current cycle
   - Last reader (when count == g_total_readers) notifies incrementer
   - Counter resets to 0 at the start of each new cycle
   - Thread-safe increment protected by g_cv_mutex

4. SYNCHRONOUS vs ASYNCHRONOUS READING:
   - Synchronous readers: Use std::mutex (g_sync_read_mutex)
     * Only ONE sync reader can read at a time
     * Readers wait for each other sequentially
   
   - Asynchronous readers: Use std::shared_lock (g_shared_mutex)
     * MULTIPLE async readers can read simultaneously
     * True concurrent reading without blocking each other

5. REAL-WORLD APPLICATIONS:
   - GPU rendering: CPU prepares frame → GPU renders → wait for completion → repeat
   - Database transactions: Master commits → slaves replicate → ack → next commit
   - Pipeline processing: Stage 1 produces → Stage 2 consumes → barrier → next batch
   - Distributed computing: Coordinator sends task → workers process → barrier → next task
   - Game loops: Physics update → all systems read state → barrier → next frame

6. WHY g_shared_mutex IS ESSENTIAL:
   - Condition variables only signal WHEN to access data
   - Mutexes provide SAFE ACCESS to the actual data
   - Without g_shared_mutex: DATA RACE on g_count (undefined behavior!)
   - shared_lock enables concurrent reads while ensuring memory synchronization

============================================================================================
)" << std::endl;

    return 0;
}
