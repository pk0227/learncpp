# Chapter 20: Functions

Functions are the primary building blocks of executable logic in C++. This chapter explores advanced function mechanics: **function pointers**, the architecture of **the stack and the heap**, **recursion and memoization**, and processing **command-line arguments**.

---

## Table of Contents
1. [1 — Function Pointers](#1--function-pointers)
2. [2 — The stack and the heap](#2--the-stack-and-the-heap)
3. [3 — Recursion](#3--recursion)
4. [4 — Command line arguments](#4--command-line-arguments)

---

## 1 — Function Pointers

### Pointers to Functions
A **function pointer** is a pointer that points to executable code (a function) in memory rather than a data value:

```cpp
// fcnPtr is a pointer to a function taking no arguments and returning an integer:
int (*fcnPtr)();
```

#### Const Function Pointers
To make the function pointer itself `const` (cannot be reassigned to another function), place `const` **after the asterisk**:
```cpp
int (*const fcnPtr)(); // Const pointer to a non-const function returning int
```
If you place `const` before `int`, it specifies that the function being pointed to returns a `const int`:
```cpp
const int (*fcnPtr)(); // Non-const pointer to a function returning const int
```

### Assigning Functions to Function Pointers
```cpp
int (*fcnPtr)(){ &foo }; // fcnPtr points to function foo
fcnPtr = &goo;          // fcnPtr now points to function goo
```

> [!IMPORTANT]
> The signature (parameter types and return type) of the function pointer **must exactly match** the signature of the target function. Mismatched return types or parameters will cause compile-time errors:
> ```cpp
> double goo();
> int (*fcnPtr)(){ &goo }; // ❌ COMPILE ERROR: return types do not match!
> ```

- **Implicit Conversion**: Unlike fundamental types, C++ implicitly converts a function name into a function pointer when needed (the address-of operator `&` is optional):
  ```cpp
  int (*fcnPtr)(){ foo }; // OK: foo implicitly converts to &foo
  ```
- **No Conversion to `void*`**: Function pointers do not convert to `void*` (or vice-versa) in standard C++, because standard C++ does not guarantee that code pointers and data pointers are the same size.
- **Null Initialization**: Function pointers can be initialized or assigned `nullptr`:
  ```cpp
  int (*fcnPtr)(){ nullptr }; // OK
  ```

### Calling a Function Using a Function Pointer
You can invoke a function pointer in two ways:
1. **Explicit Dereference**:
   ```cpp
   (*fcnPtr)(5); // Explicit dereference
   ```
2. **Implicit Dereference**:
   ```cpp
   fcnPtr(5);    // Implicit dereference (cleaner and idiomatic)
   ```

### Default Arguments Don’t Work with Function Pointers
- When a function with default arguments is called normally, the compiler **rewrites the function call at compile time** to insert the default values.
- However, when a function is called through a function pointer, the function is resolved at **runtime** (**late binding**).
- Because the call is resolved at runtime, the compiler cannot rewrite the call site, and **default arguments are NOT applied**:

```cpp
void print(int x, int y = 10);

using PrintFn = void(*)(int, int);
PrintFn p{ print };

// p(5);       // ❌ COMPILE ERROR: too few arguments for function pointer call!
p(5, 10);      // ✅ Must provide all arguments explicitly
```

### Callback Functions
- One of the most useful applications of function pointers is passing a function as an argument to another function. Functions passed in this way are called **callback functions**.
- If a function parameter is declared with a function type, the compiler automatically adjusts it to a pointer to that function type:
  ```cpp
  void selectionSort(int* arr, int size, bool (*comparisonFcn)(int, int));
  // Equivalently written as:
  void selectionSort(int* arr, int size, bool comparisonFcn(int, int));
  ```

#### Providing Default Callback Functions:
You can assign a default function pointer as a default parameter:
```cpp
void selectionSort(int* arr, int size, bool (*comparisonFcn)(int, int) = ascending);
```

### Making Function Pointers Prettier with Type Aliases
Raw function pointer syntax is notoriously verbose. Use a type alias (`using`) to make parameter signatures readable:

```cpp
// Verbose:
bool validate(int x, int y, bool (*fcnPtr)(int, int));

// Clean and maintainable:
using ValidateFunction = bool(*)(int, int);
bool validate(int x, int y, ValidateFunction pfcn);
```

### Using `std::function`
Standard library `<functional>` provides **`std::function`**, a type-erased wrapper that can hold any callable target (free functions, function pointers, lambdas, or functors):

```cpp
#include <functional>

// Accepts any callable returning bool and taking two ints:
bool validate(int x, int y, std::function<bool(int, int)> fcn);

std::function<int(int, int)> fcnPtr{ add };
fcnPtr = subtract;
```

> [!NOTE]
> `std::function` objects can **only** be invoked via implicit dereference syntax (`fcnPtr(5)`), not explicit dereference (`(*fcnPtr)(5)`).

### Type Inference with `auto`
The `auto` keyword can deduce the type of a function pointer automatically:
```cpp
auto fcnPtr{ &add }; // Deduces int (*)(int, int)
```
*Tradeoff*: While convenient, `auto` hides the parameter types and return type from the reader, making code harder to inspect without tooltips.

### 📁 Code Examples for Section 1
- [`20_1_Function_Pointers/0_function_pointer_basics.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/20_Functions/20_1_Function_Pointers/0_function_pointer_basics.cpp): Demonstrates basic function pointer syntax, explicit vs implicit invocation, `nullptr` checking, and why default arguments fail through function pointers.
- [`20_1_Function_Pointers/1_selection_sort_with_callback.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/20_Functions/20_1_Function_Pointers/1_selection_sort_with_callback.cpp): Demonstrates using raw function pointer callbacks to customize selection sort order (ascending vs descending).
- [`20_1_Function_Pointers/2_selection_sort_with_callback_type_alias.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/20_Functions/20_1_Function_Pointers/2_selection_sort_with_callback_type_alias.cpp): Demonstrates simplifying function pointer callback signatures using `using` type aliases.
- [`20_1_Function_Pointers/3_selection_sort_with_callback_std_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/20_Functions/20_1_Function_Pointers/3_selection_sort_with_callback_std_function.cpp): Demonstrates standard library `std::function<bool(int, int)>` as a flexible callback abstraction.

---

## 2 — The stack and the heap

### Memory Segments of a Program
When a program is loaded into memory, the operating system organizes memory into several distinct segments:

```
┌─────────────────────────────────────────────────────────────┐
│                    Program Memory Layout                    │
├─────────────────────────────────────────────────────────────┤
│  Code / Text Segment    │ Compiled instructions (Read-Only) │
├─────────────────────────┼───────────────────────────────────┤
│  Data Segment           │ Initialized global & static vars  │
├─────────────────────────┼───────────────────────────────────┤
│  BSS Segment            │ Zero-initialized static & globals │
├─────────────────────────┼───────────────────────────────────┤
│  Heap Segment (Free)    │ Dynamic allocations (Grows down)  │
│         ▼               │                                   │
│         ▲               │                                   │
│  Call Stack Segment     │ Local stack frames (Grows up)     │
└─────────────────────────┴───────────────────────────────────┘
```

| Memory Segment | Content | Characteristics |
|---|---|---|
| **Code / Text** | Compiled machine instructions | Typically read-only to prevent self-modifying code |
| **Data Segment** | Initialized global and static variables | Fixed size, lives throughout the program |
| **BSS Segment** | Uninitialized or zero-initialized global and static variables | Initialized to zero by the OS loader |
| **Heap** | Dynamically allocated variables (`new`, `malloc`) | Large memory pool; allocations persist until explicitly freed |
| **Call Stack** | Function parameters, local variables, return addresses | Fast LIFO stack; automatically managed during function calls |

### The Heap Segment: Pros and Cons
- **Advantages**:
  - Huge capacity (gigabytes on modern systems).
  - Can allocate large objects, vectors, and dynamic buffers that would crash the stack.
- **Disadvantages**:
  - Allocation is comparatively slow (requires OS memory lookup).
  - Memory persists until explicitly freed (risk of memory leaks).
  - Access requires two-step pointer indirection.

### The Stack Data Structure & Call Stack
A stack is a **Last-In, First-Out (LIFO)** data structure supporting three basic operations:
- `push()`: Put a new item on top of the stack.
- `pop()`: Remove the top item from the stack.
- `top()` / `peek()`: Inspect the top item.

#### CPU Registers Managing the Stack:
- **Stack Pointer (SP)**: A dedicated CPU register pointing directly to the current top of the stack. Updated automatically on every push and pop.
- **Program Counter (PC / IP)**: A CPU register holding the memory address of the next instruction to execute.

### The Call Stack in Action
When a function is called:
1. A **stack frame** (activation record) is constructed and pushed onto the call stack. It stores:
   - The **return address** (address of the instruction following the call site).
   - All function **arguments**.
   - Storage for all **local variables**.
   - Saved copies of modified CPU registers that must be restored on return.
2. The CPU updates the Program Counter to jump to the function's entry point.
3. Instructions in the function execute.

When the function finishes:
1. Saved CPU registers are restored.
2. The stack frame is popped off the stack (stack unwinding), instantly freeing memory for all local variables.
3. The return value is transferred (via registers or dedicated stack memory).
4. The CPU resumes execution at the saved return address.

### Stack Overflow
- The call stack has a fixed size limit (typically **8 MB** on Linux/Unix with GCC/Clang, **1 MB** on Windows with MSVC).
- **Stack overflow** occurs when a program attempts to allocate more memory on the stack than is available.
- Common causes:
  1. Deep or infinite recursion (stack frames pile up indefinitely).
  2. Allocating massive local arrays on the stack (e.g. `int arr[10000000];`).
- Overflowing the stack triggers an OS memory access violation (segmentation fault) and terminates the program immediately.

### Stack vs Heap Comparison

| Feature | The Stack | The Heap |
|---|---|---|
| **Speed** | Very fast (adjusts SP register) | Slower (bookkeeping, locks, free-lists) |
| **Size** | Limited (~1–8 MB) | Vast (gigabytes) |
| **Lifetime** | Automatic (freed upon block exit) | Manual (until `delete` or program ends) |
| **Access** | Direct compile-time offset | Indirect via pointer |
| **Safety** | No memory leaks; stack overflow risk | Memory leak & dangling pointer risk |

### 📁 Code Examples for Section 2
- [`20_2_The_stack_n_the_heap/1_stack_overflow.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/20_Functions/20_2_The_stack_n_the_heap/1_stack_overflow.cpp): Demonstrates stack overflow triggered by allocating a large local array on the stack.
- [`20_2_The_stack_n_the_heap/2_stack_overflow.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/20_Functions/20_2_The_stack_n_the_heap/2_stack_overflow.cpp): Demonstrates stack overflow caused by unbounded recursion exhausting available stack frames.

---

## 3 — Recursion

A **recursive function** is a function that calls itself directly or indirectly.

### Recursive Termination Conditions (Base Cases)
- A **recursive termination condition** (or base case) is a condition that, when met, causes the function to stop calling itself and return a value.
- Without a base case, recursion continues indefinitely until the call stack is exhausted, causing a **stack overflow**.

```cpp
int factorial(int n) {
    if (n <= 1)      // Recursive termination condition (base case)
        return 1;
    return n * factorial(n - 1); // Recursive call
}
```

### Tail Call Optimization (TCO)
- A **tail call** is a function call that occurs as the very last statement in a function before returning.
- Compilers can optimize recursive tail calls into iterative loops, reusing the existing stack frame and preventing stack overflow.

### Memoization
- **Memoization** is an optimization technique where expensive function results are cached (memoized) so that subsequent calls with identical inputs return immediately from cache:

```cpp
int fibonacci(int count) {
    // Static vector cache persists across calls:
    static std::vector<int> cache{ 0, 1 };

    if (count < static_cast<int>(cache.size()))
        return cache[count];

    cache.push_back(fibonacci(count - 1) + fibonacci(count - 2));
    return cache[count];
}
```

### Iteration vs Recursion
- **Iterative functions** (using `for` or `while` loops) avoid the runtime overhead of pushing and popping stack frames.
- **When to use recursion**:
  - The recursive implementation is significantly simpler and more intuitive (e.g. tree or graph traversal).
  - The recursion depth is bounded and safe from stack overflow.
  - The iterative equivalent would require manually managing a dynamic stack data structure.

> [!TIP]
> **Best Practice**: Generally favor **iteration over recursion**, unless recursion significantly simplifies the code and the recursion depth is guaranteed to remain safely bounded.

### 📁 Code Examples for Section 3
- [`20_3_Recursion/1_fibonacci_numbers_recursive.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/20_Functions/20_3_Recursion/1_fibonacci_numbers_recursive.cpp): Demonstrates naive exponential-time recursive Fibonacci.
- [`20_3_Recursion/2_fibonacci_numbers_recursive_memoization.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/20_Functions/20_3_Recursion/2_fibonacci_numbers_recursive_memoization.cpp): Demonstrates memoized recursive Fibonacci using a static cache to achieve linear time complexity.
- [`20_3_Recursion/3_fibonacci_numbers_iterative.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/20_Functions/20_3_Recursion/3_fibonacci_numbers_iterative.cpp): Demonstrates optimal iterative Fibonacci with $O(n)$ time and $O(1)$ space.

---

## 4 — Command line arguments

**Command line arguments** are optional string arguments passed by the operating system to the program upon launch, providing external inputs to `main()`.

### Syntax of `main()` with Command Line Arguments
```cpp
int main(int argc, char* argv[])
```
- **`argc` (Argument Count)**: An integer containing the total number of arguments passed to the program.
  - `argc` is always at least `1`, because `argv[0]` is always the program's executable name.
- **`argv` (Argument Vector)**: A C-style array of `char*` pointers, each pointing to a null-terminated C-style string argument. Length of `argv` is `argc`.

```cpp
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Program name: " << argv[0] << '\n';

    for (int i{ 1 }; i < argc; ++i) {
        std::cout << "Argument " << i << ": " << argv[i] << '\n';
    }
    return 0;
}
```

> [!NOTE]
> **Why Range-Based For Loops Fail on `argv`**:  
> You cannot use a range-based `for` loop on `argv` (`for (auto arg : argv)` will not compile). Because `argv` is passed as a function parameter, it is a **decayed pointer** (`char**`) with no compile-time size information. Range-for loops require non-decayed containers or arrays where `std::begin` and `std::end` can be determined.

### Converting Numeric Arguments
Command line arguments are always passed as text strings. To use numeric inputs, convert them using standard conversion utilities:

```cpp
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;

    // Approach 1: std::stringstream
    std::stringstream convert{ argv[1] };
    int value{};
    if (!(convert >> value)) {
        std::cerr << "Invalid integer!\n";
        return 1;
    }

    // Approach 2: std::stoi (C++11)
    int intVal{ std::stoi(argv[1]) };

    // Approach 3: std::from_chars (C++17 - fastest, non-allocating, non-throwing)
    // std::from_chars(argv[1], argv[1] + std::strlen(argv[1]), value);

    return 0;
}
```

### 📁 Code Examples for Section 4
- [`20_4_Command_line_arguments/1_command_line_arguments_strings.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/20_Functions/20_4_Command_line_arguments/1_command_line_arguments_strings.cpp): Demonstrates reading `argc` and iterating through string arguments in `argv`.
- [`20_4_Command_line_arguments/2_command_line_arguments_integers.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/20_Functions/20_4_Command_line_arguments/2_command_line_arguments_integers.cpp): Demonstrates converting command line arguments to integers safely using `std::stringstream`.
