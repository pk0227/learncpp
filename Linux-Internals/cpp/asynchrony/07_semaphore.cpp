/*
 * SEMAPHORE IMPLEMENTATION FOR RESOURCE LIMITING
 * 
 * Custom semaphore class to control access to limited resources in async operations.
 * 
 * SEMAPHORE CONCEPT:
 * - Counter-based synchronization primitive
 * - Allows N threads to access a resource simultaneously
 * - acquire(): Decrements counter, blocks if counter is 0
 * - release(): Increments counter, wakes one waiting thread
 * 
 * REAL-WORLD USE CASES:
 * - Database connection pools (limit concurrent connections)
 * - API rate limiting (max N requests per second)
 * - Thread pools (limit worker threads)
 * - Memory-intensive operations (limit concurrent allocations)
 * 
 * IMPLEMENTATION DETAILS:
 * - Uses mutex + condition_variable for synchronization
 * - cv.wait(lock, predicate): Blocks until predicate returns true
 * - cv.notify_one(): Wakes one waiting thread (vs notify_all which wakes all)
 * 
 * SEMAPHORE vs MUTEX:
 * - Mutex: Binary (locked/unlocked), ownership concept
 * - Semaphore: Counter-based, no ownership, can be released by any thread
 * 
 * C++20 UPDATE:
 * - Standard library now includes std::counting_semaphore<N>
 * - This implementation shows the underlying mechanism
 * 
 * INTERVIEW INSIGHTS:
 * - Why use semaphore instead of N mutexes
 * - How to prevent resource exhaustion
 * - Difference between counting semaphore and binary semaphore (mutex)
 */

#include <chrono>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <thread>
#include <vector>

class Semaphore {
public:
  // Initialize semaphore with maximum number of concurrent accesses
  explicit Semaphore(int count) : count(count) {}

  // Acquire a resource (decrement counter, block if none available)
  void acquire() {
    std::unique_lock<std::mutex> lock(mtx);
    // Wait until at least one resource is available (count > 0)
    // Lambda predicate prevents spurious wakeups
    cv.wait(lock, [&]() { return count > 0; });
    --count;  // Take one resource
  }

  // Release a resource (increment counter, wake one waiting thread)
  void release() {
    std::unique_lock<std::mutex> lock(mtx);
    ++count;  // Return one resource
    cv.notify_one();  // Wake one waiting thread (not all - more efficient)
  }

private:
  std::mutex mtx;
  std::condition_variable cv;
  int count;
};

// Global semaphore limiting resource access to 3 concurrent tasks
// Only 3 out of 10 tasks can execute simultaneously
Semaphore resource_semaphore(3);

void limited_resource(int task_id) {
  resource_semaphore.acquire();
  try {
    std::cout << "Task " << task_id << " is using the limited resource.\n";

    // Improved random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(500, 2500);

    std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));

    std::cout << "Task " << task_id << " is done using the limited resource.\n";
  } catch (const std::exception &e) {
    std::cerr << "Exception in task " << task_id << ": " << e.what() << '\n';
  }
  resource_semaphore.release();
}

int main() {
  std::vector<std::future<void>> tasks;

  for (int i = 0; i < 10; ++i) {
    tasks.push_back(std::async(std::launch::async, limited_resource, i));
  }

  for (auto &task : tasks) {
    try {
      task.get(); // Ensure exceptions are propagated
    } catch (const std::exception &e) {
      std::cerr << "Exception in main: " << e.what() << '\n';
    }
  }

  return 0;
}
