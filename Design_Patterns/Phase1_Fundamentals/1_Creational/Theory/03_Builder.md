# 🧠 Builder Design Pattern
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is Builder? (Real Meaning)

**Standard Definition:**
"Separate the construction of a complex object from its representation."

**Senior Definition:**
*   ✅ **"Solving the Telescoping Constructor Anti-Pattern"**
*   ✅ **"Emulating Named Parameters in C++"**
*   ✅ **"Enforcing Invariants & Validation"**

It is NOT just about construction; it is about creating **Immutable** objects safely and readably.

---

## 🔹 2. Modern & Correct C++ Implementation
**Fluent Interface (Method Chaining)**

```cpp
class RequestBuilder {
    Request req;
public:
    RequestBuilder& setMethod(std::string m) {
        req.method = std::move(m);
        return *this; // Enable Chaining
    }
    RequestBuilder& setUrl(std::string u) {
        req.url = std::move(u);
        return *this;
    }
    // The "Terminal" operation
    Request build() {
        // Validation logic happens HERE
        if (req.url.empty()) throw std::runtime_error("URL Required");
        return req;
    }
};

// Usage
Request r = RequestBuilder()
              .setMethod("POST")
              .setUrl("https://api.com")
              .build();
```

---

## 🔹 3. Pros — Deep Explanation

### ✅ Pro #1: Readability (Named Parameters)
*   **Problem**: `Window w(100, 200, true, false, 30);` -> What do these args mean?
*   **Solution**: `.setWidth(100).setHeight(200).setVisible(true)` is self-documenting code.

### ✅ Pro #2: Immutability
*   **The Benefit**: The `Request` object can be `const` and immutable. The `Builder` is mutable. This is critical for **Thread Safety** (Immutable objects are inherently thread-safe).

### ✅ Pro #3: Validation
*   **The Benefit**: You prevent "Half-Baked" objects. The `build()` method ensures the object is valid *before* returning it.

---

## 🔹 4. Cons — Deep Explanation

### ❌ Con #1: Verbosity
*   **The Issue**: You duplicate the class hierarchy. For every `Request`, you need `RequestBuilder`.
*   **Impact**: More boilerplate to maintain. If `Request` adds a field, `RequestBuilder` must also add a setter.

### ❌ Con #2: Performance Overhead
*   **The Issue**: You create a temporary Builder object on the stack just to create the real object.
*   **Impact**: In strict low-latency paths (HFT), this copy/move overhead might be non-zero (though usually negligible).

---

## 🔹 5. When to Use / Not Use

### ❌ Do NOT use Builder when:
*   The object has few arguments (<= 3). Just use a constructor.
*   The object is a simple "Data Bag" (DTO) or struct with public members.

### ✅ Acceptable use cases:
*   Constructors with 4+ parameters, esp. if many are optional.
*   Constructing complex Composite trees (documents, UI graphs).
*   Forcing creation of **Immutable** objects.

---

## 🔹 6. Senior-Level Interview Answer

> "I use Builder primarily to solve the 'Telescoping Constructor' problem and to simulate named parameters, which makes code reviews much easier. It's also excellent for validation—I can ensure an object acts as a transaction: either it's built completely valid via `build()`, or it throws."

---

## 🔹 7. Interview Question Bank

### Q1. Builder vs Factory?
**Answer**: "**Factory** creates the object in one shot (polymorphism focus). **Builder** creates the object step-by-step (configuration focus)."

### Q2. Can a Builder be reused?
**Answer**: "Yes. You can call `.build()` multiple times to create multiple similar objects (e.g., sending the same request to different endpoints), assuming the Builder resets (or doesn't consume) its state."

### Q3. What is the 'Director' class?
**Answer**: "In the GoF book, the Director controls the *order* of construction calls. In modern C++, we typically omit the Director and just reuse the Builder chain directly."
