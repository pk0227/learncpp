# Module 05: Advanced Comparisons and Operator Internals

This module covers modern operator overloading mechanics, C++20 three-way comparison internals, arrow proxy chaining, and custom memory management operators that senior C++ engineers use in systems architecture.

---

## Table of Contents

1. [The C++20 Spaceship Operator (<=>) and Ordering Categories](#1-the-c20-spaceship-operator--and-ordering-categories)
   - [1.1 The Evolution of Comparisons in C++20](#11-the-evolution-of-comparisons-in-c20)
   - [1.2 The Three Comparison Categories](#12-the-three-comparison-categories)
   - [1.3 Compiler-Synthesized Operators from defaulted <=> and ==](#13-compiler-synthesized-operators-from-defaulted--and-)
   - [1.4 Custom Spaceship Operator Implementation](#14-custom-spaceship-operator-implementation)
   - [📁 Code Examples for Section 1](#-code-examples-for-section-1)
2. [The Arrow Operator (->) Chaining and The Proxy Idiom](#2-the-arrow-operator---chaining-and-the-proxy-idiom)
   - [2.1 How the Compiler Recursively Chains operator->](#21-how-the-compiler-recursively-chains-operator-)
   - [2.2 The Return-by-Value Arrow Proxy Pattern](#22-the-return-by-value-arrow-proxy-pattern)
   - [2.3 Practical Application: The Thread-Safe RAII Locking Proxy](#23-practical-application-the-thread-safe-raii-locking-proxy)
   - [2.4 The Smart Reference Pattern](#24-the-smart-reference-pattern)
   - [📁 Code Examples for Section 2](#-code-examples-for-section-2)
3. [User-Defined Literals (UDLs) and Compile-Time Type Safety](#3-user-defined-literals-udls-and-compile-time-type-safety)
   - [3.1 Syntax and Structure of Literal Operators (operator"")](#31-syntax-and-structure-of-literal-operators-operator)
   - [3.2 Cooked vs Raw and Template Literal Operators](#32-cooked-vs-raw-and-template-literal-operators)
   - [3.3 Eliminating Unit Mismatch Bugs (Dimensional Analysis)](#33-eliminating-unit-mismatch-bugs-dimensional-analysis)
   - [3.4 Standard Library Literals (s, sv, ms, h)](#34-standard-library-literals-s-sv-ms-h)
   - [📁 Code Examples for Section 3](#-code-examples-for-section-3)
4. [Function Call Operator (operator()) and Inlining Advantages](#4-function-call-operator-operator-and-inlining-advantages)
   - [4.1 Functors as Stateful Callables](#41-functors-as-stateful-callables)
   - [4.2 Why Functors Outperform Function Pointers in Standard Algorithms](#42-why-functors-outperform-function-pointers-in-standard-algorithms)
   - [4.3 Multidimensional Subscripting: operator() vs C++23 operator[]](#43-multidimensional-subscripting-operator-vs-c23-operator)
   - [📁 Code Examples for Section 4](#-code-examples-for-section-4)
5. [Custom Class-Specific Memory Allocation Operators](#5-custom-class-specific-memory-allocation-operators)
   - [5.1 Class-Level operator new and operator delete (Memory Pools)](#51-class-level-operator-new-and-operator-delete-memory-pools)
   - [5.2 Placement new Mechanics and Manual Destructor Invocation](#52-placement-new-mechanics-and-manual-destructor-invocation)
   - [5.3 Sized Deallocation (C++14): operator delete(void*, std::size_t)](#53-sized-deallocation-c14-operator-deletevoid-stdsize_t)
   - [5.4 Destroying Delete (C++20): operator delete(T*, std::destroying_delete_t)](#54-destroying-delete-c20-operator-deletet-stddestroying_delete_t)
   - [📁 Code Examples for Section 5](#-code-examples-for-section-5)

---

## 1. The C++20 Spaceship Operator (<=>) and Ordering Categories

### 1.1 The Evolution of Comparisons in C++20
Prior to C++20, providing complete comparison capabilities for a class required writing up to 6 separate boiler-plate functions: `==`, `!=`, `<`, `<=`, `>`, `>=`.
C++20 introduces the **Three-Way Comparison Operator** (`<=>`), nicknamed the **spaceship operator**. Defining `<=>` allows the compiler to automatically synthesize all relational operators (`<`, `<=`, `>`, `>=`), and defining `==` synthesizes `!=`.

### 1.2 The Three Comparison Categories
The return type of `<=>` communicates mathematical properties of the comparison:

| Category | Ordering Guarantees | Example Types |
|---|---|---|
| **`std::strong_ordering`** | **Total Order**: Exactly one of `less`, `equal`, or `greater`. Satisfies **substitutability** (if `a == b`, then `f(a) == f(b)`). | Integers, pointers, `std::string` |
| **`std::weak_ordering`** | **Equivalence**: Two objects can be equivalent without being identical (non-substitutable). | Case-insensitive strings |
| **`std::partial_ordering`** | **Partial Order**: Some values are incomparable (`unordered`). | Floating-point numbers with `NaN` |

### 1.3 Compiler-Synthesized Operators from defaulted <=> and ==
```cpp
struct Employee {
    int department_id;
    int employee_id;
    // Synthesizes <=>, ==, !=, <, <=, >, >= lexicographically!
    auto operator<=>(const Employee&) const = default;
};
```

### 1.4 Custom Spaceship Operator Implementation
For classes requiring custom comparison logic:
```cpp
std::strong_ordering operator<=>(const Employee& other) const {
    if (auto cmp = m_department <=> other.m_department; cmp != 0) return cmp;
    return m_id <=> other.m_id;
}
```

### 📁 Code Examples for Section 1
- [`1_spaceship_operator_ordering_categories.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/05_Advanced_Comparisons_and_Operator_Internals/1_spaceship_operator_ordering_categories.cpp): Demonstrates `std::strong_ordering`, `std::weak_ordering` (case-insensitive string), and `std::partial_ordering` (`NaN` handling).

---

## 2. The Arrow Operator (->) Chaining and The Proxy Idiom

### 2.1 How the Compiler Recursively Chains operator->
In C++, `operator->` has a unique recursive chaining rule:
- If `x->m` is evaluated, the compiler calls `x.operator->()`.
- If the result is a raw pointer `T*`, it accesses `->m`.
- If the result is an **object** that also overloads `operator->`, the compiler **recursively calls `operator->`** on that proxy object until a raw pointer is reached!

### 2.2 The Return-by-Value Arrow Proxy Pattern
Because `operator->` can return an object by value, that temporary proxy object's lifetime spans the entire full expression. This enables injecting setup and teardown logic around member function access.

### 2.3 Practical Application: The Thread-Safe RAII Locking Proxy
How do you provide thread-safe access to an object's member methods without wrapping every single method in mutex locks?
Use the Arrow Proxy pattern:

```cpp
template <typename T>
class ThreadSafe {
    T m_obj;
    mutable std::mutex m_mtx;
    
    struct Proxy {
        std::unique_lock<std::mutex> lock;
        T* ptr;
        T* operator->() { return ptr; }
    };
public:
    Proxy operator->() {
        return Proxy(m_mtx, &m_obj);
    }
};
```
Usage: `safeAccount->deposit(100);`
1. `safeAccount.operator->()` locks the mutex and returns a temporary `Proxy`.
2. `Proxy.operator->()` returns `&m_obj`.
3. `deposit(100)` executes while holding the lock.
4. At the end of the full expression, `Proxy` destructor automatically unlocks the mutex!

### 2.4 The Smart Reference Pattern
This idiom forms the foundation of smart references, lazy loading, RPC client proxies, and database transaction decorators.

### 📁 Code Examples for Section 2
- [`2_arrow_proxy_idiom_thread_safe_locking.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/05_Advanced_Comparisons_and_Operator_Internals/2_arrow_proxy_idiom_thread_safe_locking.cpp): Implements a thread-safe RAII locking proxy wrapping a non-thread-safe class via chained `operator->`.

---

## 3. User-Defined Literals (UDLs) and Compile-Time Type Safety

### 3.1 Syntax and Structure of Literal Operators (operator"")
C++11 introduces User-Defined Literals (UDLs) via `operator""_suffix`. User-defined suffixes **must begin with an underscore `_`** (suffixes without underscores are reserved for the C++ standard library).

### 3.2 Cooked vs Raw and Template Literal Operators
- **Cooked literal**: Receives pre-parsed primitive values:
  ```cpp
  constexpr Distance operator""_km(long double val) { return Distance(val * 1000.0); }
  constexpr Distance operator""_m(long double val)  { return Distance(val); }
  ```
- **Raw literal**: Receives `const char*` array containing the exact digits written:
  ```cpp
  BigInt operator""_bignum(const char* digits);
  ```

### 3.3 Eliminating Unit Mismatch Bugs (Dimensional Analysis)
Writing `auto dist = 2.5_km + 300.0_m + 50.0_cm;` guarantees compile-time type safety, preventing accidental unit addition errors (like adding seconds to meters).

### 3.4 Standard Library Literals (s, sv, ms, h)
- `using namespace std::string_literals;` $\to$ `"hello"s` produces `std::string`.
- `using namespace std::string_view_literals;` $\to$ `"hello"sv` produces `std::string_view`.
- `using namespace std::chrono_literals;` $\to$ `100ms`, `5s`, `2h` for time durations.

### 📁 Code Examples for Section 3
- [`3_user_defined_literals_dimensional_analysis.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/05_Advanced_Comparisons_and_Operator_Internals/3_user_defined_literals_dimensional_analysis.cpp): Implements a type-safe `Distance` class with custom UDLs (`_m`, `_km`, `_cm`) and compile-time dimensional checking.

---

## 4. Function Call Operator (operator()) and Inlining Advantages

### 4.1 Functors as Stateful Callables
Overloading `operator()` creates a Function Object (Functor). Unlike free functions, functors can retain internal state across invocations.

### 4.2 Why Functors Outperform Function Pointers in Standard Algorithms
> [!IMPORTANT]
> **Senior Interview Question**: Why is `std::sort` faster when passed a functor/lambda than when passed a raw function pointer?
> 
> - **Function Pointer**: `std::sort` receives an address. The compiler must invoke the comparator via an indirect jump (`call *%rax`), **blocking inlining**.
> - **Functor / Lambda**: The comparator type is baked into the template parameter `template <class Compare>`. The compiler knows the exact call target at compile time and **inlines the comparison directly into the loop**, eliminating all call overhead and branch penalties!

### 4.3 Multidimensional Subscripting: operator() vs C++23 operator[]
- Prior to C++23, `operator[]` was strictly limited to taking exactly one argument (`matrix[row]` required returning a proxy row). Multidimensional indexing was forced to use `matrix(row, col)`.
- **C++23 Multidimensional `operator[]`**: C++23 natively supports multiple arguments in `operator[]`:
  ```cpp
  int& operator[](std::size_t r, std::size_t c) {
      return m_data[r * m_cols + c];
  }
  ```

### 📁 Code Examples for Section 4
- [`4_functor_inlining_vs_function_pointer.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/05_Advanced_Comparisons_and_Operator_Internals/4_functor_inlining_vs_function_pointer.cpp): Compares inlining mechanics of functors vs. function pointers, stateful functors, and C++23 multidimensional `operator[]`.

---

## 5. Custom Class-Specific Memory Allocation Operators

### 5.1 Class-Level operator new and operator delete (Memory Pools)
When a class declares `void* operator new(size_t)` and `void operator delete(void*)`:
- Dynamic allocations of that class bypass the general-purpose heap allocator.
- Allows routing allocations to high-performance fixed-size slab allocators or memory pools.

### 5.2 Placement new Mechanics and Manual Destructor Invocation
`void* operator new(size_t, void* ptr) { return ptr; }`
Constructs an object in a pre-allocated memory buffer without allocating heap memory:
```cpp
alignas(Widget) char buffer[sizeof(Widget)];
Widget* w = ::new (buffer) Widget(args...);
```
> [!WARNING]
> Because memory was not allocated via heap `new`, calling `delete w;` is **Undefined Behavior**! The destructor must be called **manually**:
> ```cpp
> w->~Widget();
> ```

### 5.3 Sized Deallocation (C++14): operator delete(void*, std::size_t)
C++14 introduced sized deallocation:
```cpp
void operator delete(void* ptr, std::size_t size) noexcept;
```
Provides the exact byte size of the object being destroyed, allowing memory pool allocators to immediately identify the correct size bin without storing extra size metadata per allocation.

### 5.4 Destroying Delete (C++20): operator delete(T*, std::destroying_delete_t)
In standard C++, deleting an object runs the destructor first, then calls `operator delete(void*)`.
C++20 introduces **Destroying Delete**:
```cpp
void operator delete(Node* ptr, std::destroying_delete_t);
```
The class takes over responsibility for running its own destructor, enabling polymorphic destruction without virtual destructors and tail-call optimized deallocations.

### 📁 Code Examples for Section 5
- [`5_custom_operator_new_delete_memory_pool.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/05_Advanced_Comparisons_and_Operator_Internals/5_custom_operator_new_delete_memory_pool.cpp): Implements a class-specific memory pool with overloaded `operator new`, C++14 sized deallocation, and placement new with manual destructor invocation.
