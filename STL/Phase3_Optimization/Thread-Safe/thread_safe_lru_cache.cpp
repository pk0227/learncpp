/**
 * Thread-Safe LRU Cache Implementation
 *
 * Demonstrates:
 * 1. Wrapping an existing data structure with thread safety (Coarse-Grained
 * Locking)
 * 2. Protecting shared state (unordered_map + list)
 * 3. RAII Locking pattern
 *
 * Senior-Level Question:
 * "How would you make your LRU Cache thread-safe?"
 * Answer: "For low contention, a single mutex protecting public methods.
 * for high contention, this approach bottlenecks. We would need sharding
 * (multiple maps/locks) or ConcurrentHashMaps (like Intel TBB or Java's
 * ConcurrentHashMap)."
 */

#include <iostream>
#include <list>
#include <mutex>
#include <optional>
#include <thread>
#include <unordered_map>
#include <vector>

template <typename Key, typename Value> class ThreadSafeLRUCache {
private:
  size_t capacity_;
  std::list<std::pair<Key, Value>> items_; // Stores {Key, Value} in LRU order
  std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator>
      cache_;

  // Mutex to protect BOTH items_ and cache_ since they must be consistent
  mutable std::mutex mutex_;

public:
  explicit ThreadSafeLRUCache(size_t capacity) : capacity_(capacity) {}

  /**
   * Get value for key.
   * Updates LRU position (moves to front) if found.
   */
  std::optional<Value> get(Key key) {
    std::lock_guard<std::mutex> lock(mutex_); // Lock entire operation

    auto it = cache_.find(key);
    if (it == cache_.end()) {
      return std::nullopt;
    }

    // Move to front (Most Recently Used)
    items_.splice(items_.begin(), items_, it->second);

    return it->second->second;
  }

  /**
   * Insert or Update key-value pair.
   * Evicts Least Recently Used item if at capacity.
   */
  void put(Key key, Value value) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = cache_.find(key);

    // Case 1: Key exists -> Update value and move to front
    if (it != cache_.end()) {
      it->second->second = value;
      items_.splice(items_.begin(), items_, it->second);
      return;
    }

    // Case 2: Key doesn't exist -> Insert
    // Check capacity first
    if (cache_.size() == capacity_) {
      // Evict LRU (back of list)
      auto lru = items_.back();
      cache_.erase(lru.first);
      items_.pop_back();
    }

    // Insert new item at front
    items_.push_front({key, value});
    cache_[key] = items_.begin();
  }

  /**
   * Helper for debugging/testing
   */
  void printCacheState() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << "Cache (Front->Back): ";
    for (const auto &pair : items_) {
      std::cout << "{" << pair.first << ":" << pair.second << "} ";
    }
    std::cout << "\n";
  }
};

// ============================================================================
// TEST HARNESS
// ============================================================================

void worker(ThreadSafeLRUCache<int, int> &cache, int id) {
  // Simulate some access patterns
  cache.put(id, id * 10);

  // Try to get values put by other threads
  auto val = cache.get(id == 1 ? 2 : 1);

  {
    // Safe print
    static std::mutex io_mutex;
    std::lock_guard<std::mutex> lock(io_mutex);
    std::cout << "Thread " << id << " put " << id << ":" << id * 10;
    if (val)
      std::cout << " | found neighbor: " << *val;
    std::cout << "\n";
  }
}

int main() {
  std::cout << "=== Thread-Safe LRU Cache ===\n";

  ThreadSafeLRUCache<int, int> cache(3); // Capacity 3

  std::vector<std::thread> threads;
  for (int i = 1; i <= 5; ++i) {
    threads.emplace_back(worker, std::ref(cache), i);
  }

  for (auto &t : threads) {
    t.join();
  }

  std::cout << "Final State:\n";
  cache.printCacheState();

  return 0;
}
