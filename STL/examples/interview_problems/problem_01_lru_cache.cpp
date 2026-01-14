/**
 * LRU Cache Implementation - Interview Problem
 *
 * Design a data structure that follows the constraints of a Least Recently Used
 * (LRU) cache.
 *
 * Implement the LRUCache class:
 * - LRUCache(int capacity) Initialize the LRU cache with positive size
 * capacity.
 * - int get(int key) Return the value of the key if the key exists, otherwise
 * return -1.
 * - void put(int key, int value) Update the value of the key if the key exists.
 *   Otherwise, add the key-value pair to the cache. If the number of keys
 * exceeds the capacity from this operation, evict the least recently used key.
 *
 * Both get and put must run in O(1) average time complexity.
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 problem_01_lru_cache.cpp -o
 * lru_cache Run: ./lru_cache
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <unordered_map>

// ============================================================================
// LRU CACHE IMPLEMENTATION
// ============================================================================

class LRUCache {
private:
  int capacity;

  // List stores {key, value} pairs in LRU order
  // Front = most recently used, Back = least recently used
  std::list<std::pair<int, int>> items;

  // Map: key -> iterator to position in list
  std::unordered_map<int, std::list<std::pair<int, int>>::iterator> cache;

public:
  LRUCache(int capacity) : capacity(capacity) {}

  int get(int key) {
    // Key not found
    if (cache.find(key) == cache.end()) {
      return -1;
    }

    // Move accessed item to front (most recently used)
    // splice moves element from one position to another in O(1)
    items.splice(items.begin(), items, cache[key]);

    return cache[key]->second;
  }

  void put(int key, int value) {
    // Key already exists - update value and move to front
    if (cache.find(key) != cache.end()) {
      cache[key]->second = value;
      items.splice(items.begin(), items, cache[key]);
      return;
    }

    // Capacity check - evict LRU item (back of list)
    if (cache.size() == capacity) {
      auto lru = items.back();
      cache.erase(lru.first);
      items.pop_back();
    }

    // Insert new item at front (most recently used)
    items.push_front({key, value});
    cache[key] = items.begin();
  }

  // Helper function to print cache state
  void print() const {
    std::cout << "Cache (MRU -> LRU): ";
    for (const auto &[key, value] : items) {
      std::cout << "[" << key << ":" << value << "] ";
    }
    std::cout << "\n";
  }
};

// ============================================================================
// WHY THIS DESIGN?
// ============================================================================

/*
 * CONTAINER SELECTION ANALYSIS:
 *
 * Requirements:
 * - O(1) get by key
 * - O(1) put (insert/update)
 * - O(1) eviction of LRU item
 * - Track access order
 *
 * Options considered:
 *
 * 1. vector alone:
 *    - get: O(n) - need to search
 *    - put: O(1) append, but O(n) to check if exists
 *    - evict: O(1) if we track LRU, but O(n) to remove from middle
 *    ❌ Too slow
 *
 * 2. unordered_map alone:
 *    - get: O(1) ✓
 *    - put: O(1) ✓
 *    - evict: O(n) - can't track LRU order efficiently
 *    ❌ Can't track access order
 *
 * 3. unordered_map + vector:
 *    - get: O(1) ✓
 *    - put: O(1) ✓
 *    - evict: O(1) to find LRU, but O(n) to remove from vector
 *    ❌ Removing from vector is O(n)
 *
 * 4. unordered_map + list: ✓✓✓
 *    - get: O(1) - hash map lookup
 *    - put: O(1) - hash map insert + list push_front
 *    - evict: O(1) - list pop_back
 *    - move to front: O(1) - list::splice
 *    - iterator stability: ✓ - can store iterators in map
 *    ✅ Perfect solution!
 *
 * KEY INSIGHT: list provides:
 * - O(1) insertion/deletion at any position (if you have iterator)
 * - O(1) splice (move element to different position)
 * - Iterator stability (iterators remain valid after insertions/deletions)
 *
 * unordered_map provides:
 * - O(1) lookup by key
 * - Can store iterators as values (thanks to list's iterator stability)
 */

// ============================================================================
// TEST CASES
// ============================================================================

