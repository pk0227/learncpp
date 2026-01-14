# 🧵 Concurrency & STL: The Senior Developer's Guide

> **The Golden Rule:** Standard STL containers are **NOT** thread-safe for write operations.
> You must bring your own synchronization (Mutexes, Atomic, etc.)

---

## 🚦 Thread Safety Levels

1.  **Thread-Unsafe** (Standard STL)
    *   Simultaneous Reads: ✅ **Safe**
    *   Simultaneous Writes: ❌ **Data Race** (Undefined Behavior)
    *   Read + Write: ❌ **Data Race**

2.  **Thread-Safe** (Java's `Vector`, or our wrappers)
    *   Internal synchronization prevents data races.
    *   Individual operations (`push`, `pop`) are atomic.
    *   **Pitfall:** Compound operations are still unsafe without external locks!

---

## 🛡️ Thread-Safe Implementations in this Directory

| Container | Strategy | File | Best Used For |
| :--- | :--- | :--- | :--- |
| **Queue** | `mutex` + `condition_variable` | `thread_safe_queue.cpp` | Producer-Consumer, Task Scheduling |
| **Map / Set** | `shared_mutex` (Readers-Writer) | `thread_safe_map_rwlock.cpp` | Caches, Configuration (Read-Heavy) |
| **Vector** | `mutex` (Copy on return) | `thread_safe_vector.cpp` | Logging, Collecting Results |
| **LRU Cache** | `mutex` (Coarse Grained) | `thread_safe_lru_cache.cpp` | Caching (Simple wrapper) |

---

## ⚠️ The "Reference Trap" (Interview Favorite)

**Question:** "Why can't I just wrap `std::vector` with a mutex and return `T& operator[](int i)`?"

**Answer:**
1.  Thread A gets reference: `int& x = vec[0];`
2.  Thread B does `vec.push_back(5);` -> **Triggers Reallocation!**
3.  Thread A's reference `x` is now pointing to freed memory. **CRASH.**

**Solution:**
*   Return by Value (Copy): `T get(int i)`
*   Or return `shared_ptr<T>` (Reference counting)

---

## 🔑 Key Synchronization Primitives

1.  **`std::mutex`**: Basic mutual exclusion.
2.  **`std::lock_guard<mutex>`**: RAII wrapper (locks on create, unlocks on destruct). **Always use this.**
3.  **`std::unique_lock<mutex>`**: Flexible lock (can unlock manualy). Required for `condition_variable`.
4.  **`std::shared_mutex` (C++17)**:
    *   `shared_lock`: Reading (Shared ownership)
    *   `unique_lock`: Writing (Exclusive ownership)
    *   *Great for Maps!*
5.  **`std::condition_variable`**: "Wait until X happens". Efficient signaling.

---

## 💡 Senior Tips
*   **Contention**: A single mutex effectively serializes your multi-threaded code. Only use it for short critical sections.
*   **Sharding**: For a Hash Map, create an array of 16 mutexes. Hash the key to pick which mutex to lock. This reduces contention (Java's ConcurrentHashMap approach).
*   **Lock-Free**: `std::atomic` is lock-free but hard to use for complex containers. Stick to locks for containers in interviews unless asked for "Lock-Free".
