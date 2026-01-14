# 🗺️ STL Mastery Roadmap: Zero to Architect

## 📅 The Strategy
This roadmap follows a strict **"Crawl, Walk, Run"** progression.
1.  **Phase 1**: Knowledge (What is it? How do I use it?)
2.  **Phase 2**: Wisdom (Which one should I use? How do I solve problems?)
3.  **Phase 3**: Optimization (How do I make it fast/safe?)

---

## 🟢 Phase 1: Verified Fundamentals (The "What" & "How")
*Goal: Understand every container type and their basic usage syntax.*

### Step 1: Theoretical Foundation (Read First)
*   **[01_STL_Overview.md](Phase1_Fundamentals/Theory/01_STL_Overview.md)** -> The Big Picture.
*   **[02_Design_Philosophy.md](Phase1_Fundamentals/Theory/02_Design_Philosophy.md)** -> Why STL exists.
*   **[07_Quick_Reference.md](Phase1_Fundamentals/Theory/07_Quick_Reference.md)** -> Big-O Cheat Sheet (Keep this open).

### Step 2: Sequence Containers (Arrays & Lists)
*   **Theory**: `Phase1_Fundamentals/Theory/containers/sequence_containers.md` (Vector vs List vs Deque)
*   **Practice**:
    *   `Phase1_Fundamentals/Code/containers/vector_examples.cpp` (**Master this**)
    *   `Phase1_Fundamentals/Code/containers/list_examples.cpp` (Linked Lists)
    *   `Phase1_Fundamentals/Code/containers/deque_examples.cpp` (Double-ended queues)
    *   `Phase1_Fundamentals/Code/containers/array_examples.cpp` (Fixed arrays)
    *   `Phase1_Fundamentals/Code/containers/forward_list_examples.cpp` (Singly linked logic)

### Step 3: Associative Containers (Trees & Hash Maps)
*   **Theory**:
    *   `Phase1_Fundamentals/Theory/containers/associative_containers.md` (Sets/Maps - Red-Black Trees)
    *   `Phase1_Fundamentals/Theory/containers/unordered_containers.md` (Unordered Maps - Hash Tables)
*   **Practice**:
    *   `Phase1_Fundamentals/Code/containers/set_map_examples.cpp` (Ordered)
    *   `Phase1_Fundamentals/Code/containers/unordered_examples.cpp` (Hash-based)

### Step 4: Adaptors (Restricted Interfaces)
*   **Theory**: `Phase1_Fundamentals/Theory/containers/container_adaptors.md`
*   **Practice**: `Phase1_Fundamentals/Code/containers/adaptors_examples.cpp` (Stack, Queue, Priority Queue)

### Step 5: Algorithms & Utilities
*   **Theory**: `Phase1_Fundamentals/Theory/03_Iterators_Deep_Dive.md` (The glue between containers and algorithms)
*   **Practice**:
    *   `Phase1_Fundamentals/Code/algorithms/algorithm_examples.cpp` (Sorting, Searching)
    *   `Phase1_Fundamentals/Theory/utilities/functors_lambdas.md` (Custom comparators - Theory)
    *   `Phase1_Fundamentals/Theory/utilities/utility_types.md` (Pairs, Tuples - Theory)

---

## 🟡 Phase 2: Intelligent Selection & Application (The "Wisdom")
*Goal: Apply your knowledge to solve problems efficiently.*

### Step 1: The Decision Framework
*   **[04_Container_Selection_Guide.md](Phase2_Selection_Application/Theory/04_Container_Selection_Guide.md)** -> **Crucial**. The "Flowchart" for interviews.
*   **[05_Iterator_Invalidation.md](Phase2_Selection_Application/Theory/05_Iterator_Invalidation.md)** -> The "Gotchas" (When does code crash?).
*   **`Phase1_Fundamentals/Code/algorithms/erase_remove_idiom.cpp`** -> A mandatory pattern for senior devs (Moved to Phase 1 Code but Phase 2 Concept).

### Step 2: Interview Patterns (Application)
*   **[06_Interview_Problems.md](Phase2_Selection_Application/Theory/06_Interview_Problems.md)** -> Top 10 patterns.
*   **Hands-on Coding**:
    *   `Phase2_Selection_Application/Code/interview_problems/problem_01_lru_cache.cpp` (Map + List)
    *   `Phase2_Selection_Application/Code/interview_problems/problem_02_two_sum.cpp` (Hash Map)
    *   `Phase2_Selection_Application/Code/interview_problems/problem_04_top_k_frequent.cpp` (Priority Queue)
    *   `Phase2_Selection_Application/Code/interview_problems/problem_05_group_anagrams.cpp` (String Hashing)
    *   `Phase2_Selection_Application/Code/interview_problems/problem_07_design_twitter.cpp` (System Design)
    *   *(See folder for full list)*

---

## 🔴 Phase 3: Architect-Level Optimization (The "Wizardry")
*Goal: Write thread-safe, memory-efficient, and modern code.*

### Step 1: Concurrency (Thread Safety)
*   **Theory**: `Phase3_Optimization/Thread-Safe/README.md`
*   **Code**:
    *   `Phase3_Optimization/Thread-Safe/thread_safe_queue.cpp` (Producer-Consumer)
    *   `Phase3_Optimization/Thread-Safe/thread_safe_map_rwlock.cpp` (Read-Heavy workloads)
    *   `Phase3_Optimization/Thread-Safe/thread_safe_vector.cpp` (Handling Reallocations)

### Step 2: Memory Optimization (Low Latency)
*   **Theory**: `Phase3_Optimization/Memory-Optimization/README.md`
*   **Code**: `Phase3_Optimization/Memory-Optimization/pmr_benchmark.cpp` (Custom Allocators)

### Step 3: Modern C++ (C++20)
*   **Theory**: `Phase3_Optimization/Ranges/README.md` (Pipe Syntax)
*   **Code**: `Phase3_Optimization/Ranges/ranges_demo.cpp` (Declarative programming)

---

## 🎓 Graduation Criteria
You are ready for your Senior Interview when:
1.  You can explain **Internal Memory Layout** of Vector vs Deque. (Phase 1)
2.  You can choose the right container for "Top K Elements" instantly. (Phase 2)
3.  You can explain how to make that container **Thread Safe**. (Phase 3)

**Good Luck!**
