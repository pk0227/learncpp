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

// Mutex specifically for condition variable (needed for wait/notify mechanism)
std::mutex g_cv_mutex{};

// Condition variable to notify reader threads when increment is done
std::condition_variable g_cv{};

// Flag to signal that increment operation is complete
bool g_increment_done{ false };


// ============================================================================
// INCREMENTER THREAD FUNCTION
// ============================================================================
// This thread will increment the shared value and notify all waiting readers
void Incrementer(int increment_value)
{
    std::thread::id tid{ std::this_thread::get_id() };
    std::cout << "[Incrementer " << tid << "] Started, will increment by " 
              << increment_value << "\n";

    // Simulate some work before incrementing (optional)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    {
        // EXCLUSIVE LOCK: unique_lock gives exclusive write access to g_count
        // No other thread (reader or writer) can access g_count during this time
        std::unique_lock<std::shared_mutex> ul{ g_shared_mutex };
        
        std::cout << "[Incrementer " << tid << "] Acquired exclusive lock, incrementing...\n";
        
        // Perform the increment operation
        for (int i{}; i < increment_value; i++)
        {
            g_count++;
        }
        
        std::cout << "[Incrementer " << tid << "] Increment complete. New value: " 
                  << g_count << "\n";
        
    } // unique_lock is released here automatically when ul goes out of scope

    {
        // Lock the condition variable mutex to safely update the flag
        std::lock_guard<std::mutex> lg{ g_cv_mutex };
        g_increment_done = true; // Signal that increment is complete
        
        // NOTIFY ALL WAITING THREADS
        // Wake up all reader threads that are waiting on the condition variable
        std::cout << "[Incrementer " << tid << "] Notifying all waiting readers...\n";
        g_cv.notify_all();

        std::cout << "[Incrementer " << tid << "] Finished\n";
    }
}


// ============================================================================
// READER THREAD FUNCTION (SYNCHRONOUS VERSION - SEQUENTIAL READING)
// ============================================================================
// Synchronous readers:
// 1. Wait for increment to complete (like async readers)
// 2. Read ONE AT A TIME sequentially (using regular mutex)
// This ensures only one reader is reading at any given time
void ReaderSynchronous(int reader_id)
{
    std::thread::id tid{ std::this_thread::get_id() };
    std::cout << "[SyncReader-" << reader_id << " " << tid << "] Started, waiting for increment...\n";

    // STEP 1: WAIT FOR INCREMENT TO COMPLETE (same as async readers)
    {
        std::unique_lock<std::mutex> ul{ g_cv_mutex };
        
        // wait() will:
        // 1. Release the lock and put this thread to sleep
        // 2. Wake up when notify_all() is called
        // 3. Re-acquire the lock and check the predicate (lambda function)
        // 4. If predicate is true, continue; otherwise go back to sleep
        g_cv.wait(ul, [] { return g_increment_done; });
        
        //std::cout << "[SyncReader-" << reader_id << " " << tid << "] Woke up! Increment is done.\n";
    }

    // STEP 2: READ SEQUENTIALLY (ONE AT A TIME)
    {
        // REGULAR MUTEX: Only ONE reader can acquire this lock at a time
        // This forces SEQUENTIAL reading - readers wait for each other
        std::lock_guard<std::mutex> lg{ g_sync_read_mutex };
        
        std::cout << "[SyncReader-" << reader_id << " " << tid 
                  << "] Reading value SEQUENTIALLY: " << g_count << "\n";
        
        // Simulate some reading work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        std::cout << "[SyncReader-" << reader_id << " " << tid 
                  << "] Finished reading (releasing lock for next reader)\n";
        
        std::cout << "[SyncReader-" << reader_id << " " << tid << "] Exiting\n";
        
    } // lock_guard is released here - next waiting reader can now proceed

}


