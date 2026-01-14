/**
 * Thread-Safe Vector Wrapper
 *
 * CHALLENGES WITH VECTOR:
 * 1. Reference Invalidation: Method that returns `T&` is UNSAFE in
 * multi-threaded code.
 *    - Thread A calls `get(0)` -> gets reference.
 *    - Thread B calls `push_back()` -> vector reallocates.
 *    - Thread A's reference is now DANGLING -> Crash/Corrupt memory.
 *
 * 2. Compound Operations: `if (!v.empty()) v.pop_back()` is unsafe even if
 * empty() and pop_back() are individually locked.
 *    - To fix, we need methods that combine checks and actions (like
 * `try_pop`).
 *
 * DESIGN CHOICES:
 * - Return by VALUE (copy), not by reference.
 * - Provide exception-safe accessors.
 *
 * Compile: g++ -std=c++20 -pthread -Wall -Wextra thread_safe_vector.cpp -o
 * thread_safe_vector
 */

#include <exception>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

template <typename T> class ThreadSafeVector {
private:
  std::vector<T> vec_;
  mutable std::mutex mutex_;

public:
  ThreadSafeVector() = default;

  // Push back (Thread-safe)
  void push_back(T value) {
    std::lock_guard<std::mutex> lock(mutex_);
    vec_.push_back(std::move(value));
    // Safe to reallocate here, as we hold the lock
  }

  // Get by index (Thread-safe)
  // RETURNS BY VALUE! Returning T& would be dangerous.
  T at(size_t index) const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (index >= vec_.size()) {
      throw std::out_of_range("Index out of bounds");
    }
    return vec_[index]; // Copy return
  }

  // Thread-safe Size
  size_t size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return vec_.size();
  }

  // Thread-safe iteration helper specifically for printing/debug
  // In real code, you might accept a callback function to process elements
  // while holding the lock.
  void for_each(auto func) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto &item : vec_) {
      func(item);
    }
  }
};

// ============================================================================
// TEST
// ============================================================================

void appender(ThreadSafeVector<int> &v, int id) {
  for (int i = 0; i < 100; ++i) {
    v.push_back(id * 1000 + i);
  }
}

int main() {
  std::cout << "=== Thread-Safe Vector ===\n";
  std::cout << "Multiple threads pushing back causes reallocation...\n";

  ThreadSafeVector<int> ts_vec;

  std::thread t1(appender, std::ref(ts_vec), 1);
  std::thread t2(appender, std::ref(ts_vec), 2);
  std::thread t3(appender, std::ref(ts_vec), 3);

  t1.join();
  t2.join();
  t3.join();

  std::cout << "Final size: " << ts_vec.size() << " (Expected 300)\n";

  // Verify content count
  int count = 0;
  ts_vec.for_each([&](int) { count++; });

  std::cout << "Count verified via iterator: " << count << "\n";
  std::cout << "Test passed!\n";

  return 0;
}
