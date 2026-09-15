# 🧵 Concurrency & STL: The Senior Developer's Guide

> **The Golden Rule:** Standard STL containers are **NOT** thread-safe for concurrent mutating operations. You must provide your own synchronization (Mutexes, Read/Write locks, Atomics, or Lock-Free structures).

---

## 📑 Table of Contents

1. [🚦 STL Thread-Safety Guarantees](#-stl-thread-safety-guarantees)
2. [🛡️ Thread-Safe Container Implementations in this Directory](#️-thread-safe-container-implementations-in-this-directory)
3. [⚠️ The "Reference Trap" (Interview Favorite)](#️-the-reference-trap-interview-favorite)
4. [🔑 Essential Synchronization Primitives](#-essential-synchronization-primitives)
5. [💡 Senior Architecture Patterns](#-senior-architecture-patterns)
   - [1. Lock Striping / Sharding](#1-lock-striping--sharding)
   - [2. Readers-Writer RWLock Pattern](#2-readers-writer-rwlock-pattern)
   - [3. Producer-Consumer Queue Pattern](#3-producer-consumer-queue-pattern)
6. [🎓 Senior Interview Cheat Sheet & Q&A](#-senior-interview-cheat-sheet--qa)
7. [📁 Code Examples](#-code-examples)

---

## 🚦 STL Thread-Safety Guarantees

The ISO C++ standard specifies thread safety for standard containers as follows:

1. **Concurrent Reads are Safe:**
   Multiple threads can safely read from the same container concurrently (e.g., calling `find`, `size`, `operator[]` on `const` instances) without any locking.
2. **Concurrent Modifications Cause Data Races:**
   If at least one thread modifies a container (e.g., `insert`, `push_back`, `erase`), all other concurrent accesses (reads or writes) must be externally synchronized. Otherwise, it is a **data race** resulting in **undefined behavior**.
3. **Different Container Instances:**
   Operations on different container instances performed by separate threads are always thread-safe.

```
Thread A: Read container[0]  ──┐
                               ├──► ✅ SAFE (Concurrent reads)
Thread B: Read container[1]  ──┘

Thread A: Read container[0]  ──┐
                               ├──► ❌ DATA RACE (Undefined Behavior!)
Thread B: Write container[1] ──┘
```

---

## 🛡️ Thread-Safe Container Implementations in this Directory

| Container Wrapper | Synchronization Strategy | Source File | Ideal Use Case |
|---|---|---|---|
| **Thread-Safe Queue** | `std::mutex` + `std::condition_variable` | [`thread_safe_queue.cpp`](thread_safe_queue.cpp) | Producer-Consumer pipelines, thread pools, job queues |
| **Thread-Safe Map** | `std::shared_mutex` (C++17 Readers-Writer lock) | [`thread_safe_map_rwlock.cpp`](thread_safe_map_rwlock.cpp) | Read-heavy caches, routing tables, configuration maps |
| **Thread-Safe Vector** | `std::mutex` with return-by-value | [`thread_safe_vector.cpp`](thread_safe_vector.cpp) | Append-only logging, metric collection |
| **Thread-Safe LRU Cache** | `std::mutex` guarding Map + List splice | [`thread_safe_lru_cache.cpp`](thread_safe_lru_cache.cpp) | Thread-safe bounded in-memory caching |

---

## ⚠️ The "Reference Trap" (Interview Favorite)

> **Interview Question:** *"Why can't I just wrap `std::vector` inside a class with a mutex and return `T& operator[](size_t i)`?"*

```cpp
// ❌ CRITICAL ARCHITECTURAL FLAW:
class BrokenThreadSafeVector {
    std::vector<int> data;
    std::mutex mtx;
public:
    int& operator[](size_t i) {
        std::lock_guard<std::mutex> lock(mtx);
        return data[i]; // ⚠️ Returning reference releases lock immediately!
    }
};
```

### The Failure Sequence:
1. **Thread A** calls `int& val = vec[0];`. Mutex locks, reference to element 0 is returned, and mutex **unlocks**.
2. **Thread B** calls `vec.push_back(42);`. Mutex locks. Vector capacity is full $\rightarrow$ vector **reallocates memory**, copying elements to a new heap buffer, and deletes the old heap memory!
3. **Thread A** attempts to read or write `val`.
4. **Result:** Thread A accesses freed memory $\rightarrow$ **Use-After-Free / Segmentation Fault / Memory Corruption!**

### The Senior Solution:
- **Return by Value (Copy):** `T get(size_t i)` returns a copied object safely under the lock.
- **Return Smart Pointer:** Return `std::shared_ptr<const T>` to keep ownership alive.
- **Callback Invocable:** `void with_element(size_t i, std::invocable<const T&> auto func)` executes the lambda while the lock is held.

---

## 🔑 Essential Synchronization Primitives

1. **`std::mutex`:** Basic mutual exclusion primitive.
2. **`std::lock_guard<std::mutex>`:** Strict RAII wrapper; locks in constructor, unlocks in destructor. Always prefer this over manual `lock()`/`unlock()`.
3. **`std::scoped_lock` (C++17):** Deadlock-free RAII lock for **multiple** mutexes simultaneously (uses deadlock avoidance algorithm):
   ```cpp
   std::scoped_lock lock(mtx1, mtx2); // Locks both without deadlock
   ```
4. **`std::unique_lock<std::mutex>`:** Flexible RAII wrapper supporting deferred locking, manual unlocking, and passing to `std::condition_variable`.
5. **`std::shared_mutex` (C++17):**
   - `std::shared_lock<std::shared_mutex>`: Shared read lock (multiple readers allowed).
   - `std::unique_lock<std::shared_mutex>`: Exclusive write lock (exclusive single writer).
6. **`std::condition_variable`:** Blocking notification mechanism; puts waiting threads to sleep until signaled via `notify_one()` or `notify_all()`.

---

## 💡 Senior Architecture Patterns

### 1. Lock Striping / Sharding

A single global mutex around a large `std::unordered_map` bottlenecks high-throughput multi-threaded servers.

**Lock Striping Solution:**
- Partition the container into $N$ independent shards (e.g., $N = 16$ or $32$).
- Each shard contains its own bucket map and its own independent `std::shared_mutex`.
- Hash the key to determine which shard to lock:
  ```cpp
  size_t shard_idx = std::hash<Key>{}(key) % NUM_SHARDS;
  ```
- Result: 16 concurrent threads can perform writes simultaneously without blocking one another!

### 2. Readers-Writer RWLock Pattern

```cpp
#include <shared_mutex>
#include <unordered_map>
#include <optional>

template<typename K, typename V>
class ThreadSafeLookup {
    std::unordered_map<K, V> table;
    mutable std::shared_mutex rw_mtx;

public:
    std::optional<V> get(const K& key) const {
        std::shared_lock<std::shared_mutex> lock(rw_mtx); // Shared read lock
        auto it = table.find(key);
        if (it != table.end()) return it->second;
        return std::nullopt;
    }

    void set(const K& key, const V& val) {
        std::unique_lock<std::shared_mutex> lock(rw_mtx); // Exclusive write lock
        table[key] = val;
    }
};
```

### 3. Producer-Consumer Queue Pattern

```cpp
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template<typename T>
class ThreadSafeQueue {
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cv;

public:
    void push(T item) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push(std::move(item));
        }
        cv.notify_one();
    }

    T wait_and_pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !queue.empty(); });
        T item = std::move(queue.front());
        queue.pop();
        return item;
    }
};
```

---

## 🎓 Senior Interview Cheat Sheet & Q&A

### Q1: Are container read operations thread-safe?
**A:** Yes. Multiple threads can concurrently read from the same container instance without synchronization, provided there are no concurrent writes.

### Q2: Why is `queue::pop()` split into `front()` and `pop()` in standard STL, and why does this cause issues in multithreading?
**A:** In standard STL, `pop()` returns `void` for exception safety: if `pop()` returned by value, copy-constructing the return object could throw an exception, in which case the element would have been popped from the queue and lost. However, in multithreaded wrappers, checking `empty()`, reading `front()`, and calling `pop()` in separate steps creates a race condition between threads. A thread-safe queue must combine popping and returning under a single atomic lock.

---

## 📁 Code Examples

- [`thread_safe_queue.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase3_Optimization/Thread-Safe/thread_safe_queue.cpp): Thread-safe queue using condition variables and RAII locks.
- [`thread_safe_map_rwlock.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase3_Optimization/Thread-Safe/thread_safe_map_rwlock.cpp): Read-heavy concurrent map using `std::shared_mutex`.
- [`thread_safe_vector.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase3_Optimization/Thread-Safe/thread_safe_vector.cpp): Safe vector wrapper demonstrating return-by-value to prevent dangling reference traps.
- [`thread_safe_lru_cache.cpp`](file:///home/prashanth/Learnings/learncpp/STL/Phase3_Optimization/Thread-Safe/thread_safe_lru_cache.cpp): Thread-safe LRU Cache wrapping list splicing and hash lookup.
