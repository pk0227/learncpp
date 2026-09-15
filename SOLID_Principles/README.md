# 🏛️ Modern C++ SOLID Principles: The Senior Architect's Guide

## Welcome to the Senior Engineer & Architect Workshop

This curriculum is purpose-built for **Senior and Staff C++ Software Engineers (8–10+ Years of Experience)** preparing for senior technical interviews, staff architecture rounds, and high-performance system design.

In academic courses and introductory materials, the SOLID principles are frequently presented using toy examples (e.g., shapes, animal noises, basic logging). In real-world, large-scale, high-performance C++ engineering, those simplistic abstractions fall apart. An 8–10 YoE C++ developer must understand SOLID through the lens of:
- **Zero-Cost Abstractions**: When to choose compile-time polymorphism (CRTP, C++20 Concepts, Policy-Based Design) over virtual function dispatch (`vtable` indirection, branch mispredictions, and d-cache misses).
- **Memory & Resource Ownership**: Expressing dependency relationships via value semantics, `std::unique_ptr`, `std::shared_ptr`, and non-owning borrowed views (`std::string_view`, `std::span`, references).
- **Physical Design & ABI Stability**: Header decoupling, compilation firewalls (Pimpl idiom), preventing header pollution, and avoiding recompilation cascades in multi-million-line codebases (John Lakos' Large-Scale C++ principles).
- **Formal Verification & Contracts**: Design by Contract (DbC), precondition contravariance, postcondition covariance, class invariants, and exception safety guarantees (`noexcept`).
- **Modern C++ Paradigm Evolution**: How C++20/23 concepts, ranges, and `std::variant`/pattern matching transform traditional GoF object-oriented patterns into type-safe, cache-friendly architectures.

---

## 🗺️ Curriculum Navigation

| Directory / Module | Core Focus | Key Architectural Concepts | Code Examples |
|:---|:---|:---|:---:|
| **[Roadmap](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/SOLID_Learning_Roadmap.md)** | Full Learning Path | Crawl $\rightarrow$ Walk $\rightarrow$ Run progression & Interview Matrix | — |
| **[01. Single Responsibility (SRP)](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/01_Single_Responsibility_Principle/Single_Responsibility_Principle.md)** | Separation of Concerns | LCOM metrics, SLAP, God-class refactoring, Policy-based design | 3 `.cpp` |
| **[02. Open/Closed (OCP)](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/02_Open_Closed_Principle/Open_Closed_Principle.md)** | Extensibility & Invariants | NVI idiom, CRTP, `std::variant` visitors, Plugin architecture, ABI stability | 4 `.cpp` |
| **[03. Liskov Substitution (LSP)](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/03_Liskov_Substitution_Principle/Liskov_Substitution_Principle.md)** | Behavioral Subtyping | Design by Contract (DbC), Covariance/Contravariance, Object Slicing, Concepts | 4 `.cpp` |
| **[04. Interface Segregation (ISP)](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/04_Interface_Segregation_Principle/Interface_Segregation_Principle.md)** | Role Interfaces & Decoupling | Physical header bloat, recompilation cascades, multiple inheritance, Static ISP | 3 `.cpp` |
| **[05. Dependency Inversion (DIP)](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/05_Dependency_Inversion_Principle/Dependency_Inversion_Principle.md)** | Abstraction Over Concrete | DIP vs DI vs IoC, ownership semantics, Compile-time DI, Hermetic Mocks | 4 `.cpp` |
| **[06. Senior Architect Mastery](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/06_Senior_Architect_SOLID_Mastery/SOLID_Synergy_and_Modern_Cpp.md)** | Synergy & Interview Playbook | 5-Principle synergy, HFT Order Gateway, Telemetry Engine, 25+ Interview Q&As | 2 `.cpp` |

---

## ⚙️ Compilation & Standards

All examples in this curriculum require **C++20** (tested and clean under GCC 11+, Clang 13+, and MSVC 2019+).

To compile any standalone example:
```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -O2 <filename>.cpp -o output && ./output
```

To run a syntax-only validation:
```bash
g++ -std=c++20 -fsyntax-only <filename>.cpp
```
