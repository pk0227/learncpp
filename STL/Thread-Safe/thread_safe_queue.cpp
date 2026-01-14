/**
 * Thread-Safe Queue Implementation
 *
 * Demonstrates:
 * 1. Mutex locking (std::mutex, std::unique_lock, std::lock_guard)
 * 2. Condition Variables (std::condition_variable) for signaling
 * 3. Producer-Consumer pattern
 * 4. Thread management (std::thread, std::jthread in C++20)
 * 5. Exception safety
 *
 * Compile: g++ -std=c++20 -pthread -Wall -Wextra thread_safe_queue.cpp -o
 * thread_safe_queue
 */

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <vector>

template <typename T> class ThreadSafeQueue {
private:
  std::queue<T> queue_;
  mutable std::mutex
      mutex_; // mutable allows locking in const methods like empty()
  std::condition_variable cv_;

public:
  ThreadSafeQueue() = default;
  ThreadSafeQueue(const ThreadSafeQueue &other) {
    std::lock_guard<std::mutex> lock(other.mutex_);
    queue_ = other.queue_;
  }

  // Disable assignment for simplicity/safety
  ThreadSafeQueue &operator=(const ThreadSafeQueue &) = delete;

  /**
   * Push an item into the queue.
   * Thread-safety: Locks mutex, pushes item, notifies one waiting thread.
   */
  void push(T value) {
    {
      std::lock_guard<std::mutex> lock(mutex_); // Acquire lock
      queue_.push(std::move(value));            // Critical section
    } // Lock automatically released here

    cv_.notify_one(); // Signalling does NOT require holding the lock
  }

  /**
   * Try to pop an item from the queue (Non-blocking).
   * Returns std::optional<T> which is empty if queue was empty.
   */
  std::optional<T> try_pop() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
      return std::nullopt;
    }

    T value = std::move(queue_.front());
    queue_.pop();
    return value;
  }

  /**
   * Wait for an item to be available, then pop it (Blocking).
   * Uses condition_variable to wait efficiently without busy looping.
   */
  T wait_and_pop() {
    std::unique_lock<std::mutex> lock(
        mutex_); // unique_lock is needed for cv.wait

    // Wait until queue is not empty.
    // The lambda handles spurious wakeups automatically.
    // lock is released while waiting and re-acquired when waking up.
    cv_.wait(lock, [this] { return !queue_.empty(); });

    T value = std::move(queue_.front());
    queue_.pop();
    return value;
  }

  /**
   * Check if queue is empty.
   * Note: result is only a snapshot; queue state might change immediately after
   * returning.
   */
  bool empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
  }

  /**
   * Get size of queue.
   */
  size_t size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
  }
};

// ============================================================================
// DEMONSTRATION: PRODUCER - CONSUMER
// ============================================================================

void producer(ThreadSafeQueue<int> &q, int id, int count) {
  for (int i = 0; i < count; ++i) {
    int value = id * 100 + i;
    q.push(value);

    // Simulate work
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    {
      // Just for safe printing
      static std::mutex print_mutex;
      std::lock_guard<std::mutex> lock(print_mutex);
      std::cout << "[Producer " << id << "] Pushed " << value << "\n";
    }
  }
}

void consumer(ThreadSafeQueue<int> &q, int id) {
  while (true) {
    // We use try_pop here to allow the thread to exit if queue is empty for a
    // while In a real server, you might use wait_and_pop

    // For demo: wait_and_pop would block forever if producers stop.
    // So we'll run for a fixed time or check a "done" signal (not implemented
    // here for simplicity)

    // Let's rely on try_pop with a timeout logic, or just try_pop for demo:
    auto item = q.try_pop();
    if (item) {
      {
        static std::mutex print_mutex;
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "   [Consumer " << id << "] Popped " << *item << "\n";
      }
    } else {
      // Queue empty, wait a bit then re-check or exit
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      if (q.empty())
        break; // Exit if truly empty (naive check for partial demo)
    }
  }
}

int main() {
  std::cout << "=== Thread-Safe Queue Demo ===\n\n";

  ThreadSafeQueue<int> queue;

  // Create producer threads
  std::thread p1(producer, std::ref(queue), 1, 5); // Producer 1 pushes 100..104
  std::thread p2(producer, std::ref(queue), 2, 5); // Producer 2 pushes 200..204

  // Create consumer threads
  std::thread c1(consumer, std::ref(queue), 1);
  std::thread c2(consumer, std::ref(queue), 2);

  // Wait for producers to finish
  p1.join();
  p2.join();

  // Wait for consumers to drain queue
  // (Note: in robust code, we'd use a generic "done" flag or sentinel value)
  c1.join();
  c2.join();

  std::cout << "\nAll work completed!\n";
  return 0;
}
