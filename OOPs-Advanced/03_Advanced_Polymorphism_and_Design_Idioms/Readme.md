# Module 03: Advanced Polymorphism and Design Idioms

This module covers advanced C++ idioms that merge object-oriented programming with generic template metaprogramming to achieve high-performance static polymorphism, type erasure, and robust API contracts.

---

## Table of Contents

1. [Curiously Recurring Template Pattern (CRTP) & Static Polymorphism](#1-curiously-recurring-template-pattern-crtp--static-polymorphism)
   - [1.1 Mechanics of CRTP: Deriving from a Templated Base](#11-mechanics-of-crtp-deriving-from-a-templated-base)
   - [1.2 Static Polymorphism vs Dynamic Polymorphism (Vtables vs Inlining)](#12-static-polymorphism-vs-dynamic-polymorphism-vtables-vs-inlining)
   - [1.3 Enforcing Compile-Time Interfaces with CRTP](#13-enforcing-compile-time-interfaces-with-crtp)
   - [1.4 The Object Counter and Mixin Pattern via CRTP](#14-the-object-counter-and-mixin-pattern-via-crtp)
   - [📁 Code Examples for Section 1](#-code-examples-for-section-1)
2. [Policy-Based Class Design and Mixins](#2-policy-based-class-design-and-mixins)
   - [2.1 The Philosophy of Policy-Based Design (Alexandrescu Paradigm)](#21-the-philosophy-of-policy-based-design-alexandrescu-paradigm)
   - [2.2 Policies as Template Parameters vs Deep Inheritance Trees](#22-policies-as-template-parameters-vs-deep-inheritance-trees)
   - [2.3 Combining Orthogonal Policies (Threading, Storage, Checking)](#23-combining-orthogonal-policies-threading-storage-checking)
   - [2.4 The Zero-Overhead Advantage of Policy Composition](#24-the-zero-overhead-advantage-of-policy-composition)
   - [📁 Code Examples for Section 2](#-code-examples-for-section-2)
3. [The Type Erasure Idiom](#3-the-type-erasure-idiom)
   - [3.1 The Problem: Storing Unrelated Types without a Common Base or Exposed Templates](#31-the-problem-storing-unrelated-types-without-a-common-base-or-exposed-templates)
   - [3.2 The Tripartite Architecture of Type Erasure](#32-the-tripartite-architecture-of-type-erasure)
   - [3.3 How std::function and std::any Work Internally](#33-how-stdfunction-and-stdany-work-internally)
   - [3.4 Small Buffer Optimization (SBO) for Type Erasure](#34-small-buffer-optimization-sbo-for-type-erasure)
   - [📁 Code Examples for Section 3](#-code-examples-for-section-3)
4. [Member Function Ref-Qualifiers (& and &&)](#4-member-function-ref-qualifiers--and-)
   - [4.1 The Hidden *this Value Category](#41-the-hidden-this-value-category)
   - [4.2 Syntax and Rules for Ref-Qualified Methods](#42-syntax-and-rules-for-ref-qualified-methods)
   - [4.3 Preventing Dangerous Mutations on Rvalue Temporaries](#43-preventing-dangerous-mutations-on-rvalue-temporaries)
   - [4.4 Resource Theft Optimization: Move-Enabled Getters](#44-resource-theft-optimization-move-enabled-getters)
   - [📁 Code Examples for Section 4](#-code-examples-for-section-4)
5. [Virtual Friend Functions and Double Dispatch](#5-virtual-friend-functions-and-double-dispatch)
   - [5.1 Why Friend Functions Cannot Be Virtual](#51-why-friend-functions-cannot-be-virtual)
   - [5.2 The Virtual Friend Idiom](#52-the-virtual-friend-idiom)
   - [5.3 The Double Dispatch Problem (Visitor Pattern)](#53-the-double-dispatch-problem-visitor-pattern)
   - [5.4 Eliminating dynamic_cast Cascades with Double Dispatch](#54-eliminating-dynamic_cast-cascades-with-double-dispatch)
   - [📁 Code Examples for Section 5](#-code-examples-for-section-5)

---

## 1. Curiously Recurring Template Pattern (CRTP) & Static Polymorphism

### 1.1 Mechanics of CRTP: Deriving from a Templated Base
In CRTP, a class derives from a base class template instantiated with the derived class itself:

```cpp
template <typename Derived>
class Base {
public:
    void interface() {
        // Compile-time static upcast to Derived*
        static_cast<Derived*>(this)->implementation();
    }
};

class MyDerived : public Base<MyDerived> {
public:
    void implementation() { /* concrete logic */ }
};
```

### 1.2 Static Polymorphism vs Dynamic Polymorphism (Vtables vs Inlining)
| Feature | Dynamic Polymorphism (Virtual) | Static Polymorphism (CRTP) |
|---|---|---|
| **Dispatch Timing** | Runtime via vtable | Compile-time via template instantiation |
| **Call Mechanism** | Indirect call (`call *%rax`) | Direct function call |
| **Inlining** | Blocked by indirect jump | Fully inlinable by compiler |
| **Per-Object Memory** | +8 bytes (`vptr`) | **0 bytes overhead** |
| **Branch Predictor** | Risk of branch misprediction | Direct branch or zero branch (inlined) |

### 1.3 Enforcing Compile-Time Interfaces with CRTP
If a derived class fails to implement the required method (`implementation()`), the code fails compilation immediately with a clear template error, rather than crashing at runtime via `__cxa_pure_virtual`.

### 1.4 The Object Counter and Mixin Pattern via CRTP
CRTP allows injecting reusable functionality (mixins) into unrelated classes without virtual overhead:

```cpp
template <typename T>
class InstanceCounter {
    static inline int count{0};
public:
    InstanceCounter() { ++count; }
    ~InstanceCounter() { --count; }
    static int getCount() { return count; }
};

class User : public InstanceCounter<User> {};
class Order : public InstanceCounter<Order> {};
// User and Order each receive their own independent, type-safe static counter!
```

### 📁 Code Examples for Section 1
- [`1_crtp_static_polymorphism_vs_vtable.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/03_Advanced_Polymorphism_and_Design_Idioms/1_crtp_static_polymorphism_vs_vtable.cpp): Compares memory footprint (0 vptr overhead) and dispatch mechanics between dynamic virtual polymorphism and CRTP static polymorphism.

---

## 2. Policy-Based Class Design and Mixins

### 2.1 The Philosophy of Policy-Based Design (Alexandrescu Paradigm)
Popularized by Andrei Alexandrescu in *Modern C++ Design*, Policy-Based Class Design treats class behavior as an assembly of independent, interchangeable compile-time "policies" passed as template parameters.

### 2.2 Policies as Template Parameters vs Deep Inheritance Trees
In traditional OOP, combining behaviors requires deep inheritance:
`ThreadSafeFileLogger`, `SingleThreadedConsoleLogger`, `BufferedNetworkLogger`... leading to an exponential explosion of subclasses!

Policy-based design decomposes orthogonal concerns:

```cpp
template <typename ThreadingPolicy, typename OutputPolicy>
class SmartLogger : private ThreadingPolicy, private OutputPolicy {
public:
    void log(const std::string& msg) {
        ThreadingPolicy::lock();
        OutputPolicy::write(msg);
        ThreadingPolicy::unlock();
    }
};
```

### 2.3 Combining Orthogonal Policies (Threading, Storage, Checking)
Users assemble behaviors at compile time without altering existing code:
```cpp
using FastLogger = SmartLogger<SingleThreadedPolicy, ConsolePolicy>;
using SecureLogger = SmartLogger<MultiThreadedPolicy, FilePolicy>;
```

### 2.4 The Zero-Overhead Advantage of Policy Composition
- Policies are resolved statically at compile time.
- If `SingleThreadedPolicy::lock()` is empty, the compiler completely inlines and optimizes it away into **zero CPU instructions**.

### 📁 Code Examples for Section 2
- [`2_mixin_and_policy_based_design.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/03_Advanced_Polymorphism_and_Design_Idioms/2_mixin_and_policy_based_design.cpp): Implements orthogonal threading and output policies assembled into a zero-overhead host class.

---

## 3. The Type Erasure Idiom

### 3.1 The Problem: Storing Unrelated Types without a Common Base or Exposed Templates
How can a container (like `std::vector`) store heterogeneous objects (`Circle`, `Rectangle`, `Text`) if they do **not** inherit from a shared base class, without exposing template arguments to the container?
Type Erasure provides the solution: **Value semantics on the outside, polymorphism on the inside**.

### 3.2 The Tripartite Architecture of Type Erasure
Type erasure (used by `std::function`, `std::any`, `std::move_only_function`) consists of three essential layers:
1. **Concept (Private Abstract Base)**: Defines the virtual interface.
2. **Model (Private Templated Derived Wrapper)**: Inherits from `Concept` and holds the concrete type `T` by value, delegating virtual calls to `T`'s methods.
3. **Public External Class**: Stores `std::unique_ptr<Concept>` and provides a clean value-semantic API.

```cpp
class AnyPrintable {
    struct Concept {
        virtual ~Concept() = default;
        virtual void print() const = 0;
    };
    template <typename T>
    struct Model : public Concept {
        T m_data;
        Model(T val) : m_data(std::move(val)) {}
        void print() const override { m_data.print(); }
    };
    std::unique_ptr<Concept> m_impl;
public:
    template <typename T>
    AnyPrintable(T val) : m_impl(std::make_unique<Model<T>>(std::move(val))) {}
    void print() const { m_impl->print(); }
};
```

### 3.3 How std::function and std::any Work Internally
`std::function<void()>` does not know what type of callable it holds (lambda, functor, free function pointer). It captures the callable in a templated `Model` wrapper and interacts through the `Concept` interface.

### 3.4 Small Buffer Optimization (SBO) for Type Erasure
Allocating `Concept` on the heap incurs memory allocation latency. Production implementations embed a small internal byte buffer (e.g., 32 or 64 bytes) inside the wrapper and construct small callables in-place using placement `new`, avoiding heap allocation completely.

### 📁 Code Examples for Section 3
- [`3_type_erasure_idiom_std_function_internals.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/03_Advanced_Polymorphism_and_Design_Idioms/3_type_erasure_idiom_std_function_internals.cpp): Complete implementation of a type-erased container storing completely unrelated classes in a homogeneous `std::vector`.

---

## 4. Member Function Ref-Qualifiers (& and &&)

### 4.1 The Hidden *this Value Category
Every non-static member function receives an implicit parameter: `*this`.
Normally, member functions can be called on both lvalues (`obj.method()`) and temporary rvalues (`createObj().method()`).

### 4.2 Syntax and Rules for Ref-Qualified Methods
C++11 introduces ref-qualifiers on member function declarations:
```cpp
void method() &;  // ONLY callable when *this is an lvalue
void method() &&; // ONLY callable when *this is an rvalue temporary
```

### 4.3 Preventing Dangerous Mutations on Rvalue Temporaries
Consider in-place mutation methods:
```cpp
class Matrix {
public:
    Matrix& transpose() &; // Restricted to lvalues!
};
```
- Calling `getMatrix().transpose();` fails compilation!
- This prevents the common bug where modifications are accidentally applied to a temporary object that is immediately destroyed.

### 4.4 Resource Theft Optimization: Move-Enabled Getters
Ref-qualifiers allow getters to safely steal resources from dying temporary objects:

```cpp
class Document {
    std::vector<int> m_data;
public:
    // Lvalue call: returns const reference (zero copies!)
    const std::vector<int>& getData() const & { return m_data; }

    // Rvalue temporary call: MOVES the data out! (zero copy resource theft!)
    std::vector<int> getData() && { return std::move(m_data); }
};
```
Calling `auto data = createDocument().getData();` moves the vector payload directly with zero copies!

### 📁 Code Examples for Section 4
- [`4_member_function_ref_qualifiers.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/03_Advanced_Polymorphism_and_Design_Idioms/4_member_function_ref_qualifiers.cpp): Demonstrates ref-qualified getters stealing resources from rvalues vs returning const references on lvalues, plus compile-time mutation protection.

---

## 5. Virtual Friend Functions and Double Dispatch

### 5.1 Why Friend Functions Cannot Be Virtual
A `friend` function is a non-member function. Dynamic dispatch requires an implicit `this` pointer pointing to an object containing a vtable. Non-member functions do not have a `this` pointer and therefore cannot be marked `virtual`.

### 5.2 The Virtual Friend Idiom
To make friend operators (like stream insertion `operator<<`) polymorphic, delegate to a protected virtual member function:

```cpp
class Base {
protected:
    virtual void print(std::ostream& os) const { os << "Base"; }
public:
    friend std::ostream& operator<<(std::ostream& os, const Base& b) {
        b.print(os); // Polymorphic dispatch inside non-virtual friend!
        return os;
    }
};
```

### 5.3 The Double Dispatch Problem (Visitor Pattern)
Standard virtual dispatch is **single dispatch**: the method invoked depends solely on the runtime type of *one* receiver object (`receiver->action()`).
**Double Dispatch** resolves an interaction based on the runtime types of **two distinct objects** (e.g., `collision(Shape1, Shape2)`).

### 5.4 Eliminating dynamic_cast Cascades with Double Dispatch
Instead of writing nested `if (dynamic_cast<Box*>(&s2))` cascades, Double Dispatch uses two reciprocal virtual function calls:
1. `obj1.collideWith(obj2);` (First dispatch on `obj1`).
2. Inside `obj1`: `obj2.collideWithSpecificType(*this);` (Second dispatch on `obj2`).
Both types are resolved at compile/runtime with **zero type casts**, maximum performance, and complete type safety.

### 📁 Code Examples for Section 5
- [`5_virtual_friend_and_double_dispatch.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/03_Advanced_Polymorphism_and_Design_Idioms/5_virtual_friend_and_double_dispatch.cpp): Demonstrates polymorphic `operator<<` via the Virtual Friend idiom and full collision resolution via Double Dispatch without `dynamic_cast`.
