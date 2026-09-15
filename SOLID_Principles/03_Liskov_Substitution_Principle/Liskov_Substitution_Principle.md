# 3. Liskov Substitution Principle (LSP)

> *"Let $\phi(x)$ be a property provable about objects $x$ of type $T$. Then $\phi(y)$ should be true for objects $y$ of type $S$ where $S$ is a subtype of $T$."*  
> — **Barbara Liskov & Jeannette Wing (1994)**

---

## 📑 Table of Contents

1. [Formal Definition & The Behavioral Subtyping Model](#1-formal-definition--the-behavioral-subtyping-model)
2. [Design by Contract (DbC) Formal Rules](#2-design-by-contract-dbc-formal-rules)
   - [Precondition Contravariance](#precondition-contravariance)
   - [Postcondition Covariance](#postcondition-covariance)
   - [Class Invariant Preservation](#class-invariant-preservation)
   - [The History Constraint](#the-history-constraint)
3. [The Square-Rectangle Fallacy: Math vs OOP](#3-the-square-rectangle-fallacy-math-vs-oop)
4. [C++ Specific LSP Violations](#4-c-specific-lsp-violations)
   - [Object Slicing on Pass-by-Value](#object-slicing-on-pass-by-value)
   - [Exception Specification & `noexcept` Violations](#exception-specification--noexcept-violations)
   - [Name Hiding Across Class Scopes](#name-hiding-across-class-scopes)
   - [`std::vector<bool>`: The Standard Library's LSP Smell](#stdvectorbool-the-standard-librarys-lsp-smell)
5. [Refactoring Strategies for Senior Architects](#5-refactoring-strategies-for-senior-architects)
6. [Compile-Time LSP via C++20 Concepts](#6-compile-time-lsp-via-c20-concepts)
7. [📁 Code Examples for Section 3](#7--code-examples-for-section-3)

---

## 1. Formal Definition & The Behavioral Subtyping Model

At junior levels, LSP is often summarized simply as *"derived classes must be usable through base pointers"*. In C++, this is trivialized to syntax: if it compiles via virtual inheritance, the developer assumes LSP is satisfied.

**This is completely false.** LSP is about **semantic behavior**, not syntax.

A type $S$ is a true behavioral subtype of $T$ if and only if any program written against the contract of $T$ continues to function correctly without modification when passed an instance of $S$.

```
                  Client Expectation Contract (Type T)
                ┌──────────────────────────────────────┐
                │ 1. Preconditions: x > 0              │
                │ 2. Postcondition: balance decreases   │
                │ 3. Invariant: balance >= 0           │
                └──────────────────┬───────────────────┘
                                   │
               ┌───────────────────┴───────────────────┐
               ▼                                       ▼
    ✅ COMPLIANT SUBTYPE (S)               ❌ LSP VIOLATION (S')
  Preconditions: x >= 0 (Weaker)         Preconditions: x > 100 (Strengthened!)
  Postcondition: balance decreases       Postcondition: throws UnsupportedException!
  Invariants: balance >= 0 preserved     Invariants: balance drops below 0!
```

---

## 2. Design by Contract (DbC) Formal Rules

Bertrand Meyer formalized Design by Contract, which provides the mathematical foundation of LSP:

### Precondition Contravariance
> **Rule**: A subtype cannot strengthen preconditions. It may only weaken them or keep them identical.

If base method `transfer(double amount)` accepts any `amount > 0`, a derived class method cannot reject transactions under $100. A client programmed to `Account` cannot anticipate this restriction.

### Postcondition Covariance
> **Rule**: A subtype cannot weaken postconditions. It may only strengthen them or keep them identical.

If base method `reset()` guarantees that after execution `is_initialized() == true`, a derived class cannot return with uninitialized internal state.

### Class Invariant Preservation
> **Rule**: All invariants established by the supertype must be preserved in all derived subtypes.

If `RingBuffer` maintains the invariant that `size() <= capacity()`, no derived class method may allow `size()` to exceed `capacity()`, even temporarily in public methods.

### The History Constraint
> **Rule**: Subtypes must not introduce state transitions that were disallowed by the supertype.

If `ImmutableDocument` promises that contents never change after construction, a derived `EditableDocument` violates the history constraint even if it satisfies every single read method.

---

## 3. The Square-Rectangle Fallacy: Math vs OOP

The most famous LSP violation stems from confusing **mathematical taxonomy** with **behavioral modeling**:

In geometry:
- A Square **is a** Rectangle (a rectangle whose width equals height).

In Object-Oriented Software:
- `Rectangle` establishes the behavioral contract:
  ```cpp
  r.setWidth(w);  // Mutates width, height remains UNCHANGED
  r.setHeight(h); // Mutates height, width remains UNCHANGED
  ```
- If `Square` derives from `Rectangle`, it must enforce `width == height`. Therefore, calling `setWidth(w)` silently mutates `height` as a side effect!

```cpp
void ResizeAndTest(Rectangle& r) {
    r.setWidth(5);
    r.setHeight(4);
    assert(r.getArea() == 20); // ❌ FAILS when r is a Square! Area is 16!
}
```

> [!WARNING]
> In OOP, **an object is defined strictly by its behavior, not its real-world physical attributes**. If two concepts do not share the exact same behavioral invariants under mutation, inheritance between them is an architectural defect.

---

## 4. C++ Specific LSP Violations

### Object Slicing on Pass-by-Value
In C++, passing a polymorphic object by value strips the derived class's members and resets its virtual table pointer (`vptr`) back to the base class:

```cpp
void audit(BaseSensor sensor); // ❌ SLICING! Slices CalibratedSensor to BaseSensor!
void audit(const BaseSensor& sensor); // ✅ Preserves polymorphic identity
```

### Exception Specification & `noexcept` Violations
If a base class method is declared `noexcept`, the C++ compiler guarantees that callers do not need exception handling frames. If a derived class attempts to throw, `std::terminate()` is immediately invoked:

```cpp
class Base {
public:
    virtual void compute() noexcept;
};

class Derived : public Base {
public:
    void compute() noexcept override {
        // If this throws, your program crashes with std::terminate()!
    }
};
```

Even without `noexcept`, throwing `std::logic_error("Not supported")` from a derived method (e.g., `ReadOnlyStream::write()`) violates LSP because callers expecting a writable stream never anticipated a write rejection.

### Name Hiding Across Class Scopes
In C++, declaring a function in a derived class hides all overloads with the same name in the base class unless explicitly brought into scope:

```cpp
class Base {
public:
    virtual void process(int x);
    virtual void process(double x);
};

class Derived : public Base {
public:
    using Base::process; // MANDATORY! Otherwise process(int) is hidden!
    void process(double x) override;
};
```

### `std::vector<bool>`: The Standard Library's LSP Smell
The classic C++ standard library LSP violation is `std::vector<bool>`.
- For every other `std::vector<T>`, `operator[]` returns a real reference `T&`.
- Because `std::vector<bool>` is bit-packed to save RAM (1 bit per boolean), you cannot take the address of a bit.
- Therefore, `std::vector<bool>::operator[]` returns a temporary **proxy object** (`std::vector<bool>::reference`), NOT `bool&`.
- Generic template algorithms expecting `auto& val = vec[0];` fail to compile with `std::vector<bool>`.

---

## 5. Refactoring Strategies for Senior Architects

1. **Favor Value Semantics & Immutability**:
   - If `Rectangle` and `Square` are immutable value objects, neither has setters. Both can implement a read-only `Shape` interface (`getArea()`) with zero LSP violations.
2. **Segregate Mutable Capabilities**:
   - Separate `ReadableStream` from `WritableStream`. `ReadOnlyFile` only implements `ReadableStream`, making write errors impossible at compile time.
3. **Prefer Composition Over Inheritance**:
   - Instead of `Square` inheriting from `Rectangle`, give `Square` its own class and let it compose a `Rectangle` internally if code reuse is needed.

---

## 6. Compile-Time LSP via C++20 Concepts

Modern C++ elevates LSP from a runtime wish into a **statically verified compiler constraint**:

```cpp
template <typename T>
concept ShapeContract = requires(const T& s) {
    { s.getArea() } noexcept -> std::convertible_to<double>;
    { s.describe() } -> std::convertible_to<std::string>;
};

template <ShapeContract S>
void processShape(const S& shape) {
    // 100% guaranteed substitutability verified at compile time with 0 vtables!
}
```

---

## 7. 📁 Code Examples for Section 3

- [`Code/01_lsp_violation_square_rectangle.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/03_Liskov_Substitution_Principle/Code/01_lsp_violation_square_rectangle.cpp): The classic Square-Rectangle behavioral breakdown.
- [`Code/02_lsp_violation_weakened_postconditions.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/03_Liskov_Substitution_Principle/Code/02_lsp_violation_weakened_postconditions.cpp): Demonstrates object slicing, unexpected exception throwing, and the read-only stream trap.
- [`Code/03_lsp_refactored_hierarchy.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/03_Liskov_Substitution_Principle/Code/03_lsp_refactored_hierarchy.cpp): Proper architectural refactoring via value semantics, segregated stream interfaces, and composition.
- [`Code/04_lsp_compile_time_concepts.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/03_Liskov_Substitution_Principle/Code/04_lsp_compile_time_concepts.cpp): Statically enforced Liskov contracts and `noexcept` validation using C++20 Concepts.