void testBasicOperations() {
  std::cout << "=== TEST: Basic Operations ===\n";

  LRUCache cache(2); // capacity = 2

  cache.put(1, 1);
  cache.print(); // [1:1]

  cache.put(2, 2);
  cache.print(); // [2:2] [1:1]

  assert(cache.get(1) == 1); // Returns 1, moves 1 to front
  cache.print();             // [1:1] [2:2]

  cache.put(3, 3); // Evicts key 2 (LRU)
  cache.print();   // [3:3] [1:1]

  assert(cache.get(2) == -1); // Returns -1 (not found)

  cache.put(4, 4); // Evicts key 1 (LRU)
  cache.print();   // [4:4] [3:3]

  assert(cache.get(1) == -1); // Returns -1 (not found)
  assert(cache.get(3) == 3);  // Returns 3
  assert(cache.get(4) == 4);  // Returns 4

  std::cout << "✓ All basic tests passed\n\n";
}

void testUpdateExisting() {
  std::cout << "=== TEST: Update Existing Key ===\n";

  LRUCache cache(2);

  cache.put(1, 1);
  cache.put(2, 2);
  cache.print(); // [2:2] [1:1]

  cache.put(1, 10); // Update key 1, moves to front
  cache.print();    // [1:10] [2:2]

  assert(cache.get(1) == 10);

  std::cout << "✓ Update test passed\n\n";
}

void testLargeCapacity() {
  std::cout << "=== TEST: Large Capacity ===\n";

  LRUCache cache(100);

  // Fill cache
  for (int i = 0; i < 100; ++i) {
    cache.put(i, i * 10);
  }

  // Access some elements
  assert(cache.get(50) == 500);
  assert(cache.get(75) == 750);

  // Add one more (should evict 0, which is LRU)
  cache.put(100, 1000);
  assert(cache.get(0) == -1); // Evicted
  assert(cache.get(1) == 10); // Still there

  std::cout << "✓ Large capacity test passed\n\n";
}

void testCapacityOne() {
  std::cout << "=== TEST: Capacity = 1 ===\n";

  LRUCache cache(1);

  cache.put(1, 1);
  assert(cache.get(1) == 1);

  cache.put(2, 2); // Evicts 1
  assert(cache.get(1) == -1);
  assert(cache.get(2) == 2);

  cache.put(3, 3); // Evicts 2
  assert(cache.get(2) == -1);
  assert(cache.get(3) == 3);

  std::cout << "✓ Capacity=1 test passed\n\n";
}

// ============================================================================
// ALTERNATIVE IMPLEMENTATION (using std::map for ordered iteration)
// ============================================================================

class LRUCacheOrdered {
private:
  int capacity;
  int timestamp = 0;

  // Map: key -> {value, timestamp}
  std::unordered_map<int, std::pair<int, int>> cache;

public:
  LRUCacheOrdered(int capacity) : capacity(capacity) {}

  int get(int key) {
    if (cache.find(key) == cache.end()) {
      return -1;
    }

    // Update timestamp
    cache[key].second = ++timestamp;
    return cache[key].first;
  }

  void put(int key, int value) {
    if (cache.find(key) != cache.end()) {
      cache[key] = {value, ++timestamp};
      return;
    }

    // Evict if full
    if (cache.size() == capacity) {
      // Find LRU (smallest timestamp)
      auto lru = std::min_element(cache.begin(), cache.end(),
                                  [](const auto &a, const auto &b) {
                                    return a.second.second < b.second.second;
                                  });
      cache.erase(lru);
    }

    cache[key] = {value, ++timestamp};
  }
};

/*
 * COMPARISON: List-based vs Timestamp-based
 *
 * List-based (LRUCache):
 * - get: O(1)
 * - put: O(1)
 * - evict: O(1)
 * - Space: O(n) for list + O(n) for map = O(n)
 * ✅ Better for interviews (true O(1))
 *
 * Timestamp-based (LRUCacheOrdered):
 * - get: O(1)
 * - put: O(1) if no eviction, O(n) if eviction (need to find min timestamp)
 * - evict: O(n)
 * - Space: O(n)
 * ❌ Not true O(1) for eviction
 *
 * VERDICT: List-based is superior for LRU cache!
 */

// ============================================================================
// MAIN
// ============================================================================

int main() {
  std::cout << "LRU Cache Implementation\n";
  std::cout << "========================\n\n";

  testBasicOperations();
  testUpdateExisting();
  testLargeCapacity();
  testCapacityOne();

  std::cout << "All tests passed! ✓\n";

  return 0;
}
