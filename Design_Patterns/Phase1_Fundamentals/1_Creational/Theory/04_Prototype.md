# 🧠 Prototype Design Pattern
> **Deep, Practical, Senior-Level Understanding**
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is Prototype? (Real Meaning)

**Standard Definition:**
"Specify the kinds of objects to create using a prototypical instance, and create new objects by copying this prototype."

**Senior Definition:**
*   ✅ **"Clone-to-Avoid-Reconstruction"**
*   ✅ **"Deferred Initialization via Copying"**
*   ✅ **"The Foundation of Object Pools and Plugin Registries"**

It solves the problem of **expensive initialization**. Instead of parsing a config file or fetching from a DB every time, you parse once, cache the result as a prototype, and `clone()` it cheaply.

---

## 🔹 2. Modern & Correct C++ Implementation

**The Prototype Interface:**
```cpp
class Shape {
public:
    // The key: polymorphic deep copy through base pointer
    virtual std::unique_ptr<Shape> clone() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double radius_;
    std::string color_;
public:
    Circle(double r, std::string c) : radius_(r), color_(std::move(c)) {}

    // Copy ctor handles deep copy (= default works if no raw pointers)
    Circle(const Circle&) = default;

    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Circle>(*this); // Delegates to copy ctor
    }
};
```

**Prototype Registry (named template store):**
```cpp
class ShapeRegistry {
    std::unordered_map<std::string, std::unique_ptr<Shape>> prototypes_;
public:
    void registerPrototype(std::string key, std::unique_ptr<Shape> s) {
        prototypes_[std::move(key)] = std::move(s);
    }
    std::unique_ptr<Shape> create(const std::string& key) const {
        return prototypes_.at(key)->clone(); // Returns a fresh deep copy
    }
};
```

---

## 🔹 3. Pros — Deep Explanation

### ✅ Pro #1: Avoids Expensive Re-initialization
*   **The Benefit**: Clone a fully-initialized object instead of rebuilding from scratch (DB query, file parse, network call).
*   **Example**: A game level — parse and initialize once, then `clone()` for each new playthrough session.

### ✅ Pro #2: Decouples Client from Concrete Types
*   **The Benefit**: Client calls `prototype->clone()` without knowing if it's a `Circle`, `Square`, or `HexagonWithHole`.
*   **Impact**: Excellent for plugin architectures where concrete types are dynamically registered at runtime.

### ✅ Pro #3: Thread Safety by Independence
*   **The Benefit**: Each clone is a **completely independent new object**. No shared mutable state between the original and its clone.
*   **Impact**: Multiple threads can use cloned objects concurrently without any locks.

---

## 🔹 4. Cons — Deep Explanation

### ❌ Con #1: Deep Copy Complexity
*   **The Issue**: If the object contains raw pointers, circular references, or OS handles (file descriptors, sockets), writing a correct deep copy is non-trivial.
*   **Impact**: A shallow copy leaves both original and clone pointing to the same sub-object, leading to double-free errors or silent data corruption.
*   **Rule**: If all members are value types or smart pointers, `= default` copy constructor works correctly. Otherwise, write it explicitly.

### ❌ Con #2: clone() vs Copy Constructor Confusion
*   **The Issue**: C++ already has copy constructors. Why add a virtual `clone()`?
*   **Answer**: Copy constructors require knowing the **concrete type at compile time**. `clone()` works **polymorphically** through a base class pointer — you can duplicate a `Shape*` without knowing if it's a `Circle` or `Rectangle`.

---

## 🔹 5. When to Use / Not Use

### ❌ Do NOT use Prototype when:
*   Object creation is cheap (just call `make_unique`).
*   There is no polymorphic hierarchy (just use the copy constructor directly).
*   The object has complex non-copyable resources (OS handles, GPU buffers) that cannot be duplicated.

### ✅ Acceptable use cases:
*   **Game Dev**: Spawning many enemy entities from a single template ("boss prefab").
*   **Document Editors**: "Duplicate Layer" in Photoshop or "Copy Node" in a node graph.
*   **Object Pools**: Pool recycles prototype instances instead of constructing new ones from zero.
*   **Plugin Systems**: Factories that `register` and `clone()` type-erased objects.
*   **Configuration Objects**: Clone a "default config" and then override only the fields that differ.

---

## 🔹 6. Senior-Level Interview Answer

> "I use Prototype to avoid the cost of full re-initialization when I need many similar objects. The critical implementation detail in C++ is that `clone()` must perform a **deep copy** — not a shallow bitwise copy — since raw pointers in a shallow copy are aliased between original and clone. I implement `clone()` by delegating to the copy constructor, and I use `= default` only if all members are value types or smart pointers. For multi-threaded systems, I protect the shared prototype registry with a `std::shared_mutex`, using a read-lock for `create()` and a write-lock for `register()`."

---

## 🔹 7. Interview Question Bank

### Q1. Prototype vs Factory?
**Answer**: "**Factory** creates objects from scratch using a specification. **Prototype** creates them by copying an existing fully-initialized instance. Prototype wins when initialization is expensive; Factory wins when construction logic must be centralized or when object families need to be enforced."

### Q2. How do you correctly clone an object with raw pointer members?
**Answer**: "Write an explicit copy constructor that allocates new heap memory for each raw pointer member and deep copies the data. Better yet, redesign the class to use `unique_ptr` or `shared_ptr`, which copy correctly using `= default`."

### Q3. What is a 'Prototype Registry'?
**Answer**: "A `std::unordered_map<std::string, unique_ptr<Shape>>` of named prototype objects. Clients call `registry.create('circle')`, which internally calls `clone()` on the cached prototype and returns a fresh copy. It combines Prototype with a Factory-like named lookup."

### Q4. Is `clone()` thread-safe?
**Answer**: "Cloning itself is safe as long as you don't **modify** the prototype during cloning. The resulting clone is always an independent object, safe for unrestricted concurrent use. If the prototype is shared and mutable, protect `clone()` with a `std::shared_mutex` (read-lock, since clone is a read operation on the prototype)."

### Q5. What is the difference between shallow copy and deep copy in the context of Prototype?
**Answer**: "A **shallow copy** copies only the pointer value, so both the original and clone point to the same heap object. If one modifies or frees it, the other is corrupted. A **deep copy** allocates new memory and copies the actual data, giving each object complete independence. In C++, `std::string`, `std::vector`, and smart pointers all perform deep copies by default."
