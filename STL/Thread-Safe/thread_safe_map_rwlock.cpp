/**
 * Thread-Safe Map using Readers-Writer Lock (C++17)
 *
 * STANDARD STL MAP/UNORDERED_MAP IS NOT THREAD-SAFE!
 * - Multiple readers: SAFE
 * - One writer, multiple readers: UNSAFE (Data Race)
 *
 * Solution: Readers-Writer Lock pattern using `std::shared_mutex`
 * - READERS (get, find, count): Shared Lock (`std::shared_lock`) -> Multiple
 * threads can read simultaneously.
 * - WRITERS (put, erase): Unique Lock (`std::unique_lock`) -> Only one thread
 * can write (blocks everyone else).
 *
 * Performance:
 * Significantly faster than `std::mutex` when Reads >> Writes (typical for
 * caches/config).
 *
 * Compile: g++ -std=c++17 -pthread -Wall -Wextra thread_safe_map_rwlock.cpp -o
 * thread_safe_map
 */

#include <chrono>
#include <iostream>
#include <mutex>
#include <optional>
#include <shared_mutex> // C++17
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

template <typename Key, typename Value> class ThreadSafeMap {
private:
  std::unordered_map<Key, Value> map_;
  mutable std::shared_mutex rw_mutex_; // key component!

public:
  // WRITER: Insert or Update
  // Uses std::unique_lock for EXCLUSIVE access
  void put(Key key, Value value) {
    std::unique_lock<std::shared_mutex> lock(rw_mutex_);
    map_[key] = value;
    // Lock released here
  }

  // READER: Get value
  // Uses std::shared_lock for SHARED access (multiple threads can be here)
  std::optional<Value> get(Key key) const {
    std::shared_lock<std::shared_mutex> lock(rw_mutex_);
    auto it = map_.find(key);
    if (it != map_.end()) {
      return it->second;
    }
    return std::nullopt;
  }

  // READER: Check existence
  bool contains(Key key) const {
    std::shared_lock<std::shared_mutex> lock(rw_mutex_);
    return map_.find(key) != map_.end();
  }

  // WRITER: Erase
  void erase(Key key) {
    std::unique_lock<std::shared_mutex> lock(rw_mutex_);
    map_.erase(key);
  }
};

// ============================================================================
// PERFORMANCE DEMO: Readers vs Writers
// ============================================================================

void reader(ThreadSafeMap<int, std::string> &map, int id) {
  for (int i = 0; i < 50; ++i) {
    auto val = map.get(i % 5); // Read keys 0-4 repeatedly
    // Simulate read processing
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  // Just to show activity
  static std::mutex print_mut;
  std::lock_guard<std::mutex> lock(print_mut);
  std::cout << "Reader " << id << " finished.\n";
}

void writer(ThreadSafeMap<int, std::string> &map, int id) {
  for (int i = 0; i < 10; ++i) {
    map.put(i % 5, "Value_" + std::to_string(id) + "_" + std::to_string(i));
    // Simulate slower write
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  static std::mutex print_mut;
  std::lock_guard<std::mutex> lock(print_mut);
  std::cout << "Writer " << id << " finished.\n";
}

int main() {
  std::cout << "=== Thread-Safe Map (Readers-Writer Lock) ===\n";
  std::cout << "Starting 10 readers and 2 writers...\n\n";

  ThreadSafeMap<int, std::string> map;

  // Populate initial data
  for (int i = 0; i < 5; ++i)
    map.put(i, "Init");

  std::vector<std::thread> threads;

  // Start 10 readers (High concurrency desired)
  for (int i = 0; i < 10; ++i) {
    threads.emplace_back(reader, std::ref(map), i);
  }

  // Start 2 writers (Low concurrency, blocks readers)
  for (int i = 0; i < 2; ++i) {
    threads.emplace_back(writer, std::ref(map), i + 100);
  }

  for (auto &t : threads) {
    t.join();
  }

  std::cout << "\nOperation complete. Final contents:\n";
  for (int i = 0; i < 5; ++i) {
    auto val = map.get(i);
    if (val)
      std::cout << i << ": " << *val << "\n";
  }

  return 0;
}
