# 🧠 Strategy Design Pattern
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is Strategy? (Real Meaning)

**Standard Definition:**
"Define a family of algorithms, encapsulate each one, and make them interchangeable."

**Senior Definition:**
*   ✅ **"Runtime Dependency Injection of Logic"**
*   ✅ **"Plug-and-play Brains"**

Instead of hardcoding logic inside a `Sorter` class, you pass a `SortingStrategy`. The `Sorter` is the body; the `Strategy` is the brain.

---

## 🔹 2. Modern & Correct C++ Implementation

### A. Classic OOP (Virtual Interface)
Good for stateful strategies.

```cpp
class ISort { 
    virtual void sort(std::vector<int>& d) = 0; 
    virtual ~ISort() = default;
};

class QuickSort : public ISort { ... };

class Context {
    std::unique_ptr<ISort> strategy;
public:
    void setStrategy(std::unique_ptr<ISort> s) { strategy = std::move(s); }
};
```

### B. Modern C++ (std::function)
Lightweight, functional approach.

```cpp
class Context {
    // Holds ANY callable (function pointer, lambda, functor)
    using Strategy = std::function<void(std::vector<int>&)>;
    Strategy algorithm;

public:
    void setStrategy(Strategy s) { algorithm = s; }
    void run(std::vector<int>& d) { if(algorithm) algorithm(d); }
};

// Usage
Context ctx;
ctx.setStrategy([](auto& d) { std::sort(d.begin(), d.end()); });
```

---

## 🔹 3. Pros — Deep Explanation

### ✅ Pro #1: Flattening Inheritance Hierarchies
*   **The Benefit**: Instead of `MallardDuck`, `RubberDuck`, `DecoyDuck` (Inheritance hell), you have ONE `Duck` class with a `FlyBehavior` member.
*   **Impact**: Favor Composition over Inheritance.

### ✅ Pro #2: Runtime Switching
*   **The Benefit**: You can switch from `GPSRouting` to `CompassRouting` if the GPS signal is lost, *while the application is running*.

---

## 🔹 4. Cons — Deep Explanation

### ❌ Con #1: Client Awareness
*   **The Issue**: The Client must know *which* strategy to pick. The Client needs to understand the difference between `LRUCache` and `LFUCache`.
*   **Impact**: Leaks domain knowledge to the client.

### ❌ Con #2: Communication Overhead
*   **The Issue**: The Strategy often needs data from the Context to do its job.
*   **Impact**: You pass data via arguments. If the method signature changes, all strategies break. Or you expose `Context` internals, breaking encapsulation.

---

## 🔹 5. When to Use / Not Use

### ❌ Do NOT use Strategy when:
*   The algorithms never change. Just use a private method.
*   There are only 2 variations. Just use a boolean flag (`bool useQuickSort`). Patterns add complexity; don't use them for triviality.

### ✅ Acceptable use cases:
*   Payment Processing (CreditCard, PayPal, Crypto).
*   Compression Algorithms (Zip, Rar, 7z).
*   Pathfinding (A*, Dijkstra).

---

## 🔹 6. Senior-Level Interview Answer

> "I utilize Strategy to separate 'What we do' from 'How we do it'. In modern C++, I often skip the heavy abstract base class hierarchy and just use `std::function` for simpler, stateless strategies. This reduces boilerplate and allows me to inject Lambdas directly for testing."

---

## 🔹 7. Interview Question Bank

### Q1. Strategy vs State?
**Answer**: "Structurally identical.
*   **Strategy**: Client-driven. 'I want to format as JSON'.
*   **State**: Internal-driven. 'I am in Error Mode, so I behave differently'. The client usually doesn't manually set the state."

### Q2. Strategy vs Template Method?
**Answer**: 
*   **Template Method**: Inheritance (Static). Override a method in a subclass.
*   **Strategy**: Composition (Dynamic). Inject an object. Strategy is more testable and flexible."

### Q3. How to avoid heap allocation?
**Answer**: "Use `std::variant` (C++17) to store the strategies if the set is known and fixed. Or use `std::function` with Small Buffer Optimization (SBO)."
