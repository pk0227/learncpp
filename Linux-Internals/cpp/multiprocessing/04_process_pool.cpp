/*
 * PROCESS POOL IMPLEMENTATION
 * 
 * Custom process pool that limits concurrent processes and manages task execution.
 * 
 * DESIGN PATTERN:
 * - Fixed number of worker slots (not worker processes)
 * - When slot available: fork new process for task
 * - When slots full: wait for any process to finish, reuse slot
 * - Destructor waits for all remaining processes
 * 
 * KEY CONCEPTS:
 * - Resource limiting (max N concurrent processes)
 * - Dynamic process creation/destruction
 * - Exit code communication (parent ← child)
 * 
 * PROCESS LIFECYCLE:
 * 1. Parent forks child
 * 2. Child executes task
 * 3. Child exits with status code
 * 4. Parent collects exit code via wait()
 * 
 * WAIT MECHANICS:
 * - wait(&status): Blocks until ANY child terminates
 * - WIFEXITED(status): True if child exited normally
 * - WEXITSTATUS(status): Extracts 8-bit exit code
 * - Exit code range: 0-255
 * 
 * LIMITATIONS:
 * - No inter-process communication except exit code
 * - Process creation overhead for each task
 * - Only 8-bit return value (limited data)
 * - No task queuing (tasks lost if not submitted)
 * 
 * COMPARISON WITH THREAD POOL:
 * Thread Pool: Reuses threads, faster, shared memory
 * Process Pool: Creates/destroys processes, isolated memory
 * 
 * BETTER ALTERNATIVES:
 * - Worker processes that persist and pull from task queue
 * - Shared memory or message queue for communication
 * - Thread pool for most use cases (lower overhead)
 * 
 * INTERVIEW INSIGHTS:
 * - When to use process pool vs thread pool
 * - Overhead of process creation
 * - How to implement proper task queue
 * - Exit code limitations and workarounds
 */

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>
class ProcessPool {
public:
  ProcessPool(size_t num_workers) : num_workers(num_workers) {
    worker_pids.reserve(num_workers);  // Pre-allocate to avoid reallocations
  }

  void add_task(const std::function<void(int)> &task, int task_id) {
    // If pool is full, wait for a process to finish
    if (worker_pids.size() >= num_workers) {
      // Wait for ANY child to finish (not specific one)
      int status;
      pid_t pid = wait(&status);  // Blocks here
      if (WIFEXITED(status)) {    // Check if exited normally (not killed)
        int exit_status = WEXITSTATUS(status);  // Get 8-bit exit code
        std::cout << "Task result collected: " << exit_status << std::endl;
      }

      // Remove the finished worker process from the list
      auto it = std::find(worker_pids.begin(), worker_pids.end(), pid);
      if (it != worker_pids.end()) {
        worker_pids.erase(it);
      }
    }

    pid_t pid = fork();

    if (pid == 0) {
      // Child process
      task(task_id);
      exit(task_id * 2);
    } else {
      // Parent process
      worker_pids.push_back(pid);
    }
  }

  ~ProcessPool() {
    // Wait for all worker processes to finish
    while (!worker_pids.empty()) {
      int status;
      pid_t pid = wait(&status);
      if (WIFEXITED(status)) {
        int exit_status = WEXITSTATUS(status);
        std::cout << "Task result collected: " << exit_status << std::endl;
      }
      worker_pids.pop_back();
    }
  }

private:
  size_t num_workers;
  std::vector<pid_t> worker_pids;
};

void worker_task(int task_id) {
  std::cout << "Task " << task_id << " is starting..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "Task " << task_id << " is finished." << std::endl;
}

int main() {
  const size_t num_tasks = 10;
  const size_t num_workers = 3;

  ProcessPool pool(num_workers);

  for (size_t i = 0; i < num_tasks; ++i) {
    pool.add_task(worker_task, i);
  }

  return 0;
}
