# Chapter 22: Move Semantics and Smart Pointers

Raw pointers in C++ provide direct hardware access and flexibility, but they carry substantial risks: memory leaks, dangling pointers, double-free crashes, and manual lifetime management. **Smart pointers** and **move semantics**—introduced in **C++11**—revolutionize resource management in modern C++ by automating object destruction through RAII (Resource Acquisition Is Initialization) and eliminating expensive, redundant deep copies through safe ownership transfers.

---

### Table of Contents
1. [1 — Introduction to Smart Pointers and Move Semantics](#1--introduction-to-smart-pointers-and-move-semantics)
2. [2 — R-value References](#2--r-value-references)
3. [3 — Move Constructors and Move Assignment](#3--move-constructors-and-move-assignment)
4. [4 — std::move](#4--stdmove)
5. [5 — std::unique_ptr](#5--stdunique_ptr)
6. [6 — std::shared_ptr](#6--stdshared_ptr)
7. [7 — Circular Dependency Issues with std::shared_ptr, and std::weak_ptr](#7--circular-dependency-issues-with-stdshared_ptr-and-stdweak_ptr)

---

## 1 — Introduction to Smart Pointers and Move Semantics

The fundamental problem with raw pointers is that they have **no inherent cleanup mechanism**. Even when a programmer diligently writes `delete ptr;` at the end of a function, early returns, thrown exceptions, or branching logic frequently bypass the `delete` statement, resulting in permanent memory leaks:

```cpp
void processData()
{
    int* ptr = new int{42};

    if (someCondition())
        return; // LEAK: ptr is never deleted!

    // ...
    delete ptr;
}
```

### Smart Pointer Classes to the Rescue

Classes in C++ contain **destructors** that automatically execute whenever an object of that class goes out of scope. If a class allocates (or acquires) a resource in its constructor and deallocates it in its destructor, that resource is guaranteed to be reclaimed whenever the class instance is destroyed, regardless of how control exits the scope (normal exit, early return, or thrown exception).

A **smart pointer** is a composition class designed to manage a dynamically allocated resource and guarantee its destruction when the smart pointer object goes out of scope.

```cpp
template <typename T>
class SimpleSmartPtr
{
    T* m_ptr{};
public:
    explicit SimpleSmartPtr(T* ptr = nullptr) : m_ptr{ptr} {}
    ~SimpleSmartPtr() { delete m_ptr; } // Automatically frees resource!

    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
};
```

### Why Smart Pointers Use Move Semantics Instead of Copy Semantics

If a smart pointer class uses default copy operations, copying one smart pointer to another produces a **shallow copy**—both smart pointers now contain the same raw memory address:

```
Smart Pointer A: m_ptr -------------\
                                     >----> [ Dynamic Resource ]
Smart Pointer B: m_ptr -------------/
```

When `Smart Pointer A` goes out of scope, its destructor deletes the resource. When `Smart Pointer B` subsequently goes out of scope, its destructor attempts to delete the exact same resource a second time, resulting in a catastrophic **double-free crash**.

To avoid this, one might consider performing a **deep copy** (duplicating the underlying heap resource). However, deep copying:
1. **May be impossible**: Certain system resources (file handles, network sockets, active thread handles, mutexes) cannot be duplicated.
2. **May be extremely expensive**: Duplicating large heap buffers, dynamic arrays, high-resolution textures, or database records incurs severe CPU and memory allocation overhead.
3. **May break semantics**: A unique ownership model requires that exactly one entity owns the resource at any given moment.

Because of these limitations, smart pointers rely on **move semantics**: instead of duplicating the resource, ownership is **transferred** from the source to the destination, and the source pointer is reset to `nullptr`.

### 📁 Code Examples for Section 1
- [`22_3_Move_constructors_n_move_assignment/1_simple_smart_pointer.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_3_Move_constructors_n_move_assignment/1_simple_smart_pointer.cpp): Demonstrates a basic smart pointer class managing dynamically allocated memory via RAII and shows why shallow copying raw pointers leads to undefined behavior and double-free crashes.

---

## 2 — R-value References

Prior to C++11, C++ only possessed l-value references (declared with a single ampersand `&`). C++11 introduced **r-value references** (declared with two ampersands `&&`) to distinguish temporary, expiring objects from persistent named objects.

### L-value References Review

An **l-value reference** can only be initialized with a modifiable l-value:

| Reference Type | Initialized With | Can Modify? |
|---|:---:|:---:|
| **Modifiable l-values** | Yes | Yes |
| **Non-modifiable l-values** | No | No |
| **R-values (temporaries/literals)** | No | No |

An **l-value reference to const** (`const Type&`) can bind to modifiable l-values, non-modifiable l-values, and r-values alike, but the referenced value cannot be modified:

| Reference to Const (`const Type&`) | Initialized With | Can Modify? |
|---|:---:|:---:|
| **Modifiable l-values** | Yes | No |
| **Non-modifiable l-values** | Yes | No |
| **R-values** | Yes | No |

> [!NOTE]
> L-value references to const objects are foundational in C++ because they allow passing any argument (l-value or r-value) into a function efficiently without creating a copy.

### R-value References

An **r-value reference** is a reference designed to bind **only** to r-values (temporaries, literals, or expiring values):

```cpp
int x{5};
int& lref{x};       // OK: l-value reference binds to l-value x
// int&& rref1{x};  // Compile error: r-value reference cannot bind to l-value!
int&& rref2{5};     // OK: r-value reference binds to r-value literal 5
```

Binding characteristics of r-value references:

| Non-Const R-value Reference (`Type&&`) | Initialized With | Can Modify? |
|---|:---:|:---:|
| **Modifiable l-values** | No | No |
| **Non-modifiable l-values** | No | No |
| **R-values** | **Yes** | **Yes** |

| Const R-value Reference (`const Type&&`) | Initialized With | Can Modify? |
|---|:---:|:---:|
| **Modifiable l-values** | No | No |
| **Non-modifiable l-values** | No | No |
| **R-values** | **Yes** | No |

### Two Primary Properties of R-value References

1. **Lifetime Extension**: Initializing an r-value reference with a temporary object extends the temporary object's lifespan to match the lifespan of the r-value reference itself (l-value references to const can also extend lifetimes).
2. **Modifiability**: Unlike `const Type&`, a non-const r-value reference `Type&&` **allows modifying the temporary object**.

### R-value References as Function Parameters

R-value references are most commonly utilized as function parameters to implement **function overloads** that differentiate between l-value arguments (which must be preserved) and r-value arguments (which can be moved/stolen):

```cpp
void print(const int& lref)
{
    std::cout << "l-value reference to const: " << lref << '\n';
}

void print(int&& rref)
{
    std::cout << "r-value reference: " << rref << '\n';
}

int a{10};
print(a);  // Calls print(const int&) -> passed an l-value
print(20); // Calls print(int&&)       -> passed an r-value (exact match)
```

### Rvalue Reference Variables Are Themselves Lvalues

> [!IMPORTANT]
> **Value category and type are independent concepts!**
> A named variable whose type is an r-value reference is itself an **l-value** when used in an expression:
> ```cpp
> int&& ref{5}; // ref has type int&&, but 'ref' is a named variable, so it is an LVALUE!
> fun(ref);     // Calls fun(const int&), NOT fun(int&&)!
> ```
> Because `ref` is a named variable with identity, it evaluates as an l-value. Since `int&&` cannot bind to an l-value, `fun(ref)` can only bind to `fun(const int&)`.

### Returning an R-value Reference

> [!WARNING]
> **Never return an r-value reference from a function.**
> Just as with returning an l-value reference to a local variable, returning an r-value reference to an automatic local variable returns a **dangling reference** when the local object is destroyed upon function exit. Always return by value instead.

### 📁 Code Examples for Section 2
- [`22_2_R-value_references/rvalue.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_2_R-value_references/rvalue.cpp): Demonstrates basic syntax and binding rules of r-value references, modifying r-values, lifetime extension, and parameter overloading.
- [`22_2_R-value_references/rvalue_comprehensive.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_2_R-value_references/rvalue_comprehensive.cpp): Comprehensive 10-section demonstration covering lvalue vs rvalue binding tables, function overload resolution, the "named rvalue reference is an lvalue" rule, dangling reference hazards, and move constructor simulation.

---

## 3 — Move Constructors and Move Assignment

By default, C++ provides an implicit copy constructor and copy assignment operator that perform shallow copies. For classes managing dynamic resources, C++11 introduced two dedicated move functions:
- **Move constructor**: `ClassName(ClassName&& other) noexcept;`
- **Move assignment operator**: `ClassName& operator=(ClassName&& other) noexcept;`

Instead of allocating new memory and deep-copying data, move operations **steal the resources** of the source object by copying the internal pointers and resetting the source pointer to `nullptr`.

```cpp
template <typename T>
class AutoPtr
{
    T* m_ptr{};
public:
    explicit AutoPtr(T* ptr = nullptr) : m_ptr{ptr} {}

    ~AutoPtr() { delete m_ptr; }

    // Move constructor: transfers ownership and nulls out source
    AutoPtr(AutoPtr&& source) noexcept : m_ptr{source.m_ptr}
    {
        source.m_ptr = nullptr; // Critical: source must no longer own memory!
    }

    // Move assignment operator
    AutoPtr& operator=(AutoPtr&& source) noexcept
    {
        if (this == &source)
            return *this;

        delete m_ptr;           // 1. Release our existing resource
        m_ptr = source.m_ptr;   // 2. Steal resource from source
        source.m_ptr = nullptr; // 3. Reset source pointer to null

        return *this;
    }
};
```

### Calling Rules: Copy vs. Move

| Invocation Scenario | Function Invoked |
|---|---|
| Argument is an **l-value** (named object) | **Copy constructor** or **copy assignment** |
| Argument is an **r-value** (temporary, literal, or `std::move`) | **Move constructor** or **move assignment** |
| Argument is an **r-value**, but move functions are **not defined** | **Copy constructor** or **copy assignment** (fallback) |

### The Key Insight Behind Move Semantics

- When copying from an **l-value**, the compiler cannot modify the source object because the caller might read it again later in the program. Therefore, a deep copy is necessary.
- When constructing or assigning from an **r-value**, the source is a temporary object that will be **destroyed immediately** at the end of the full expression. Stealing its internal buffer is completely safe because the temporary will never be inspected again.
- **Move semantics is an optimization opportunity**: moving transforms an $O(N)$ deep allocation into an $O(1)$ shallow pointer swap.

### Moved-From Objects Must Remain in a Valid State

> [!IMPORTANT]
> When moving resources away from an object, the moved-from object must be left in a **valid state** so that its destructor can execute cleanly without triggering undefined behavior (e.g. setting pointers to `nullptr` so `delete` safely no-ops).

### Why `noexcept` is Critical for Move Operations

Move constructors and move assignment operators should always be marked `noexcept`:
- Standard library containers like `std::vector` offer the **strong exception guarantee** during dynamic resizing and reallocations.
- During reallocation, `std::vector` inspects elements using `std::move_if_noexcept()`.
- If a class's move constructor is **not** marked `noexcept`, `std::vector` will silently fall back to making expensive **deep copies** of every element during every reallocation to ensure exceptions cannot corrupt data.

### Implicit Move Generation Rules

The compiler will automatically synthesize an implicit move constructor and move assignment operator **only if ALL of the following are true**:
1. No user-declared **copy constructor** exists.
2. No user-declared **copy assignment operator** exists.
3. No user-declared **move constructor** exists.
4. No user-declared **move assignment operator** exists.
5. No user-declared **destructor** exists.

The implicit move functions perform a **memberwise move**:
- If a member has a move constructor/assignment, it is moved.
- Otherwise, the member is **copied**.
- **Pointers are copied, not moved!** Classes with raw pointer members must define move functions manually.

### The Rule of Five and Rule of Zero

- **The Rule of Five**: If a class defines or deletes any of the five special member functions (Destructor, Copy Constructor, Copy Assignment, Move Constructor, Move Assignment), it should explicitly define or delete all five.
- **The Rule of Zero**: Design classes to manage resources using existing standard RAII types (`std::unique_ptr`, `std::vector`, `std::string`) so that none of the five special functions need to be written manually.

### Move Semantics and `std::swap` Recursion Hazard

Implementing move operations using `std::swap()` is dangerous:
- `std::swap(a, b)` internally calls the move constructor and move assignment operator of its arguments!
- Calling `std::swap` inside your class's move constructor or move assignment results in **infinite recursion and stack overflow**.
- **Solution**: Implement move operations using a custom `swap()` member function that directly swaps internal member variables without invoking `std::swap` on the class itself.

```cpp
void AutoPtr::swap(AutoPtr& other) noexcept
{
    std::swap(m_ptr, other.m_ptr); // Swaps raw pointers, does NOT call AutoPtr move!
}

AutoPtr::AutoPtr(AutoPtr&& source) noexcept
{
    swap(source); // Safe move via custom swap!
}
```

### Guaranteed Copy Elision (C++17)

Since **C++17**, when an object is initialized from a **prvalue** (pure rvalue, such as a temporary or direct return value), the compiler is mandated to construct the object directly in place. This is called **guaranteed copy elision**. In C++17, neither the copy constructor nor the move constructor is called, even if they are deleted!

### 📁 Code Examples for Section 3
- [`22_3_Move_constructors_n_move_assignment/2_copy_or_move_constructor_on_pass_by_return_by_value.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_3_Move_constructors_n_move_assignment/2_copy_or_move_constructor_on_pass_by_return_by_value.cpp): Demonstrates when copy vs move constructors are called during pass-by-value and return-by-value.
- [`22_3_Move_constructors_n_move_assignment/3_test_implicit_move.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_3_Move_constructors_n_move_assignment/3_test_implicit_move.cpp): Verifies conditions under which the compiler generates implicit move constructors and memberwise move behaviors.
- [`22_3_Move_constructors_n_move_assignment/4_user_defined_unique_smart_pointer.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_3_Move_constructors_n_move_assignment/4_user_defined_unique_smart_pointer.cpp): Implements a fully functional move-only unique smart pointer class disabling copy operations.
- [`22_3_Move_constructors_n_move_assignment/5_issue_with_std_swap.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_3_Move_constructors_n_move_assignment/5_issue_with_std_swap.cpp): Demonstrates infinite recursion bugs caused by invoking `std::swap()` inside a move constructor.
- [`22_3_Move_constructors_n_move_assignment/6_issue_with_user_swap.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_3_Move_constructors_n_move_assignment/6_issue_with_user_swap.cpp): Demonstrates safe copy-and-swap and move-and-swap patterns using a dedicated non-recursive member `swap()` function.
- [`22_3_Move_constructors_n_move_assignment/I_no_semantics_check_on_returning_prvalue.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_3_Move_constructors_n_move_assignment/I_no_semantics_check_on_returning_prvalue.cpp): Demonstrates C++17 guaranteed copy elision for prvalues bypassing both copy and move constructors.
- [`22_3_Move_constructors_n_move_assignment/I_semantics_check_on_returning_lvalue.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_3_Move_constructors_n_move_assignment/I_semantics_check_on_returning_lvalue.cpp): Demonstrates returning named local variables by value, showing automatic move fallback when NRVO is not applied.
- [`22_3_Move_constructors_n_move_assignment/performance/main.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_3_Move_constructors_n_move_assignment/performance/main.cpp): Measures the real-world performance benefit of move semantics versus copy semantics on large dynamic arrays.
- [`22_3_Move_constructors_n_move_assignment/performance/dynamic_array.hpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_3_Move_constructors_n_move_assignment/performance/dynamic_array.hpp): Header definition for the move-enabled dynamic array template benchmarked above.

---

## 4 — std::move

When we have a persistent named variable (an l-value) whose value we no longer need, we want a mechanism to treat that l-value as an r-value so that its resources can be moved rather than copied.

`std::move` (defined in `<utility>`) is a standard library function template that casts its argument into an r-value reference:

```cpp
#include <utility>
#include <string>

std::string a{"Hello, World!"};
std::string b{std::move(a)}; // Forces invocation of std::string move constructor!
```

### What `std::move` Actually Does

> [!IMPORTANT]
> `std::move` **does not move anything itself at runtime and generates zero machine code instructions!**
> It is purely a compile-time cast equivalent to:
> ```cpp
> static_cast<std::remove_reference_t<T>&&>(arg)
> ```
> By casting the expression to an r-value (specifically an **xvalue** / expiring value), `std::move` instructs compiler overload resolution to select move functions over copy functions.

### Moved-From Objects: Valid but Unspecified State

The C++ standard dictates: *“Unless otherwise specified, moved-from objects of types defined in the C++ standard library shall be placed in a valid but unspecified state.”*

- **Valid state**: The object must still satisfy all its class invariants, and its destructor must run safely without crashing.
- **Unspecified state**: You cannot predict what contents the object holds. For instance, a moved-from `std::vector` may be empty or retain capacity.

Rules for using moved-from objects:
1. **Do not read values**: Avoid inspecting the value of moved-from objects (e.g. calling `v.front()` or indexing `v[0]`).
2. **Reassignment is safe**: It is completely safe to assign a brand new value to a moved-from object via `operator=`.
3. **Resetting/Clearing is safe**: Calling state-resetting member functions (e.g. `v.clear()`, `v.empty()`, `ptr.reset()`) is valid.

### Using `std::move` with Containers and Sorting

`std::move` is widely used when populating containers to avoid duplicate copies:

```cpp
std::vector<std::string> words;
std::string input{"Optimization"};
words.push_back(std::move(input)); // Resource stolen into vector buffer; input emptied
```

Similarly, sorting algorithms (such as quicksort, bubblesort, and `std::sort`) use `std::move` internally within swap operations to avoid expensive deep copies while exchanging elements.

### `std::move_if_noexcept`

`std::move_if_noexcept` is a conditional variant of `std::move`:
- If the object has a `noexcept` move constructor, it returns an r-value reference (enabling move semantics).
- If the move constructor might throw an exception, it returns a const l-value reference (falling back to copy semantics).

### 📁 Code Examples for Section 4
- [`22_4_std_move/1_move_over_copy.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_4_std_move/1_move_over_copy.cpp): Demonstrates using `std::move` to explicitly cast l-values to r-values to invoke move constructors and move assignment operators.
- [`22_4_std_move/2_std_move_for_vectors.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_4_std_move/2_std_move_for_vectors.cpp): Demonstrates using `std::move` when inserting l-values into `std::vector` to prevent redundant heap allocations.

---

## 5 — std::unique_ptr

`std::unique_ptr` (introduced in **C++11**) is the modern replacement for the deprecated and removed `std::auto_ptr`. It should be used to manage dynamically allocated objects that have **exclusive, non-shared ownership**.

> [!NOTE]
> `std::auto_ptr` (from C++98) was deprecated in C++11 and completely **removed in C++17** because its copy constructor silently transferred ownership, causing dangerous bugs when passed to functions or stored in containers. `std::unique_ptr` fixes this by explicitly deleting copy operations and requiring move semantics.

### Always Allocate Smart Pointers on the Stack

Smart pointers must **always be allocated on the stack** (as local automatic variables or composition members of classes). Never dynamically allocate a smart pointer with `new std::unique_ptr<T>`—doing so would defeat the entire purpose of RAII by requiring manual deletion of the smart pointer!

### Basic Usage and Accessing the Managed Object

`std::unique_ptr` overloads `operator*` and `operator->` to mimic a raw pointer:
- `operator*` returns a reference to the managed object.
- `operator->` returns a pointer to the managed object.
- An implicit conversion to `bool` (`operator bool()`) checks whether the `std::unique_ptr` currently owns a resource (`if (ptr) { ... }`).
- `.get()` returns the underlying raw pointer without releasing ownership.
- `.release()` relinquishes ownership and returns the raw pointer (caller is now responsible for deleting it).
- `.reset(newPtr)` deallocates the current resource and takes ownership of `newPtr`.

```cpp
#include <memory>

std::unique_ptr<int> ptr{new int{100}};
if (ptr)
{
    *ptr = 200; // operator*
}

// Copying is prohibited:
// std::unique_ptr<int> copyPtr = ptr; // Compile ERROR: copy constructor is deleted!

// Moving transfers ownership:
std::unique_ptr<int> movedPtr = std::move(ptr); // OK: ptr becomes nullptr
```

### `std::make_unique` (C++14)

C++14 introduced `std::make_unique<T>(args...)`, which constructs the object and returns a `std::unique_ptr`:

```cpp
auto res = std::make_unique<Fraction>(3, 5); // Preferred modern C++ syntax
```

#### Why `std::make_unique` is Strongly Recommended

1. **Simpler syntax**: Avoids repeating the type name (`std::unique_ptr<Fraction>(new Fraction(3, 5))`).
2. **Exception safety before C++17**: Consider:
   ```cpp
   someFunction(std::unique_ptr<T>(new T), mightThrowException());
   ```
   Prior to C++17, argument evaluation order was unspecified. A compiler could execute `new T`, then call `mightThrowException()`, and only then construct `std::unique_ptr`. If `mightThrowException()` threw an exception, `T` was leaked because the managing smart pointer was never created. `std::make_unique` eliminates this gap by encapsulating allocation and pointer construction together.

### Passing and Returning `std::unique_ptr`

- **Returning from a function**: `std::unique_ptr` can be safely returned **by value**. Move semantics (or C++17 guaranteed copy elision) efficiently transfers ownership to the caller.
- **Passing by value**: Pass `std::unique_ptr` by value when the function **intends to take ownership** of the resource. Callers must pass the argument via `std::move`.
- **Passing for inspection/use**: If a function only needs to use or inspect the resource without claiming ownership, **do not pass `std::unique_ptr`**! Instead, pass the underlying object by reference (`const T&` or `T&`) or raw pointer (`T*`).

### Misusing `std::unique_ptr`

> [!WARNING]
> 1. **Never let two smart pointers independently manage the same raw pointer**:
>    ```cpp
>    int* raw = new int{5};
>    std::unique_ptr<int> p1{raw};
>    std::unique_ptr<int> p2{raw}; // BUG: Both will attempt to delete 'raw'! Double-free crash!
>    ```
> 2. **Never manually `delete` the raw pointer held by a smart pointer**:
>    ```cpp
>    delete p1.get(); // BUG: p1 will delete it again when it goes out of scope!
>    ```
> 3. **Never bind a smart pointer to stack-allocated memory**:
>    ```cpp
>    int x = 10;
>    std::unique_ptr<int> p{&x}; // BUG: Destructor will call delete on stack memory!
>    ```

### 📁 Code Examples for Section 5
- [`22_5_std_unique_ptr/1_std_unique_pointer.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_5_std_unique_ptr/1_std_unique_pointer.cpp): Demonstrates `std::unique_ptr` creation, `std::make_unique`, `operator*`, `operator->`, null-checks, ownership transfer with `std::move`, and passing/returning unique pointers.
- [`22_5_std_unique_ptr/2_misusing_std_unique_ptr.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_5_std_unique_ptr/2_misusing_std_unique_ptr.cpp): Highlights common anti-patterns with `std::unique_ptr` (duplicate ownership, manual deletion, stack address binding) and explains how `std::make_unique` prevents them.

---

## 6 — std::shared_ptr

`std::shared_ptr` (introduced in **C++11**) solves scenarios where multiple independent entities need **shared co-ownership** of a dynamically allocated resource.

- A resource managed by `std::shared_ptr` is kept alive as long as **at least one** `std::shared_ptr` points to it.
- When the last `std::shared_ptr` managing the resource is destroyed or reassigned, the resource is automatically deallocated.
- Internally, `std::shared_ptr` maintains a **reference count** tracking the number of active co-owners.

```cpp
#include <memory>
#include <iostream>

auto ptr1 = std::make_shared<int>(42);
std::cout << "Count: " << ptr1.use_count() << '\n'; // Count: 1

{
    std::shared_ptr<int> ptr2{ptr1}; // Cloned via copy constructor
    std::cout << "Count: " << ptr1.use_count() << '\n'; // Count: 2
} // ptr2 goes out of scope; count decrements to 1

std::cout << "Count: " << ptr1.use_count() << '\n'; // Count: 1
// When ptr1 goes out of scope, count reaches 0 -> memory deleted!
```

### Digging into `std::shared_ptr`: The Control Block

Unlike `std::unique_ptr` (which stores only a single raw pointer), `std::shared_ptr` internally stores **two pointers**:
1. A pointer to the **managed resource**.
2. A pointer to a dynamically allocated **control block**.

The control block manages:
- The **strong reference count** (number of `std::shared_ptr` owners).
- The **weak reference count** (number of `std::weak_ptr` observers).
- Custom deleters and allocators (if any).

```
std::shared_ptr
┌──────────────┐       ┌────────────────────────┐
│ Resource Ptr │ ────> │ Managed Object         │
├──────────────┤       └────────────────────────┘
│ Control Blk  │ ────> ┌────────────────────────┐
└──────────────┘       │ Shared Count: 2        │
                       │ Weak Count:   0        │
                       └────────────────────────┘
```

### `std::make_shared` vs. Direct `std::shared_ptr` Constructor

Always prefer `std::make_shared<T>()`:
- When writing `std::shared_ptr<T>(new T)`, **two separate heap allocations** occur: one for `T`, and a second for the control block.
- `std::make_shared<T>()` combines both the managed object and the control block into **a single contiguous memory allocation**, significantly reducing memory fragmentation and allocator overhead.

> [!WARNING]
> Always clone a `std::shared_ptr` by **copying an existing `std::shared_ptr`**, never by passing the raw resource pointer twice!
> ```cpp
> Resource* res = new Resource;
> std::shared_ptr<Resource> sp1{res}; // Allocates Control Block 1 (count = 1)
> std::shared_ptr<Resource> sp2{res}; // BUG: Allocates independent Control Block 2 (count = 1)!
> ```
> Both pointers will assume they are the sole owner, leading to a double-free crash when the first one goes out of scope!

### Converting `std::unique_ptr` to `std::shared_ptr`

A `std::unique_ptr` can be seamlessly converted into a `std::shared_ptr` because unique ownership can always be upgraded to shared ownership:

```cpp
std::unique_ptr<Resource> up = std::make_unique<Resource>();
std::shared_ptr<Resource> sp = std::move(up); // Ownership transferred to shared_ptr
```

However, a `std::shared_ptr` **cannot** be converted into a `std::unique_ptr`, because a shared resource cannot guarantee that no other co-owners exist.

> [!TIP]
> If you are authoring a factory function and are uncertain whether callers will need unique or shared ownership, **always return `std::unique_ptr` by value**. Callers who only need unique ownership can keep it, while callers who require shared ownership can seamlessly convert it to `std::shared_ptr`.

### 📁 Code Examples for Section 6
- [`22_6_std_shared_ptr/1_std_shared_ptr.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_6_std_shared_ptr/1_std_shared_ptr.cpp): Demonstrates creation of `std::shared_ptr`, `use_count()`, `std::make_shared`, single vs dual control block allocation, and converting `std::unique_ptr` to `std::shared_ptr`.

---

## 7 — Circular Dependency Issues with std::shared_ptr, and std::weak_ptr

While `std::shared_ptr` provides robust shared ownership, it has a serious vulnerability: **circular references** (also known as a **cyclical reference** or **cycle**).

A circular reference occurs when two or more objects reference each other via `std::shared_ptr`:

```
┌──────────┐        shared_ptr        ┌──────────┐
│ Object A │ ───────────────────────> │ Object B │
│          │ <─────────────────────── │          │
└──────────┘        shared_ptr        └──────────┘
```

When local variables pointing to `Object A` and `Object B` go out of scope:
1. `Object A`'s reference count drops from 2 to 1 (because `Object B` still holds a `shared_ptr` to `A`).
2. `Object B`'s reference count drops from 2 to 1 (because `Object A` still holds a `shared_ptr` to `B`).
3. Because neither reference count reaches 0, **neither object is ever deleted!** Both objects are permanently leaked in memory.

### Reductive Cycles

A cycle can even occur within a **single object**: if an object holds a `std::shared_ptr` pointing to **itself**, its reference count will never drop to 0, permanently preventing destruction.

### Enter `std::weak_ptr`

`std::weak_ptr` (introduced in **C++11**) was designed specifically to break shared pointer cycles.
- A `std::weak_ptr` is an **observer**: it can observe and access a resource managed by `std::shared_ptr`, but it **does not contribute to the reference count**.
- When the last `std::shared_ptr` managing an object goes out of scope, the object is deallocated even if `std::weak_ptr` instances are still pointing to it!

```cpp
#include <memory>
#include <string>

class Person
{
    std::string m_name;
    std::weak_ptr<Person> m_partner; // Using weak_ptr breaks circular dependency!
public:
    explicit Person(std::string_view name) : m_name{name} {}

    friend bool partnerUp(std::shared_ptr<Person>& p1, std::shared_ptr<Person>& p2)
    {
        p1->m_partner = p2;
        p2->m_partner = p1; // No cyclic ownership!
        return true;
    }
};
```

### Using `std::weak_ptr`

Because `std::weak_ptr` does not own the resource, it cannot be dereferenced directly (it provides no `operator*` or `operator->`). To use the managed resource, you must convert the `std::weak_ptr` into a `std::shared_ptr` using **`.lock()`**:

```cpp
std::shared_ptr<Person> partner = m_partner.lock(); // Converts weak_ptr to shared_ptr
if (partner)
{
    // Object is still alive! Safe to access via 'partner'
}
else
{
    // Object has already been destroyed!
}
```

### Avoiding Dangling Pointers with `expired()`

You can check whether the underlying object has been destroyed using the **`.expired()`** member function:
- Returns `false` if the resource is still alive (`use_count() > 0`).
- Returns `true` if the resource has been destroyed (`use_count() == 0`).

> [!CAUTION]
> Do not rely on `.expired()` followed by `.lock()` in multithreaded programs, as another thread could destroy the resource between the two calls. Instead, call `.lock()` directly and check if the returned `std::shared_ptr` is non-null!

### Summary of Smart Pointer Types

| Smart Pointer | Ownership Model | Copyable? | Movable? | Primary Use Case |
|---|---|:---:|:---:|---|
| `std::unique_ptr` | **Exclusive** (single owner) | No | Yes | Default choice for dynamic resources, factory functions |
| `std::shared_ptr` | **Shared** (co-ownership) | Yes | Yes | Multiple owners sharing data, graph nodes, caches |
| `std::weak_ptr` | **Observer** (non-owning) | Yes | Yes | Breaking shared pointer cycles, cache lookups, observers |

### 📁 Code Examples for Section 7
- [`22_7_circular_dependency_issues_with_std_shared_ptr_n_std_weak_ptr.cpp/1_circular_reference_issue.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_7_circular_dependency_issues_with_std_shared_ptr_n_std_weak_ptr.cpp/1_circular_reference_issue.cpp): Demonstrates the circular reference memory leak using `std::shared_ptr`.
- [`22_7_circular_dependency_issues_with_std_shared_ptr_n_std_weak_ptr.cpp/2_circular_reference_issue_solution.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_7_circular_dependency_issues_with_std_shared_ptr_n_std_weak_ptr.cpp/2_circular_reference_issue_solution.cpp): Resolves the circular reference memory leak by replacing one owning link with `std::weak_ptr`.
- [`22_7_circular_dependency_issues_with_std_shared_ptr_n_std_weak_ptr.cpp/3_reductive_circular_reference_issue.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_7_circular_dependency_issues_with_std_shared_ptr_n_std_weak_ptr.cpp/3_reductive_circular_reference_issue.cpp): Demonstrates a reductive circular reference where an object retains a `std::shared_ptr` to itself.
- [`22_7_circular_dependency_issues_with_std_shared_ptr_n_std_weak_ptr.cpp/4_weak_ptr_statuc_check.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/22_Move_Semantics_n_Smart_Pointers/22_7_circular_dependency_issues_with_std_shared_ptr_n_std_weak_ptr.cpp/4_weak_ptr_statuc_check.cpp): Demonstrates using `weak_ptr::expired()` and `weak_ptr::lock()` to safely test and access managed resources.
