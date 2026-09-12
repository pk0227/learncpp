# Chapter 5: Constants and Strings

This chapter explores compile-time programming with constants, integer literal representations across different numeral systems, dynamically allocated string manipulation via `std::string`, and zero-copy string inspection using `std::string_view`.

---

### Table of Contents
1. [Numeral Systems and Literals](#1--numeral-systems-and-literals)
2. [Compile-Time Programming and Constexpr](#2--compile-time-programming-and-constexpr)
3. [Strings: std::string](#3--strings-stdstring)
4. [Strings: std::string_view](#4--strings-stdstring_view)
5. [std::string vs std::string_view (Deep-Dive and Safety Rules)](#5--stdstring-vs-stdstring_view-deep-dive-and-safety-rules)

---

## 1 — Numeral Systems and Literals

### Integer Bases and Literal Prefixes
In C++, integer literals can be specified in four primary numeral bases:
- **Decimal (base 10)**: Standard numbers with no prefix (digits `0`–`9`).
- **Hexadecimal (base 16)**: Prefixed with `0x` or `0X` (digits `0`–`9`, `a`–`f`, `A`–`F`).
- **Octal (base 8)**: Prefixed with `0` (digits `0`–`7`).
- **Binary (base 2)**: Prefixed with `0b` or `0B` (digits `0`–`1`, introduced in C++14).

```cpp
int dec = 36;        // Decimal: 36
int hex = 0x24;      // Hexadecimal: 2*16 + 4 = 36
int oct = 044;       // Octal: 4*8 + 4 = 36
int bin = 0b00100100;// Binary: 32 + 4 = 36
```

> [!WARNING]
> A leading zero (`0`) indicates an **octal literal** in C++! For example, writing `044` produces the decimal value `36`, not `44`. Never pad decimal integers with leading zeros in source code.

### Digit Separators (C++14)
Introduced in C++14, the single quotation mark (`'`) acts as an optional **digit separator** to enhance readability of long numeric literals:
```cpp
int million    { 1'000'000 };
int binaryByte { 0b1010'0101 };
int hexWord    { 0x00FF'AABB };
```
The compiler ignores digit separators during compilation.

### Outputting Binary with `std::bitset`
By default, standard output streams format integers in decimal. While `<iostream>` provides `std::hex` and `std::oct` manipulators, there is no native stream manipulator for binary output prior to C++20/C++23:
- **`std::bitset<N>`** (defined in `<bitset>`): Provides a fixed-width `N`-bit container that outputs in binary:
```cpp
#include <bitset>
#include <iostream>

int val = 36;
std::bitset<8> bits{ static_cast<unsigned long long>(val) };
std::cout << bits << '\n'; // Prints: 00100100
```
- **Modern C++ Formatted Output**:
  - In C++20: `std::cout << std::format("{:b}", 36);`
  - In C++23: `std::println("{:b}", 36);`

### 📁 Code Examples for Section 1
- [`5_3_numeral_systems.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_3_numeral_systems.cpp): Demonstrates representing values in decimal, hexadecimal, octal, and binary, visualizing bits with `std::bitset`, and formatting bases with `std::format`.

---

## 2 — Compile-Time Programming and Constexpr

### Why Compile-Time Programming?
Compile-time programming enables computations to be evaluated by the compiler during build time rather than deferred to runtime.

Benefits:
- **Performance**: Faster runtime execution and smaller binary sizes by pre-computing values.
- **Predictability**: The compiler enforces compile-time requirements and generates compile errors if evaluation fails.
- **Safety**: Undefined behavior and arithmetic errors can be caught at compile-time.
- **Quality**: Logic bugs are caught early in the development cycle before deployment.

### Core Features in C++
Compile-time evaluation in C++ relies on several foundational pillars:
- `constexpr` variables
- `constexpr` functions
- Templates (including non-type template parameters)
- `static_assert` compile-time assertions
- All of these depend on **constant expressions**.

### Constant Expressions
A **constant expression** is an expression that can be fully evaluated at compile-time:
- It must consist only of literals, `constexpr` objects, operators whose operands are constant expressions, and `constexpr` function calls with constant arguments.
- A **non-constant expression** involves runtime variables, system calls, or non-constexpr function calls.

### Const vs Constexpr Variables
| Feature | `const` Variable | `constexpr` Variable |
|---|---|---|
| **Mutability** | Value cannot change after initialization | Value cannot change after initialization |
| **Evaluation Time** | Can be runtime **or** compile-time | **Must be compile-time** |
| **Guaranteed Constant?** | No guarantee of compile-time value | Guaranteed to evaluate at compile-time |
| **Usable in Constant Expressions?** | Only if initialized with constant expression (integral types) | **Always** usable in constant expressions |

```cpp
int userInput{};
std::cin >> userInput;

const int y{ userInput };     // Runtime constant: cannot change, but value unknown at compile-time!
constexpr int x{ 5 };         // Compile-time constant: value known to compiler
// constexpr int z{ userInput }; // COMPILE ERROR: userInput is not a constant expression!
```

> [!IMPORTANT]
> `const` does NOT mean compile-time! It simply means read-only. `constexpr` enforces that the value is known and computed during compilation.

### Function Parameters
- Function parameters are initialized at runtime when a function is invoked.
- Declaring a parameter `const` creates a **runtime constant** within the function body.
- Function parameters **cannot be `constexpr`**.
- If you need a function argument that is strictly guaranteed to be a compile-time constant, use **non-type template parameters (NTTP)**:
```cpp
template <int N>
void doSomething() {
    constexpr int val = N; // OK: N is a compile-time constant expression!
}
```

### Constexpr Functions
A **constexpr function** is a function that is capable of being evaluated at compile-time when used in a constant expression:
- Evaluates at compile-time when called in a context that requires a constant expression.
- Evaluates at runtime when called with runtime arguments or in non-constant contexts.

```cpp
constexpr int cmax(int x, int y) {
    return (x > y) ? x : y;
}

int main() {
    int m1{ cmax(5, 6) };           // Evaluates at runtime (context does not require constant)
    const int m2{ cmax(5, 6) };     // Evaluates at runtime
    constexpr int m3{ cmax(5, 6) }; // Evaluates at compile-time! (Context requires constant)

    int a = 5, b = 6;
    int m4{ cmax(a, b) };           // Evaluates at runtime (arguments are runtime variables)
    // constexpr int m5{ cmax(a, b) }; // COMPILE ERROR: a and b are not constant expressions!
}
```

### Constant Expression Rules

#### Allowed in Constant Expressions
- Literals (e.g. `5`, `1.2`, `'c'`)
- Built-in operators applied to constant expressions
- `sizeof(...)` and `alignof(...)`
- `constexpr` variables
- `constexpr` function calls (provided all arguments are constant expressions)
- `const` integral variables with constant initializers (legacy C++ rule)

#### Not Allowed in Constant Expressions
- Non-const variables
- `const` non-integral variables (e.g. `const double d = 3.14;` prior to `constexpr`)
- Calls to non-constexpr functions
- Function parameters (even within constexpr functions)
- Hardware stream I/O (`std::cin`, `std::cout`)
- Dynamic allocations without compile-time deallocation (`new`, `delete`)
- `throw`, `typeid`

### Evaluation Timing
Constant expressions are *capable* of compile-time evaluation. The compiler is required to evaluate them at compile-time only when the surrounding context demands it:
1. **Always (Required)**: Context demands a constant expression (e.g. `constexpr` variable initializer, array bound, template argument).
2. **Likely (Optimization)**: Context does not require a constant expression, but arguments are constant expressions and the compiler optimizes.
3. **Possibly (As-if rule)**: Optimized by the compiler if values are deducible at compile-time.
4. **Never**: Expression contains runtime variables or non-constexpr operations.

### Best Practices for Constexpr
- Use `constexpr` whenever a variable has a value known at compile-time.
- Use `const` for variables whose values cannot change after initialization, but whose values are only known at runtime.
- Remember that `constexpr` works for non-integral types as well (e.g. `constexpr double gravity{ 9.8 };`).
- Prefer `constexpr` functions over macros for compile-time calculations.
- Use non-type template parameters when an argument must be a compile-time constant.

### 📁 Code Examples for Section 2
- [`5_6_constexpr_const-1.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_6_constexpr_const-1.cpp): Demonstrates the differences between `const` runtime variables and `constexpr` compile-time constants.
- [`5_6_constexpr_const-2.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_6_constexpr_const-2.cpp): Demonstrates constexpr functions and using non-type template parameters for compile-time arguments.
- [`5_6_constexpr_const-3.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_6_constexpr_const-3.cpp): Demonstrates evaluation timing and compile-time vs runtime execution contexts.

---

## 3 — Strings: std::string

### C-Style Strings and Their Limitations
In traditional C, strings are represented as null-terminated arrays of characters (`const char[]`):
- Assigning new values to C-style string arrays is cumbersome and error-prone.
- They do not track their capacity and easily cause buffer overruns.
- Modern C++ strongly favors `std::string` for string storage and `std::string_view` for read-only viewing.

### Introducing `std::string`
Defined in `<string>`, `std::string` is a standard container class that manages a dynamically allocated, null-terminated sequence of characters:
```cpp
#include <string>

std::string empty{};              // Empty string ("")
std::string name{ "Alex" };       // Initialized with string literal
name = "John";                    // Reassigning is clean and safe
```

> [!NOTE]
> Numeric characters stored in strings are text characters, not integers: `std::string myID{ "45" };` stores characters `'4'` and `'5'`, not the number `45`.

### Input and Output with `std::string`
- `operator<<`: Outputs the string to a stream.
- `operator>>`: Extracts word-by-word, stopping at the first whitespace character.
- `std::getline()`: Reads an entire line of text including whitespace until a newline is encountered:

```cpp
#include <iostream>
#include <string>

std::string name{};
std::cout << "Enter full name: ";
std::getline(std::cin >> std::ws, name); // std::ws discards leading whitespace
```

> [!TIP]
> Always use the `std::ws` input manipulator with `std::getline(std::cin >> std::ws, str)`. It discards any leftover newline characters remaining in the input stream from previous `operator>>` extractions.

### String Length
To query the length of a string:
- `name.length()` / `name.size()`: Returns the number of characters as an **unsigned** `std::size_t`.
- `std::ssize(name)` (introduced in **C++20**): Returns the length as a **signed** integral type (`std::ptrdiff_t`), avoiding signed/unsigned conversion warnings in loops and indexing:

```cpp
std::string name{ "Alex" };
std::size_t len = name.length();            // Unsigned
auto signedLen  = std::ssize(name);         // Signed (C++20)
```

### Dynamic Memory and Performance
- `std::string` allocates heap memory dynamically to store characters exceeding small string optimization (SSO) limits.
- **Copying `std::string` is expensive**: Passing `std::string` by value creates a full deep copy of the string data.
- **Pass by `std::string_view` (or `const std::string&`)**: Always avoid passing `std::string` by value for read-only function parameters.
- **Returning by value is efficient**: Modern C++ uses **move semantics** and return value optimization (RVO), so returning a local `std::string` by value transfers ownership without deep-copying.

### String Literals as `std::string`
Standard double-quoted literals (`"hello"`) produce C-style string literals (`const char[N]`).
Using namespace `std::string_literals`, you can append the **`s`** suffix to create a `std::string` object literal:
```cpp
using namespace std::string_literals;

auto s1 = "hello";  // const char*
auto s2 = "hello"s; // std::string
```

### Constexpr Strings
- In C++17 and earlier, `constexpr std::string` variables were not supported.
- In **C++20**, `std::string` gained support for **transient dynamic allocations** within `constexpr` functions (memory allocated must be freed before compile-time evaluation finishes).
- However, non-empty `std::string` objects still cannot be persisted as `constexpr` variables across translation units. For compile-time string constants, always use **`std::string_view`**.

### Best Practices for `std::string`
- Prefer `std::string` over C-style strings for owning, mutating, or storing text data.
- Use `std::getline(std::cin >> std::ws, str)` for full-line input.
- Avoid passing `std::string` by value; use `std::string_view` for read-only parameters.
- Rely on move semantics when returning local `std::string` instances by value.
- Use `std::ssize()` in C++20 to avoid unsigned mismatch warnings.

### 📁 Code Examples for Section 3
- [`5_7_str_getline.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_7_str_getline.cpp): Demonstrates using `std::getline()` with `std::ws` to read full lines without skipping input.
- [`5_7_str_move_semantic.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_7_str_move_semantic.cpp): Demonstrates returning `std::string` by value leveraging compiler move semantics and RVO.
- [`5_7_str_size.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_7_str_size.cpp): Demonstrates string length querying using `.length()` vs `std::ssize()`.

---

## 4 — Strings: std::string_view

### The Problem with `std::string`
Initializing and copying `std::string` objects incurs expensive memory allocations and string copying:
```cpp
void printString(std::string str) { // Incurs a copy on every invocation!
    std::cout << str << '\n';
}

std::string s{ "Hello, world!" };
printString(s); // Creates a second copy of the entire string
```

### Introducing `std::string_view` (C++17)
Introduced in C++17 (header `<string_view>`), `std::string_view` provides a lightweight, non-owning view of a sequence of characters:
- Internally, it consists simply of a **pointer** to the first character and a **length**.
- Copying a `std::string_view` is extremely fast (just copying a pointer and an integer, typically 16 bytes on 64-bit platforms).
- It does not allocate memory on the heap.

```cpp
#include <iostream>
#include <string_view>

void printSV(std::string_view str) { // Fast, zero-copy pass by value!
    std::cout << str << '\n';
}
```

### Flexible Initialization
A `std::string_view` can view characters from diverse sources:
- C-style string literals: `std::string_view s1{ "Hello" };`
- `std::string` objects: `std::string_view s2{ myStdString };`
- Another `std::string_view`: `std::string_view s3{ s2 };`

Function parameters taking `std::string_view` seamlessly accept any of these sources without temporary copies:
```cpp
printSV("C-style literal"); // No std::string created
printSV(myStdString);       // No copy of myStdString
printSV(anotherView);       // Fast copy of view
```

### Conversion Between `std::string_view` and `std::string`
- `std::string` implicitly converts to `std::string_view`.
- However, `std::string_view` does **not** implicitly convert to `std::string` (because creating a `std::string` is an expensive allocation).
- To convert a `std::string_view` to a `std::string`, you must do so **explicitly**:
```cpp
std::string_view sv{ "Hello" };
std::string str{ sv }; // OK: explicit construction
// std::string str2 = sv; // COMPILE ERROR: implicit conversion disallowed!
```

### Reassignment
Assigning a new value to a `std::string_view` simply changes what it is viewing; it does not modify the underlying data:
```cpp
std::string name{ "Alex" };
std::string_view sv{ name };
sv = "John"; // sv now views "John". name is unchanged and still "Alex"!
```

### String View Literals (`"sv"`)
Using namespace `std::string_view_literals`, you can use the **`sv`** suffix to create string view literals directly:
```cpp
using namespace std::string_view_literals;

std::cout << "foo\n";   // const char*
std::cout << "goo\n"s;  // std::string
std::cout << "moo\n"sv; // std::string_view
```

### Constexpr `std::string_view`
Unlike `std::string`, `std::string_view` has full `constexpr` support across all C++ standards since C++17. It is the preferred way to define compile-time string constants:
```cpp
constexpr std::string_view greeting{ "Hello, world!" }; // Fully supported
```

### 📁 Code Examples for Section 4
- [`5_8_string_view-1.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_8_string_view-1.cpp): Demonstrates zero-copy read-only access with `std::string_view`.
- [`5_8_string_view-2.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_8_string_view-2.cpp): Demonstrates initializing `std::string_view` from C-style strings, `std::string`, and other views.
- [`5_8_string_view-3.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_8_string_view-3.cpp): Demonstrates explicit conversion from `std::string_view` to `std::string`.
- [`5_8_string_view-4.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_8_string_view-4.cpp): Demonstrates string literal suffixes (`""s` vs `""sv`).

---

## 5 — std::string vs std::string_view (Deep-Dive and Safety Rules)

### Owners and Viewers Analogy
- **`std::string` is an Owner**:
  - Owns its character data in memory.
  - Creates an independent copy of its initializer.
  - Automatically manages allocation and deallocation.
- **`std::string_view` is a Viewer**:
  - Does not own the memory it points to.
  - Observes an existing string without copying.
  - Is completely dependent on the lifetime of the underlying string.

> [!CAUTION]
> If the string being viewed is destroyed or modified, the `std::string_view` is left **dangling**. Accessing a dangling view produces **undefined behavior**!

### Improper Use Cases (Dangling Views)

#### 1. Viewing a Destroyed Local Variable
```cpp
std::string_view sv{};
{
    std::string s{ "Hello" };
    sv = s;
} // s is destroyed here!
// std::cout << sv; // UNDEFINED BEHAVIOR: sv is dangling!
```

#### 2. Viewing a Temporary `std::string` Return Value
```cpp
std::string getName() { return "Alex"; }

std::string_view sv{ getName() }; // The temporary string returned by getName() is destroyed at the end of this statement!
// std::cout << sv; // UNDEFINED BEHAVIOR: sv is dangling!
```

#### 3. Viewing a Temporary `std::string` Literal (`"s"`)
```cpp
using namespace std::string_literals;
std::string_view sv{ "Alex"s }; // "Alex"s creates a temporary std::string that is destroyed immediately!
// std::cout << sv; // UNDEFINED BEHAVIOR!
```

#### 4. Modifying the Underlying String
Modifying a `std::string` may trigger memory reallocation, moving characters to a new heap buffer and invalidating existing views:
```cpp
std::string s{ "Hello" };
std::string_view sv{ s };
s += " World and more characters that trigger reallocation";
// sv now points to deallocated memory!
```

#### Revalidating an Invalidated View
If a view becomes invalid because the underlying string was reallocated, you can re-validate it by reassigning:
```cpp
sv = s; // sv is valid again
```

### Returning `std::string_view`

#### Safe Return Cases
1. **Returning C-style string literals**: String literals have static storage duration and live for the entire lifetime of the program:
   ```cpp
   std::string_view getGreeting() {
       return "Hello, world!"; // Safe: string literal lives forever
   }
   ```
2. **Returning a `std::string_view` passed as a parameter**:
   ```cpp
   std::string_view firstHalf(std::string_view sv) {
       return sv.substr(0, sv.size() / 2); // Safe: references caller's data
   }
   ```

#### Unsafe Return Cases
Returning a view of a local `std::string` variable creates a dangling view immediately upon function exit:
```cpp
std::string_view badReturn() {
    std::string local{ "oops" };
    return local; // DANGER: local destroyed on return, caller receives dangling view!
}
```

### View Modification Functions
`std::string_view` allows you to shrink the viewing window without modifying the original string:
- `remove_prefix(n)`: Shrinks the view by moving the start pointer forward `n` characters.
- `remove_suffix(n)`: Shrinks the view by moving the end pointer backward `n` characters.

```cpp
std::string_view str{ "Peach" };
str.remove_prefix(1); // Views "each"
str.remove_suffix(2); // Views "ea"
str = "Peach";        // Reset view back to full string
```

### Substrings and Null-Termination
A `std::string_view` can represent any slice or substring of a string.

> [!WARNING]
> **CRITICAL WARNING**: Do NOT assume a `std::string_view` is null-terminated!
> - When `std::string_view` views a substring or uses `remove_suffix()`, there is no null terminator (`'\0'`) at the end of the viewed slice.
> - Calling `sv.data()` and passing the resulting pointer to functions expecting a null-terminated C string (`strlen`, `printf("%s")`, `std::atoi`, `fopen`) will read past the end of the view, causing **buffer overruns and crashes**!
> - If a null-terminated string is required, convert explicitly to `std::string` first:
>   ```cpp
>   std::string nullTerminated{ sv };
>   printf("%s\n", nullTerminated.c_str());
>   ```

### Quick Guide: When to Use `std::string` vs `std::string_view`

| Context | Use `std::string` | Use `std::string_view` |
|---|---|---|
| **Variables** | When you need to own, modify, or store user input | For read-only views or compile-time symbolic constants |
| **Function Parameters** | Rare: only when function needs to own or modify its own copy | **Default choice** for read-only string parameters |
| **Return Types** | When returning local strings or newly constructed strings | Only when returning string literals or forwarded view parameters |

### 📁 Code Examples for Section 5
- [`5_8_string_view_improper_use-1.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_8_string_view_improper_use-1.cpp): Demonstrates dangling view caused by local `std::string` going out of scope.
- [`5_8_string_view_improper_use-2.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_8_string_view_improper_use-2.cpp): Demonstrates dangling view caused by initializing with temporary return values.
- [`5_8_string_view_improper_use-3.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_8_string_view_improper_use-3.cpp): Demonstrates string reallocation invalidating an existing string view.
- [`5_8_string_view_modifications.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_8_string_view_modifications.cpp): Demonstrates shrinking views using `remove_prefix()` and `remove_suffix()`.
- [`5_8_string_view_non_null_termination.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_8_string_view_non_null_termination.cpp): Demonstrates non-null-terminated string views and the danger of passing `sv.data()` to C APIs.
- [`5_8_string_view_return-1.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_8_string_view_return-1.cpp): Demonstrates safely returning string view of static C-style string literals.
- [`5_8_string_view_return-2.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_8_string_view_return-2.cpp): Demonstrates safely returning a string view parameter.
- [`5_8_string_view_return-3.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_8_string_view_return-3.cpp): Demonstrates the danger of returning string views of local variables.
- [`5_8_string_view_revalidate.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/5_Constants_and_Strings/5_8_string_view_revalidate.cpp): Demonstrates revalidating an invalidated string view.
