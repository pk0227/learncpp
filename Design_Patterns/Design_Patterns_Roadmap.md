# 🏗️ Design Patterns Roadmap: The Senior Architect's Path

## 📅 The Strategy: Crawl, Walk, Run
This is not just a catalogue of patterns. It is a progression from "Knowing the Syntax" to "Designing Systems".

> [!NOTE]
> **Scope Clarification**: This module focuses on **Patterns** (Architecture/Structure). Low-level *Linux Network Protocols* (Sockets, TCP/IP headers) are covered in a separate module.

### 🟢 Phase 1: Verified Fundamentals (The "Crawl")
*Goal: Master the standard "Gang of Four" patterns with **Modern C++20** safety/syntax.*

*   **Creational** (Object Construction)
    *   [ ] **Singleton**: Thread-safe "Meyers Singleton" (No Double-Checked Locking mess).
    *   [ ] **Factory**: Separating creation from usage.
    *   [ ] **Builder**: Fluent interfaces for complex config objects.
*   **Structural** (Object Composition)
    *   [ ] **Adapter**: Making incompatible interfaces work together.
    *   [ ] **Decorator**: Is dynamic composition better than inheritance?
    *   [ ] **Proxy**: Controlling access (and the Pimpl Idiom prelude).
*   **Behavioral** (Object Interaction)
    *   [ ] **Observer**: The backbone of Event Driven Architecture (Signals/Slots).
    *   [ ] **Strategy**: Runtime algorithm switching (vs Template Policy Pattern).
    *   [ ] **State**: Finite State Machines (FSM) without `if/else` chains.

### 🟡 Phase 2: Modern C++ Idioms (The "Walk")
*Goal: Use patterns that are unique to C++'s power (Templates, RAII, Compile-time).*

*   [ ] **CRTP (Curiously Recurring Template Pattern)**: Static Polymorphism. (High Performance).
*   [ ] **Pimpl Idiom (Pointer to Implementation)**: Compilation firewalls and ABI stability.
*   [ ] **Type Erasure**: How `std::function` and `std::any` work magic (Duck Typing in C++).

### 🔴 Phase 3: System Architecture (The "Run")
*Goal: Architectural patterns for Concurrency and High-Performance Systems.*

*   **Concurrency Patterns**
    *   [ ] **Thread-Safe Interface**: Locking granularity.
    *   [ ] **Active Object**: Decoupling execution from invocation (Async).
    *   [ ] **Thread Pool**: Managing worker resources efficiently.
*   **Networking Architecture (Pattern Level)**
    *   [ ] **Reactor**: Non-blocking Event Loops (The `epoll` architecture).
    *   [ ] **Proactor**: Async Completion Handlers (The `io_uring` architecture).

### ⚔️ Phase 4: Interview Preparation (The "Final Boss")
*   [ ] **Top 20 Design Pattern Questions**: "When to use Strategy vs State?", "Singleton vs Static Class?"
*   [ ] **System Design Scenarios**: "Design a Cross-Platform Logging Library", "Design a Thread-Safe Notification System".