// ============================================================================
// READER THREAD FUNCTION (ASYNCHRONOUS VERSION - CONCURRENT READING)
// ============================================================================
// Asynchronous readers:
// 1. Wait for increment to complete (like sync readers)
// 2. Read CONCURRENTLY/SIMULTANEOUSLY (using shared_lock)
// Multiple async readers can read at the same time without blocking each other
void ReaderAsynchronous(int reader_id)
{
    std::thread::id tid{ std::this_thread::get_id() };
    std::cout << "[AsyncReader-" << reader_id << " " << tid << "] Started, waiting for increment...\n";

    // STEP 1: WAIT FOR INCREMENT TO COMPLETE (same as sync readers)
    {
        std::unique_lock<std::mutex> ul{ g_cv_mutex };
        g_cv.wait(ul, [] { return g_increment_done; });
        
        //std::cout << "[AsyncReader-" << reader_id << " " << tid << "] Woke up! Increment is done.\n";
    }

    // STEP 2: READ CONCURRENTLY (ALL AT ONCE)
    {
        // SHARED_LOCK: Multiple readers can acquire shared_lock SIMULTANEOUSLY
        // This allows CONCURRENT reads - all readers can read at the same time
        std::shared_lock<std::shared_mutex> sl{ g_shared_mutex };
        
        std::cout << "[AsyncReader-" << reader_id << " " << tid 
                  << "] Reading value CONCURRENTLY: " << g_count << "\n";
        
        // Simulate some reading work (same duration as sync readers)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        std::cout << "[AsyncReader-" << reader_id << " " << tid 
                  << "] Finished reading (other readers can still be reading)\n";
        
        std::cout << "[AsyncReader-" << reader_id << " " << tid << "] Exiting\n";
    } // shared_lock is released here

}


