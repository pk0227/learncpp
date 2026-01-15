# ⚔️ The Senior C++ Design Patterns Interview Guide

> **Rule of Thumb**: For a Senior (7+ years) role, the interviewer assumes you know *what* a Singleton is. They want to know *why* it fails, *how* to make it thread-safe, and *when* to use Dependency Injection instead.

---

## 🟢 Part 1: Creational (Object Construction)

### Q1: Why is Singleton considered an "Anti-Pattern"?
**Answer**:
1.  **Global State**: It introduces hidden dependencies. A function `foo()` might rely on `Logger::getInstance()`, but this isn't visible in the function signature `void foo()`.
2.  **Testing**: You cannot easily "Mock" a Singleton. If Test A modifies the Singleton, Test B (running in parallel) might fail.
3.  **Concurrency**: Accessing a global object requires synchronization (locks), which can become a bottleneck.

### Q2: How do you implement a Thread-Safe Singleton in C++11 or later?
**Answer**: Use the "Meyers Singleton".
```cpp
static Singleton& getInstance() {
    static Singleton instance; // Guaranteed thread-safe init by C++11 standard
    return instance;
}
```
*Note*: Avoid "Double-Checked Locking" with raw pointers unless you are an expert in `std::atomic` memory orders (`std::memory_order_acquire` / `release`).

### Q3: Factory Method vs Abstract Factory. Go.
**Answer**:
*   **Factory Method**: Rely on **Inheritance**. A class has a virtual `create()` method. Subclasses override it to return a specific product (e.g., `LinuxWindow`). Creates **ONE** product.
*   **Abstract Factory**: Rely on **Composition**. An object (the factory) is passed to the client. It has multiple methods (`createButton`, `createScrollbar`) to create a **FAMILY** of related products.

### Q4: When should you use the Builder Pattern?
**Answer**: When an object has:
1.  Many constructor parameters (avoiding the "Telescoping Constructor" anti-pattern).
2.  Optional parameters.
3.  Immutable final state (you build it, then `build()` returns a `const` object).

---

## 🟡 Part 2: Structural (Object Composition)

### Q5: Adapter vs Proxy vs Decorator. They all wrapp objects. What's the difference?
**Answer**: Focus on **Intent**.
*   **Adapter**: Changes the **Interface**. (Make Square peg fit Round hole).
*   **Decorator**: Adds **Behavior** (dynamically). (Add "Milk" to "Coffee"). Interface stays the same.
*   **Proxy**: Controls **Access**. (Lazy loading, Check permissions). Interface stays the same.

### Q6: What is the "Pimpl Idiom" and why is it crucial for C++ libraries?
**Answer**: "Pointer to Implementation".
1.  **ABI Stability**: You can add private members to the `Impl` class without changing the size of the main class. This allows you to update a DLL without forcing clients to recompile.
2.  **Compilation Speed**: Changes to private implementation details don't cascade to all including files.

### Q7: Explain the Composite Pattern. What is the trade-off?
**Answer**: Treating individual objects and compositions of objects uniformly (e.g., a File and a Folder).
*   **Trade-off**: You often have to put methods like `add()` or `remove()` in the Base Component interface, which breaks the **Interface Segregation Principle** (because a Leaf/File cannot `add()` children).

---

## 🔴 Part 3: Behavioral (Object Interaction)

### Q8: Strategy vs State. They look identical in UML.
**Answer**:
*   **Strategy**: Client configures the object: "Use the QuickSort strategy". Use this when you want to switch algorithms.
*   **State**: The Object changes itself: "I was 'Connecting', now I am 'Connected'". Use this for FSM (Finite State Machines).

### Q9: How do you implement Observer without memory leaks (Lapsed Listener)?
**Answer**:
The Subject should hold `std::weak_ptr<Observer>` instead of raw pointers or `shared_ptr`.
*   If `shared_ptr`, the Subject keeps the Observer alive forever (cycle).
*   If `weak_ptr`, the Subject can check `if (auto ptr = weak.lock())` before notifying.

---

## 🟣 Part 4: Modern C++ Idioms & Systems

### Q10: What is CRTP? Give a performance use case.
**Answer**: Curiously Recurring Template Pattern. `class Derived : public Base<Derived>`.
*   **Use Case**: Static Polymorphism. Implementing an image processing filter where calling a `virtual` function for every pixel (millions of times) is too slow. CRTP resolves the function call at compile-time (inlining).

### Q11: Explain "Type Erasure". How does `std::function` work?
**Answer**: It bridges the gap between inheritance and templates. It allows storing unrelated types that satisfy an interface.
*   It uses a Template Constructor to accept any type `T`.
*   It wraps `T` in a concrete `Model<T>` subclass that inherits from an abstract `Concept` base.
*   It stores a pointer to `Concept`.

### Q12: How do you handle C10k (10,000 connections)? Thread-per-client or...?
**Answer**: Thread-per-client fails (stack memory exhaustion).
*   **Solution**: **Reactor Pattern** (Non-blocking I/O).
*   Use `epoll` (Linux) to monitor 10k sockets on a **single thread**.
*   When a socket is readable, dispatch a lightweight task.
*   If the task is CPU-heavy, hand it off to a **Thread Pool**.

### Q13: What is the "ABA Problem" in lock-free programming?
**Answer**: In a CAS (Compare-And-Swap) loop, you read value A. Someone changes it to B, then back to A. Your CAS succeeds, but the state *did* change (e.g., the pointer A was freed and re-allocated).
*   **Solution**: Use Hazard Pointers or Tagged Pointers (Versioning).

---

## 📝 Whiteboard Challenge Checklist
1.  **Rule of 5**: Did you write the Move Constructor?
2.  **Virtual Destructor**: Did you add `virtual ~Base() = default`?
3.  **const correctness**: Did you mark getters as `const`?
4.  **noexcept**: Did you mark move ops and destructors as `noexcept`?
5.  **Smart Pointers**: No `new` or `delete`.
