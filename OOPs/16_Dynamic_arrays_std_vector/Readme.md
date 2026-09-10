# Chapter 16: Dynamic Arrays — `std::vector`

---

## Table of Contents
1. [1 — Introduction to Containers and Arrays](#1--introduction-to-containers-and-arrays)
2. [2 — Introduction to std::vector and List Constructors](#2--introduction-to-stdvector-and-list-constructors)
3. [3 — std::vector and the Unsigned Length and Subscript Problem](#3--stdvector-and-the-unsigned-length-and-subscript-problem)
4. [4 — Passing std::vector](#4--passing-stdvector)
5. [5 — Returning std::vector, and an Introduction to Move Semantics](#5--returning-stdvector-and-an-introduction-to-move-semantics)
6. [6 — Arrays and Loops](#6--arrays-and-loops)
7. [7 — Arrays, Loops, and Sign Challenge Solutions](#7--arrays-loops-and-sign-challenge-solutions)
8. [8 — Range-Based For Loops (For-Each)](#8--range-based-for-loops-for-each)
9. [9 — Array Indexing and Length Using Enumerators](#9--array-indexing-and-length-using-enumerators)
10. [10 — std::vector Resizing and Capacity](#10--stdvector-resizing-and-capacity)
11. [16.11 — std::vector and Stack Behavior](#1611--stdvector-and-stack-behavior)
12. [16.12 — std::vector\<bool\>](#1612--stdvectorbool)

---

## 1 — Introduction to Containers and Arrays

### Containers
- In general programming, a **container** is a data type that provides storage for a **collection of unnamed objects** (called **elements**).
- We typically use containers when we need to work with a **set of related values**.
- The elements of a container do **not** have their own names, so that the container can have as many elements as we want without having to give each element a unique name.
- Each container provides some method to access these elements, but **how** depends on the specific type of container.

### The Length of a Container
- In programming, the number of elements in a container is often called its **length** (or sometimes **count**).
- In C++, the term **`size`** is also commonly used for the number of elements in a container.

### Container Operations
- Programming languages offer different container types with varying supported operations and performance.
- Some allow **fast random access** but slow insertion/removal; others allow **fast insertion/removal** but only sequential access.
- Each container has **strengths and limitations**.
- **Choosing the right one greatly impacts performance and code maintainability.**

### Element Types
- Most containers are **homogeneous** $\rightarrow$ **all elements must be the same type**.
- Some have preset types (e.g., `std::string` $\rightarrow$ `char`), but usually the type is **user-defined**.
- In C++, containers are **class templates**, so you specify the element type when instantiating.
- This makes containers flexible without needing new types for each element.
- **Heterogeneous containers** (mixed types) are common in scripting languages like Python.

### Containers in C++
- In C++, the term **container** refers specifically to the class types in the **Containers library**. These are the official **“container classes”** defined by the C++ standard.
- Some types behave like containers in general programming, but are **not considered containers in C++** because they do not meet all required container specifications (such as providing certain member functions or types). Examples include:
  - **C-style arrays**
  - **`std::string`**
  - **`std::vector<bool>`**
- Even though `std::string` and `std::vector<bool>` behave like containers, they are missing some required features, so they are often called **pseudo-containers**.

### Introduction to Arrays
- **Arrays** store a sequence of values in **contiguous memory**, allowing **fast, direct access to any element**.
- Because they are simple and efficient, arrays are often used when working with groups of related values.
- C++ provides three main array types:
  1. **C-style arrays**:
     - Inherited from C
     - Part of the core language
     - **Fast but unsafe** and behave inconsistently by modern standards
  2. **`std::vector` (C++98)**:
     - A **dynamic array** container class
     - **Safest and most flexible** of all array types
  3. **`std::array` (C++11)**:
     - A **safer, fixed-size replacement** for C-style arrays
     - Often **more efficient than `std::vector`** for small fixed-size arrays
- All array container classes follow similar interfaces, so learning one (like `std::vector`) makes it easier to use the others.

---

## 2 — Introduction to std::vector and List Constructors

### Introduction to `std::vector`
- **`std::vector`** is one of the container classes in the C++ standard containers library that implements an array.
- `std::vector` is defined in the **`<vector>`** header as a **class template**, with a template type parameter that defines the type of the elements.
- Thus, `std::vector<int>` declares a `std::vector` whose elements are of type `int`.

### Initializing a `std::vector` with a List of Values
```cpp
std::vector<int> primes{ 2, 3, 5, 7 };
std::vector vowels { 'a', 'e', 'i', 'o', 'u' }; // vector containing 5 char elements with values 'a', 'e', 'i', 'o', and 'u'.
                                                // Uses CTAD (C++17) to deduce element type char (preferred).
```

### List Constructors and Initializer Lists
- Containers have a special constructor called a **list constructor** (taking `std::initializer_list<T>`).
- When we provide a container with an initializer list of values, the list constructor is called, and the container is constructed using that list of values! The list constructor does three things:
  1. **Ensures the container has enough storage** to hold all the initialization values (if needed).
  2. **Sets the length of the container** to the number of elements in the initializer list (if needed).
  3. **Initializes the elements** to the values in the initializer list (in sequential order).

### Accessing Array Elements Using the Subscript Operator (`operator[]`)
- The subscript operator **returns a reference to the actual element**, not a copy. Once we’ve accessed an array element, we can use it just like a normal object (e.g. assign a value to it, output it, etc.).
- **Indexes are actually a distance (offset)** from the first element of the array.

#### Subscript Out of Bounds
> [!WARNING]
> - For an array of length $N$, valid indices are **$0$ to $N-1$**.
> - **`operator[]` does NOT check bounds $\rightarrow$ invalid indices cause undefined behavior (UB)!**
> - Easy to avoid negative indices, but remember:
>   - The last element is at **$N-1$**.
>   - Index **$N$** is one past the end $\rightarrow$ **invalid access!**

### Arrays are Contiguous in Memory
- The elements of an array are **always allocated contiguously in memory** (all adjacent in memory, with no gaps between them).
- Arrays don't have any per-element overhead. This allows the compiler to quickly calculate the address of any element:
  $$\text{Address of } arr[i] = \text{Base Address} + (i \times \text{sizeof}(T))$$
- Arrays are one of the few container types that support **efficient random access**, allowing direct access to any element (unlike sequential access, which requires traversing in order).
- Random access to array elements is typically efficient and makes arrays very easy to use. This is a primary reason why arrays are often preferred over other containers.

### Constructing a `std::vector` of a Specific Length
- `std::vector` has an **explicit constructor**: `explicit std::vector<T>(std::size_t count)`.
- `std::size_t` defines the length of the `std::vector` to construct:
  ```cpp
  std::vector<int> data( 10 ); // vector containing 10 int elements, value-initialized to 0
  ```
> [!IMPORTANT]
> One important thing about using this constructor: **it must be called using direct initialization `()`**.

### Non-Empty Initializer Lists Prefer List Constructors
To understand why the length constructor must be called using direct initialization, consider:
```cpp
std::vector<int> data{ 10 }; // Creates a vector of length 1 containing the single value 10!
```
When constructing a class type object using an initializer list:
- **If the initializer list is empty `{}`**, the **default constructor is preferred** over the list constructor.
- **If the initializer list is non-empty `{...}`**, a **matching list constructor is preferred** over other matching constructors.
- **Fallback Rule**: If the initializer list is non-empty, but the values inside **cannot be converted to the element type**, the compiler falls back to other matching constructors (e.g., `std::vector<Foo> v{ 12 }` where `Foo` cannot be constructed from `12` falls back to `explicit std::vector(size_type count)`, creating 12 default `Foo` objects).

```cpp
// Copy init
std::vector<int> v1 = 10;     // 10 not an initializer list, copy init won't match explicit constructor: compilation error

// Direct init
std::vector<int> v2(10);      // 10 not an initializer list, matches explicit single-argument constructor (10 zeroes)

// List init
std::vector<int> v3{ 10 };    // { 10 } interpreted as initializer list, matches list constructor (1 element: 10)

// Copy list init
std::vector<int> v4 = { 10 }; // { 10 } interpreted as initializer list, matches list constructor
std::vector<int> v5({ 10 });  // { 10 } interpreted as initializer list, matches list constructor

// Default init
std::vector<int> v6 {};       // {} is empty initializer list, matches default constructor (empty vector)
std::vector<int> v7 = {};     // {} is empty initializer list, matches default constructor (empty vector)
```

> [!NOTE]
> - If a class does not have a list constructor but then one is added later, this will change which constructor is called for all objects initialized using a non-empty initializer list.
> - `v6` and `v7` are both initialized using empty initializer lists $\rightarrow$ default constructor takes precedence.
> - **Rule of thumb**: When constructing a container (or any type that has a list constructor) with initializers that are **not element values** (such as length), **use direct initialization `()`**.

### When a `std::vector` is a Member of a Class Type
```cpp
#include <vector>
struct Foo
{
    std::vector<int> v1(8); // compile error: direct initialization not allowed for member default initializers
};
```
This doesn’t work because **direct (parenthesis) initialization is disallowed for non-static member default initializers**.

When providing a default initializer for a member of a class type:
- We must use either **copy initialization** or **list initialization** (direct or copy).
- **CTAD is not allowed** (so we must explicitly specify the element type).

**The Solution:**
```cpp
struct Foo
{
    std::vector<int> v{ std::vector<int>(8) }; // OK: uses list init with temporary vector of length 8
};
```
This creates a `std::vector` with a length/capacity of 8, and uses that as the initializer for `v`.

### Const and constexpr `std::vector`
Objects of type `std::vector` can be made `const`:
```cpp
const std::vector<int> prime { 2, 3, 5, 7, 11 }; // prime and its elements cannot be modified
```
A `const std::vector` must be initialized, and then cannot be modified. The elements of such a vector are treated as if they were `const`.

> [!IMPORTANT]
> - **Disallowed**: The element type of a `std::vector` must not be defined as `const` (e.g., `std::vector<const int>` is **ill-formed/disallowed** because elements must be assignable/erasable).
> - **`constexpr` limitations**: `std::vector` **cannot** be made `constexpr` in the ordinary (non-transient) sense — it cannot persist from compile-time into runtime. If you need a `constexpr` array for static lookup tables at runtime, use **`std::array`**.
> - **C++20 Update**: In C++20, `std::vector` gained limited `constexpr` support for **transient compile-time allocations** inside `constexpr`/`consteval` functions (where all allocations are freed before compile-time evaluation completes).

---

### 📁 Code Examples for Section 2
- [`16_1_Introduction_to_std_vector_n_list_constructors/1_vectors.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_1_Introduction_to_std_vector_n_list_constructors/1_vectors.cpp): Demonstrates list initialization `{}` vs direct initialization `()`, member vector default initialization workaround `{std::vector<int>(20)}`, constructor fallback behavior with user-defined types (`std::vector<Foo> v3{12}`), and `const std::vector`.

---

## 3 — std::vector and the Unsigned Length and Subscript Problem

### The Container Length Sign Problem
- Standard library container classes use **unsigned values** for length (and indices). This is problematic, as it makes it almost impossible to avoid unsigned values when using these types.
- Thus, we are stuck with this design choice and the unnecessary complexity it causes.

### Review: Sign Conversions are Narrowing Conversions, Except When `constexpr`
- **Sign conversions are narrowing conversions** because signed and unsigned types cannot represent each other’s full value range.
- When narrowing conversions occur:
  - Compilers **reject them** in contexts that disallow narrowing (such as brace list initialization `{}`).
  - Compilers may emit warnings elsewhere (e.g. copy initialization).
- **The `constexpr` exception**: If the signed value is **`constexpr`** and safely fits within the destination type’s range without loss of data, **the conversion is NOT considered narrowing**. The compiler verifies it at compile time!
```cpp
int s { 5 };
[[maybe_unused]] unsigned int u { s }; // compile error: list initialization disallows narrowing conversion (-Werror)
foo(s);                                // possible warning: copy initialization allows narrowing conversion

constexpr int s2 { 5 };                // now constexpr
[[maybe_unused]] unsigned int u2 { s2 }; // OK: s2 is constexpr and fits safely, NOT narrowing
foo(s2);                                 // OK: constexpr non-narrowing conversion
```

### The Length and Indices of `std::vector` Have Type `size_type`
- `std::size_t` is a typedef for some large unsigned integral type, usually `unsigned long` or `unsigned long long`.
- Each standard library container defines a nested typedef member named **`size_type`** (written as `T::size_type`), which is an alias for the type used for the length (and indices) of the container.
- For example, member function `std::vector::size()` returns a value of `size_type`.
- `size_type` is almost always an alias for `std::size_t`, but can be overridden (in rare cases) by custom allocators.
- All standard library containers except `std::array` use `std::allocator` to allocate memory:
  - `std::allocator<T>::size_type` is defined as `std::size_t`.
  - Therefore, `T::size_type` defaults to `std::size_t`.
- When accessing the `size_type` member, we must scope qualify it with the fully templated name:
  ```cpp
  std::vector<int>::size_type
  ```

### Getting the Length: `size()`, `std::size()`, and `std::ssize()`

#### 1. Member function `.size()`
```cpp
std::vector prime { 2, 3, 5, 7, 11 };
std::cout << "length: " << prime.size() << '\n'; // returns unsigned size_type (std::size_t)
```
- Unlike `std::string` and `std::string_view` (which have both `length()` and `size()`), `std::vector` **only has `size()`**.

#### 2. Non-member function `std::size()` (C++17)
```cpp
std::vector prime { 2, 3, 5, 7, 11 };
std::cout << "length: " << std::size(prime); // returns unsigned size_type (std::size_t)
```
- `std::size()` also works on non-decayed C-style arrays.
- To store the result in a signed variable, use `static_cast`:
  ```cpp
  int length { static_cast<int>(prime.size()) };
  ```

#### 3. Non-member function `std::ssize()` (C++20)
- C++20 introduced **`std::ssize()`**, which returns the length as a **large signed integral type** (usually **`std::ptrdiff_t`**, the signed counterpart to `std::size_t`):
```cpp
std::vector prime{ 2, 3, 5, 7, 11 };
std::cout << "length: " << std::ssize(prime); // C++20: returns signed std::ptrdiff_t
```
- **This is the only function of the three that returns the length as a signed type.**
- Assigning `std::ssize()`:
  - To `int`: use `static_cast<int>(std::ssize(prime))` because `int` may be narrower than `std::ptrdiff_t`.
  - Or use `auto`:
    ```cpp
    auto length { std::ssize(prime) }; // deduces std::ptrdiff_t
    ```

### Accessing Array Elements: `operator[]` vs `at()`
| Function | Bounds Checking | On Out-of-Bounds | Performance |
|---|---|---|---|
| **`operator[]`** | **No** bounds checking | **Undefined Behavior (UB)** | **Fastest** (direct pointer offset) |
| **`.at(index)`** | **Runtime bounds checking** | Throws **`std::out_of_range`** exception | Slightly slower (extra branch) |

```cpp
std::cout << prime.at(3); // Prints element at index 3
std::cout << prime.at(9); // Invalid index: throws std::out_of_range exception!
```

### Indexing `std::vector` with Signed Values

#### 1. With a `constexpr` signed int (Safe)
```cpp
std::cout << prime[3] << '\n';     // OK: literal 3 converted to std::size_t, not narrowing
constexpr int index { 3 };
std::cout << prime[index] << '\n'; // OK: constexpr index implicitly converts safely
```

#### 2. With a non-`constexpr` signed int (Problematic)
```cpp
int index { 3 };
std::cout << prime[index] << '\n'; // Possible warning/error: signed to unsigned narrowing conversion
```

#### 3. Recommended Alternative: Index the underlying C-style array via `.data()`
```cpp
int index { 3 };                          // non-constexpr signed value
std::cout << prime.data()[index] << '\n'; // OK: no sign conversion warnings!
```
- Under the hood, `std::vector` holds its elements in contiguous memory.
- The **`.data()`** member function returns a pointer to this underlying memory (`T*`).
- Since pointer indexing in C++ natively supports signed offsets, **no sign conversion occurs or warns**.

---

### 📁 Code Examples for Section 3
- [`16_2_std_vector_n__unsigned_length_n_subscript_problem/1_simple_narrow_conversion_example.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_2_std_vector_n__unsigned_length_n_subscript_problem/1_simple_narrow_conversion_example.cpp): Demonstrates compiler rejection of narrowing signed-to-unsigned conversion in brace init, vs compile-time safety when using `constexpr`.
- [`16_2_std_vector_n__unsigned_length_n_subscript_problem/2_containers_unsigned_length_subscript_issue.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_2_std_vector_n__unsigned_length_n_subscript_problem/2_containers_unsigned_length_subscript_issue.cpp): Demonstrates `size()`, `std::size()`, `std::ssize()`, type casting with `static_cast`, `auto`, and `operator[]` vs `at()` exception handling.

---

## 4 — Passing std::vector

### Pass by (Const) Reference
> [!IMPORTANT]
> **Passing a `std::vector` by value causes an expensive deep copy of all elements.**
> Therefore, **pass `std::vector` by `(const) reference`** to avoid unnecessary copies:
> ```cpp
> void printVector(const std::vector<int>& arr);
> ```

### Passing `std::vector` of Different Element Types
- We cannot pass a `std::vector<double>` to a parameter of type `const std::vector<int>&`.
- CTAD does **not** work for function parameters.
- **Solution: Function Template**:
  ```cpp
  template <typename T>
  void printVector(const std::vector<T>& arr);
  ```

### Passing Using a Generic Template or Abbreviated Function Template
- We can create a function template that accepts any container or object:
  ```cpp
  // Generic template
  template <typename T>
  void passByRef(const T& arr);

  // C++20 Abbreviated Function Template
  void passByRef(const auto& arr);
  ```
- **Pros**: Works with `std::vector`, `std::array`, and any other iterable or subscriptable type.
- **Cons**: May accept unexpected types that compile syntactically but do not make sense semantically.

### Asserting on Array Length
- The compiler will happily allow passing an array that has fewer elements than the function expects.
- Options:
  1. **Runtime assert**:
     ```cpp
     assert(arr.size() >= 4); // catches bug in Debug mode
     ```
     Because `std::vector::size()` is a runtime function, we can only use runtime `assert` (not `static_assert`).
  2. **Use `std::array`**: If the length is known at compile time, use `std::array` so you can enforce the size via `static_assert`.
  3. **Best practice**: Avoid writing functions that rely on arbitrary minimum container lengths; prefer generic traversals or range algorithms.

---

### 📁 Code Examples for Section 4
- [`16_3_Passing_std_vector/1_pass_by_reference_vector.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_3_Passing_std_vector/1_pass_by_reference_vector.cpp): Shows how to pass vectors by `const auto&` (C++20 abbreviated function template) and access elements safely using `.data()`.

---

## 5 — Returning std::vector, and an Introduction to Move Semantics

### Pass by Const Reference, Return by Value
- While passing `std::vector` by value is expensive, **returning a `std::vector` by value is completely fine and recommended**.

### Copy Semantics
- **Copy semantics** refers to the rules that determine how copies of objects are made.
- When we say a type **supports copy semantics**, we mean that objects of that type are **copyable**, because the rules for making such copies have been defined.
- Copy semantics are typically implemented via the **copy constructor** (and **copy assignment operator**).

### Move Semantics
- **Move semantics** refers to transferring ownership of data from one object to another. We say that the data has been **moved**. The cost of such a move is typically **trivial**.
- When the temporary source object is destroyed at the end of the expression, it **no longer has any data to destroy** — so we don't pay that cleanup cost either.
- **Rule**: In move semantics, any data member that **can** be moved is moved; any data member that **cannot** be moved is copied.

### How Move Semantics is Invoked
Move semantics is invoked when **all** of the following are true:
1. The type of the object **supports move semantics** (e.g., has a move constructor/move assignment operator).
2. The object is being initialized with (or assigned) an **rvalue (temporary) object** of the same type.
3. The copy/move is **not elided** by the compiler.
- Both `std::vector` and `std::string` fully support move semantics!

> [!TIP]
> **Summary**: Expensive-to-copy types shouldn’t be passed by value, but if they are **move-capable**, they can and should be **returned by value**.

### Deep Dive: The 4 Steps in Function Call Data Transfer
```cpp
#include <iostream>
#include <vector>

std::vector<int> doSomething(std::vector<int> v2)
{
    std::vector v3 { v2[0] + v2[0] }; // 3 -- construct value to be returned to caller
    return v3;                        // 4 -- actually return value
}

int main()
{
    std::vector v1 { 5 };                    // 1 -- construct value to be passed to function
    std::cout << doSomething(v1)[0] << '\n'; // 2 -- actually pass value

    std::cout << v1[0] << '\n';

    return 0;
}
```

Copies 1 and 3 **cannot be optimized away** — we must construct the input vector and the return vector. `std::vector` is an owner of its data, so it necessarily makes a copy of its initializer.

**Copy 2** — passing `v1` to `v2` (by value):
- **Pass by reference/address?** ✅ Yes — **best option**. Avoids the copy entirely, works with both lvalues and rvalues.
- **Can it be elided?** ❌ No — elision only applies to redundant copies, not argument passing.
- **Out parameter?** ❌ No — we're passing data *to* the function, not getting data *out*.
- **Move semantics?** ❌ No — `v1` is an **lvalue**. Moving from it would leave `v1` empty, causing UB when printing `v1[0]` afterward.

**Copy 4** — returning `v3` from function back to caller:
- **Return by reference/address?** ❌ No — `v3` is a local variable; its reference becomes **dangling** when the function returns.
- **Can it be elided (NRVO)?** ✅ Possibly — compiler can construct `v3` directly in the caller's scope (as-if rule), but this is **not guaranteed**.
- **Out parameter?** ✅ Technically possible — pass an empty `std::vector` by non-const ref and fill it. But this has significant downsides: ugly calling semantics, doesn't work with types that don't support assignment, and is hard to write for both lvalue and rvalue arguments.
- **Move semantics?** ✅ Yes — since `v3` is destroyed when the function returns, its data can be **moved** to the caller instead of copied. **Move semantics is invoked automatically when returning by value.**

> [!TIP]
> Elision is the best option for Copy 4 but is out of our control. The next best option for move-capable types is **move semantics**, which is automatically used when elision doesn't apply. **To summarize: pass move-capable types by `const` reference, and return them by value.**

---

### 📁 Code Examples for Section 5
- [`16_4_Returning_std_vector_n_introduction_to_move_semantics/1_move_semantics_std_vectors.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_4_Returning_std_vector_n_introduction_to_move_semantics/1_move_semantics_std_vectors.cpp): Demonstrates returning `std::vector` by value, NRVO/RVO, and resource stealing via move constructors.

---

## 6 — Arrays and Loops

### Traversal and Iteration
- Accessing each element of a container in sequence is called **traversal** (or **iteration**).

### Scalability: Templates + Arrays + Loops
- **Arrays**: Provide storage for multiple objects without naming each element.
- **Loops**: Provide traversal without explicitly writing statements for each element.
- **Templates**: Parameterize element types.
- **Together**: They allow writing generic, reusable algorithms that scale to any number of elements of any compatible type.

### Common Array Operations
- **Calculate aggregate values**: Sum, average, variance.
- **Search**: Finding exact matches, counting matches, locating minimum/maximum elements.
- **Transform**: Modifying each element in place (e.g. doubling values, string formatting).
- **Reorder**: Sorting, reversing, shuffling. (Prefer standard library algorithms like `std::sort`).

### Off-by-One Errors
> [!WARNING]
> When iterating using index loops, always use **`index < length`**, **NEVER `index <= length`**!
> An array of length $N$ has valid elements from $0$ to $N-1$. Index $N$ is one past the end.

---

### 📁 Code Examples for Section 6
- [`16_5_Arrays_n_loops/1_array_n_loops.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_5_Arrays_n_loops/1_array_n_loops.cpp): Shows template function `calculateAverage` traversing vectors of both `int` and `double` using index-based loops.

---

## 7 — Arrays, Loops, and Sign Challenge Solutions

### The Unsigned Reverse Loop Bug
Because standard container sizes and indices are unsigned (`std::size_t`), reversing an array with an unsigned index contains **two severe bugs**:
```cpp
for (std::size_t index{ arr.size() }; index >= 0; index--) // BUG!
```
1. **Out-of-bounds start**: `arr[arr.size()]` accesses an invalid element on the first iteration (valid indices are $0$ to $N-1$).
2. **Infinite loop / wrap-around**: Because `index` is unsigned, `index >= 0` is **always true**. When `index` reaches `0` and is decremented (`index--`), it wraps around to `SIZE_MAX`, causing an infinite loop and crashes.

### Leaving Signed/Unsigned Conversion Warnings Off
> [!WARNING]
> Some developers disable signed/unsigned conversion warnings because indexing standard containers with signed types generates many warnings. However, **this is not recommended** — disabling these warnings also suppresses **legitimate sign-conversion bugs** that can lead to real defects.

### Unsigned Loop Variables
- Containers define `size_type` (usually `std::size_t`).
- In templates, dependent names require `typename`:
  ```cpp
  for (typename std::remove_reference_t<decltype(arr)>::size_type i{0}; i < arr.size(); ++i)
  ```
- Because this is verbose, most developers simply use `std::size_t` directly for forward loops.
- **Rule of thumb**: Prefer `std::size_t` for indexing unless working with custom allocators.

### Using a Signed Loop Variable
Favoring signed types for loop indices improves safety and avoids wrap-around bugs. Three things are needed:
1. **Choose a signed type**:
   - `int`: Fine for small/medium arrays.
   - `std::ptrdiff_t`: Standard signed counterpart to `std::size_t`.
   - `using Index = std::ptrdiff_t;`: Clean type alias.
   - **C++23 `0Z` suffix**: Signed size literal (`auto i{ 0Z };`).
2. **Obtain signed length**:
   - Pre-C++20: `static_cast<std::ptrdiff_t>(arr.size())`
   - C++20: **`std::ssize(arr)`**
3. **Avoid signed/unsigned indexing warnings**:
   - Index the underlying pointer via **`arr.data()[index]`**!

```cpp
// Sane reverse traversal using std::ssize and .data()
for (auto index{ std::ssize(arr) - 1 }; index >= 0; --index)
{
    std::cout << arr.data()[index] << ' ';
}
```

### Why `arr.data()[index]` is the Best Indexing Option
- We can use signed loop variables and indices.
- We don't have to define any custom types or type aliases.
- The readability hit from using `.data()` is small.
- There is no performance cost in optimized (release) builds.

> [!TIP]
> **The Sane Choice**: Avoid integral indexing altogether whenever possible! Use **range-based for loops**, iterators, or standard algorithms (`std::ranges`).
> **Rule of thumb**: Avoid array indexing with integral values whenever possible.

---

### 📁 Code Examples for Section 7
- [`16_6_Arrays_loops_n_sign_challenge_solutions/1_arrays_loops_sign_issues.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_6_Arrays_loops_n_sign_challenge_solutions/1_arrays_loops_sign_issues.cpp): Demonstrates reverse iteration using `Index = std::ptrdiff_t`, `std::ssize()`, and safe access via `ref.data()[index]`.

---

## 8 — Range-based For Loops (for-each)

### Syntax
```cpp
for (element_declaration : array_object)
    statement;
```
- C++ provides range-based for loops to traverse containers **without explicit indexing**. They are simpler, safer, and work with common array types such as `std::vector`, `std::array`, and C-style arrays.
- **Empty containers**: If the container has no elements, the loop body simply does not execute.

> [!TIP]
> **Best Practice**: Favor **range-based for loops** over regular index-based for-loops when traversing containers.

### Element Declaration: `auto` vs `auto&` vs `const auto&`
- Use **type deduction (`auto`)** with range-based for loops to have the compiler deduce the element type. This avoids redundant typing and prevents accidental type mismatches:
  ```cpp
  for (auto elem : arr) { /* ... */ }
  ```
- **Maintainability benefit**: If the element type of the array is ever updated (e.g. from `int` to `long`), `auto` will **automatically deduce the updated element type**, ensuring the loop stays in sync and preventing unwanted implicit conversions.
- Avoid copying expensive types (e.g. `std::string`) in loops — **prefer referencing elements** to avoid unnecessary copies:
  ```cpp
  for (const auto& word : words)
      std::cout << word << '\n';
  ```

| Type | Behavior | When to Use |
|---|---|---|
| **`auto`** | Copies each element by value | Cheap-to-copy fundamental types (`int`, `double`), or when modifying a local copy |
| **`auto&`** | Binds a non-const reference | When you need to **modify the original elements in place** |
| **`const auto&`** | Binds a const reference | **Default choice** for non-trivial types (`std::string`, `std::vector`) to avoid expensive copies |

### Traversal Restrictions
- **Decayed C-style arrays**: Range-based for loops do **not** work on decayed C-style arrays because pointer decays lose array bounds.
- **Enumerations**: Cannot iterate through plain enums.

### Getting the Index of the Current Element
1. **Manual counter**: Maintain an outer integer counter.
2. **C++20 Init-Statement**:
   ```cpp
   for (int i{ 0 }; const auto& elem : arr)
   {
       std::cout << "Index " << i << ": " << elem << '\n';
       ++i;
   }
   ```
   This keeps `i` strictly scoped inside the loop.

### Reverse Traversal in Range-based For Loops
- Prior to C++20: Traditional loops or reverse iterators were required.
- **C++20 Ranges View**:
  ```cpp
  #include <ranges>

  for (const auto& elem : std::views::reverse(arr))
      std::cout << elem << ' ';
  ```
  Creates a non-owning, zero-overhead reverse view.

---

### 📁 Code Examples for Section 8
- [`16_7_Range-based_for_loops_for-each/1_range_based_for_loop.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_7_Range-based_for_loops_for-each/1_range_based_for_loop.cpp): Demonstrates range-based for loops with `const auto&` and C++20 `std::views::reverse` on strings.

---

## 9 — Array Indexing and Length Using Enumerators

- One of the **bigger documentation problems with arrays** is that integer indices do not provide any information to the programmer about the **meaning of the index**. For example, `testScores[2]` tells you nothing about *which student* index 2 represents.

### Self-Documenting Indices with Unscoped Enumerations
- Plain integer indices (`arr[3]`) are **magic numbers** — opaque and error-prone.
- Unscoped enumerators implicitly convert to `std::size_t`.
- Because enumerators are **compile-time constants (`constexpr`)**, converting an enumerator to `std::size_t` is **NOT a narrowing conversion** $\rightarrow$ no compiler warnings!

### Non-`constexpr` Enum Variables
- An enum *variable* (`Students::Names student`) is **not** `constexpr`.
- If an enum defaults to a signed underlying type, indexing with an enum variable can trigger sign conversion warnings.
- **Fix**: Specify the underlying type explicitly:
  ```cpp
  enum Names : unsigned int
  {
      Rama, Laxmana, Bharata, Shathragna, max_student
  };
  ```

### The Count Enumerator Pattern
- Adding a final enumerator (`max_student`, `count`) whose automatic value equals the total number of prior enumerators:
  ```cpp
  std::vector<int> testScores(Students::max_student);
  assert(std::size(testScores1) == Students::max_student);
  ```
- **Assertions**:
  - `constexpr` arrays (`std::array`): Use **`static_assert`**.
  - Non-`constexpr` dynamic arrays (`std::vector`): Use **runtime `assert`**.

### Scoped Enumerations (`enum class`) and Solutions
`enum class` enumerators do **not** implicitly convert to integers. To index an array using an `enum class`, use one of three solutions:
1. **`static_cast`**:
   ```cpp
   testScores[static_cast<std::size_t>(Students::Rama)] = 95;
   ```
2. **Unary `operator+` Overload**:
   ```cpp
   constexpr auto operator+(Students s) noexcept
   {
       return static_cast<std::underlying_type_t<Students>>(s);
   }
   testScores[+Students::Rama] = 95;
   ```
3. **C++23 `std::to_underlying`** (from `<utility>`):
   ```cpp
   testScores[std::to_underlying(Students::Rama)] = 95;
   ```

---

### 📁 Code Examples for Section 9
- [`16_8_Array_indexing_n_length_using_enumerators/1_constexpr_unscped_enums_for_indexing.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_8_Array_indexing_n_length_using_enumerators/1_constexpr_unscped_enums_for_indexing.cpp): Unscoped enumerator indexing with compile-time safety and self-documenting code.
- [`16_8_Array_indexing_n_length_using_enumerators/2_non_constexpr_unscped_enums_for_indexing.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_8_Array_indexing_n_length_using_enumerators/2_non_constexpr_unscped_enums_for_indexing.cpp): Using non-constexpr enum variables with `: unsigned int` underlying type to avoid sign warnings.
- [`16_8_Array_indexing_n_length_using_enumerators/3_count_enum_assert_array_length.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_8_Array_indexing_n_length_using_enumerators/3_count_enum_assert_array_length.cpp): Sizing vectors and runtime asserting length using the count enumerator pattern.
- [`16_8_Array_indexing_n_length_using_enumerators/4_count_enum_class_assert_array_length.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_8_Array_indexing_n_length_using_enumerators/4_count_enum_class_assert_array_length.cpp): Indexing with scoped `enum class` via overloaded unary `operator+` and `std::underlying_type_t`.

---

## 10 — std::vector Resizing and Capacity

### Fixed-Size vs Dynamic Arrays
- **Fixed-size arrays (`std::array`, C-style arrays)**: Length must be known at compile time and cannot change.
- **Dynamic arrays (`std::vector`)**: Length can be resized dynamically at runtime.

### Resizing a `std::vector`
```cpp
vi.resize(8); // changes size to 8
```
1. **Existing elements are preserved**.
2. **New elements are value-initialized** (0 for fundamental types, default constructor for class types).
3. **Resizing smaller**: Truncates elements beyond the new size (destructors called), but capacity is typically unchanged.

### Length vs Capacity
- **Length (`.size()`)**: Number of elements currently in use.
- **Capacity (`.capacity()`)**: Total allocated storage slots currently available without needing reallocation.
> [!IMPORTANT]
> **Array indexing is strictly based on length (`size()`), NOT capacity!**
> Accessing `v[i]` where `size() <= i < capacity()` is **undefined behavior**!

### Reallocation and Iterator Invalidation
When storage is exceeded, `std::vector` performs a **reallocation**:
1. Allocates a new, larger block of contiguous memory.
2. Copies or moves existing elements from old memory to new memory.
3. Destroys elements in the old memory and frees the old block.
4. Updates internal pointers, length, and capacity.

> [!WARNING]
> **Iterator / Reference Invalidation**:
> **A reallocation invalidates ALL existing pointers, references, and iterators pointing to elements of the vector!** Any subsequent access through an old reference or pointer is undefined behavior (dangling memory access).

### Why Differentiate Length and Capacity?
- If `std::vector` only tracked its length, every `resize()` request would trigger an expensive reallocation to the new exact length.
- **Separating length and capacity** allows `std::vector` to be smarter: it can avoid reallocations when length changes don't exceed the allocated capacity.
- Tracking capacity separately from length allows `std::vector` to avoid some reallocations when length is changed.

### Shrinking Capacity: `shrink_to_fit()`
- Resizing smaller decreases length, but does **not** free capacity.
- **`shrink_to_fit()`**: A non-binding request asking the implementation to free unused capacity to match `size()`.

> [!NOTE]
> `shrink_to_fit()` is non-binding — implementations may honor, partially honor, or ignore it.

---

### 📁 Code Examples for Section 10
- [`16_9_std_vector_resizing_n_capacity/1_resizing_vector.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_9_std_vector_resizing_n_capacity/1_resizing_vector.cpp): Shows `resize()` doubling size with value-initialization, and shrinking size.
- [`16_9_std_vector_resizing_n_capacity/2_capacity_on_resizing_vector.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_9_std_vector_resizing_n_capacity/2_capacity_on_resizing_vector.cpp): Demonstrates how capacity grows and stays retained after shrinking, plus `shrink_to_fit()`.

---

## 16.11 — std::vector and Stack Behavior

### What is a Stack?
- A **LIFO (Last-In, First-Out)** container data type.
- **Core operations**:
  - `push`: Put new element on top of stack.
  - `pop`: Remove top element from stack.
- **Optional operations**:
  - `top / peek`: Look at top element without removing it.
  - `empty`: Check if stack has no elements.
  - `size`: Count of elements currently on the stack.

### Stacks in C++
- Some languages have a dedicated stack type, but this is limiting because a **pure stack interface can't support tasks like iterating without modification**.
- In C++, stack operations are provided by containers like **`std::vector`**, `std::deque`, and `std::list`, allowing them to be used as stacks while retaining their full container functionality.

### Stack Behavior in `std::vector`
| Function | Stack Operation | Behavior | Notes |
|---|---|---|---|
| **`push_back(val)`** | Push | Adds element to the end | Triggers reallocation if `size == capacity` |
| **`emplace_back(args...)`** | Push | Constructs element in-place at end | Often more efficient; avoids temporaries |
| **`pop_back()`** | Pop | Removes top element from the end | Returns `void`; does NOT return removed value |
| **`back()`** | Top / Peek | Returns reference to the top element | Undefined behavior if vector is empty |
| **`empty()`** | Empty check | Returns `true` if `size() == 0` | Fast check |

> [!NOTE]
> Both `push_back()` and `emplace_back()` will **increment the length** of a `std::vector`, and will trigger a reallocation if the current capacity is not sufficient.

### Extra Capacity from Pushing (Growth Factor)
- Compilers automatically allocate extra capacity when growing to amortize reallocation costs:
  - **GCC / Clang**: Doubles capacity ($\times 2$).
  - **MSVC (Visual Studio)**: Multiplies capacity by $1.5$ ($\times 1.5$).

### `resize()` vs `reserve()` for Stack Behavior
> [!CAUTION]
> **Do NOT use `resize()` to preallocate space for a stack!**
> - `resize(N)` creates $N$ zero/default-initialized elements. Calling `push_back()` will append items at index $N$, $N+1$, leaving $N$ unwanted dummy items at the bottom of the stack!
> - **Use `reserve(N)` instead**: `reserve(N)` increases capacity without changing size (`size() == 0`). `push_back()` then inserts at index $0$ without reallocations.

### `push_back()` vs `emplace_back()`
- **Existing object**: If the object already exists, `push_back(obj)` and `emplace_back(obj)` do the same thing (copy/move).
- **Temporary object**: `emplace_back(arg1, arg2)` forwards constructor arguments directly to construct the object in-place inside the vector, avoiding a temporary object and move/copy.
- **Explicit constructors**: `push_back({arg})` will **not** invoke `explicit` constructors, whereas `emplace_back(arg)` **can** invoke explicit constructors.
- **Aggregate initialization**: Supported in `emplace_back()` starting in C++20.

---

### 📁 Code Examples for Section 11 (16.11)
- [`16_10_std_vector_n_stack_behavior/1_stack_using_vector.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_10_std_vector_n_stack_behavior/1_stack_using_vector.cpp): Basic stack operations (`push_back`, `pop_back`, `back`, `empty`) tracking size and capacity.
- [`16_10_std_vector_n_stack_behavior/2_resize_issue_stack_using_vector.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_10_std_vector_n_stack_behavior/2_resize_issue_stack_using_vector.cpp): Highlights why `resize()` corrupts stack behavior and why `reserve()` is the correct tool.
- [`16_10_std_vector_n_stack_behavior/3_emplace_back.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_10_std_vector_n_stack_behavior/3_emplace_back.cpp): In-place construction with `emplace_back` vs temporary copies with `push_back`, and explicit constructor behavior.
- [`16_10_std_vector_n_stack_behavior/4_stack_operations_using_vector.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_10_std_vector_n_stack_behavior/4_stack_operations_using_vector.cpp): Clean functional encapsulation of stack operations with formatted status output.
- [`16_10_std_vector_n_stack_behavior/5_stack_operations_using_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/16_Dynamic_arrays_std_vector/16_10_std_vector_n_stack_behavior/5_stack_operations_using_class.cpp): Complete fixed-capacity generic Stack class template returning `std::optional<T>` for safe `pop()` and `getTop()`.

---

## 16.12 — std::vector\<bool\>

### What is `std::vector<bool>`?
- `std::vector<bool>` is a **specialized class template specialization** of `std::vector` for type `bool`.
- It packs boolean values as **individual bits** ($\approx 8$ booleans per byte) to minimize memory usage. This is achieved via **class template specialization**.

### Basic Usage
- Works similarly to a normal `std::vector` for:
  - **Initialization**: `std::vector<bool> v{ true, false, true };`
  - **Indexing**: `v[0]`, `v.at(1)`
  - **Iteration**: range-based for loops work
- Values print as `0` and `1` when streamed with `std::cout`.

### Comparison with `std::bitset`
- `std::bitset` is designed specifically for **bit manipulation** with fixed compile-time size and bitwise operations (`&`, `|`, `^`, `<<`, `>>`).
- `std::vector<bool>` does **not** provide bit-manipulation functions; it is intended to behave like a dynamic array of booleans.

### Tradeoffs and Major Flaws
1. **Memory Overhead**: The vector object itself has header overhead; for small vectors, it may use more memory, not less.
2. **Performance Penalties**: Bit manipulation requires masking and shifting on every read/write, which can be slower than byte access.
3. **Not a True C++ Container ❌**:
   - **Not contiguous**: Individual bits do not have discrete memory addresses in C++.
   - **Does not store `bool` objects**: It stores packed bits.
   - **`operator[]` does NOT return `bool&`**: It returns a temporary proxy object (`std::vector<bool>::reference`).
   - You cannot take the address of an element: `&v[0]` will not yield `bool*`.
4. **Incompatibility with Generic Code**:
   - `operator[]` does **not** return `bool&` — it returns a proxy reference type.
   - Generic code that works for `std::vector<T>` often **breaks** for `T = bool`.
   - **The `auto` Type Deduction Trap**:
     ```cpp
     auto b = v[0]; // b is std::vector<bool>::reference, NOT bool!
     ```
     If the vector is modified or goes out of scope, using `b` results in undefined behavior (dangling reference to internal bit).
     **Best Practice**: Always explicitly specify `bool`:
     ```cpp
     bool b = v[0]; // Forces conversion to a real bool copy
     ```
5. **Thread-Safety Hazard**: Concurrent writes to adjacent bits in the same byte from different threads are not thread-safe and cause data races.

### Why `std::vector<bool>` Is Discouraged
- Its behavior **differs from all other `std::vector<T>` specializations**, making it a special case that surprises developers.
- Causes subtle **bugs and incompatibilities** in generic code.
- There is **no way to opt out** of the optimized bit-packed version — if you use `std::vector<bool>`, you always get the specialization.
- **Modern consensus**: avoid using it.

### Recommended Alternatives
- **`std::bitset<N>`**: When the number of bits is known at compile time (supports full bitwise operations).
- **`std::vector<char>` / `std::vector<uint8_t>`**: When you need a dynamic, resizable container of boolean flags that behaves as a real standard container without bit-proxy headaches.
- **`boost::dynamic_bitset`**: When you need a dynamically resizable bitset with explicit bit-manipulation operations.

> [!TIP]
> **Best Practice**: Avoid `std::vector<bool>`. Favor `std::bitset`, `std::vector<char>`, or third-party dynamic bitsets instead.