// ============================================================================
// MAIN FUNCTION - DEMONSTRATION
// ============================================================================
int main()
{
    std::cout << "============================================================\n";
    std::cout << "EXAMPLE 1: SYNCHRONOUS READERS (Sequential Reading)\n";
    std::cout << "All readers wait for increment, then read ONE AT A TIME\n";
    std::cout << "===========================================================\n\n";

    {
        // Reset state
        g_count = 0;
        g_increment_done = false;

        std::vector<std::thread> threads{};
        int num_readers{ 5 };
        int increment_value{ 10 };

        std::cout << "[Main] Launching " << num_readers << " synchronous readers...\n";

        // Launch synchronous reader threads FIRST - they will wait for the incrementer
        for (int i{ 0 }; i < num_readers; i++)
        {
            threads.push_back(std::thread(ReaderSynchronous, i + 1));
        }

        // Small delay to ensure readers are waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::cout << "[Main] Launching incrementer thread...\n\n";

        // Launch the incrementer thread - this will wake up all waiting readers
        threads.push_back(std::thread(Incrementer, increment_value));

        // Wait for all threads to complete
        for (auto& th : threads)
        {
            if (th.joinable())
                th.join();
        }

        std::cout << "\n[Main] All threads completed. Final value: " << g_count << "\n";
        std::cout << "[Main] Notice: Readers read SEQUENTIALLY (one after another)\n";
        std::cout << "\n========================================================\n\n";
    }

    // ----------------------------------------------------------------------------

    std::cout << "================================================================\n";
    std::cout << "EXAMPLE 2: ASYNCHRONOUS READERS (Concurrent Reading)\n";
    std::cout << "All readers wait for increment, then read CONCURRENTLY\n";
    std::cout << "==============================================================\n\n";

    {
        // Reset state
        g_count = 0;
        g_increment_done = false;

        std::vector<std::thread> threads{};
        int num_async_readers{ 5 };
        int increment_value{ 20 };

        std::cout << "[Main] Launching " << num_async_readers << " asynchronous readers...\n";

        // Launch async readers - they will also wait for increment
        for (int i{ 0 }; i < num_async_readers; i++)
        {
            threads.push_back(std::thread(ReaderAsynchronous, i + 1));
        }

        // Small delay to ensure readers are waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::cout << "[Main] Launching incrementer thread...\n\n";

        // Launch incrementer
        threads.push_back(std::thread(Incrementer, increment_value));

        // Wait for all threads to complete
        for (auto& th : threads)
        {
            if (th.joinable())
                th.join();
        }

        std::cout << "\n[Main] All threads completed. Final value: " << g_count << "\n";
        std::cout << "[Main] Notice: Readers read CONCURRENTLY (all at the same time)\n";
        std::cout << "\n===========================================================\n\n";
    }

    // ----------------------------------------------------------------------------

    std::cout << "===================================================================\n";
    std::cout << "EXAMPLE 3: MIXED MODE (Both Sequential and Concurrent Readers)\n";
    std::cout << "ALL wait for increment, but sync readers read sequentially,\n";
    std::cout << "while async readers read concurrently\n";
    std::cout << "=================================================================\n\n";

    {
        // Reset state
        g_count = 100; // Start from a different value
        g_increment_done = false;

        std::vector<std::thread> threads{};
        int num_sync_readers{ 3 };
        int num_async_readers{ 3 };
        int increment_value{ 50 };

        std::cout << "[Main] Launching " << num_sync_readers << " synchronous readers...\n";
        std::cout << "[Main] Launching " << num_async_readers << " asynchronous readers...\n";

        // Launch synchronous readers - they will wait, then read sequentially
        for (int i{ 0 }; i < num_sync_readers; i++)
        {
            threads.push_back(std::thread(ReaderSynchronous, i + 1));
        }

        // Launch asynchronous readers - they will wait, then read concurrently
        for (int i{ 0 }; i < num_async_readers; i++)
        {
            threads.push_back(std::thread(ReaderAsynchronous, i + 1));
        }

        // Small delay
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::cout << "[Main] Launching incrementer thread...\n\n";

        // Launch incrementer
        threads.push_back(std::thread(Incrementer, increment_value));

        // Wait for all threads to complete
        for (auto& th : threads)
        {
            if (th.joinable())
                th.join();
        }

        std::cout << "\n[Main] All threads completed. Final value: " << g_count << "\n";
        std::cout << "[Main] Notice: Async readers can overlap, sync readers cannot\n";
        std::cout << "\n==========================================================\n\n";
    }

    std::cout << "\n" << R"(
==================================================================
KEY CONCEPTS DEMONSTRATED:
==================================================================

1. BOTH READER TYPES WAIT FOR INCREMENT:
   - Synchronous readers: Wait via condition variable
   - Asynchronous readers: Wait via condition variable
   - Neither type reads until increment is complete (guaranteed updated value)

2. DIFFERENCE IS IN READING BEHAVIOR AFTER INCREMENT:
   - Synchronous readers: Read SEQUENTIALLY using std::mutex
     * Only 1 sync reader can read at a time
     * Other sync readers must wait their turn
     * Total time = 100ms × num_sync_readers (sequential)
   
   - Asynchronous readers: Read CONCURRENTLY using std::shared_lock
     * ALL async readers can read simultaneously
     * No waiting for other async readers
     * Total time = 100ms (parallel)

3. SYNCHRONIZATION PRIMITIVES:
   - std::condition_variable: Makes all readers wait for increment signal
   - std::mutex: Forces sequential access (sync readers)
   - std::shared_mutex + std::shared_lock: Allows concurrent access (async readers)
   - std::unique_lock: Exclusive write access for incrementer

4. PERFORMANCE IMPLICATIONS:
   - Synchronous reading: Slower but simpler, one reader at a time
   - Asynchronous reading: Faster with concurrent reads, better throughput
   - Mixed mode: Flexibility to choose based on reader requirements

5. THREAD SAFETY GUARANTEES:
   - All readers see the updated value (no stale data)
   - Writer has exclusive access during increment (no race conditions)
   - Async readers don't interfere with each other (shared_lock)
   - Sync readers are properly serialized (mutex)

===========================================================================
)" << std::endl;

    return 0;
}
