# Chapter 10: Type Conversion, Type Aliases, and Type Deduction

The process of converting data from one type to another type is called **type conversion**.

The value of an object is stored as a sequence of bits, and the data type tells the compiler how to interpret those bits into meaningful values. Different data types represent the "same" value differently (for example, the integer value `3` might be stored in memory as binary `0000 0000 0000 0000 0000 0000 0000 0011`, whereas floating-point value `3.0` might be stored as IEEE-754 binary `0100 0000 0100 0000 0000 0000 0000 0000`). Copying raw bits directly between mismatched types via `std::memcpy` or pointer reinterpretation yields corrupted or meaningless values. Proper type conversion produces a temporary object of the target type that represents the intended semantic value.

---

## Table of Contents
1. [10.0 — Introduction to Type Conversion and Implicit Type Conversion](#100----introduction-to-type-conversion-and-implicit-type-conversion)
2. [10.1 — Numeric Promotions](#101----numeric-promotions)
3. [10.2 — Numeric Conversions](#102----numeric-conversions)
4. [10.3 — Narrowing Conversions, List Initialization, and Constexpr Initializers](#103----narrowing-conversions-list-initialization-and-constexpr-initializers)
5. [10.4 — Arithmetic Conversions (Usual Arithmetic Conversions)](#104----arithmetic-conversions-usual-arithmetic-conversions)
6. [10.5 — Explicit Type Conversion and Casting](#105----explicit-type-conversion-and-casting)
7. [10.6 — Type Aliases](#106----type-aliases)
8. [10.7 — Type Deduction for Variables and Functions (auto)](#107----type-deduction-for-variables-and-functions-auto)

---

## 10.0 — Introduction to Type Conversion and Implicit Type Conversion

**Implicit type conversion** (also called **automatic type conversion** or **coercion**) is performed automatically by the compiler whenever one data type is required by the language syntax, but a value of a different data type is supplied.

### Key Fundamental Principles of Type Conversion
1. **Conversions do not change the data being converted**: The conversion process takes the original data as read-only input, performs calculation/transformation, and produces a new converted result.
2. **Conversions produce a temporary object**: When converting a value to another type, the conversion creates a temporary object of the target type initialized with the converted value.

```cpp
double d { 3 }; // int literal 3 is converted to double 3.0 via a temporary object
```

### When Implicit Type Conversion Occurs
Implicit type conversion is ubiquitous in C++ and occurs in at least six distinct programming contexts:

1. **Variable initialization**: When the type of the initializer expression differs from the declared type of the variable:
   ```cpp
   double d { 3 }; // int value 3 converted to type double
   ```
2. **Variable assignment**: When the assigned value differs from the variable's declared type:
   ```cpp
   d = 6; // int value 6 converted to type double
   ```
3. **Function arguments**: When an argument passed to a function differs from the parameter type:
   ```cpp
   void printDouble(double val);
   printDouble(5); // int argument 5 converted to double parameter 5.0
   ```
4. **Function return statements**: When the expression in a `return` statement differs from the function's return type:
   ```cpp
   float getPi() { return 3.14159; } // double literal converted to float
   ```
5. **Conditional expressions**: When a non-boolean expression is supplied to an `if`, `while`, or `for` condition:
   ```cpp
   if (5) { /* int value 5 converted to bool true */ }
   ```
6. **Binary arithmetic operators**: When operands of binary operators (`+`, `-`, `*`, `/`) have mismatched types:
   ```cpp
   double div { 4.0 / 3 }; // int 3 converted to double 3.0 before division
   ```

Numeric type conversions are broadly partitioned into two distinct categories:
- **Numeric Promotion**: Safe, hardware-friendly, always value-preserving.
- **Numeric Conversion**: Potentially unsafe, wider range of conversions where data or precision may be lost.

### 📁 Code Examples for Section 10.0
- [`1_understanding_conversion_n_need.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/1_understanding_conversion_n_need.cpp): Demonstrates how data types dictate bit interpretation and why raw bit copying via `std::memcpy` produces garbage values compared to proper type conversion.
- [`2_when_implicit_conversion_happens.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/2_when_implicit_conversion_happens.cpp): Demonstrates the 6 common contexts where implicit type conversion occurs (initialization, assignment, return values, function arguments, conditionals, and expressions).

---

## 10.1 — Numeric Promotions

**Numeric promotion** is the type conversion of certain narrower numeric types (such as `char` or `short`) to certain wider numeric types (typically `int` or `double`) that can be processed efficiently by CPU hardware registers.

### Why Numeric Promotions Exist
Modern CPU architectures are designed to perform arithmetic calculations and register manipulations most efficiently on their native register width (typically 32-bit or 64-bit words). Processing narrower types (such as 8-bit `char` or 16-bit `short`) often requires additional masking, sign-extension, or microcode emulation instructions. To ensure maximum execution speed, the C++ standard mandates numeric promotions so that narrow types are widened before arithmetic operations are carried out.

### Numeric Promotion Categories
1. **Integral promotions**
2. **Floating-point promotions**

#### Integral Promotions
- Types `bool`, `char`, `signed char`, `unsigned char`, `signed short`, and `unsigned short` all get promoted to `int`.
- If `int` cannot hold the entire range of the source type (e.g. `unsigned short` on systems where `int` and `short` have the same 16-bit width), those values get promoted to `unsigned int`.
- Types `wchar_t`, `char8_t`, `char16_t`, and `char32_t` also undergo integral promotion to `int`, `unsigned int`, `long`, `unsigned long`, `long long`, or `unsigned long long` (whichever first accommodates their full range).

```cpp
char c { 'A' };
// 'A' (ASCII 65) is promoted from char to int before passing to function or operator
```

#### Floating-Point Promotions
- A value of type `float` can be promoted to a value of type `double`.

> [!NOTE]
> While integral promotion is strictly **value-preserving**, it does not necessarily preserve the **signedness** of the type. For example, an `unsigned short` whose values fit inside a signed `int` is promoted to a *signed* `int`.

> [!IMPORTANT]
> Some widening conversions (such as `char` to `short`, or `int` to `long`) are **NOT** considered numeric promotions. They are classified as **numeric conversions** because they do not promote directly to `int` or `double`.

### 📁 Code Examples for Section 10.1
- [`10_1_Numeric_Promotions/3_floating_point_promotion.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_1_Numeric_Promotions/3_floating_point_promotion.cpp): Demonstrates floating-point promotion from `float` to `double` in function calls and expressions.
- [`10_1_Numeric_Promotions/4_Integral_promotions.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_1_Numeric_Promotions/4_Integral_promotions.cpp): Demonstrates integral promotion of narrower types (`char`, `bool`, `short`) to `int` or `unsigned int`.

---

## 10.2 — Numeric Conversions

**Numeric conversions** encompass all numeric type conversions that do not qualify as numeric promotions. Unlike promotions, numeric conversions are not guaranteed to be safe or value-preserving.

### Five Categories of Numeric Conversions
1. Converting an integral type to any other integral type (excluding integral promotions, e.g. `int` to `long`, or `int` to `short`).
2. Converting a floating-point type to any other floating-point type (excluding floating-point promotions, e.g. `double` to `float`, or `double` to `long double`).
3. Converting a floating-point type to any integral type (e.g. `double` to `int`).
4. Converting an integral type to any floating-point type (e.g. `int` to `double`).
5. Converting an integral or floating-point type to a `bool` (e.g. `int` to `bool`, `double` to `bool`).

### Safety Classifications of Numeric Conversions

| Classification | Safety | Description & Consequences | Example |
|---|---|---|---|
| **Value-preserving** | Safe | Target type can represent every value of source type without loss. | `short` $\rightarrow$ `long`, `int` $\rightarrow$ `double` (on 32-bit int) |
| **Reinterpretive** | Unsafe | Bit pattern is preserved, but semantic interpretation changes (signed vs unsigned). Modulo wrap-around occurs. | `int` $\rightarrow$ `unsigned int` (`-5` becomes `4294967291u`) |
| **Lossy** | Unsafe | Target type cannot represent source value; data or precision is discarded. | `double` $\rightarrow$ `int` (truncates fraction), `double` $\rightarrow$ `float` (rounds) |

> [!WARNING]
> Reinterpretive signed/unsigned conversions alter negative values drastically due to two's-complement wrap-around arithmetic:
> ```cpp
> int n { -5 };
> unsigned int u { n }; // u becomes 4294967291 on 32-bit platforms!
> ```

### 📁 Code Examples for Section 10.2
- [`10_2_Numeric_Conversions/5_Numeric_conversions.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_2_Numeric_Conversions/5_Numeric_conversions.cpp): Demonstrates the fundamental categories of numeric conversions across integral and floating-point types.
- [`10_2_Numeric_Conversions/6_Value_preserving_conversions.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_2_Numeric_Conversions/6_Value_preserving_conversions.cpp): Demonstrates safe, value-preserving numeric conversions where the target type can represent all values of the source type.
- [`10_2_Numeric_Conversions/7_Reinterpretive_conversions.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_2_Numeric_Conversions/7_Reinterpretive_conversions.cpp): Demonstrates reinterpretive signed/unsigned conversions where bit patterns are preserved but semantics change, causing modulo wrap-around.
- [`10_2_Numeric_Conversions/8_Lossy_conversions.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_2_Numeric_Conversions/8_Lossy_conversions.cpp): Demonstrates lossy conversions resulting in truncation of fractional parts (`double` to `int`) or loss of precision (`double` to `float`).
- [`10_2_Numeric_Conversions/9_unsafe_misc.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_2_Numeric_Conversions/9_unsafe_misc.cpp): Demonstrates miscellaneous unsafe conversions and compiler warning behaviors.

---

## 10.3 — Narrowing Conversions, List Initialization, and Constexpr Initializers

A **narrowing conversion** is a potentially unsafe numeric conversion where the destination type may not be capable of holding all potential values of the source type.

### Defined Narrowing Conversions

| Conversion Type | Risk Description | Allowed with `constexpr`? | Example |
|---|---|---|---|
| **Int $\leftarrow$ Float** | Fractional part is lost (truncated toward zero). | ❌ Never allowed | `int x { 3.14 }; // Error` |
| **Float $\leftarrow$ Double** | Precision loss or overflow to infinity. | ✅ Allowed if value fits exactly | `float f { 3.0 }; // OK` |
| **Float $\leftarrow$ Int** | Very large integers may exceed significand precision and round. | ✅ Allowed if exactly representable | `float f { 42 }; // OK` |
| **Smaller Int / Sign Change $\leftarrow$ Int** | Integer overflow or sign reinterpretation. | ✅ Allowed if value fits exactly | `unsigned char c { 100 }; // OK` |

### List Initialization (`{}`) Disallows Narrowing Conversions
One of the most important safety guarantees introduced in C++11 list-initialization (brace initialization) is the strict prohibition of narrowing conversions:

```cpp
// 1. Compile-time constant (constexpr) values:
unsigned char c1 { 100 }; // OK: 100 is constexpr and fits in [0, 255]
// unsigned char c2 { 300 }; // COMPILE ERROR: 300 does not fit in unsigned char

// 2. Runtime variables:
int n { 100 };
// unsigned char c3 { n }; // COMPILE ERROR: runtime int to unsigned char is narrowing
```

> [!TIP]
> **Best Practice**: Avoid narrowing conversions whenever possible. If an intentional narrowing conversion is required, use `static_cast` to make your intent explicit and eliminate compiler warnings.

### 📁 Code Examples for Section 10.3
- [`10_3_Narrowing_conversions_list_initialization_constexpr_initializers/1_float_to_int.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_3_Narrowing_conversions_list_initialization_constexpr_initializers/1_float_to_int.cpp): Demonstrates narrowing conversion prevention when converting floating-point to integral types under list-initialization.
- [`10_3_Narrowing_conversions_list_initialization_constexpr_initializers/2_double_to_float.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_3_Narrowing_conversions_list_initialization_constexpr_initializers/2_double_to_float.cpp): Demonstrates precision loss when converting `double` to `float` and compiler diagnostic behavior.
- [`10_3_Narrowing_conversions_list_initialization_constexpr_initializers/3_int_to_float.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_3_Narrowing_conversions_list_initialization_constexpr_initializers/3_int_to_float.cpp): Demonstrates potential precision loss when converting large `int` values to `float` and resolving it via `static_cast`.
- [`10_3_Narrowing_conversions_list_initialization_constexpr_initializers/4_int_to_int.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_3_Narrowing_conversions_list_initialization_constexpr_initializers/4_int_to_int.cpp): Demonstrates narrowing checks between wider and narrower integer types, contrasting compile-time constexpr checks with runtime values.

---

## 10.4 — Arithmetic Conversions (Usual Arithmetic Conversions)

In C++, binary operators (such as `+`, `-`, `*`, `/`, `%`, `<`, `>`, `==`) require both operands to have the **exact same type**. If operands of different types are supplied, C++ applies a standardized set of conversion rules called the **Usual Arithmetic Conversions (UAC)** to find a common type.

### The UAC Priority Rules Hierarchy

```
[Step 1: Floating-Point Check]
   ├── If either operand is long double  --> convert other to long double
   ├── Else if either operand is double  --> convert other to double
   └── Else if either operand is float   --> convert other to float

[Step 2: Integral Promotions]
   └── Apply integral promotions to both operands (e.g. char, short -> int)

[Step 3: Signed vs. Unsigned Resolution]
   ├── Same signedness                   --> narrower type converts to wider type
   ├── Unsigned rank >= Signed rank      --> signed operand converted to unsigned!
   ├── Signed can hold all unsigned vals --> unsigned converted to signed
   └── Otherwise                         --> both converted to unsigned version of signed type
```

> [!WARNING]
> **The Classic Signed vs. Unsigned Comparison Trap**:
> When comparing a signed integer with an unsigned integer of equal rank, the signed operand is converted to unsigned:
> ```cpp
> std::cout << std::boolalpha << (-3 < 5u) << '\n'; // Prints: false!
> ```
> Why? Because `-3` is converted to `unsigned int`, wrapping around to `4294967293u`, which is far greater than `5u`!
> Similarly, `5u - 10` converts `-10` to unsigned, yielding `4294967291u`.

### Inspecting Resulting Types with `typeid`
The `typeid` operator (from `<typeinfo>`) allows runtime inspection of an expression's type name. On compilers such as GCC and Clang, the output of `typeid.name()` is mangled (e.g. `'j'` for `unsigned int`, `'d'` for `double`); use `abi::__cxa_demangle` from `<cxxabi.h>` to retrieve the clean human-readable type.

```cpp
#include <iostream>
#include <typeinfo>

int i { 2 };
double d { 3.5 };
std::cout << typeid(i + d).name() << '\n'; // 'd' (double)
```

### 📁 Code Examples for Section 10.4
- [`10_4_Arithmetic_conversions/1_typeid_example.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_4_Arithmetic_conversions/1_typeid_example.cpp): Demonstrates Usual Arithmetic Conversions in binary expressions, the signed/unsigned comparison hazard (`-3 < 5u`), and inspecting types using `<typeinfo>` and ABI demangling.

---

## 10.5 — Explicit Type Conversion and Casting

**Explicit type conversion** (casting) occurs when the programmer intentionally requests a conversion using an explicit casting operator.

C++ supports 5 distinct types of casts:
1. `static_cast`
2. `dynamic_cast`
3. `const_cast`
4. `reinterpret_cast`
5. C-style casts (legacy C syntax)

The first four are referred to as **named casts**.

### Comparison of Cast Types

| Cast | Description | Safety Level |
|---|---|---|
| `static_cast` | Performs compile-time checked conversions between related types (e.g. numeric, base/derived). | Safe (compile-time checked) |
| `dynamic_cast` | Performs runtime-checked type conversions on polymorphic pointers or references. | Safe (runtime validation) |
| `const_cast` | Adds or strips `const` or `volatile` qualifiers. | Unsafe (UB if modifying originally const data) |
| `reinterpret_cast` | Reinterprets the raw bit pattern of an object as an unrelated type. | Unsafe (implementation-dependent) |
| C-style cast | Tries a combination of `static_cast`, `const_cast`, and `reinterpret_cast`. | Highly Unsafe (avoids diagnostics) |

### Why C-Style Casts Must Be Avoided
Syntax: `(double)x` or `double(x)` (function-style cast).
- **Ambiguous intent**: The compiler will silently fall back from a `static_cast` to a `const_cast` or `reinterpret_cast` without warning.
- **Difficult to search**: Finding `(int)` in a codebase is prone to thousands of false positives; named casts like `static_cast<int>` are effortlessly grep-able.
- **Access control bypass**: A C-style cast can cast a derived pointer to an inaccessible (privately inherited) base class, bypassing C++ encapsulation rules.

### `static_cast` Mechanics
Syntax: `static_cast<new_type>(expression)`
- Evaluates at compile time and produces a temporary object of the target type.
- Performs compile-time type validation; if types are unrelated, compilation fails.
- Prevents accidental removal of `const` or unsafe pointer bit-reinterpretation.
- Invokes explicit constructors of target class types during initialization.

```cpp
int x { 10 };
int y { 4 };
double result { static_cast<double>(x) / y }; // x converted to double 10.0; result is 2.5
```

### Casting vs. Initializing a Temporary Object (`static_cast<T>(x)` vs. `T{ x }`)
Three critical differences distinguish `static_cast<T>(x)` from direct-list-initialized temporaries `T{ x }`:

1. **Narrowing conversions**:
   - `T{ x }` uses list-initialization, which disallows narrowing conversions. If the conversion risks data loss, it causes a compile-time error.
   - `static_cast<T>(x)` tells the compiler the conversion is intentional, permitting deliberate narrowing.
2. **Intent and searchability**:
   - `static_cast` clearly documents a type conversion in code reviews and static analysis tools.
3. **Simple type specifiers limitation**:
   - Direct-list-initialization of a temporary only allows single-word type names (simple type specifiers). `int{ x }` is valid, but `unsigned int{ x }` is a syntax error! To initialize a multi-word temporary, one must use `static_cast<unsigned int>(x)` or create a type alias.

```cpp
// unsigned int u1 { unsigned int{ 5 } }; // SYNTAX ERROR: multi-word type not allowed here
unsigned int u2 { static_cast<unsigned int>(5) }; // OK
```

> [!TIP]
> Prefer `static_cast` over initializing temporary objects when a type conversion is desired.

### 📁 Code Examples for Section 10.5
- [`10_5_Explicit_type_conversion/1_c_and_static_casting.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_5_Explicit_type_conversion/1_c_and_static_casting.cpp): Demonstrates the perils of C-style casting compared to type-safe `static_cast`.
- [`10_5_Explicit_type_conversion/2_casting_vs_initializing_temp_obj.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_5_Explicit_type_conversion/2_casting_vs_initializing_temp_obj.cpp): Compares `static_cast<T>(x)` with temporary object list-initialization `T{x}`, demonstrating narrowing differences.
- [`10_5_Explicit_type_conversion/3_simple_type_specifiers.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_5_Explicit_type_conversion/3_simple_type_specifiers.cpp): Demonstrates the simple type specifier limitation where multi-word types cannot be list-initialized as temporary objects without a type alias.

---

## 10.6 — Type Aliases

A **type alias** is a user-defined identifier that serves as a synonym for an existing type. Type aliases do not create new, distinct types—they are alternate names for existing types.

### Declaring Type Aliases: `typedef` vs. `using`

```cpp
// 1. Legacy typedef syntax (C-style):
typedef double distance_t;
typedef int (*ActionPtr)(double, char); // Alias name buried in the middle

// 2. Modern C++11 alias declaration (using):
using distance_t = double;
using ActionPtr = int (*)(double, char); // Alias name cleanly on the left
```

### Why the `using` Syntax Is Strongly Preferred
- **Readability**: Follows the intuitive `alias = target;` assignment pattern.
- **Function pointers and complex types**: The alias name is clearly separated from parameter lists and return types.
- **Template alias support**: `using` declarations can be templated (alias templates); `typedef` cannot:
  ```cpp
  template <typename T>
  using StringMap = std::map<std::string, T>;
  ```

### Scope and Strong-Type Caveat
- Type aliases follow standard C++ scoping rules (local inside functions, class scope inside structs/classes, namespace scope).
- **Aliases do not provide type safety**:
  ```cpp
  using Miles = double;
  using Kilometers = double;
  Miles m { 10.0 };
  Kilometers k { m }; // Compiles with NO warning or error! Miles and Kilometers are identical to double.
  ```

### 📁 Code Examples for Section 10.6
- [`10_6_Type_aliases_and_type_deduction/1_type_aliases.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_6_Type_aliases_and_type_deduction/1_type_aliases.cpp): Demonstrates defining type aliases using modern `using` syntax versus legacy `typedef` for complex types and function pointers.

---

## 10.7 — Type Deduction for Variables and Functions (auto)

**Type deduction** (or **type inference**) is a compile-time feature where the compiler automatically deduces the type of an object, expression, or function return from its initializer or return statement.

### Type Deduction for Objects
```cpp
auto d { 5.0 }; // deduced as double
auto sum { add(5, 6) }; // deduced as return type of add()
```

Rules and Restrictions:
- **Initializer is mandatory**:
  ```cpp
  // auto a;     // COMPILE ERROR: cannot deduce type without initializer
  // auto b { }; // COMPILE ERROR: cannot deduce type from empty braces
  ```
- **Literal suffixes can guide deduction**:
  ```cpp
  auto a { 1.23f }; // float
  auto b { 5u };    // unsigned int
  auto c { 10LL };  // long long
  ```

### Dropping Modifiers (`const` and References)
- By default, `auto` deduces value types: it drops top-level `const` qualifiers:
  ```cpp
  const int x { 5 };
  auto y { x };       // y is int (const dropped)
  const auto z { x }; // z is const int (const explicitly reapplied)
  ```
- `constexpr` variables are implicitly `const`, and this `const` is dropped by `auto` unless explicitly requested.

### Type Deduction for String Literals
- C-style string literals deduce to `const char*`, not `std::string`:
  ```cpp
  auto s1 { "Hello" }; // type is const char*
  ```
- To deduce `std::string` or `std::string_view`, include `<string>` or `<string_view>` and use the user-defined literal suffixes `s` or `sv` from namespace `std::literals`:
  ```cpp
  using namespace std::literals;
  auto s2 { "Hello"s };  // std::string
  auto s3 { "Hello"sv }; // std::string_view
  ```

### Type Deduction for Functions (C++14)
In C++14, a function's return type can be deduced using `auto`:
```cpp
auto add(int x, int y)
{
    return x + y; // deduced as int
}
```

> [!CAUTION]
> Function return type deduction rules:
> 1. All `return` statements in the function must resolve to the exact same type; otherwise, the compiler generates a deduction conflict error.
> 2. Functions using `auto` return type must be fully defined before any call site (a forward declaration `auto foo();` cannot be used to call the function).

### Trailing Return Type Syntax (C++11)
C++11 introduced trailing return types, where the return type is specified after the parameter list following an arrow `->`:
```cpp
auto add(int x, int y) -> int
{
    return x + y;
}
```
Trailing return syntax is especially valuable when:
- The return type depends on template parameters via `decltype`:
  ```cpp
  template <typename T, typename U>
  auto multiply(T t, U u) -> decltype(t * u) { return t * u; }
  ```
- Simplifying declarations of lambdas and complex member functions.

### Function Parameter Type Deduction (C++20 Abbreviated Function Templates)
Prior to C++20, using `auto` as a function parameter was illegal in normal functions.
In C++20, `auto` parameters are valid:
```cpp
void print(auto x)
{
    std::cout << x << '\n';
}
```
> [!IMPORTANT]
> In function parameter lists, `auto` does **NOT** perform standard type deduction. Instead, it triggers an **abbreviated function template**, serving as direct shorthand for:
> ```cpp
> template <typename T>
> void print(T x) { ... }
> ```

### 📁 Code Examples for Section 10.7
- [`10_6_Type_aliases_and_type_deduction/2_type_deduction.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_6_Type_aliases_and_type_deduction/2_type_deduction.cpp): Demonstrates type deduction with `auto`, const dropping, literal suffixes (`f`, `u`), and string literal deductions (`s`, `sv`).
- [`10_6_Type_aliases_and_type_deduction/3_type_deduction_on_functions.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_6_Type_aliases_and_type_deduction/3_type_deduction_on_functions.cpp): Demonstrates function return type deduction (`auto`), return consistency requirements, and forward declaration restrictions.
- [`10_6_Type_aliases_and_type_deduction/4_type_deduction_on_functions.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_6_Type_aliases_and_type_deduction/4_type_deduction_on_functions.cpp): Demonstrates C++20 `auto` function parameters and how they trigger abbreviated function templates.
- [`10_6_Type_aliases_and_type_deduction/5_trailing_return_type.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/10_TypeConversion_TypeAliases_TypeDeduction/10_6_Type_aliases_and_type_deduction/5_trailing_return_type.cpp): Demonstrates C++11 trailing return type syntax (`auto func() -> ReturnType`) and expression-dependent return types with `decltype`.
