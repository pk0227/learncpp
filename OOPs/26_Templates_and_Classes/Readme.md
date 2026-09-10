# Chapter 26: Templates and Classes

Templates are one of C++'s most powerful features for generic programming, allowing functions and classes to operate with generic types without sacrificing type safety or runtime efficiency. While function templates allow algorithms to be written generically, **class templates** extend this capability to data structures, enabling containers like `std::vector`, `std::array`, and custom abstractions to be written once and instantiated for any arbitrary type.

This chapter explores how to define class templates, organize template declarations and definitions across translation units, use compile-time non-type parameters, and customize template behavior using full and partial template specialization.

---

### Table of Contents
1. [1 — Template classes](#1--template-classes)
2. [2 — Template non-type parameters](#2--template-non-type-parameters)
3. [3 — Function template specialization](#3--function-template-specialization)
4. [4 — Class template specialization](#4--class-template-specialization)
5. [5 — Partial template specialization](#5--partial-template-specialization)
6. [6 — Partial template specialization for pointers](#6--partial-template-specialization-for-pointers)

---

## 1 — Template classes

A template is not an actual class or function — it is a **stencil** used by the compiler to generate concrete classes or functions when instantiated. Because of this, templates do not behave in quite the same way as traditional non-template classes during compilation and linking.

### The Problem with Splitting Templates into Header and Code Files

With non-template classes, standard C++ best practice is to place the class definition in a header file (`.h` or `.hpp`) and the member function definitions in a separate source file (`.cpp`). In this way, the member functions are compiled into an object file (`.o`) once, and other source files simply include the header.

**However, with templates, this traditional model breaks down:**
- Just like with function templates, the compiler will **only instantiate a class template if the template is used** (e.g. declaring a variable `GenericArray<int> intArray;`) in a translation unit.
- In order to perform the instantiation, the compiler **must see both**:
  1. The **full class template definition** (not just forward declarations).
  2. The **full member function template definitions** for the specific template types needed.
- If member function definitions are placed in a separate `.cpp` file that is compiled independently:
  - When the compiler compiles `main.cpp`, it sees the header declarations but not the member definitions, so it cannot generate the machine code for `GenericArray<int>::resize()`. It assumes another translation unit will provide the symbol.
  - When the compiler compiles `GenericArray.cpp`, it doesn't see any usages of `GenericArray<int>`, so it instantiates nothing!
  - During the link phase, the linker fails with **undefined reference** errors for all invoked member functions.

```
       Translation Unit (main.cpp)               Translation Unit (GenericArray.cpp)
┌─────────────────────────────────────────┐    ┌─────────────────────────────────────────┐
│ #include "GenericArray.hpp"             │    │ #include "GenericArray.hpp"             │
│ GenericArray<int> arr{ 5 };             │    │ template<typename T>                    │
│ arr.resize(10);                         │    │ void GenericArray<T>::resize(...) {...} │
│                                         │    │                                         │
│ Compiler expects external symbol for    │    │ No instances created because no types   │
│ GenericArray<int>::resize()             │    │ are used here!                          │
└────────────────────┬────────────────────┘    └────────────────────┬────────────────────┘
                     │                                              │
                     └──────────────────┐        ┌──────────────────┘
                                        ▼        ▼
                               ┌─────────────────────────┐
                               │       Linker Error      │
                               │   Undefined Reference!  │
                               └─────────────────────────┘
```

### Solutions to the Template Organization Problem

#### Approach 1: The Single-Header Approach (Standard & Recommended)
The simplest and most common solution is to place the entire template class definition **and all member function definitions** directly in the header file (`GenericArray.hpp`):
- **Pros**: Simple, portable, and universally supported. When client code includes the header, the compiler has full visibility of both declarations and definitions, enabling immediate instantiation and aggressive inlining.
- **Cons**: If the template class is included in many files across a large project, every translation unit compiles duplicate copies of the template functions, which are later deduplicated by the linker. This can increase compile times.

```cpp
// GenericArray.hpp
#ifndef GENERIC_ARRAY_HPP
#define GENERIC_ARRAY_HPP

template <typename T>
class GenericArray {
    T* m_data{};
    int m_length{};
public:
    GenericArray(int length = 0);
    ~GenericArray() { delete[] m_data; }
    void resize(int newLength);
};

// Member functions defined in the same header!
template <typename T>
void GenericArray<T>::resize(int newLength) {
    // Implementation visible to all includers
}

#endif
```

#### Approach 2: The Three-File Approach (Explicit Instantiation)
An alternative pattern separates code into three files:
1. **Header file (`Array.hpp`)**: Contains the class template definition and member function declarations.
2. **Implementation file (`Array.cpp`)**: Contains the member function template definitions.
3. **Explicit instantiation file (`templates.cpp`)**: Includes both `Array.hpp` and `Array.cpp`, and explicitly instantiates the specific template types that the project requires:

```cpp
// templates.cpp
#include "GenericArray.hpp"
#include "GenericArray.cpp" // Included here to expose template definitions

// Explicit instantiations: tell compiler to generate full machine code for these types
template class GenericArray<int>;
template class GenericArray<double>;
```

| Strategy | Structure | Pros | Cons |
|---|---|---|---|
| **Single-Header** *(Recommended)* | Everything in `.hpp` | Simple, works with any type, zero linker issues | Increases build times in massive projects |
| **Three-File Approach** | `.hpp` + `.cpp` + `templates.cpp` | Compiles template code once into a single object file | Rigid: only explicitly listed types can be instantiated |

> [!NOTE]
> Member functions defined outside the class template body must repeat the template parameter list:
> ```cpp
> template <typename T>
> void GenericArray<T>::resize(int newLength) { /* ... */ }
> ```

### 📁 Code Examples for Section 1
- [`26_1_Template_classes/GenericArray.hpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_1_Template_classes/GenericArray.hpp): Class template header defining the `GenericArray<T>` container with memory management, indexing, and resizing declarations.
- [`26_1_Template_classes/GenericArray.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_1_Template_classes/GenericArray.cpp): Member function template definitions for `GenericArray<T>` separated from the header.
- [`26_1_Template_classes/templates.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_1_Template_classes/templates.cpp): Demonstrates explicit template instantiation (`template class GenericArray<int>;`) compiling concrete instances into a single translation unit.
- [`26_1_Template_classes/main.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_1_Template_classes/main.cpp): Complete test program validating instantiation, bounds checking, dynamic reallocation, copy semantics, and element insertion/deletion.

---

## 2 — Template non-type parameters

While standard template type parameters (`typename T` or `class T`) act as placeholders for data types, templates can also accept **non-type parameters**.

### What is a Non-Type Template Parameter?
A **template non-type parameter** is a template parameter where the type is predefined and is substituted for a **`constexpr` value** passed in as a template argument at compile time:

```cpp
template <typename T, int size> // size is a non-type parameter of type int!
class StaticArray {
    T m_array[size]{}; // Fixed compile-time size without dynamic allocation!
public:
    T* getArray() { return m_array; }
    T& operator[](int index) { return m_array[index]; }
};

StaticArray<int, 12> intArray;       // Valid: size is 12 (constexpr literal)
constexpr int bufferSize{ 64 };
StaticArray<double, bufferSize> buf; // Valid: bufferSize is constexpr
```

### Permitted Types for Non-Type Parameters
A non-type parameter can be any of the following types:
1. An **integral type** (`int`, `char`, `long`, `std::size_t`, etc.)
2. An **enumeration type** (scoped or unscoped enum)
3. A **pointer or reference** to a class object
4. A **pointer or reference** to a function
5. A **pointer or reference** to a class member function
6. **`std::nullptr_t`**
7. A **floating-point type** *(since C++20)*

> [!IMPORTANT]
> Non-type template arguments **must be constant expressions (`constexpr`)**. Non-const runtime variables cannot be passed as non-type template arguments because template instantiation occurs strictly at compile time:
> ```cpp
> int size{ 6 };
> StaticArray<double, size> arr; // COMPILE ERROR: 'size' is not usable in a constant expression!
> ```

### Why Use Non-Type Template Parameters?
- **Zero dynamic memory overhead**: Allows fixed-size buffer allocations on the stack (as used by `std::array<T, N>`).
- **Compile-time safety**: Array sizes become part of the type signature (`StaticArray<int, 5>` and `StaticArray<int, 10>` are distinct types), preventing inadvertent operations between differently sized buffers.
- **Compiler optimization**: Knowing buffer sizes at compile time allows loop unrolling and vectorization.

### 📁 Code Examples for Section 2
- [`26_2_Template_non-type_parameters/1_static_array.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_2_Template_non-type_parameters/1_static_array.cpp): Implements a stack-allocated fixed array using a non-type parameter `template <typename T, int size> class StaticArray`, demonstrating bounds, constexpr requirements, and stack memory layout.

---

## 3 — Function template specialization

When instantiating a function template for a given type, the compiler stencils out a copy of the templated function and substitutes the template type parameter with the concrete argument type. The resulting function has identical implementation details for every type.

However, occasionally a generic algorithm is inefficient or unsuitable for a specific data type, requiring a specialized implementation.

### Full vs Partial Specialization
- **Full specialization**: When **all** template parameters are explicitly specialized with concrete types/values.
- **Partial specialization**: When **only some** of the template parameters are specialized.

> [!IMPORTANT]
> **Function templates can only be fully specialized.** C++ does not allow partial specialization of function templates! (Only class templates can be partially specialized).

### Syntax for Full Function Template Specialization
In order to specialize a template, the compiler must first see the **primary template declaration**:

```cpp
#include <iostream>

// 1. Primary function template
template <typename T>
void printStorage(const T& value) {
    std::cout << value << '\n';
}

// 2. Full specialization for double (scientific formatting)
template <> // Empty angle brackets: all template parameters are fully specialized
void printStorage<double>(const double& value) {
    std::cout << std::scientific << value << '\n';
}
```

Key rules:
1. `template <>` tells the compiler that a full specialization follows with no remaining unbound template parameters.
2. `printStorage<double>` specifies which primary template is being specialized and for what type.
3. The specialization's signature must match the primary template (substituting `double` wherever `T` appeared).

### Precedence: Non-Template Functions vs Specializations

> [!TIP]
> If a matching **non-template function** and a matching **function template specialization** both exist, **the non-template function always takes precedence**:
> ```cpp
> void print(double val);               // Non-template overload (Takes priority!)
> template <> void print<double>(double val); // Template specialization
> ```
> **Best Practice (Herb Sutter's Rule):** Prefer overloading non-template functions instead of specializing function templates. Overload resolution rules for non-template functions are more intuitive and avoid subtle template-resolution edge cases.

### One Definition Rule (ODR) and Deleting Specializations
- **ODR in Headers**: Full template specializations are **not implicitly inline**. If you define a specialization in a header file included by multiple translation units, mark it `inline` to prevent multiple-definition linker errors:
  ```cpp
  template <>
  inline void printStorage<double>(const double& value) { /* ... */ }
  ```
- **Deleting Specializations**: Just like regular functions, template specializations can be deleted using `= delete` to disable a template for specific types:
  ```cpp
  template <>
  void printStorage<void*>(const void*&) = delete; // Prevents calling printStorage with void*
  ```

### 📁 Code Examples for Section 3
- [`26_3_Function_template_specialization/1_function_template_specialization.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_3_Function_template_specialization/1_function_template_specialization.cpp): Demonstrates primary function templates and full template specialization for `double` to print values in scientific notation.

---

## 4 — Class template specialization

Class template specialization allows writing a customized implementation of an entire class for a particular data type (or combination of types).

### Independence of Class Template Specializations
A specialized class template is treated as a **completely independent class**, even though it shares the same template name and instantiation syntax. This means:
- The specialization can have completely different member variables.
- It can have different member functions, different return types, and different access specifiers.
- It is implemented from scratch to optimize for the target type.

### Example: Compact `Storage8<bool>` Bitset
A generic `Storage8<T>` class holds 8 elements of type `T`. For `bool`, storing 8 separate `bool` variables would consume 8 bytes of memory. Using class template specialization, we can compress `Storage8<bool>` into a **single 8-bit unsigned char** using bit manipulation:

```cpp
#include <cstdint>

// Primary class template: stores 8 values of type T
template <typename T>
class Storage8 {
    T m_array[8]{};
public:
    void set(int index, const T& value) { m_array[index] = value; }
    const T& get(int index) const { return m_array[index]; }
};

// Full class template specialization for bool: compresses 8 bools into 1 byte!
template <>
class Storage8<bool> {
    std::uint8_t m_data{ 0 }; // 8 bits total!
public:
    void set(int index, bool value) {
        auto mask{ static_cast<std::uint8_t>(1 << index) };
        if (value)
            m_data |= mask;
        else
            m_data &= ~mask;
    }

    bool get(int index) const {
        auto mask{ static_cast<std::uint8_t>(1 << index) };
        return (m_data & mask) != 0;
    }
};
```

When an object `Storage8<int>` is created, the compiler instantiates the primary generic template (consuming 32 bytes). When `Storage8<bool>` is created, the compiler instantiates our optimized 1-byte specialization!

### Specializing Individual Member Functions
You can also specialize an individual member function without specializing the entire class:

```cpp
template <typename T>
class Storage {
    T m_value{};
public:
    Storage(T val) : m_value{ val } {}
    void print() const { std::cout << m_value << '\n'; }
};

// Specialize ONLY the print() member function for double
template <>
void Storage<double>::print() const {
    std::cout << std::scientific << m_value << '\n';
}
```

### Where to Define Class Template Specializations
- **In the same header**: Define specialized classes and functions in the same header file **immediately below the primary template definition**. This guarantees that whenever client code includes the header, the compiler sees both definitions.
- **Never use separate optional headers**: Avoid placing specializations in optional secondary headers. If a translation unit compiles without including the specialization header, it will silently use the primary template, causing unpredictable and inconsistent runtime behavior (and severe ODR violations).

### 📁 Code Examples for Section 4
- [`26_4_Class_template_specialization/1_class_template_specialization.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_4_Class_template_specialization/1_class_template_specialization.cpp): Demonstrates basic class template specialization for `Storage<double>`.
- [`26_4_Class_template_specialization/2_class_template_specialization_for_bool.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_4_Class_template_specialization/2_class_template_specialization_for_bool.cpp): Demonstrates the `Storage8<bool>` bitset optimization compressing 8 boolean flags into a single byte via bitwise masks.
- [`26_4_Class_template_specialization/3_specializing_member_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_4_Class_template_specialization/3_specializing_member_function.cpp): Demonstrates specializing a single member function (`Storage<double>::print()`) while leaving the rest of the generic class template unchanged.

---

## 5 — Partial template specialization

**Partial template specialization** allows specializing a template when **some, but not all**, template parameters are fixed, or when parameters are constrained (e.g. specialized for pointers or arrays).

| Specialization Type | Template Header | Class Name | Description |
|---|---|---|---|
| **Primary Template** | `template <typename T, int size>` | `StaticArray` | Generic unconstrained template |
| **Partial Specialization** | `template <typename T>` | `StaticArray<T, 50>` | `size` is fixed to 50; `T` remains variable |
| **Full Specialization** | `template <>` | `StaticArray<double, 50>` | Both `T` and `size` are fully specified |

### Functions Cannot Be Partially Specialized
> [!WARNING]
> In C++, **functions (both non-member functions and member functions) cannot be partially specialized!**
> Only class templates can be partially specialized.

```cpp
// Primary template
template <typename T, int size>
class StaticArray {
    T m_array[size]{};
public:
    void print() { /* ... */ }
};

// ILLEGAL: Cannot partially specialize an individual member function!
// template <typename T>
// void StaticArray<T, 50>::print() { /* COMPILE ERROR */ }
```

### Workarounds for Member Function Partial Specialization
Because member functions cannot be partially specialized independently, you can use the following standard design patterns:
1. **Partially specialize the entire class**: Create a partial specialization `template <typename T> class StaticArray<T, 50>` containing the customized member function.
2. **Inherit from a common base**: If partially specializing the entire class introduces redundant duplicate code for common functions, extract shared logic into a common generic base class (`StaticArrayBase<T, size>`), and only override or add specialized functions in the derived class.
3. **Delegate to a helper struct or function**: Have the member function call a free function or static struct member that can be overloaded:
   ```cpp
   template <typename T, int size>
   void StaticArray<T, size>::print() {
       printHelper(*this); // Dispatches via function overload resolution!
   }
   ```

### 📁 Code Examples for Section 5
- [`26_5_Partial_template_specialization/1_partial_template_non_member_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_5_Partial_template_specialization/1_partial_template_non_member_function.cpp): Demonstrates that non-member functions cannot be partially specialized, but can achieve equivalent behavior through function overloading.
- [`26_5_Partial_template_specialization/2_partial_template_member_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_5_Partial_template_specialization/2_partial_template_member_function.cpp): Demonstrates customizing member function behavior by partially specializing the enclosing class.
- [`26_5_Partial_template_specialization/3_partial_template_member_function_redundant.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_5_Partial_template_specialization/3_partial_template_member_function_redundant.cpp): Illustrates code redundancy when duplicating entire classes for partial specialization and explores cleaner refactoring strategies.

---

## 6 — Partial template specialization for pointers

When using class templates with pointer types, subtle and dangerous issues frequently arise regarding **ownership**, **lifetimes**, and **copying semantics**.

### The Problem with Pointers in Generic Templates
In a generic class template `Storage<T>`, the class stores and copies values of type `T`. When instantiated with a pointer type (e.g. `Storage<double*>`):
1. **Shallow Copying vs Deep Copying**: Copying `Storage<double*>` only copies the pointer address, not the allocated value. Both containers point to the exact same memory.
2. **Dangling Pointers**: If the pointee falls out of scope, `Storage` holds a dangling pointer.
3. **Resource Leaks vs Double-Free Bugs**:
   - If `Storage` does not delete the pointer in its destructor, dynamically allocated memory leaks.
   - If `Storage` *does* call `delete m_value` in its destructor, copying `Storage` triggers a catastrophic **double-free** crash when both destructors run!
4. **Address Printing**: A generic `std::cout << m_value` outputs the raw memory address (e.g. `0x7ffe0d...`) rather than the dereferenced numerical value.

### Solving Pointer Issues with Partial Specialization
We can partially specialize `Storage` specifically for pointer types:

```cpp
template <typename T>
class Storage<T*> // Partial specialization: T is still generic, but constrained to pointers!
{
    // Custom implementation tailored specifically for pointer types
};
```

### 1. Modern Best Practice: Managing Ownership with `std::unique_ptr`
Rather than writing fragile manual memory management, `Storage<T*>` can manage lifetime and ownership using `std::unique_ptr<T>`. This creates a deep copy on the heap and guarantees automatic cleanup upon destruction:

```cpp
#include <iostream>
#include <memory>

template <typename T>
class Storage {
    T m_value{};
public:
    Storage(T val) : m_value{ val } {}
    void print() const { std::cout << m_value << '\n'; }
};

// Partial specialization for pointer types
template <typename T>
class Storage<T*> {
    std::unique_ptr<T> m_value{}; // Automatically frees memory upon destruction!
public:
    // Deep copy allocated on the heap!
    Storage(T* val) : m_value{ std::make_unique<T>(val ? *val : 0) } {}

    void print() const {
        if (m_value)
            std::cout << *m_value << '\n'; // Dereferences pointer to print value!
    }
};
```

### 2. Restricting Pointer Types Completely
In containers where storing pointers is fundamentally unsafe or violates design intent, you can explicitly forbid pointer instantiations using compile-time type traits and `static_assert`:

```cpp
#include <type_traits>

template <typename T>
class Storage {
    // Forbid pointer types and std::nullptr_t at compile time!
    static_assert(!std::is_pointer_v<T> && !std::is_null_pointer_v<T>,
                  "Storage<T*> and Storage<nullptr> are strictly disallowed!");

    T m_value{};
public:
    Storage(T val) : m_value{ val } {}
};
```

Any attempt to instantiate `Storage<int*>` or `Storage<std::nullptr_t>` immediately halts compilation with an informative, user-friendly error message.

### 📁 Code Examples for Section 6
- [`26_6_Partial_template_specialization_for_pointers/1_partial_template_specialization_for_pointers.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_6_Partial_template_specialization_for_pointers/1_partial_template_specialization_for_pointers.cpp): Demonstrates the fundamental pitfalls of naive pointer storage in templates (shallow copies and address printing).
- [`26_6_Partial_template_specialization_for_pointers/2_partial_template_specialization_for_pointers_better.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_6_Partial_template_specialization_for_pointers/2_partial_template_specialization_for_pointers_better.cpp): Demonstrates deep copying and manual dynamic memory management in `Storage<T*>`.
- [`26_6_Partial_template_specialization_for_pointers/3_partial_template_specialization_for_pointers_restrict_not_allow_pointers.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_6_Partial_template_specialization_for_pointers/3_partial_template_specialization_for_pointers_restrict_not_allow_pointers.cpp): Demonstrates enforcing compile-time restrictions against pointer types using `static_assert(!std::is_pointer_v<T>)`.
- [`26_6_Partial_template_specialization_for_pointers/4_partial_template_specialization_for_pointers_allow_pointers.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/26_Templates_and_Classes/26_6_Partial_template_specialization_for_pointers/4_partial_template_specialization_for_pointers_allow_pointers.cpp): Demonstrates modern robust pointer handling using `std::unique_ptr<T>` for automatic heap allocation and exception-safe destruction.
