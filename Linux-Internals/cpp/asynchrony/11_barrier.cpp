/*
 * BARRIER SYNCHRONIZATION WITH C++20
 * 
 * Demonstrates std::barrier for coordinating multiple threads at synchronization points.
 * 
 * BARRIER CONCEPT:
 * - Synchronization point where all threads must arrive before any can proceed
 * - Like a gate that opens only when all threads reach it
 * - Reusable: barrier resets after all threads pass through
 * 
 * USE CASES:
 * - Parallel algorithms with phases (all threads must finish phase N before starting N+1)
 * - Iterative computations where each iteration depends on previous
 * - Coordinating worker threads in parallel processing
 * 
 * KEY METHOD:
 * - arrive_and_wait(): Thread blocks until all N threads have arrived
 * - Barrier counter initialized to N (number of participating threads)
 * - When Nth thread arrives, all threads are released simultaneously
 * 
 * BARRIER vs OTHER PRIMITIVES:
 * - vs Mutex: Barrier synchronizes all threads, mutex serializes access
 * - vs Latch: Latch is one-time use, barrier can be reused for multiple phases
 * - vs Semaphore: Semaphore limits count, barrier waits for exact count
 * 
 * C++20 FEATURE:
 * - std::barrier introduced in C++20 (requires -std=c++20)
 * - Optional completion function can run when all arrive
 * - Template parameter allows custom completion behavior
 * 
 * PRACTICAL EXAMPLE:
 * - Phase 1: All tasks do initialization work
 * - Barrier: Wait for all to finish phase 1
 * - Phase 2: All tasks proceed with dependent work
 * 
 * INTERVIEW POINTS:
 * - When to use barrier vs other synchronization primitives
 * - How barriers prevent race conditions in phased algorithms
 * - Performance implications of synchronization points
 */

#include <barrier>
#include <chrono>
#include <coroutine>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 5);

// Task worker that performs work in two synchronized phases
class task_worker {
public:
  task_worker(int task_id, std::barrier<> *barrier)
      : task_id_(task_id), barrier_(barrier) {}

  void operator()() {
    phase_1();                     // Do phase 1 work
    barrier_->arrive_and_wait();   // Wait for ALL tasks to finish phase 1
    phase_2();                     // Only proceed when all are ready
  }

private:
  void phase_1() {
    std::cout << "Task " << task_id_ << " starting phase 1..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(dis(gen)));
    std::cout << "Task " << task_id_ << " finished phase 1" << std::endl;
  }

  void phase_2() {
    std::cout << "Task " << task_id_ << " starting phase 2..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(dis(gen)));
    std::cout << "Task " << task_id_ << " finished phase 2" << std::endl;
  }

  int task_id_;
  std::barrier<> *barrier_;
};

int main() {
  const int num_tasks = 5;
  std::barrier<> barrier(num_tasks);
  std::vector<std::thread> tasks;

  for (int i = 0; i < num_tasks; ++i) {
    tasks.emplace_back(task_worker(i, &barrier));
  }

  for (auto &task : tasks) {
    task.join();
  }

  return 0;
}
