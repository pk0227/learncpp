/*
 * LIVELOCK - THREADS ACTIVE BUT NOT PROGRESSING
 * 
 * Demonstrates livelock: threads keep executing but make no progress.
 * 
 * LIVELOCK vs DEADLOCK:
 * - Deadlock: Threads blocked forever (not running)
 * - Livelock: Threads running but not progressing (too polite!)
 * - Both: System stuck, no forward progress
 * 
 * SCENARIO:
 * - Alice wants tool_a then tool_b
 * - Bob wants tool_b then tool_a
 * - Both repeatedly acquire first lock
 * - Both fail to acquire second lock (try_lock fails)
 * - Both release first lock and yield
 * - Repeat infinitely (livelock!)
 * 
 * LIVELOCK ANALOGY:
 * - Two people in hallway, both step aside
 * - Both step back, both step aside again
 * - Keep moving but never pass (hallway livelock)
 * 
 * WHY try_lock CAUSES LIVELOCK:
 * - try_lock() fails → release first lock → yield
 * - Other thread does same thing
 * - Symmetric behavior = livelock
 * - Neither makes progress
 * 
 * SOLUTION STRATEGIES:
 * 1. Lock ordering: Always acquire locks in same order
 * 2. Backoff: Random delay before retry (breaks symmetry)
 * 3. std::scoped_lock: Acquires multiple locks atomically
 * 4. Priority: One thread gets precedence
 * 
 * FIXED VERSION:
 * - Use std::scoped_lock(tool_a, tool_b) for all threads
 * - Acquires both locks atomically, no livelock possible
 * - See deadlock_philosophers_solved.cpp for example
 * 
 * DETECTION:
 * - Threads show 100% CPU (unlike deadlock)
 * - Progress metrics don't increase
 * - Log shows repeated yield/retry pattern
 * 
 * REAL-WORLD EXAMPLES:
 * - Network protocols with symmetric retry
 * - Distributed systems with competing updates
 * - Database transactions with retry logic
 * - Optimistic locking with collisions
 * 
 * INTERVIEW CONCEPTS:
 * - Difference between deadlock and livelock
 * - How to prevent livelock
 * - Why random backoff helps
 * - std::scoped_lock benefits
 */

#include <thread>
#include <mutex>
#include <iostream>
#include <vector>

// Global task counter
int task_count = 5000;

// Worker function
void worker(std::mutex &first_tool, std::mutex &second_tool, const std::string &worker_name) {
    while (task_count > 0) {
        first_tool.lock();
        if (!second_tool.try_lock()) {
            first_tool.unlock();
            std::cout << worker_name << " is yielding to avoid livelock." << std::endl;
            std::this_thread::yield();
        } else {
            if (task_count > 0) {
                task_count--;
                std::cout << worker_name << " completed a task. Remaining: " << task_count << std::endl;
            }
            second_tool.unlock();
            first_tool.unlock();
        }
    }
}

int main() {
    // Mutexes representing tools
    std::mutex tool_a, tool_b;

    // Worker threads
    std::vector<std::thread> workers;
    workers.emplace_back(worker, std::ref(tool_a), std::ref(tool_b), "Alice");
    workers.emplace_back(worker, std::ref(tool_b), std::ref(tool_a), "Bob");
    workers.emplace_back(worker, std::ref(tool_a), std::ref(tool_b), "Charlie");
    workers.emplace_back(worker, std::ref(tool_b), std::ref(tool_a), "Diana");

    // Join threads
    for (auto &worker : workers) {
        worker.join();
    }

    std::cout << "The workers have completed their tasks." << std::endl;
    return 0;
}
