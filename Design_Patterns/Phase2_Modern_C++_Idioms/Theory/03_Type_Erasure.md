# 🧠 Type Erasure Pattern
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is Type Erasure? (Real Meaning)

**Standard Definition:**
"Allow a variable to hold objects of various types."

**Senior Definition:**
*   ✅ **"Static Duck Typing with Value Semantics"**
*   ✅ **"Bridging Templates and Inheritance"**

It allows you to say: *"I don't care if you are a `Dog`, a `Car`, or a `FunctionPointer`. As long as you have a `.run()` method, I can store you."* (e.g., `std::function`, `std::any`).

---

## 🔹 2. Modern & Correct C++ Implementation
**The Wrapper Pattern (External Polymorphism)**

```cpp
class Runnable {
    // 1. Internal Concept (The Interface)
    struct Concept { 
        virtual void run() = 0; 
        virtual ~Concept() = default; 
    };
    
    // 2. Templated Model ( The Wrapper)
    template<typename T>
    struct Model : Concept {
        T data;
        Model(T x) : data(std::move(x)) {}
        void run() override { data.run(); } // Forwarding
    };
    
    std::unique_ptr<Concept> pImpl;

public:
    // 3. Templated Constructor (The Bridge)
    template<typename T>
    Runnable(T x) : pImpl(std::make_unique<Model<T>>(std::move(x))) {}

    void run() { pImpl->run(); }
};
```

---

## 🔹 3. Pros — Deep Explanation

### ✅ Pro #1: Non-Intrusive Polymorphism
*   **The Benefit**: The classes `Dog` and `Car` do **NOT** need to inherit from `IRunnable`. They just need to *have* the method.
*   **Impact**: You can wrap 3rd-party classes or built-in types without touching their source code.

### ✅ Pro #2: Value Semantics
*   **The Benefit**: You treat `Runnable` like an `int`. You can put it in a `vector`, copy it, move it. No more managing raw `Base*` pointers.

---

## 🔹 4. Cons — Deep Explanation

### ❌ Con #1: Boilerplate Hell
*   **The Issue**: Implementing a full Type Erasure wrapper (with Copy Ctor, Move Ctor, SBO) is incredibly difficult and verbose.
*   **Impact**: Hard to maintain.

### ❌ Con #2: Virtual Dispatch Cost
*   **The Issue**: Under the hood, it still uses a virtual function call (`Concept::run`). It is not as fast as a raw template.

---

## 🔹 5. When to Use / Not Use

### ❌ Do NOT use Type Erasure when:
*   A simple `std::variant` (Tagged Union) suffices (i.e., you know all possible types at compile time).
*   You control the hierarchy (just use standard Inheritance).
*   You are afraid of boilerplate.

### ✅ Acceptable use cases:
*   Generic Callback Systems (`std::function`).
*   Heterogeneous Collections of unrelated objects.
*   Library interfaces that want to accept "Any object that looks like X".

---

## 🔹 6. Senior-Level Interview Answer

> "I use Type Erasure when I need polymorphic behavior for unrelated types without forcing a shared inheritance hierarchy. The classic example is `std::function`. It gives me the best of both worlds: Value semantics (easy memory management) and Polymorphism. However, purely custom type erasure is complex to write, so I prefer `std::variant` if the set of types is closed."

---

## 🔹 7. Interview Question Bank

### Q1. How does `std::function` work?
**Answer**: "It defines an internal abstract base class for the 'Callable' concept and uses a templated derived class to wrap the specific lambda or function pointer. It often uses **Small Buffer Optimization (SBO)** to store small lambdas directly inside the object to avoid heap allocation."

### Q2. Type Erasure vs `void*`?
**Answer**: 
*   **void***: Loses ALL type info. Unsafe. You must cast it back blindly.
*   **Type Erasure**: Keeps the *Interface* (how to use it) alive while hiding the *Concrete Type*. It is type-safe."

### Q3. Type Erasure vs Templates?
**Answer**: 
*   **Templates**: Compile-time. Fast. Cannot store different types in one list.
*   **Type Erasure**: Runtime. Slower. specific types are erased, so they CAN be stored in one list."
