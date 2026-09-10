# Chapter 17: Fixed-size arrays: std::array and C-style arrays

Containers provide storage for a collection of unnamed objects (called **elements**). Arrays allocate their elements contiguously in memory and allow fast, direct access to any element via subscripting.

C++ features three primary array types:
1. `std::vector` (dynamic array)
2. `std::array` (fixed-size container introduced in C++11)
3. C-style arrays (built-in language fixed-size arrays)

---

## Table of Contents
1. [1 — Introduction to std::array](#1--introduction-to-stdarray)
2. [2 — std::array length and indexing](#2--stdarray-length-and-indexing)
3. [3 — Passing and returning std::array](#3--passing-and-returning-stdarray)
4. [4 — std::array of class types, and brace elision](#4--stdarray-of-class-types-and-brace-elision)
5. [5 — Arrays of references via std::reference_wrapper](#5--arrays-of-references-via-stdreference_wrapper)
6. [6 — std::array and enumerations](#6--stdarray-and-enumerations)
7. [7 — Introduction to C-style arrays](#7--introduction-to-c-style-arrays)
8. [8 — C-style array decay](#8--c-style-array-decay)
9. [9 — Pointer arithmetic and subscripting](#9--pointer-arithmetic-and-subscripting)
10. [10 — C-style strings](#10--c-style-strings)
11. [11 — C-style string symbolic constants](#11--c-style-string-symbolic-constants)
12. [12 — Multidimensional C-style Arrays](#12--multidimensional-c-style-arrays)
13. [13 — Multidimensional std::array](#13--multidimensional-stdarray)

---

## 1 — Introduction to std::array

- **Containers** provide storage for a collection of unnamed objects (called **elements**).
- Arrays allocate their elements contiguously in memory, and allow fast, direct access to any element via subscripting.
- C++ has three different array types that are commonly used: `std::vector`, `std::array` (introduced in C++11), and C-style arrays.

### Fixed-Size Arrays vs Dynamic Arrays
- In a **fixed-size array**, the length of the array must be known at the point of instantiation, and that length cannot be changed afterward.
- C-style arrays and `std::array` are both fixed-size arrays.
- **Dynamic arrays** can be resized at runtime. `std::vector` is a dynamic array.

#### So why not use dynamic arrays for everything?
- Dynamic arrays are convenient but involve tradeoffs.
- `std::vector` is slightly slower than fixed-size arrays and has very limited `constexpr` support (transient compile-time allocations only in C++20). In modern C++, this lack of `constexpr` is the main drawback, since `constexpr` arrays enable more robust and better-optimized code.

> [!TIP]
> **Best Practice**: Use `std::array` for `constexpr` arrays, and `std::vector` for non-`constexpr` arrays.

### Defining a `std::array`
To define a `std::array`, include `<array>`:

```cpp
#include <array>

std::array<int, 5> a{}; // A std::array of 5 ints
```

- A `std::array` declaration requires two template arguments:
  1. The first (`int`) is a **type template argument** defining the element type.
  2. The second (`5`) is an **integral non-type template argument** (NTTP) defining the array length.

### The Length of a `std::array` Must Be a Constant Expression
- Unlike `std::vector`, which can be sized and resized at runtime, `std::array` has a fixed size that **must be a constant expression**: an integer literal, a `constexpr` variable, or an enumerator.

```cpp
std::array<int, 7> a{};                   // Using a literal constant

constexpr int len{ 8 };
std::array<int, len> b{};                 // Using a constexpr variable

std::array<int, Colors::max_colors> c{};  // Using an enumerator
```

> [!NOTE]
> Non-const variables and runtime constants cannot be used as the array length.

- Surprisingly, a `std::array` can have length `0`, in which case it contains no data. Accessing elements (including via `operator[]`) is undefined behavior. Use `empty()` to check whether a `std::array` is zero-length.

### Aggregate Initialization of a `std::array`
- `std::array` is an **aggregate** (it has no user-declared constructors), so it uses **aggregate initialization** with brace-enclosed initializer lists.
- Elements are initialized in order, starting from index 0.
- If no initializer is provided, elements are default-initialized (often left uninitialized for fundamental types).
- Prefer **value initialization** with empty braces `{}` to ensure elements are zero-initialized.
- **Too many initializers** $\rightarrow$ compile-time error.
- **Too few initializers** $\rightarrow$ remaining elements are value-initialized.

```cpp
std::array<int, 5> a{ 1, 2, 3, 4, 5 }; // Fully initialized
std::array<int, 5> b{ 1, 2 };          // b[2], b[3], b[4] are value-initialized (0)
std::array<int, 5> c{};                // All elements value-initialized (0)
std::array<int, 5> d;                  // Default-initialized -> elements are uninitialized (avoid!)
```

> [!TIP]
> **Best Practice**: Use brace initialization and prefer `{}` when no explicit initial values are given.

### Const and Constexpr `std::array`
- A `std::array` can be declared `const`; all of its elements are then treated as `const`.
- `std::array` fully supports `constexpr`, allowing compile-time initialization, constant evaluation, and superior compiler optimizations.
- `constexpr` support is a key advantage of `std::array` over `std::vector`.

```cpp
const std::array<int, 5> a{ 2, 3, 5, 7, 11 };      // Read-only array
constexpr std::array<int, 5> b{ 2, 3, 5, 7, 11 };  // Compile-time constant array
```

### Class Template Argument Deduction (CTAD) for `std::array`
- Since **C++17**, CTAD lets the compiler deduce both the element type and array size of a `std::array` directly from its initializers.
- This syntax is preferred when available; pre-C++17 code required explicit template arguments.
- **CTAD cannot partially deduce template arguments** (you cannot omit just the type or just the size).

```cpp
constexpr std::array a1{ 9, 7, 5, 3, 1 }; // Deduced as std::array<int, 5>
constexpr std::array a2{ 9.7, 7.31 };     // Deduced as std::array<double, 2>

// Not allowed (partial deduction fails):
// std::array<int> a3{ 1, 2, 3 }; // Compile error
// std::array<5>   a4{ 1, 2, 3 }; // Compile error
```

### Omitting Just the Array Length Using `std::to_array`
- Function Template Argument Deduction (TAD) supports partial template argument omission, unlike CTAD.
- Since **C++20**, `std::to_array` (in `<array>`) allows omitting the array length while explicitly specifying the element type, or deducing both:

```cpp
constexpr auto a1 = std::to_array<int, 5>({ 9, 7, 5 }); // Type and size specified
constexpr auto a2 = std::to_array<int>({ 9, 7, 5 });    // Type specified, size deduced (size 3)
constexpr auto a3 = std::to_array({ 9, 7, 5 });         // Type and size deduced
constexpr auto s  = std::to_array<short>({ 9, 7, 5 });  // Deduces size 3 of type short
```

- `std::to_array` creates a temporary array and copies/moves into it, so it is slightly less direct than direct `std::array` construction.

> [!TIP]
> **Guideline**: Prefer direct `std::array` (with CTAD) when possible; use `std::to_array` when the element type cannot be deduced directly (e.g. `short`).

### Accessing Array Elements Using `operator[]`
- The most common way to access `std::array` elements is with the subscript operator (`operator[]`).
- Indexing is zero-based.
- `operator[]` performs **no bounds checking**; invalid indices trigger **undefined behavior**.

### 📁 Code Examples for Section 1
- [`17_6_Introduction_to_std_array_and_indexing/1_std_array_definition_and_ctad.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_6_Introduction_to_std_array_and_indexing/1_std_array_definition_and_ctad.cpp): Demonstrates `std::array` definition, aggregate value initialization, C++17 CTAD, and C++20 `std::to_array`.

---

## 2 — std::array length and indexing

### The Length of a `std::array` Has Type `std::size_t`
- `std::array` is a class template declared as:
  ```cpp
  template<typename T, std::size_t N>
  struct array;
  ```
- The array length `N` has type `std::size_t` (an unsigned integral type).
- When defining a `std::array`, `N` must be a `constexpr` value of type `std::size_t` or convertible to it.
- Signed integral constants are allowed because they are converted to `std::size_t` at compile time without narrowing issues.

> [!NOTE]
> **C++23 Literal Suffix**: In C++23, the `uz` / `UZ` literal suffix was introduced for `std::size_t` (e.g., `0uz` deduces as `std::size_t`, whereas `0` deduces as `int`).

### `size_type` Is Always `std::size_t`
- `std::array` defines a nested type alias `size_type`, which represents container sizes and indices.
- For `std::array`, `size_type` is always an alias of `std::size_t`.
- The template parameter uses `std::size_t` directly because `size_type` is not yet in scope during template declaration. Everywhere else in `std::array`, `size_type` is used.

### Getting the Length of a `std::array`
Three standard mechanisms retrieve the length:

| Method | Type | Standard | Return Type | Description |
|---|---|---|---|---|
| `arr.size()` | Member function | C++11 | `size_type` (`std::size_t`) | Unsigned element count |
| `std::size(arr)` | Non-member function | C++17 | `std::size_t` | Equivalent to `arr.size()` |
| `std::ssize(arr)` | Non-member function | C++20 | Signed (`std::ptrdiff_t`) | Signed integral element count |

```cpp
constexpr std::array arr{ 9, 7, 5, 3, 1 };
arr.size();        // unsigned size_type (5)
std::size(arr);    // unsigned size_type (5)
std::ssize(arr);   // signed integral type (5)
```

> [!NOTE]
> `std::ssize()` is the only standard option that returns a **signed** length, preventing accidental unsigned underflow in countdown loops.

### Getting the Length as a Constexpr Value
- Because a `std::array`'s length is part of its type, `size()`, `std::size()`, and `std::ssize()` all return `constexpr` values, even when called on a **non-constexpr** object:
  ```cpp
  std::array arr{ 9, 7, 5, 3, 1 };
  constexpr int length{ std::size(arr) }; // OK!
  ```

#### Important Caveat (Pre-C++23)
- When a `std::array` is passed to a function by (const) reference, calling `std::size(arr)` in a constant expression produced a compile error pre-C++23 due to a core language defect:
  ```cpp
  void f(const std::array<int, 5>& arr) {
      // constexpr int n{ std::size(arr) }; // Error pre-C++23!
  }
  ```
- This defect is addressed in C++23 (P2280).
- **Workaround**: Use a function template with the array length as an NTTP parameter:
  ```cpp
  template <auto N>
  void printLength(const std::array<int, N>&) {
      constexpr auto len{ N }; // Always a constant expression
      std::cout << len << '\n';
  }
  ```

### Subscripting: `operator[]` vs `at()`
- `operator[]` is the primary indexing operator: zero overhead, no bounds checking. Out-of-bounds access is **undefined behavior**.
- `at()` provides **runtime bounds checking** by throwing `std::out_of_range` if the index is invalid.
- Both expect indices of type `size_type` (`std::size_t`).
- Using a non-constexpr signed index can produce signed/unsigned narrowing warnings.

### `std::get()` Does Compile-Time Bounds Checking
- `operator[]` has no bounds checking; `at()` performs runtime checking.
- For `constexpr` indices, use `std::get<Index>(arr)`:
  ```cpp
  constexpr std::array prime{ 2, 3, 5, 7, 11 };
  std::cout << std::get<3>(prime); // OK -> 7
  // std::cout << std::get<9>(prime); // COMPILE ERROR: index out of bounds!
  ```
- `std::get` takes the index as a non-type template argument and enforces bounds at compile time via `static_assert`.

### 📁 Code Examples for Section 2
- [`17_6_Introduction_to_std_array_and_indexing/2_std_array_length_and_indexing.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_6_Introduction_to_std_array_and_indexing/2_std_array_length_and_indexing.cpp): Demonstrates `size()`, `std::size()`, signed `std::ssize()`, runtime `at()` bounds checking, and compile-time `std::get<Index>()` bounds validation.

---

## 3 — Passing and returning std::array

### Passing `std::array`
- Passing a `std::array` by value makes an **expensive copy of all elements**. Prefer passing by **const reference**.
- Element type and length are part of the type signature:
  ```cpp
  void passByRef(const std::array<int, 5>& arr) {
      std::cout << arr[0] << '\n';
  }
  ```
- CTAD does not work on function parameters; the compiler cannot deduce template arguments there without a template function.

### Function Templates for `std::array`
To accept any `std::array` element type or length, write a function template parameterizing both `T` and `N`:

```cpp
template <typename T, std::size_t N>
void passByRef(const std::array<T, N>& arr) {
    static_assert(N != 0, "Array cannot be empty");
    std::cout << arr[0] << '\n';
}
```

- In **C++20**, non-type template parameters can use `auto` to deduce the type of `N`:
  ```cpp
  template <typename T, auto N>
  void printLength(const std::array<T, N>& arr) {
      std::cout << N << '\n';
  }
  ```

### Static Asserting on Array Length
- To avoid runtime undefined behavior with fixed indices (e.g. accessing index 3), validate length at compile time:
  ```cpp
  template <typename T, std::size_t N>
  void printElement3(const std::array<T, N>& arr) {
      static_assert(N > 3, "Array must have at least 4 elements");
      std::cout << arr[3] << '\n';
  }
  ```

### Returning `std::array` from Functions
Returning a `std::array` by value copies (or moves) all elements. Unlike `std::vector` (which just swaps an internal heap pointer), `std::array` stores elements inline, so moving a `std::array` moves every single element individually.

Two conventional return designs:

```
┌─────────────────────────────────────────────────────────────┐
│                    Returning std::array                     │
├──────────────────────────────┬──────────────────────────────┤
│       Return by Value        │    Return via Out-Parameter  │
├──────────────────────────────┼──────────────────────────────┤
│ ✔ Clean, intuitive syntax    │ ✔ No element copying         │
│ ✔ Enables single-line init   │ ✔ Efficient for large arrays │
│ ✘ Copies elements            │ ✘ Unconventional in modern   │
│ ✘ Explicit template args     │ ✘ Cannot produce temporary   │
└──────────────────────────────┴──────────────────────────────┘
```

```cpp
// Return by value (ideal for small arrays or cheap-to-copy types):
template <typename T, std::size_t N>
std::array<T, N> inputArray() {
    std::array<T, N> arr{};
    for (std::size_t i{ 0 }; i < N; ++i)
        std::cin >> arr[i];
    return arr; // Compiler applies copy elision (RVO)
}

// Return via out-parameter (for very large fixed arrays):
template <typename T, std::size_t N>
void inputArray(std::array<T, N>& arr) {
    for (std::size_t i{ 0 }; i < N; ++i)
        std::cin >> arr[i];
}
```

> [!TIP]
> **Key Insight**: Use return-by-value for small, cheap-to-copy arrays; consider `std::vector` if large dynamic arrays or true $O(1)$ move semantics are needed.

### 📁 Code Examples for Section 3
- [`17_6_Introduction_to_std_array_and_indexing/3_passing_and_returning_std_array.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_6_Introduction_to_std_array_and_indexing/3_passing_and_returning_std_array.cpp): Demonstrates passing `std::array` by const reference via function templates (`std::size_t N` and C++20 `auto N`), `static_assert` compile-time length checks, and returning by value with RVO.

---

## 4 — std::array of class types, and brace elision

### Defining and Assigning to a `std::array` of Structs
Normal assignment works with single braces because the compiler already knows each element is a `House`:

```cpp
struct House { int number{}; int stories{}; int rooms{}; };

std::array<House, 3> houses{};
houses[0] = { 13, 1, 7 }; // Single braces OK for assignment
```

### Initializing with Explicit Element Type (CTAD)
When explicitly constructing each element, single braces work fine:

```cpp
constexpr std::array houses{
    House{ 13, 1, 7 },
    House{ 14, 2, 5 },
    House{ 15, 2, 4 }
}; // CTAD deduces std::array<House, 3>
```

### Why Explicit Type Without Constructor Calls Fails
The following does **not** compile:

```cpp
// ❌ Does NOT compile
constexpr std::array<House, 3> houses{
    { 13, 1, 7 },
    { 14, 2, 5 },
    { 15, 2, 4 }
};
```

**The Reason**: `std::array` is an aggregate containing a single internal C-style array data member (e.g. `T _M_elems[N]`). The compiler interprets `{ 13, 1, 7 }` as an attempt to initialize that internal C-style array as a whole. The remaining initializers (`{ 14, 2, 5 }`...) are then rejected as superfluous initializers.

### The Correct Way: Double Braces
Wrap the elements in an extra pair of braces:

```cpp
constexpr std::array<House, 3> houses{{
    { 13, 1, 7 },
    { 14, 2, 5 },
    { 15, 2, 4 }
}}; // Compiles cleanly!
```
- **Outer `{}`**: Initializes the `std::array` aggregate object.
- **Inner `{}`**: Initializes the internal C-style array member.
- **Inner-inner `{}`**: Initializes each individual `House` struct.

> [!IMPORTANT]
> **One-Line Rule to Memorize**:
> - Element assignment: single braces ✅
> - Initialization with explicit type (`House{...}`): single braces ✅
> - Initialization without explicit type: **double braces required** ✅

### Brace Elision (Why Scalars Work with Single Braces)
```cpp
constexpr std::array<int, 5> a{ 1, 2, 3, 4, 5 };   // OK!
constexpr std::array<int, 5> b{{ 1, 2, 3, 4, 5 }}; // Also OK!
```
- **Brace elision** is a C++ language rule permitting the omission of inner braces when initializing sub-aggregates with scalar types (like `int` or `double`).
- Structs and arrays do not permit brace elision unless the element type is explicitly instantiated.

### 📁 Code Examples for Section 4
- [`17_1_std_array_of_class_types_n_brace_elision/1_brace_elision.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_1_std_array_of_class_types_n_brace_elision/1_brace_elision.cpp): Demonstrates aggregate initialization mechanics, failure mode of single braces without CTAD, and double brace resolution.

---

## 5 — Arrays of references via std::reference_wrapper

### Arrays Hold Objects, Not References
- Array elements must be assignable objects with real storage.
- References are not objects (they have no storage address of their own and cannot be reseated), so raw reference arrays are illegal:
  ```cpp
  std::array<int&, 2> arr; // ❌ COMPILE ERROR: array of references is illegal
  ```

#### What Actually Happens with Reference Decay:
```cpp
int x{ 1 }, y{ 2 };
int& r1 = x;
int& r2 = y;
std::array arr{ r1, r2 }; // ✅ Deduces as std::array<int, 2>, NOT references!
```
The references decay to the underlying values, copying them into the array.

### Workaround: `std::reference_wrapper`
To store reference-like semantics in an array, use `std::reference_wrapper<T>` from `<functional>`:

```cpp
#include <array>
#include <functional>

int x{ 1 }, y{ 2 }, z{ 3 };
std::array<std::reference_wrapper<int>, 3> arr{ x, y, z };

arr[1].get() = 5;    // Modifies y
std::cout << arr[1]; // Prints 5 (implicitly converts to int&)
```

#### Key Properties of `std::reference_wrapper`:
- Behaves like a modifiable, reseatable reference.
- Implicitly converts to `T&`.
- Call `.get()` to access and mutate the referenced object through assignment.

### Recommended Helpers: `std::ref` and `std::cref`
Use standard helper functions for cleaner deduction:

```cpp
int x{ 5 };
auto r  = std::ref(x);  // std::reference_wrapper<int>
auto cr = std::cref(x); // std::reference_wrapper<const int>
```

> [!TIP]
> Prefer `std::ref` and `std::cref` when initializing containers of reference wrappers. This applies to all standard containers (`std::vector`, `std::array`, etc.).

### 📁 Code Examples for Section 5
- [`17_2_Arrays_of_references_via_std_reference_wrapper/1_std_reference_wrapper_for_std_array.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_2_Arrays_of_references_via_std_reference_wrapper/1_std_reference_wrapper_for_std_array.cpp): Demonstrates creating arrays of references using `std::reference_wrapper`, mutating variables through array elements via `.get()`, and const correctness with `std::cref`.

---

## 6 — std::array and enumerations

### Using Enumerators to Index a `std::array`
- Both unscoped and scoped enumerations can be used to index `std::array`.
- Unscoped enumerators implicitly convert to `std::size_t`, giving semantic meaning to array indices rather than using opaque numeric constants:
  ```cpp
  enum Student { rama, laxmana, hanuma, max_students };
  std::array<int, max_students> testScores{};
  testScores[Student::hanuma] = 98; // Clear, self-documenting intent
  ```
- Scoped enumerators (`enum class`) require `static_cast<std::size_t>(e)` or `std::to_underlying(e)` (C++23) to convert to an index.

### Catching Missing Initializers with `static_assert`
- When using CTAD, the compiler deduces the array size from the count of provided initializers.
- If a new enumerator is added to the enumeration but forgotten in the array initializer list, the array is silently shorter than intended.
- Accessing that element later causes an out-of-bounds read (**undefined behavior**).
- **Solution**: Define a sentinel enumerator (e.g. `max_students`, `max_colors`) at the end of the enum, and enforce equality at compile time:

```cpp
static_assert(std::size(testScores) == Student::max_students, 
              "Array size does not match enum count!");
```

### Better Enum I/O Using `constexpr std::array`
Standard C++ streams read and print enums as numbers. A `constexpr std::array` mapping enum values to string names provides clean bidirectional I/O:

```cpp
namespace Color {
    enum Type { red, green, blue, max_colors };
    constexpr std::array colorNames{ "red", "green", "blue" };

    // 1. Enum to String
    constexpr std::string_view getColorName(Type c) {
        return colorNames[static_cast<std::size_t>(c)];
    }

    // 2. String to Enum
    constexpr std::optional<Type> getColorFromName(std::string_view name) {
        for (std::size_t i{ 0 }; i < std::size(colorNames); ++i) {
            if (colorNames[i] == name)
                return static_cast<Type>(i);
        }
        return std::nullopt;
    }
}
```

### Why Range-Based For Loops Don't Work Directly with Enumerations
- Enumerations are fundamental types, not containers; they lack `begin()` and `end()` functions. Writing `for (auto c : Color)` triggers a compile-time error.
- **Two Idiomatic Workarounds**:
  1. Traditional loop with integer casting:
     ```cpp
     for (int i{ 0 }; i < Color::max_colors; ++i)
         std::cout << static_cast<Color::Type>(i) << '\n';
     ```
  2. Helper `constexpr std::array` containing all enum values:
     ```cpp
     constexpr std::array allColors{ Color::red, Color::green, Color::blue };
     for (auto c : allColors) // Clean range-based for loop!
         std::cout << c << '\n';
     ```

### 📁 Code Examples for Section 6
- [`17_3_std_array_n_enumerations/1_static assert_using_enum.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_3_std_array_n_enumerations/1_static%20assert_using_enum.cpp): Demonstrates using `static_assert` to detect missing array initializers against enum counts at compile time.
- [`17_3_std_array_n_enumerations/2_constexpr_arrays_better_enum_input_n_output.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_3_std_array_n_enumerations/2_constexpr_arrays_better_enum_input_n_output.cpp): Demonstrates bidirectional enum $\leftrightarrow$ string mapping using `constexpr std::array` and `std::optional`.
- [`17_3_std_array_n_enumerations/3_range-based_for-loops_NOT_ALLOWED_n_enumerations.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_3_std_array_n_enumerations/3_range-based_for-loops_NOT_ALLOWED_n_enumerations.cpp): Explains why enums cannot be used directly in range-for loops and implements helper array iteration.

---

## 7 — Introduction to C-style arrays

- Because C-style arrays are the only array type built directly into the core language, standard library containers (`std::array`, `std::vector`) are typically implemented internally using C-style arrays.

### Declaring a C-style Array
```cpp
int arr[30]{}; // 30 ints, value-initialized to 0
```
- Uses `[]` syntax.
- Length must be $\ge 1$ and a constant expression. Zero-length fixed C-style arrays are illegal in C++.

### Indexing
- Uses `operator[]`.
- Performs no bounds checking $\rightarrow$ undefined behavior on out-of-bounds access.
- Index type can be any integral type (signed or unsigned), avoiding signed/unsigned conversion warnings common with `std::size_t`.

### Aggregate Initialization and Omitting Length
```cpp
int a[5]{ 1, 2, 3, 4, 5 };       // Fully initialized
int b[5]{};                      // All 0
int primes[]{ 2, 3, 5, 7, 11 };  // Length deduced as 5
// int bad[]{};                  // COMPILE ERROR: cannot deduce size 0
```

> [!TIP]
> **Best Practice**: Omit the array length when explicitly initializing all elements to avoid size mismatch bugs.

### Limitations of C-style Arrays
- No CTAD (not a template).
- Element type cannot use `auto`.
- **Does not support assignment**:
  ```cpp
  int arr[]{ 1, 2, 3 };
  // arr = { 4, 5, 6 }; // COMPILE ERROR!
  ```
  Must assign element-by-element or use `std::copy(std::begin(src), std::end(src), std::begin(arr))`.

### `sizeof` vs `std::size()` with C-style Arrays
- `sizeof(arr)` returns the **total size in bytes** ($\text{elements} \times \text{sizeof(element)}$).
- `std::size(arr)` (C++17) returns the **element count** (requires non-decayed array).
- Pre-C++17 macro `sizeof(arr)/sizeof(arr[0])` is dangerous because it silently yields wrong answers when arrays decay to pointers.

### 📁 Code Examples for Section 7
- [`17_7_C_style_arrays_and_decay/1_c_style_array_basics.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_7_C_style_arrays_and_decay/1_c_style_array_basics.cpp): Demonstrates C-style array declaration, length deduction, `sizeof` byte computation vs `std::size()`, and `std::copy` assignment workaround.

---

## 8 — C-style array decay

### The C-style Array Passing Challenge
- In C, copying an entire array into a function parameter would be slow and memory-intensive.
- Furthermore, functions must accept arrays of any length without requiring unique overloads for `int[5]`, `int[100]`, etc.
- **The C Solution**: **Array-to-pointer decay**. When an array is passed to a function, it automatically converts into a pointer to its first element (`int*` pointing to `&arr[0]`).

```cpp
void printElementZero(int arr[1000]) { // arr is NOT an array! It is an int*
    std::cout << arr[0];
}
```

### The Hidden Danger of Array Decay
- The compiler completely **ignores the size** inside parameter brackets (`int arr[1000]` is rewritten as `int* arr`).
- The function receives a bare pointer and has **no size information**:
  ```cpp
  int small[2]{ 1, 2 };
  printElementZero(small); // Compiles without error!
  ```

### Exceptions to Array Decay
An array does **NOT** decay when:
1. Used with `sizeof` or `typeid`.
2. Taking the address of the array (`&arr` yields `int(*)[N]`, pointer to array).
3. Passed as a class/struct member.
4. Passed **by reference** (`const int (&arr)[N]`).

### Subscripting Actually Operates on the Decayed Pointer
- In C++, subscripting an array `arr[i]` is identical to pointer indexing:
  $$\text{arr}[i] \equiv *(\text{arr} + i)$$
- Because `arr` decays to `int*`, `operator[]` works directly on pointers.

### Parameter Syntax Best Practice
- Declaring parameters as `int* arr` makes it ambiguous whether it points to a single item or an array.
- Declaring parameters as `int arr[]` signals that a decayed array is expected:
  ```cpp
  void printFirst(const int arr[]); // Clear intent, still decayed pointer
  ```

### Modern Alternatives to Raw Array Decay
```
┌─────────────────────────────────────────────────────────────┐
│                 Handling Sequences in C++                   │
├───────────────────┬─────────────────────────────────────────┤
│ std::array<T, N>& │ Fixed length, compile-time safe         │
│ std::span<T>      │ C++20 non-owning, bounds-aware view     │
│ std::vector<T>&   │ Dynamic resizable container             │
└───────────────────┴─────────────────────────────────────────┘
```

### 📁 Code Examples for Section 8
- [`17_7_C_style_arrays_and_decay/2_c_style_array_decay.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_7_C_style_arrays_and_decay/2_c_style_array_decay.cpp): Demonstrates array decay to pointers in function parameters, `sizeof` pointer trap, and modern safe traversal using C++20 `std::span`.

---

## 9 — Pointer arithmetic and subscripting

### What Is Pointer Arithmetic?
- Pointer arithmetic performs arithmetic on a pointer to make it point to a different element of the **same array**, not an arbitrary byte address.

| Expression | Meaning |
|---|---|
| `p + 1` | Address of next element (`arr[1]`) |
| `p + 2` | Address of element two steps ahead (`arr[2]`) |
| `p - 1` | Address of previous element |
| `++p` | Advance pointer to next element |
| `--p` | Retreat pointer to previous element |

> [!IMPORTANT]
> **Key Rule**: Pointer arithmetic moves in units of the **pointed-to type** (`sizeof(T)` bytes), never raw bytes.

### Why Subscripting Works
$$\text{arr}[i] \equiv *(\text{arr} + i) \equiv *(i + \text{arr}) \equiv i[\text{arr}]$$

### Indices Are Relative, Not Absolute
Indices are offsets relative to the pointer:
```cpp
const int arr[]{ 9, 8, 7, 6, 5 };
const int* p = &arr[3]; // Points to 6

p[0];  // 6 (current)
p[1];  // 5 (next)
p[-1]; // 7 (previous)
```

### Array Traversal via Pointer Arithmetic
Under the hood, range-based for loops over C-style arrays compile directly to pointer arithmetic:

```cpp
const int* begin = arr;
const int* end   = arr + std::size(arr); // One-past-the-end

for (; begin != end; ++begin) {
    std::cout << *begin << ' ';
}
```

> [!WARNING]
> Pointer arithmetic is **only defined** when staying within the same array or pointing to the one-past-the-end element. Computing addresses beyond that is **undefined behavior**.

### 📁 Code Examples for Section 9
- [`17_8_Pointer_arithmetic_and_traversal/1_pointer_arithmetic_and_traversal.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_8_Pointer_arithmetic_and_traversal/1_pointer_arithmetic_and_traversal.cpp): Demonstrates pointer offsets, subscript identity, negative relative indexing (`p[-1]`), and canonical `begin`/`end` pointer loops.

---

## 10 — C-style strings

- C-style strings are null-terminated `char` arrays ending with `'\0'`:
  ```cpp
  char str1[] = "hello"; // 6 chars: 'h','e','l','l','o','\0'
  ```
- Decay to `char*` or `const char*`.
- `std::strlen(str)` (from `<cstring>`) returns length **excluding** the null terminator.
- Input safely using `std::cin.getline(buf, std::size(buf))`.

### Legacy C String Functions vs Modern Recommendations

| Function | Purpose | Risk / Modern Recommendation |
|---|---|---|
| `strlen(s)` | String length | Safe on valid null-terminated strings |
| `strcpy(dest, src)` | Copy string | Buffer overflow hazard; avoid |
| `strncpy(dest, src, n)` | Bound copy | Still error-prone (no guaranteed `\0`); avoid |
| `strcat(dest, src)` | Concatenate string | Overflow hazard; avoid |
| `strcmp(s1, s2)` | Compare strings | Use `==` on `std::string` or `std::string_view` |

> [!TIP]
> Prefer `std::string_view` for read-only strings and `std::string` for modifiable strings.

### 📁 Code Examples for Section 10
- [`17_9_C_style_strings/1_c_style_strings_and_symbolic_constants.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_9_C_style_strings/1_c_style_strings_and_symbolic_constants.cpp): Demonstrates null-termination mechanics, `std::strlen` vs `sizeof`, array copies vs literal pointers, and `std::string_view`.

---

## 11 — C-style string symbolic constants

Two ways to define symbolic string constants:
```cpp
const char name[]{ "Alex" };         // Independent 5-char array copy
const char* const color{ "Orange" }; // Pointer to string literal in read-only memory
```

### Memory Differences
- `const char[]` creates a new array copy each time.
- `const char*` points directly to the string literal in the program's static memory segment; compilers consolidate duplicate string literals to save memory.

### `std::cout` Output Pitfall with `char*`
- Non-char pointers print the memory address (`0x...`).
- `char*` and `const char*` are treated by `std::cout` as null-terminated C-strings.

> [!WARNING]
> **The `&char` Undefined Behavior Trap**:
> ```cpp
> char c{ 'Q' };
> std::cout << &c; // Undefined Behavior!
> ```
> `std::cout` assumes `&c` points to the start of a C-style string and continues printing adjacent bytes until it encounters an accidental `\0` in memory, outputting garbage or causing memory faults.  
> **Fix**: Cast to `const void*`:
> ```cpp
> std::cout << static_cast<const void*>(&c); // Prints actual address safely
> ```

> [!TIP]
> Avoid C-style string symbolic constants. Use `constexpr std::string_view` in modern C++.

### 📁 Code Examples for Section 11
- Covered in [`17_9_C_style_strings/1_c_style_strings_and_symbolic_constants.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_9_C_style_strings/1_c_style_strings_and_symbolic_constants.cpp).

---

## 12 — Multidimensional C-style Arrays

### Motivation
A 3×3 Tic-Tac-Toe grid cannot be naturally represented by a 1D array. A **two-dimensional (2D) array** is an array of arrays:
```cpp
int a[3][5]; // 3 rows, 5 columns
```

### Row-Major Memory Layout
C++ stores multidimensional arrays in **row-major order**: row by row in contiguous linear memory:
```
[0][0] [0][1] [0][2] [0][3] [0][4]  (Row 0)
[1][0] [1][1] [1][2] [1][3] [1][4]  (Row 1)
[2][0] [2][1] [2][2] [2][3] [2][4]  (Row 2)
```

### Initializing 2D Arrays
```cpp
// Nested braces (recommended)
int array[3][5] {
    { 1, 2, 3, 4, 5 },
    { 6, 7, 8, 9, 10 },
    { 11, 12, 13, 14, 15 }
};

// Leftmost dimension can be omitted:
int array[][5] {
    { 1, 2, 3, 4, 5 },
    { 6, 7, 8, 9, 10 }
};
```

### Iterating Over 2D Arrays
Always iterate **rows outer, columns inner** to access memory contiguously and maximize CPU cache efficiency:

```cpp
for (std::size_t row{ 0 }; row < std::size(arr); ++row) {
    for (std::size_t col{ 0 }; col < std::size(arr[0]); ++col) {
        std::cout << arr[row][col] << ' ';
    }
}
```

### Cartesian Coordinates vs Array Indices
- Cartesian coordinate system: $\{x, y\}$ where $x$ is horizontal (column) and $y$ is vertical (row).
- Array subscripting: `[row][col]`.
- **Mapping**: Cartesian $\{x, y\} \longrightarrow \text{Array } [y][x]$.

### 📁 Code Examples for Section 12
- [`17_4_Multidimensional_C-style_Arrays/1_multi_dimensional_C_style_array.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_4_Multidimensional_C-style_Arrays/1_multi_dimensional_C_style_array.cpp): Demonstrates declaring 2D C-style arrays, dimension preservation in function templates via reference parameters (`T (&ref)[R][C]`), and row-major nested range-for traversal.

---

## 13 — Multidimensional std::array

### Nesting `std::array`
`std::array` is inherently one-dimensional. Multidimensional arrays must be formed by nesting:

```cpp
std::array<std::array<int, 4>, 3> arr {{
    { 1, 2, 3, 4 },
    { 5, 6, 7, 8 },
    { 9, 10, 11, 12 }
}};
```
- Requires **double braces** due to aggregate initialization rules.
- Type syntax reverses dimensions: `std::array<std::array<int, 4>, 3>` represents 3 rows and 4 columns (`[3][4]`).

### Simplifying with Alias Templates
To restore clean row-first ordering, use an alias template:

```cpp
template <typename T, std::size_t Row, std::size_t Col>
using Array2d = std::array<std::array<T, Col>, Row>;

Array2d<int, 3, 4> arr {{
    { 1, 2, 3, 4 },
    { 5, 6, 7, 8 },
    { 9, 10, 11, 12 }
}};
```

### Safe Dimension Queries
Query dimensions from type metadata rather than indexing runtime objects:

```cpp
template <typename T, std::size_t Row, std::size_t Col>
constexpr std::size_t rowLength(const Array2d<T, Row, Col>&) { return Row; }

template <typename T, std::size_t Row, std::size_t Col>
constexpr std::size_t colLength(const Array2d<T, Row, Col>&) { return Col; }
```

### Flattening Multidimensional Arrays
Storing nested `std::array` incurs syntax and nesting overhead. A flattened 1D array with a 2D view wrapper preserves contiguous storage while offering 2D subscripting:

$$\text{index} = \text{row} \times \text{cols} + \text{col}$$

```cpp
template <typename T, std::size_t Row, std::size_t Col>
class ArrayFlat2DView {
    std::reference_wrapper<std::array<T, Row * Col>> m_arr;
public:
    T& operator()(int r, int c) {
        return m_arr.get()[r * Col + c];
    }
};
```

### Future Modern C++ Multidimensional Features
- **`std::mdspan` (C++23)**: Standard non-owning multidimensional span view over contiguous data (`std::mdspan view{ arr.data(), 3, 4 }`). Uses multidimensional `operator[](r, c)`.
- **`std::mdarray` (C++26)**: Proposed owning container combining `std::array` storage with `std::mdspan` ergonomics.

### 📁 Code Examples for Section 13
- [`17_5_Multidimensional_std_array/1_multi_dimensional_std_array.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_5_Multidimensional_std_array/1_multi_dimensional_std_array.cpp): Demonstrates nested `std::array`, type alias templates, and compile-time dimension query functions.
- [`17_5_Multidimensional_std_array/2_multi_dimensional_flat_single_std_array.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/17_Fixed-size_arrays_std_array_n_C-style_arrays/17_5_Multidimensional_std_array/2_multi_dimensional_flat_single_std_array.cpp): Demonstrates 1D flattened storage with a 2D view wrapper, 2D index mapping (`row * cols + col`), and proxy row subscripting.
