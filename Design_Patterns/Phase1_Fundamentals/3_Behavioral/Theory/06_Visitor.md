# 🧠 Visitor Design Pattern
> **Deep, Practical, Senior-Level Understanding**
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is Visitor? (Real Meaning)

**Standard Definition:**
"Represent an operation to be performed on elements of an object structure without changing the classes of the elements on which it operates."

**Senior Definition:**
*   ✅ **"Open-Closed for Operations, Closed for Types"**
*   ✅ **"Separating Algorithms from Data Structures"**
*   ✅ **"Double Dispatch — the solution to the Single Dispatch limitation"**

**The Core Problem**: You have a stable set of types (`Circle`, `Rectangle`, `Triangle`) but you keep needing to add new operations on them (`draw`, `serialize`, `calculateArea`, `exportToSVG`). Without Visitor, each new operation forces you to add a virtual method to ALL classes — violating Open-Closed Principle.

---

## 🔹 2. Two Implementations — Classic GoF vs Modern C++17

### A. Classic Visitor (Double Dispatch via Virtual Functions)

```cpp
// Forward declarations
class Circle; class Rectangle;

// Visitor Interface
class ShapeVisitor {
public:
    virtual void visit(Circle& c) = 0;
    virtual void visit(Rectangle& r) = 0;
    virtual ~ShapeVisitor() = default;
};

// Element Interface
class Shape {
public:
    virtual void accept(ShapeVisitor& v) = 0;  // Double dispatch key
    virtual ~Shape() = default;
};

// Concrete Element
class Circle : public Shape {
public:
    double radius;
    void accept(ShapeVisitor& v) override { v.visit(*this); }  // "I am a Circle"
};

// Concrete Visitor (an operation)
class AreaCalculator : public ShapeVisitor {
public:
    double totalArea = 0.0;
    void visit(Circle& c) override {
        totalArea += 3.14159 * c.radius * c.radius;
    }
    void visit(Rectangle& r) override {
        totalArea += r.width * r.height;
    }
};
```

### B. Modern C++17: `std::variant` + `std::visit` (Preferred!)

```cpp
#include <variant>

struct Circle    { double radius; };
struct Rectangle { double width, height; };
struct Triangle  { double base, height; };

// A type-safe sum type — holds exactly one Shape at a time
using Shape = std::variant<Circle, Rectangle, Triangle>;

// Operation as a "visitor" struct with overloaded operator()
struct AreaCalculator {
    double operator()(const Circle& c)    { return 3.14159 * c.radius * c.radius; }
    double operator()(const Rectangle& r) { return r.width * r.height; }
    double operator()(const Triangle& t)  { return 0.5 * t.base * t.height; }
};

// Usage
Shape s = Circle{5.0};
double area = std::visit(AreaCalculator{}, s);  // Compile-time safe!
```

**Why `std::variant` is superior:**
- **Exhaustiveness checking**: The compiler ERRORS if you forget to handle a type.
- **No raw pointers**: Value semantics, stack-allocated.
- **Faster**: No vtable overhead, often inlined by the compiler.
- **Simpler**: No `accept()` boilerplate, no forward declarations.

---

## 🔹 3. Pros — Deep Explanation

### ✅ Pro #1: Open/Closed for Operations
*   **The Benefit**: Add new operations (new Visitor) without touching existing element classes.
*   **Example**: Add `JSONSerializer`, `XMLSerializer`, `DebugPrinter` to `Circle`/`Rectangle` without modifying them.

### ✅ Pro #2: Centralizes Related Logic
*   **The Benefit**: All logic for one operation (`area calculation`) lives in ONE place (one Visitor class). Without Visitor, area logic is scattered across all shape classes.

### ✅ Pro #3: Accumulating State Across Elements
*   **The Benefit**: A Visitor can accumulate results while visiting (e.g., `totalArea += ...` across all shapes in a scene graph).

---

## 🔹 4. Cons — Deep Explanation

### ❌ Con #1: Closed for Types (The Fundamental Tradeoff)
*   **The Issue**: Adding a NEW type (e.g., `Hexagon`) to the hierarchy requires modifying **ALL existing Visitor interfaces** to add `visit(Hexagon&)`.
*   **Impact**: Visitor trades extensibility for operations for extensibility for types. Choose based on which axis changes more.

### ❌ Con #2: Breaks Encapsulation
*   **The Issue**: Visitors often need access to private or protected members of the elements they visit.
*   **Solution**: Either expose data via getters (weakens encapsulation) or declare the Visitor as a `friend`.

### ❌ Con #3: Double Dispatch Complexity (Classic version)
*   **The Issue**: The `accept(visitor)` → `visitor.visit(*this)` double-dispatch mechanism is non-obvious.
*   **Solution**: Use `std::variant` + `std::visit` which makes the dispatch explicit and compiler-verified.

---

## 🔹 5. When to Use / Not Use

### ❌ Do NOT use Visitor when:
*   **Types change frequently** but operations are stable. Adding a new type to every existing Visitor is painful.
*   The object hierarchy is simple (2 types). Overengineering.

### ✅ Acceptable use cases:
*   **Compilers**: AST (Abstract Syntax Tree) traversal. Nodes (stable) + operations (type-checker, code-gen, optimizer).
*   **Document Processing**: Document elements (stable) + export formats (HTML, PDF, Markdown).
*   **GUI**: Widget hierarchy (stable) + operations (render, layout, hit-test, serialize).
*   **Game Engines**: Entity components (stable) + system operations (physics update, render, AI tick).

---

## 🔹 6. Senior-Level Interview Answer

> "I default to `std::variant` + `std::visit` over the classic GoF Visitor because it gives exhaustiveness checking at compile time — if you add a new shape type and forget to handle it in a visitor, it won't compile. The classic `virtual accept()` pattern silently lets you miss types until runtime. The tradeoff is the same: Visitor is ideal when your type hierarchy is stable but your operations are growing. If new types are added frequently, I consider a different design, like Strategy per type."

---

## 🔹 7. Interview Question Bank

### Q1. What is "Double Dispatch" and why does Visitor need it?
**Answer**: "C++ uses single dispatch — virtual function resolution depends on the type of ONE object (`this`). Visitor needs to dispatch on TWO types simultaneously: the type of the element AND the type of the visitor. `accept(visitor)` resolves the element type; `visitor.visit(*this)` resolves the visitor type. Two virtual calls = double dispatch."

### Q2. `std::visit` vs classic Visitor?
**Answer**: "`std::visit` with `std::variant` gives exhaustiveness checking (compiler error if a type isn't handled) and no runtime overhead from vtables. Classic Visitor is more flexible for open hierarchies (types from different libraries) but has more boilerplate and silent incompleteness risk."

### Q3. How do you handle a new type in the classic Visitor?
**Answer**: "Add a new pure virtual `visit(NewType&)` to the Visitor base class. This FORCES all concrete visitors to implement it (compiler error if not). The pain is intentional — it ensures you don't accidentally leave a new type unhandled in any existing operation."

### Q4. How do you make a Visitor thread-safe?
**Answer**: "If the Visitor only reads element state, multiple Visitor instances can run concurrently on separate threads safely. If the Visitor accumulates state (like `totalArea`), either: (1) Use a separate Visitor instance per thread, then merge results. (2) Protect the shared state with a mutex. Option 1 (no sharing) is always preferred."

### Q5. Visitor vs Strategy?
**Answer**: "**Strategy** replaces one algorithm for doing the same thing (sort this list differently). **Visitor** adds a completely new operation to an existing type hierarchy (now also serialize these shapes, in addition to drawing them). Strategy is intra-type; Visitor is cross-type."
