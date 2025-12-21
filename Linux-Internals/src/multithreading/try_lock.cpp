#include <thread>
/*
 * TRY_LOCK - NON-BLOCKING LOCK ATTEMPT
 * 
 * CONCEPT:
 * - try_lock(): Attempts to acquire lock without blocking
 * - Returns immediately (true if acquired, false if not)
 * - Useful for avoiding deadlock or doing other work while waiting
 * 
 * USE CASES:
 * - Deadlock avoidance (back off if can't acquire all locks)
 * - Responsive UI (don't block, show busy indicator instead)
 * - Lock-free fast path (try lock, fall back to blocking)
 * 
 * PATTERN:
 * if (mutex.try_lock()) {
 *   // Got lock, do work
 *   mutex.unlock();
 * } else {
 *   // Couldn't get lock, do something else
 * }
 * 
 * WARNING: Can lead to livelock if not careful (see livelock.cpp)
 * 
 * INTERVIEW: Difference between lock() and try_lock()? When to use each?
 */

#include <mutex>
#include <chrono>

unsigned int items_on_list = 0;
std::mutex list_mutex;

void worker(const char* name) {
    int items_to_add = 0;
    while (items_on_list <= 20) {
        if (items_to_add && list_mutex.try_lock()) { // add item(s) to shared items_on_list
            items_on_list += items_to_add;
            printf("%s added %u item(s) to the list.\n", name, items_to_add);
            items_to_add = 0;
            std::this_thread::sleep_for(std::chrono::milliseconds(300)); // time spent writing
            list_mutex.unlock();
        } else { // look for other tasks
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // time spent searching
            items_to_add++;
            printf("%s found another task to add.\n", name);
        }
    }
}

int main() {
    std::thread alice(worker, "Alice");
    std::thread bob(worker, "Bob");
    auto start_time = std::chrono::steady_clock::now();
    alice.join();
    bob.join();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count();
    printf("Elapsed Time: %.2f seconds\n", elapsed_time / 1000.0);

    return 0;
}
