/*
 * THREAD STARVATION - UNFAIR SCHEDULING
 * 
 * Demonstrates starvation: some threads never get chance to execute.
 * 
 * STARVATION DEFINED:
 * - Thread ready to run but never scheduled
 * - System makes progress but unfairly
 * - Some threads dominate resource access
 * - Starved threads wait indefinitely
 * 
 * WHY STARVATION HAPPENS:
 * - Unfair lock acquisition (some threads always win)
 * - Priority inversion
 * - Greedy algorithms
 * - No fairness guarantees from OS scheduler
 * 
 * THIS EXAMPLE:
 * - 200 threads compete for single mutex
 * - High contention on task_lock
 * - Some threads acquire lock many times
 * - Some threads acquire very few times
 * - Unfair distribution of tasks
 * 
 * MUTEX FAIRNESS:
 * - std::mutex: No fairness guarantee!
 * - OS may favor recently released thread (cache warm)
 * - Or favor higher priority threads
 * - Some threads may be starved
 * 
 * TYPICAL OUTPUT:
 * - Some threads: 100+ tasks
 * - Some threads: 0-10 tasks
 * - Highly uneven distribution
 * - Total tasks = 5000 (correct, but unfair)
 * 
 * FAIRNESS vs PERFORMANCE:
 * - Fair locks: Slower (FIFO queue overhead)
 * - Unfair locks: Faster (cache locality)
 * - Trade-off: fairness costs performance
 * 
 * SOLUTIONS:
 * 1. Fair mutex (custom implementation with queue)
 * 2. Work-stealing queues (each thread has queue)
 * 3. Batch processing (limit consecutive acquisitions)
 * 4. Priority adjustments (boost starved threads)
 * 
 * WHEN STARVATION CRITICAL:
 * - Real-time systems (deadlines)
 * - Interactive applications (UI responsiveness)
 * - Fairness-sensitive applications
 * 
 * WHEN ACCEPTABLE:
 * - Throughput-focused batch processing
 * - Short-lived programs
 * - When eventual execution OK
 * 
 * INTERVIEW INSIGHTS:
 * - Starvation vs deadlock vs livelock
 * - Is std::mutex fair? (No!)
 * - How to implement fair mutex
 * - Real-world examples of starvation
 * - Priority inversion problem
 */

#include <thread>
#include <mutex>
#include <array>
#include <iostream>

int task_count = 5000;
std::mutex task_lock;

void worker() {
    int tasks_completed = 0;
    while (true) {
        std::scoped_lock lock(task_lock);
        if (task_count > 0) {
            task_count--;
            tasks_completed++;
        } else {
            break;
        }
    }
    std::cout << "Worker " << std::this_thread::get_id() << " completed " << tasks_completed << " tasks.\n";
}

int main() {
    std::array<std::thread, 200> workers;
    for (auto& worker_thread : workers) {
        worker_thread = std::thread(worker);
    }
    for (auto& worker_thread : workers) {
        worker_thread.join();
    }
    std::cout << "The workers are done with their tasks.\n";
    return 0;
}
