# 🏗️ Design Patterns Roadmap: The Senior Architect's Path

## 📅 The Strategy: Crawl, Walk, Run
This is not just a catalogue of patterns. It is a progression from "Knowing the Syntax" to "Designing Systems".

> [!NOTE]
> **Scope Clarification**: This module focuses on **Patterns** (Architecture/Structure). Low-level *Linux Network Protocols* (Sockets, TCP/IP headers) are covered in a separate module.

---

### 🟢 Phase 1: Verified Fundamentals (The "Crawl")
*Goal: Master the standard "Gang of Four" patterns with **Modern C++20** safety/syntax.*

*   **Creational** (Object Construction)
    *   [x] **Singleton**: Thread-safe "Meyers Singleton" (No Double-Checked Locking mess).
    *   [x] **Factory**: Separating creation from usage. Factory Method vs Abstract Factory.
    *   [x] **Extensible Factory**: Dynamic registry, self-registering types, plugin framework architecture.
    *   [x] **Builder**: Fluent interfaces for complex config objects. Solves telescoping constructor.
    *   [x] **Prototype**: Clone-to-avoid-reconstruction. Deep copy via virtual `clone()` method.
*   **Structural** (Object Composition)
    *   [x] **Adapter**: Making incompatible interfaces work together. Object vs Class Adapter.
    *   [x] **Decorator**: Dynamic composition via recursive wrapping. Alternative to inheritance.
    *   [x] **Proxy**: Controlling access: Virtual (lazy), Protection, Smart Reference.
    *   [x] **Composite**: Tree hierarchies with uniform treatment. File system, UI widget trees.
    *   [x] **Flyweight**: RAM optimization via intrinsic (shared) vs extrinsic (unique) state split.
*   **Behavioral** (Object Interaction)
    *   [x] **Observer**: Event-Driven Architecture backbone. `weak_ptr` to solve Lapsed Listener.
    *   [x] **Strategy**: Runtime algorithm switching vs Template Policy Pattern.
    *   [x] **State**: Finite State Machines (FSM) without `if/else` chains.
    *   [x] **Command**: Encapsulating requests as objects. Undo/Redo, Macro, Job Queues.
    *   [x] **Iterator**: Sequential traversal without exposing internals. C++20 Ranges & Views.
    *   [x] **Visitor**: Operations on stable type hierarchies. Classic double-dispatch + `std::variant`.

---

### 🟡 Phase 2: Modern C++ Idioms (The "Walk")
*Goal: Use patterns that are unique to C++'s power (Templates, RAII, Compile-time, Data-Oriented).*

*   [x] **RAII (Resource Acquisition Is Initialization)**: The foundation of safe C++. `unique_lock`, custom file/socket wrappers.
*   [x] **Pimpl Idiom (Pointer to Implementation)**: Compilation firewalls and ABI stability.
*   [x] **CRTP (Curiously Recurring Template Pattern)**: Static Polymorphism. Zero-overhead inlining for hot paths.
*   [x] **Type Erasure**: How `std::function` and `std::any` work magic (Duck Typing in C++).
*   [x] **Component Pattern & Data-Oriented Design (ECS)**: Entities as IDs, Components as plain data structs, Systems as pure functions. Cache-friendly SOA layout for multicore scaling.

---

### 🔴 Phase 3: System Architecture (The "Run")
*Goal: Architectural patterns for Concurrency and High-Performance Systems.*

*   **Concurrency Patterns**
    *   [x] **Thread-Safe Interface**: Locking granularity. `shared_mutex` for reader-writer scenarios.
    *   [x] **Active Object**: Decoupling execution from invocation (Async task queue).
    *   [x] **Thread Pool**: Managing worker resources efficiently. `condition_variable`, graceful shutdown.
*   **Networking Architecture (Pattern Level)**
    *   [x] **Reactor**: Non-blocking Event Loops (The `epoll` architecture). Solving C10K.
    *   [ ] **Proactor**: Async Completion Handlers (The `io_uring` architecture).

---

### ⚔️ Phase 4: Interview Preparation (The "Final Boss")
*   [x] **Top Design Pattern Interview Questions**: "When to use Strategy vs State?", "Singleton vs Static Class?", "Classic Visitor vs std::variant?"
*   [x] **System Design Scenarios**: "Design a Cross-Platform Logging Library", "Design a High-Frequency Trading System", "Design a Web Server for 10k Clients".

---

## 📊 Coverage Summary

| Category | Pattern | Status | File |
|:---|:---|:---|:---|
| Creational | Singleton | ✅ | `1_Creational/Code/singleton.cpp` |
| Creational | Factory (Abstract) | ✅ | `1_Creational/Code/abstract_factory.cpp` |
| Creational | Extensible Factory | ✅ | `1_Creational/Code/extensible_factory.cpp` |
| Creational | Builder | ✅ | `1_Creational/Code/builder.cpp` |
| Creational | Prototype | ✅ | `1_Creational/Code/prototype.cpp` |
| Structural | Adapter | ✅ | `2_Structural/Code/adapter.cpp` |
| Structural | Decorator | ✅ | `2_Structural/Code/decorator.cpp` |
| Structural | Proxy | ✅ | `2_Structural/Code/proxy.cpp` |
| Structural | Composite | ✅ | `2_Structural/Code/composite.cpp` |
| Structural | Flyweight | ✅ | `2_Structural/Code/flyweight.cpp` |
| Behavioral | Observer | ✅ | `3_Behavioral/Code/observer.cpp` |
| Behavioral | Strategy | ✅ | `3_Behavioral/Code/strategy.cpp` |
| Behavioral | State | ✅ | `3_Behavioral/Code/state.cpp` |
| Behavioral | Command | ✅ | `3_Behavioral/Code/command.cpp` |
| Behavioral | Iterator | ✅ | `3_Behavioral/Code/iterator.cpp` |
| Behavioral | Visitor | ✅ | `3_Behavioral/Code/visitor.cpp` |
| C++ Idioms | RAII | ✅ | `Phase2/Code/raii_patterns.cpp` |
| C++ Idioms | Pimpl | ✅ | `Phase2/Code/pimpl_pattern.cpp` |
| C++ Idioms | CRTP | ✅ | `Phase2/Code/crtp.cpp` |
| C++ Idioms | Type Erasure | ✅ | `Phase2/Code/type_erasure.cpp` |
| C++ Idioms | Component / ECS | ✅ | `Phase2/Code/component_dod.cpp` |
| Architecture | Thread Pool | ✅ | `Phase3/Concurrency/Code/thread_pool.cpp` |
| Architecture | Reactor | ✅ | `Phase3/Networking/Code/reactor_pattern.cpp` |
| Architecture | Proactor | 🔲 | _Planned_ |
| Interview Prep | Q&A Bank | ✅ | `Phase4/01_Interview_Questions.md` |
| Interview Prep | System Design | ✅ | `Phase4/02_System_Design_Scenarios.md` |
