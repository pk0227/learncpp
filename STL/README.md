# 🚀 C++ STL Interview Workspace

**Welcome, Prashanth!**

This workspace is structured to help you master the C++ Standard Template Library (STL) for senior-level interviews.

---

## 📑 Table of Contents

1. [🧭 Where to Start?](#-where-to-start)
2. [📂 Quick Links](#-quick-links)
3. [🗺️ Curriculum Architecture](#️-curriculum-architecture)
   - [Phase 1: Fundamentals (Theory & Code)](#phase-1-fundamentals-theory--code)
   - [Phase 2: Selection & Application](#phase-2-selection--application)
   - [Phase 3: Architect-Level Optimization](#phase-3-architect-level-optimization)
4. [🛠️ Compilation Instructions](#️-compilation-instructions)

---

## 🧭 Where to Start?

> 👉 **[OPEN THE ROADMAP: STL_Learning_Roadmap.md](STL_Learning_Roadmap.md)**  
> *Follow this file for the structured "Crawl $\rightarrow$ Walk $\rightarrow$ Run" progression.*

---

## 📂 Quick Links

1. **[Master Index](Phase1_Fundamentals/Theory/1_General/00_Master_Index.md)** — Complete navigation hub for all materials, guides, and source files.
2. **[Quick Reference](Phase1_Fundamentals/Theory/1_General/07_Quick_Reference.md)** — Big-O cheat sheet and invalidation reference for last-minute review.
3. **[Container Selection Guide](Phase2_Selection_Application/Theory/04_Container_Selection_Guide.md)** — The senior-level decision flowchart.
4. **[Iterator Invalidation Guide](Phase2_Selection_Application/Theory/05_Iterator_Invalidation.md)** — Essential memory stability rules and bug avoidance.

---

## 🗺️ Curriculum Architecture

### Phase 1: Fundamentals (Theory & Code)
- **General Foundations:**
  - [STL Overview](Phase1_Fundamentals/Theory/1_General/01_STL_Overview.md)
  - [Design Philosophy](Phase1_Fundamentals/Theory/1_General/02_Design_Philosophy.md)
- **Containers:**
  - [Sequence Containers](Phase1_Fundamentals/Theory/2_Containers/sequence_containers.md)
  - [Associative Containers](Phase1_Fundamentals/Theory/2_Containers/associative_containers.md)
  - [Unordered Containers](Phase1_Fundamentals/Theory/2_Containers/unordered_containers.md)
  - [Container Adaptors](Phase1_Fundamentals/Theory/2_Containers/container_adaptors.md)
- **Iterators, Algorithms & Utilities:**
  - [Iterators Deep Dive](Phase1_Fundamentals/Theory/3_Iterators/03_Iterators_Deep_Dive.md)
  - [Algorithms Overview](Phase1_Fundamentals/Theory/4_Algorithms/algorithms_overview.md)
  - [Functors & Lambdas](Phase1_Fundamentals/Theory/5_Utilities/functors_lambdas.md)
  - [Utility Types](Phase1_Fundamentals/Theory/5_Utilities/utility_types.md)

### Phase 2: Selection & Application
- [Container Selection Guide](Phase2_Selection_Application/Theory/04_Container_Selection_Guide.md)
- [Iterator Invalidation & Pitfalls](Phase2_Selection_Application/Theory/05_Iterator_Invalidation.md)
- [Real Interview Problems Catalog](Phase2_Selection_Application/Theory/06_Interview_Problems.md)
- [Compilable Interview Solutions](Phase2_Selection_Application/Code/interview_problems/README.md)

### Phase 3: Architect-Level Optimization
- [Thread-Safe Containers](Phase3_Optimization/Thread-Safe/README.md)
- [Memory Management with std::pmr](Phase3_Optimization/Memory-Optimization/README.md)
- [Modern C++20 Ranges](Phase3_Optimization/Ranges/README.md)
- [Parallel Algorithms & Execution Policies](Phase3_Optimization/Parallel-Algorithms/README.md)

---

## 🛠️ Compilation Instructions

All code files are designed for modern C++20 compilation:

```bash
# Compile any Phase 1 or Phase 2 example:
g++ -std=c++20 -O2 -Wall -Wextra filename.cpp -o app

# Compile Phase 3 Parallel Algorithms (linking TBB if on Linux):
g++ -std=c++20 -O2 parallel_algorithms_demo.cpp -o parallel_demo -ltbb
```

---
*Happy Coding! 🚀*
