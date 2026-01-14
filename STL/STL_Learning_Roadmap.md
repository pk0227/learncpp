# 🗺️ STL Mastery Roadmap: Zero to Architect

You have a lot of content here! Don't panic.
Follow this **step-by-step sequence** to master the material efficiently.

---

## 🟢 Level 1: The Foundation (Standard STL)
*Goal: Refresh your memory on how standard containers working.*

1.  **[01_STL_Overview.md](01_STL_Overview.md)**
    *   Start here for the high-level picture.
2.  **`examples/containers/vector_examples.cpp`**
    *   Review the basics of the most important container.
3.  **`examples/containers/set_map_examples.cpp`**
    *   Understand the difference between Maps (key-value) and Sets (keys only).
4.  **[04_Container_Selection_Guide.md](04_Container_Selection_Guide.md)**
    *   **Crucial:** Learn *when* to use what. This is a common interview question.

---

## 🟡 Level 2: The Interview Core
*Goal: Solve standard coding interview problems.*

5.  **`examples/algorithms/algorithm_examples.cpp`**
    *   Skim this to remember useful tools like `std::sort`, `std::find`, `std::accumulate`.
6.  **[06_Interview_Problems.md](06_Interview_Problems.md)**
    *   **Deep Dive:** Read the problem statements and "Container Selection" logic.
7.  **`examples/interview_problems/`** (Practice these!)
    *   `problem_01_lru_cache.cpp` (Classic!)
    *   `problem_02_two_sum.cpp`
    *   `problem_07_design_twitter.cpp` (Great system design practice)

---

## 🟠 Level 3: The "Senior" Differentiators
*Goal: Understand the "gotchas" that junior devs miss.*

8.  **[05_Iterator_Invalidation.md](05_Iterator_Invalidation.md)**
    *   **Must Read:** Learn when your pointers/iterators become dangling.
9.  **`examples/containers/unordered_examples.cpp`**
    *   Understand Hash Maps vs Tree Maps (O(1) vs O(log n)).

---

## 🔴 Level 4: Architect / Staff Engineer Topics (NEW)
*Goal: Prove you can build scalable, safe, and fast systems.*

### A. Concurrency (Thread Safety)
*Start here if applying for Backend/Systems roles.*
10. **`Thread-Safe/README.md`** -> Read the theory.
11. **`Thread-Safe/thread_safe_queue.cpp`** -> The "Hello World" of concurrency.
12. **`Thread-Safe/thread_safe_map_rwlock.cpp`** -> Advanced locking (Readers-Writer).

### B. Modern C++ (C++20)
*Start here if applying for Modern C++ roles.*
13. **`Ranges/README.md`** -> Why we don't use `begin()/end()` anymore.
14. **`Ranges/ranges_demo.cpp`** -> See the clean "Pipe Syntax".

### C. Performance (Memory)
*Start here for HFT / Game Dev / Low-Latency roles.*
17. **`Memory-Optimization/README.md`** -> Creating custom allocators.
18. **`Memory-Optimization/pmr_benchmark.cpp`** -> The 3x speedup demo.

---

## 📚 Appendix: Comprehensive Reference (For Deep Dives)
*These files are excellent for reference if you get stuck on a specific container, but you don't need to memorize them all.*

### 📂 Theory & Guides (The "Why")
*   **`02_Design_Philosophy.md`**: Understand the architecture (Iterators + Containers + Algorithms).
*   **`03_Iterators_Deep_Dive.md`**: How iterators actually work (Input vs Output vs Random Access).
*   **`07_Quick_Reference.md`**: A cheat sheet for Big-O complexity and operations.
*   **`containers/sequence_containers.md`**: Detailed guide on Vector/List/Deque.
*   **`containers/associative_containers.md`**: Detailed guide on Set/Map.
*   **`containers/unordered_containers.md`**: Detailed guide on Hash Maps.
*   **`containers/container_adaptors.md`**: Detailed guide on Stack/Queue.

### 📂 Container Examples (`examples/containers/`)
*   **`vector_examples.cpp`**: The bread and butter. **(Read this)**
*   **`list_examples.cpp`** & **`forward_list_examples.cpp`**: Linked lists. Read primarily if you need `splice` operations.
*   **`deque_examples.cpp`**: Double-ended queue. Good for sliding windows.
*   **`set_map_examples.cpp`**: Sorted associative containers (Red-Black Trees).
*   **`unordered_examples.cpp`**: Hash maps/sets. **(Read this)**
*   **`adaptors_examples.cpp`**: `stack`, `queue`, `priority_queue` (Heaps).
*   **`array_examples.cpp`**: Fixed-size arrays (stack allocated).

### 📂 Algorithm Examples (`examples/algorithms/`)
*   **`algorithm_examples.cpp`**: A massive cheat sheet of `sort`, `find`, `transform`, etc.
*   **`erase_remove_idiom.cpp`**: **Critical.** You *will* be asked how to remove elements from a vector efficiently.

### 📂 Utilities (`examples/utilities/`)
*   **`functors_lambdas.cpp`**: How to write custom comparators.
*   **`utility_types.cpp`**: `std::pair`, `std::tuple`, `std::optional`, `std::variant`.

---

## �💡 How to Study
*   **Day 1:** Level 1 & 2 (Refresh fundamentals).
*   **Day 2:** Level 3 (Deep understanding).
*   **Day 3:** Level 4 (Pick **one** topic of interest, e.g., Thread Safety).
*   **Day 4:** Level 4 (Pick the remaining topics).
*   **Reference:** Use the Appendix files whenever you need specific syntax (e.g., "How do I iterator over a map again?").

**Good Luck! You are prepared for anything they throw at you.**
