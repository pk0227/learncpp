#include <thread>
#include <mutex>
#include <chrono>
/*
 * SHARED MUTEX - READER-WRITER LOCK
 * 
 * Demonstrates std::shared_mutex for optimized read-heavy scenarios.
 * 
 * SHARED MUTEX CONCEPT:
 * - Exclusive lock: Only one thread (writers)
 * - Shared lock: Multiple threads simultaneously (readers)
 * - Readers don't block each other
 * - Writers block everyone (readers and other writers)
 * 
 * LOCK MODES:
 * - lock(): Exclusive (write) access
 * - lock_shared(): Shared (read) access
 * - Multiple shared locks OK, exclusive lock blocks all
 * 
 * WHEN TO USE:
 * - Read-heavy workloads (many reads, few writes)
 * - Data structures accessed frequently but modified rarely
 * - Configuration that's read often, updated seldom
 * - Caches, lookup tables, shared state
 * 
 * PERFORMANCE:
 * - Readers: Don't serialize (huge win for read-heavy)
 * - Writers: Same cost as regular mutex
 * - Overhead: Slightly more than regular mutex
 * - Worth it when reads >> writes (e.g., 90% reads)
 * 
 * RAII WRAPPERS:
 * - std::unique_lock<std::shared_mutex>: Exclusive (write)
 * - std::shared_lock<std::shared_mutex>: Shared (read)
 * - Auto unlock on scope exit (exception-safe)
 * 
 * FAIRNESS ISSUES:
 * - Readers can starve writers (continuous read locks)
 * - Implementation-dependent fairness
 * - Some implementations prevent writer starvation
 * 
 * UPGRADE/DOWNGRADE:
 * - Cannot atomically upgrade shared → exclusive!
 * - Must unlock shared, then lock exclusive
 * - Risks: Another thread might modify in between
 * - Solution: Use exclusive from start if might write
 * 
 * SHARED_MUTEX vs MUTEX:
 * - Mutex: All threads serialize (one at a time)
 * - Shared_mutex: Readers concurrent, writers exclusive
 * - Mutex: Simpler, lower overhead
 * - Shared_mutex: Better for read-heavy (>70% reads)
 * 
 * INTERVIEW GOLD:
 * - When to use shared_mutex
 * - Why not always use shared_mutex? (overhead)
 * - Reader-writer problem variants
 * - Upgrade lock challenges
 * - How to prevent reader starvation of writers
 * 
 * REQUIRES: C++17 for std::shared_mutex, C++14 for std::shared_timed_mutex
 */

#include <shared_mutex>
#include <array>
#include <cstdio>

char ITEMS[5][15] = {"Laptops", "Monitors", "Keyboards", "Mice", "Printers"};
int item_stock[5] = {10, 15, 20, 25, 30};
std::shared_mutex inventory_mutex;

void inventory_reader(const int id) {
    for (int i = 0; i < 5; i++) {
        inventory_mutex.lock_shared();
        printf("Reader-%d sees %s stock is %d\n", id, ITEMS[i], item_stock[i]);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        inventory_mutex.unlock_shared();
    }
}

void inventory_writer(const int id) {
    for (int i = 0; i < 5; i++) {
        inventory_mutex.lock();
        item_stock[i] = item_stock[i] + (id + 1); // Writers update stock
        printf("Writer-%d updated %s stock to %d\n", id, ITEMS[i], item_stock[i]);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        inventory_mutex.unlock();
    }
}

int main() {
    // create ten reader threads but only two writer threads
    std::array<std::thread, 10> readers;
    for (unsigned int i = 0; i < readers.size(); i++) {
        readers[i] = std::thread(inventory_reader, i);
    }
    std::array<std::thread, 2> writers;
    for (unsigned int i = 0; i < writers.size(); i++) {
        writers[i] = std::thread(inventory_writer, i);
    }

    // wait for readers and writers to finish
    for (unsigned int i = 0; i < readers.size(); i++) {
        readers[i].join();
    }
    for (unsigned int i = 0; i < writers.size(); i++) {
        writers[i].join();
    }

    return 0;
}
