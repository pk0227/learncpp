/*
 * BASIC ASYNCHRONOUS PROGRAMMING DEMONSTRATION
 * 
 * This program demonstrates the fundamental difference between synchronous and asynchronous
 * approaches to handling I/O-bound tasks in modern C++.
 * 
 * KEY CONCEPTS:
 * - Synchronous Execution: Tasks run sequentially, one after another
 *   Total time = sum of all task times (2s + 1s = 3 seconds)
 * 
 * - Asynchronous Execution: Tasks run concurrently using std::async
 *   Total time ≈ max of all task times (max(2s, 1s) = ~2 seconds)
 * 
 * IMPORTANT COMPONENTS:
 * - std::async: Launches async tasks that may run on separate threads
 * - std::launch::async: Policy flag ensuring task runs asynchronously
 * - std::future<T>: Handle to retrieve the result of async operation
 * - future.get(): Blocks until result is available and retrieves it
 * 
 * PERFORMANCE IMPACT:
 * - Synchronous: Wastes CPU time waiting for I/O operations
 * - Asynchronous: CPU can work on multiple I/O operations simultaneously
 * 
 * USE CASES:
 * - Network requests, file I/O, database queries, API calls
 * - Any scenario with multiple independent I/O-bound operations
 */

#include <chrono>
#include <future>
#include <iostream>
#include <thread>

// Simulates a synchronous I/O-bound task (e.g., network request, file read)
// Blocks the calling thread for 'sleep_time' seconds
void sync_task(int task_id, int sleep_time) {
  std::cout << "Task " << task_id << " started." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(sleep_time));  // Simulates I/O wait
  std::cout << "Task " << task_id << " finished." << std::endl;
}

void sync_main() {
  auto start_time = std::chrono::steady_clock::now();

  sync_task(1, 2);
  sync_task(2, 1);

  auto elapsed_time = std::chrono::steady_clock::now() - start_time;
  std::cout
      << "Synchronous execution took "
      << std::chrono::duration_cast<std::chrono::seconds>(elapsed_time).count()
      << " seconds." << std::endl;
}

void async_task(int task_id, int sleep_time) {
  std::cout << "Task " << task_id << " started." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
  std::cout << "Task " << task_id << " finished." << std::endl;
}

void async_main() {
  auto start_time = std::chrono::steady_clock::now();

  auto future1 = std::async(std::launch::async, async_task, 1, 2);
  auto future2 = std::async(std::launch::async, async_task, 2, 1);

  future1.wait();
  future2.wait();

  auto elapsed_time = std::chrono::steady_clock::now() - start_time;
  std::cout
      << "Asynchronous execution took "
      << std::chrono::duration_cast<std::chrono::seconds>(elapsed_time).count()
      << " seconds." << std::endl;
}

int main() {
  sync_main();
  async_main();

  return 0;
}
