/*
 * FUTURES WITH PACKAGED TASKS AND MANUAL THREAD MANAGEMENT
 * 
 * This demonstrates using std::packaged_task for more fine-grained control over async execution.
 * 
 * KEY COMPONENTS:
 * - std::packaged_task<R(Args...)>: Wraps a callable object and provides a future for its result
 * - get_future(): Returns the future associated with this packaged task
 * - std::thread: Explicitly creates a thread (unlike std::async which may reuse threads)
 * 
 * WORKFLOW:
 * 1. Create packaged_task wrapping the function
 * 2. Get future from the packaged_task
 * 3. Move the task to a thread (packaged_task is move-only)
 * 4. Join threads to wait for completion
 * 5. Retrieve results via futures
 * 
 * PACKAGED_TASK vs ASYNC:
 * - packaged_task: More control, manual thread management required
 * - std::async: Simpler API, automatic thread management
 * 
 * USE CASES:
 * - When you need explicit control over which thread executes the task
 * - Integration with custom thread pools
 * - When you want to separate task creation from execution
 * 
 * NOTE: This doesn't use true callbacks; square_callback is called manually after get()
 */

#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int slow_square(int x) {
  std::cout << "Starting slow square computation for " << x << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "Finished slow square computation for " << x << std::endl;
  return x * x;
}

void square_callback(std::future<int> &future) {
  std::cout << "Square callback called with result: " << future.get()
            << std::endl;
}

int main() {
  auto start = std::chrono::high_resolution_clock::now();

  // Create packaged tasks wrapping the slow_square function
  std::packaged_task<int(int)> task1(slow_square);
  std::packaged_task<int(int)> task2(slow_square);

  // Get futures before moving tasks (can't get future after move)
  std::future<int> future1 = task1.get_future();
  std::future<int> future2 = task2.get_future();

  // Move tasks into threads (packaged_task is not copyable, only movable)
  // The threads will execute the tasks with the provided arguments
  std::thread t1(std::move(task1), 3);
  std::thread t2(std::move(task2), 4);

  t1.join();
  t2.join();

  square_callback(future1);
  square_callback(future2);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();
  std::cout << "Asynchronous approach time: " << duration << "ms" << std::endl;

  return 0;
}
