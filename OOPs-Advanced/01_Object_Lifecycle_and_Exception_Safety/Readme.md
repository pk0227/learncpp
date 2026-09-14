# Module 01: Object Lifetime, Constructor Failures & Exception Safety

This module covers the critical object lifecycle mechanics, failure modes, and exception-safety guarantees that senior C++ engineers must master for mission-critical systems and senior/staff-level technical interviews.

---

## Table of Contents

1. [The Object Lifetime Invariant and The Constructor Exception Trap](#1-the-object-lifetime-invariant-and-the-constructor-exception-trap)
   - [1.1 When Does an Object's Lifetime Begin? (ISO C++ [basic.life])](#11-when-does-an-objects-lifetime-begin-iso-c-basiclife)
   - [1.2 What Happens When a Constructor Throws?](#12-what-happens-when-a-constructor-throws)
   - [1.3 Why the Destructor Never Runs for Partially Constructed Objects](#13-why-the-destructor-never-runs-for-partially-constructed-objects)
   - [1.4 The Raw Pointer Resource Leak Hazard](#14-the-raw-pointer-resource-leak-hazard)
   - [1.5 Sub-Object Destruction Order During Constructor Failure](#15-sub-object-destruction-order-during-constructor-failure)
   - [1.6 The Modern Solution: RAII and Rule of Zero](#16-the-modern-solution-raii-and-rule-of-zero)
   - [📁 Code Examples for Section 1](#-code-examples-for-section-1)
2. [Function-Try-Blocks on Constructors](#2-function-try-blocks-on-constructors)
   - [2.1 Syntax and Structure of Function-Try-Blocks](#21-syntax-and-structure-of-function-try-blocks)
   - [2.2 The Inability to Suppress Exceptions (Implicit Rethrow)](#22-the-inability-to-suppress-exceptions-implicit-rethrow)
   - [2.3 The "Dangling Sub-Object" Hazard: Accessing Members in Catch Block is UB](#23-the-dangling-sub-object-hazard-accessing-members-in-catch-block-is-ub)
   - [2.4 Legitimate Use Cases: Exception Translation and Logging](#24-legitimate-use-cases-exception-translation-and-logging)
   - [📁 Code Examples for Section 2](#-code-examples-for-section-2)
3. [Move Semantics, noexcept, and the std::vector Reallocation Dilemma](#3-move-semantics-noexcept-and-the-stdvector-reallocation-dilemma)
   - [3.1 The Three Exception Safety Guarantees (Basic, Strong, No-Throw)](#31-the-three-exception-safety-guarantees-basic-strong-no-throw)
   - [3.2 Why std::vector Reallocation Requires the Strong Guarantee](#32-why-stdvector-reallocation-requires-the-strong-guarantee)
   - [3.3 The Role of std::move_if_noexcept and std::is_nothrow_move_constructible](#33-the-role-of-stdmove_if_noexcept-and-stdis_nothrow_move_constructible)
   - [3.4 The Silent Performance Cliff: Falling Back to Deep Copies](#34-the-silent-performance-cliff-falling-back-to-deep-copies)
   - [3.5 Best Practice: Marking Move Constructors and Move Assignment noexcept](#35-best-practice-marking-move-constructors-and-move-assignment-noexcept)
   - [📁 Code Examples for Section 3](#-code-examples-for-section-3)
4. [The Copy-and-Swap Idiom](#4-the-copy-and-swap-idiom)
   - [4.1 The Deficiencies of Naive Copy Assignment Operators](#41-the-deficiencies-of-naive-copy-assignment-operators)
   - [4.2 Self-Assignment Branching Overhead vs Robustness](#42-self-assignment-branching-overhead-vs-robustness)
   - [4.3 Mechanics of Copy-and-Swap: Pass-by-Value and Non-Throwing Swap](#43-mechanics-of-copy-and-swap-pass-by-value-and-non-throwing-swap)
   - [4.4 How Copy-and-Swap Unifies Copy and Move Assignment](#44-how-copy-and-swap-unifies-copy-and-move-assignment)
   - [4.5 Exception Safety Guarantees Provided by Copy-and-Swap](#45-exception-safety-guarantees-provided-by-copy-and-swap)
   - [📁 Code Examples for Section 4](#-code-examples-for-section-4)
5. [Construction and Destruction Order in Complex Hierarchies](#5-construction-and-destruction-order-in-complex-hierarchies)
   - [5.1 Standard Construction Sequence (Virtual Bases, Non-Virtual Bases, Members)](#51-standard-construction-sequence-virtual-bases-non-virtual-bases-members)
   - [5.2 Why Member Initialization Order Depends Solely on Class Declaration Order](#52-why-member-initialization-order-depends-solely-on-class-declaration-order)
   - [5.3 Standard Destruction Sequence (Exact Mirror Reversal)](#53-standard-destruction-sequence-exact-mirror-reversal)
   - [5.4 Virtual Function Dispatch During Construction and Destruction](#54-virtual-function-dispatch-during-construction-and-destruction)
   - [5.5 The Pure Virtual Function Call Hazard (__cxa_pure_virtual)](#55-the-pure-virtual-function-call-hazard-__cxa_pure_virtual)
   - [📁 Code Examples for Section 5](#-code-examples-for-section-5)

---

## 1. The Object Lifetime Invariant and The Constructor Exception Trap

### 1.1 When Does an Object's Lifetime Begin? (ISO C++ [basic.life])
- According to the ISO C++ Standard ([basic.life]), the lifetime of an object of class type begins **only when its constructor completes execution without throwing an exception**.
- If a constructor exits prematurely by throwing an exception, the object is considered by the runtime to have **never existed**.

### 1.2 What Happens When a Constructor Throws?
- When an exception is thrown from the constructor body or during the initialization of any sub-objects (base classes or non-static members), normal construction is immediately aborted.
- Stack unwinding begins immediately from the throw point.

### 1.3 Why the Destructor Never Runs for Partially Constructed Objects
- Because an object's lifetime never formally began, the C++ runtime **will never invoke the destructor of that class**.
- **Design Rationale**: A class destructor relies on the assumption that all class invariants have been established and all member pointers/handles are valid. Invoking a destructor on a half-initialized object would inevitably trigger undefined behavior (such as calling `delete` on an uninitialized pointer or closing an invalid operating system handle).

### 1.4 The Raw Pointer Resource Leak Hazard
Consider a class that manages two raw heap-allocated buffers:

```cpp
class Danger {
    int* p1;
    int* p2;
public:
    Danger() : p1(new int[100]), p2(new int[200]) {
        // If allocation of p2 throws std::bad_alloc,
        // OR an exception is thrown inside this constructor body...
    }
    ~Danger() {
        delete[] p1;
        delete[] p2;
    }
};
```

> [!WARNING]
> If `p2` allocation fails, or if any statement inside the constructor body throws an exception:
> 1. The destructor `~Danger()` is **never called**!
> 2. The memory allocated for `p1` is **permanently leaked**!
> This is the classic **Constructor Exception Trap**.

### 1.5 Sub-Object Destruction Order During Constructor Failure
- While the class destructor itself is bypassed, C++ guarantees that all **fully constructed sub-objects** (base classes and data members initialized *before* the exception was thrown) are destroyed in the **exact reverse order** of their construction.
- If a member is a class type whose constructor successfully completed, its destructor runs cleanly during stack unwinding.

### 1.6 The Modern Solution: RAII and Rule of Zero
- Never store raw owning pointers in a class that manages multiple resources.
- Wrap resources in standard RAII handles like `std::unique_ptr`:

```cpp
class Safe {
    std::unique_ptr<int[]> p1;
    std::unique_ptr<int[]> p2;
public:
    Safe() : p1(std::make_unique<int[]>(100)), p2(std::make_unique<int[]>(200)) {
        // If p2 throws, p1 is already a fully constructed std::unique_ptr sub-object.
        // Its destructor executes automatically during stack unwinding, cleanly freeing p1!
    }
};
```

### 📁 Code Examples for Section 1
- [`1_exception_in_constructor_destructor_trap.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/01_Object_Lifecycle_and_Exception_Safety/1_exception_in_constructor_destructor_trap.cpp): Demonstrates the constructor exception trap with raw pointers leaking vs. safe automatic cleanup via `std::unique_ptr`.

---

## 2. Function-Try-Blocks on Constructors

### 2.1 Syntax and Structure of Function-Try-Blocks
- Regular `try...catch` blocks placed inside the constructor body cannot catch exceptions thrown from the **member initializer list** or **base class constructors**.
- A **function-try-block** encloses the entire constructor, including the initializer list:

```cpp
class Derived : public Base {
    Member m_member;
public:
    Derived(int x) try 
        : Base(x), m_member(x) 
    {
        // Constructor body
    } 
    catch (const std::exception& e) 
    {
        // Catches exceptions from Base, m_member, or constructor body
    }
};
```

### 2.2 The Inability to Suppress Exceptions (Implicit Rethrow)
- In normal functions, a `catch` block can handle an exception and return a default value, suppressing propagation.
- **In a constructor function-try-block, suppressing an exception is impossible**:
  - The standard mandates: if execution reaches the closing brace of a constructor catch block, the runtime **automatically and implicitly executes `throw;`** (rethrowing the active exception).
  - Attempting to exit with `return;` is illegal or results in an immediate rethrow.

> [!IMPORTANT]
> A constructor function-try-block **cannot swallow or suppress an exception**. An object whose constructor failed cannot be allowed to exist in a partially constructed state. The caller must always receive the exception.

### 2.3 The "Dangling Sub-Object" Hazard: Accessing Members in Catch Block is UB
- By the time control enters the constructor's function-try-block `catch` clause:
  1. Any member sub-objects constructed before the throw have **already been destroyed** by stack unwinding!
  2. Any base class sub-objects have **already been destroyed**!

> [!CAUTION]
> Accessing any non-static member variable or calling any member function inside the `catch` block of a constructor function-try-block is **Undefined Behavior (UB)**! Only static members and constructor parameters may be safely inspected.

### 2.4 Legitimate Use Cases: Exception Translation and Logging
Given these strict limitations, why do function-try-blocks exist?
1. **Exception Translation**: Catching low-level library/OS exceptions thrown during base/member initialization and rethrowing a domain-specific exception.
2. **Logging & Diagnostics**: Logging boundary construction failures before the exception propagates to the caller.
3. **External Non-Memory Resource Cleanup**: Cleaning up external locks, shared memory, or database transactions acquired prior to sub-object initialization.

### 📁 Code Examples for Section 2
- [`2_function_try_blocks_in_constructors.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/01_Object_Lifecycle_and_Exception_Safety/2_function_try_blocks_in_constructors.cpp): Demonstrates constructor function-try-block syntax, sub-object destruction timing, and mandatory implicit rethrow behavior.

---

## 3. Move Semantics, noexcept, and the std::vector Reallocation Dilemma

### 3.1 The Three Exception Safety Guarantees (Basic, Strong, No-Throw)
| Guarantee | Formal Definition |
|---|---|
| **Basic Guarantee** | If an exception throws, no resources leak and all objects remain in valid (though unspecified) states. Invariants hold. |
| **Strong Guarantee** | **Commit or Rollback**: If an operation throws, program state is restored exactly as it was before the operation began. |
| **No-Throw (`noexcept`)** | The operation is guaranteed to never throw under any condition. |

### 3.2 Why std::vector Reallocation Requires the Strong Guarantee
- When `std::vector::push_back()` exceeds current capacity, it must:
  1. Allocate a larger contiguous memory buffer.
  2. Transfer existing elements from the old buffer to the new buffer.
  3. Construct the new element at the end.
  4. Deallocate the old buffer.
- `std::vector::push_back()` guarantees the **Strong Exception Guarantee**.
- If copying elements from old to new buffer throws, `std::vector` deallocates the new buffer; the old buffer remains completely intact.
- **The Move Dilemma**:
  - If an element's move constructor throws halfway through (e.g., on element 4 of 10):
    - Elements 0 to 3 have already been moved into the new buffer (leaving old elements 0–3 in moved-from states).
    - Element 4 threw an exception.
    - Elements 5 to 9 remain in the old buffer.
  - The old buffer **cannot be restored** because elements 0–3 were already modified by move semantics! The Strong Guarantee would be broken!

### 3.3 The Role of std::move_if_noexcept and std::is_nothrow_move_constructible
- To prevent this state corruption, the standard library uses `std::move_if_noexcept(x)`:
  - If `std::is_nothrow_move_constructible_v<T>` is `true`, it casts to an rvalue reference (`T&&`), enabling move semantics.
  - If `false` (and `T` is copyable), it casts to `const T&`, **forcing a deep copy**!

### 3.4 The Silent Performance Cliff: Falling Back to Deep Copies
- If a developer implements a move constructor but forgets to mark it `noexcept`:
  ```cpp
  class Widget {
  public:
      Widget(Widget&& other); // Missing noexcept!
  };
  ```
- During vector reallocation, `std::vector` detects that `Widget`'s move constructor might throw.
- **Consequence**: `std::vector` **silently ignores the move constructor** and performs expensive deep copies for every element during reallocation!
- This causes massive performance degradation in high-throughput systems with zero warnings from the compiler.

### 3.5 Best Practice: Marking Move Constructors and Move Assignment noexcept
- Move operations transfer ownership of pointers and handles; they should almost never allocate or throw.
- **Always mark move constructors and move assignment operators `noexcept`**:

```cpp
Widget(Widget&& other) noexcept;
Widget& operator=(Widget&& other) noexcept;
```

### 📁 Code Examples for Section 3
- [`3_noexcept_move_vector_reallocation.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/01_Object_Lifecycle_and_Exception_Safety/3_noexcept_move_vector_reallocation.cpp): Measures copy counts vs. move counts during vector capacity expansion, showing 100% move utilization when `noexcept` is present vs. silent fallback to copies when omitted.

---

## 4. The Copy-and-Swap Idiom

### 4.1 The Deficiencies of Naive Copy Assignment Operators
A naive copy assignment operator typically looks like this:

```cpp
MyString& MyString::operator=(const MyString& other) {
    if (this == &other) return *this; // Self-assignment check
    delete[] m_data;                  // Release old resource
    m_data = new char[other.m_len];   // Allocate new resource
    std::memcpy(m_data, other.m_data, other.m_len);
    m_len = other.m_len;
    return *this;
}
```

> [!WARNING]
> 1. **Exception Safety Failure**: If `new char[other.m_len]` throws `std::bad_alloc`, `m_data` has **already been deleted**! `*this` is corrupted with a dangling pointer, violating even the Basic Guarantee.
> 2. **Code Duplication**: Memory allocation and copy logic is duplicated between copy constructor and assignment operator.

### 4.2 Self-Assignment Branching Overhead vs Robustness
- The `if (this == &other)` branch is required in naive code to prevent deleting one's own data before reading it.
- In 99.99% of normal executions, objects are not self-assigned. This branch introduces unnecessary branch-prediction overhead in tight loops.

### 4.3 Mechanics of Copy-and-Swap: Pass-by-Value and Non-Throwing Swap
- **Step 1**: Implement a non-throwing friend `swap` function using ADL (Argument-Dependent Lookup):
  ```cpp
  friend void swap(MyString& first, MyString& second) noexcept {
      using std::swap;
      swap(first.m_data, second.m_data);
      swap(first.m_len, second.m_len);
  }
  ```
- **Step 2**: Implement the unified assignment operator passing the parameter **by value**:
  ```cpp
  MyString& operator=(MyString other) noexcept {
      swap(*this, other);
      return *this;
  }
  ```

### 4.4 How Copy-and-Swap Unifies Copy and Move Assignment
Because `other` is passed by value:
1. **When passed an lvalue (`str1 = str2;`)**:
   - The copy constructor runs to initialize `other`. If allocation throws, it happens **before entering `operator=`**, leaving `str1` completely untouched!
   - `swap(*this, other)` swaps `str1`'s old buffer into `other`.
   - When `operator=` exits, `other` is destroyed, executing the destructor and freeing the old buffer automatically!
2. **When passed an rvalue (`str1 = std::move(str2);` or `str1 = MyString("temp");`)**:
   - The move constructor runs to initialize `other` (zero heap allocation).
   - `swap(*this, other)` transfers the new buffer into `str1`.
   - The old buffer is cleaned up when `other` leaves scope.

### 4.5 Exception Safety Guarantees Provided by Copy-and-Swap
- **Strong Exception Guarantee**: If copying parameter `other` fails, the target object was never touched. Once inside `operator=`, the swap is strictly `noexcept`.
- **Inherent Self-Assignment Safety**: `str1 = str1;` creates a copy of `str1`, swaps with `str1`, and cleans up the copy cleanly without any explicit branching.

### 📁 Code Examples for Section 4
- [`4_copy_and_swap_strong_exception_guarantee.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/01_Object_Lifecycle_and_Exception_Safety/4_copy_and_swap_strong_exception_guarantee.cpp): Full implementation of copy-and-swap with unified assignment, self-assignment verification, and move-assignment reuse.

---

## 5. Construction and Destruction Order in Complex Hierarchies

### 5.1 Standard Construction Sequence (Virtual Bases, Non-Virtual Bases, Members)
The ISO C++ standard specifies a strict, deterministic order of construction:
1. **Virtual Base Classes**:
   - Initialized first, regardless of where they appear in the inheritance hierarchy.
   - Initialized in depth-first, left-to-right order as declared in the inheritance graph.
   - **Critical Rule**: Virtual base classes are **always initialized directly by the most-derived class constructor**. Intermediate base class initializers for virtual bases are ignored.
2. **Non-Virtual Direct Base Classes**:
   - Initialized in the exact order they appear in the class declaration's base-specifier list (left-to-right).
3. **Non-Static Data Members**:
   - Initialized in the order they are **declared in the class definition**.
4. **Constructor Body**:
   - The code inside `{ ... }` of the constructor executes last.

### 5.2 Why Member Initialization Order Depends Solely on Class Declaration Order
Consider:
```cpp
class Example {
    int a;
    int b;
public:
    Example(int val) : b(val), a(b + 1) {} // Order in list: b then a
};
```

> [!WARNING]
> Member `a` is declared before `b` in the class definition.
> The compiler initializes `a` **first** using `b + 1`. But `b` is uninitialized garbage!
> **Result: Undefined Behavior!**
> **Rationale**: Destructors must destroy members in the exact reverse order of construction. If initialization order depended on the constructor's initializer list, different constructors could initialize members in different orders, making a single deterministic destructor order impossible!

### 5.3 Standard Destruction Sequence (Exact Mirror Reversal)
Destruction occurs in the **exact reverse order** of construction:
1. Destructor body executes first.
2. Non-static data members are destroyed in **reverse order of class declaration**.
3. Non-virtual direct base classes are destroyed in **reverse order of base-specifier list** (right-to-left).
4. Virtual base classes are destroyed last, in **reverse order of construction**.

### 5.4 Virtual Function Dispatch During Construction and Destruction
What happens when a virtual function is called inside a Base class constructor?

```cpp
class Base {
public:
    Base() { print(); }
    virtual void print() { std::cout << "Base\n"; }
};
class Derived : public Base {
    int* data;
public:
    Derived() : data(new int(42)) {}
    void print() override { std::cout << "Derived: " << *data << "\n"; }
};
```

- **Output**: Prints `"Base"`, **NOT** `"Derived"`!
- **Why?**:
  - During the execution of `Base::Base()`, the `Derived` sub-object has not been constructed yet!
  - If dynamic dispatch invoked `Derived::print()`, it would attempt to access `*data` (an uninitialized pointer), causing an immediate crash or undefined behavior.
  - The C++ standard mandates that during construction of `Base`, the dynamic type of the object **is `Base`**. The compiler updates the vptr at each level of the inheritance hierarchy.

### 5.5 The Pure Virtual Function Call Hazard (__cxa_pure_virtual)
- If a base class constructor calls a pure virtual function (`= 0`) indirectly:
  ```cpp
  class Abstract {
  public:
      Abstract() { call_pure(); }
      void call_pure() { pure_func(); } // Indirect call defeats compiler warning
      virtual void pure_func() = 0;
  };
  ```
- Because the dynamic type is `Abstract`, the vtable entry points to the runtime error handler `__cxa_pure_virtual`.
- The program aborts immediately: `"pure virtual method called"`!

> [!IMPORTANT]
> **Golden Rule**: Never call virtual functions from constructors or destructors.

### 📁 Code Examples for Section 5
- [`5_construction_destruction_order_complex_hierarchies.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs-Advanced/01_Object_Lifecycle_and_Exception_Safety/5_construction_destruction_order_complex_hierarchies.cpp): Complete execution trace of virtual bases, multiple direct bases, member sub-objects, and virtual call suppression during construction and destruction.
