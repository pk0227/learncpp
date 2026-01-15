/**
 * @file thread_pool.cpp
 * @brief Thread Pool Pattern (System Architecture)
 *
 * THE PROBLEM:
 * Creating a std::thread for every task is expensive (kernel overhead).
 *
 * THE SOLUTION:
 * A pool of worker threads that wake up, execute a task from a queue, and sleep
 * again.
 *
 * KEY FEATURES:
 * - std::queue<std::function<void()>> for tasks.
 * - std::condition_variable for signaling.
 * - Graceful shutdown logic.
 */

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
  // Initialize pool with hardware_concurrency threads
  explicit ThreadPool(size_t threads = std::thread::hardware_concurrency()) {
    for (size_t i = 0; i < threads; ++i) {
      workers_.emplace_back([this] {
        while (true) {
          std::function<void()> task;
          {
            // Wait for a task or stop signal
            std::unique_lock<std::mutex> lock(queue_mutex_);
            condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });

            // Exit if stopped and queue is empty
            if (stop_ && tasks_.empty()) {
              return;
            }

            // Grab the task
            task = std::move(tasks_.front());
            tasks_.pop();
          }

          // Execute task outside the lock!
          task();
        }
      });
    }
  }

  // Enqueue a generic void() function or lambda
  template <class F, class... Args>
  auto enqueue(F &&f, Args &&...args)
      -> std::future<typename std::invoke_result<F, Args...>::type> {

    using return_type = typename std::invoke_result<F, Args...>::type;

    // Wrap the task in a packaged_task to retrieve the return value later
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);

      if (stop_) {
        throw std::runtime_error("enqueue on stopped ThreadPool");
      }

      // Store type-erased void() lambda
      tasks_.emplace([task]() { (*task)(); });
    }

    condition_.notify_one();
    return res;
  }

  // Destructor joins all threads
  ~ThreadPool() {
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);
      stop_ = true;
    }
    condition_.notify_all(); // Wake up everyone to die

    for (std::thread &worker : workers_) {
      if (worker.joinable()) {
        worker.join();
      }
    }
  }

private:
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;

  std::mutex queue_mutex_;
  std::condition_variable condition_;
  bool stop_ = false;
};

// ===================================
// DEMONSTRATION
// ===================================
int main() {
  std::cout << "--- Thread Pool Demo ---\n";
  ThreadPool pool(4);
  std::vector<std::future<int>> results;

  // Launch 8 tasks on 4 threads
  for (int i = 0; i < 8; ++i) {
    results.emplace_back(pool.enqueue([i] {
      std::cout << "Task " << i << " running on thread "
                << std::this_thread::get_id() << "\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      return i * i;
    }));
  }

  for (auto &&result : results) {
    std::cout << "Result: " << result.get() << "\n";
  }

  return 0;
}
