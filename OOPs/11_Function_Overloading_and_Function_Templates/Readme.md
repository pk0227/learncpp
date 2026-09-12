# Chapter 11: Function Overloading and Function Templates

Function overloading and templates are the twin pillars of polymorphism and generic programming in modern C++. Function overloading enables multiple functions to share the same name with different parameter signatures, while function templates allow the compiler to automatically generate specialized functions across diverse types from a single blueprint.

---

## Table of Contents
1. [11.1 — Function Overload Differentiation](#111----function-overload-differentiation)
2. [11.2 — Function Overload Resolution and Ambiguous Matches](#112----function-overload-resolution-and-ambiguous-matches)
3. [11.3 — Default Arguments](#113----default-arguments)
4. [11.4 — Function Templates](#114----function-templates)
5. [11.5 — Function Templates with Multiple Template Types](#115----function-templates-with-multiple-template-types)
6. [11.6 — Non-type Template Parameters (NTTP)](#116----non-type-template-parameters-nttp)
7. [11.7 — Using Function Templates in Multiple Files](#117----using-function-templates-in-multiple-files)

---

## 11.1 — Function Overload Differentiation

**Function overloading** allows multiple functions to share the same name within the same scope, provided the compiler can unambiguously differentiate between them based on their signatures.

### Differentiating Factors for Overloaded Functions
The compiler can distinguish overloaded functions based on:
1. **Number of parameters**:
   ```cpp
   void print(int x);
   void print(int x, int y); // Differentiated by parameter count
   ```
2. **Type of parameters**:
   ```cpp
   void print(int x);
   void print(double x);     // Differentiated by parameter type
   void print(...);          // Ellipses differentiate overloads
   ```
   - **Excludes typedefs and type aliases**: Since `using Count = int;` is merely an alias for `int`, `void print(Count)` and `void print(int)` are identical declarations and cause a redefinition error.
   - **Excludes top-level `const` on pass-by-value parameters**: `void print(int)` and `void print(const int)` have identical parameter types because the argument is passed by value (copied).
3. **Function-level qualifiers (for member functions)**:
   - `const` and `volatile` qualifiers:
     ```cpp
     void show() const;
     void show();        // Overloaded based on const qualifier
     ```
   - **Ref-qualifiers** (`&` for lvalues, `&&` for rvalues):
     ```cpp
     void process() &;   // Called on lvalue objects
     void process() &&;  // Called on rvalue objects
     ```

### Non-Differentiating Factors
The following factors **cannot** be used to differentiate overloaded functions:
- **Return type**:
  ```cpp
  // int getRandomValue();
  // double getRandomValue(); // COMPILE ERROR: cannot overload functions differing solely by return type
  ```
- **Type aliases / typedefs**:
  ```cpp
  using Age = int;
  // void set(int a);
  // void set(Age a); // COMPILE ERROR: redefinition of void set(int)
  ```
- **Top-level `const` on value parameters**:
  ```cpp
  // void display(int x);
  // void display(const int x); // COMPILE ERROR: redefinition
  ```

> [!IMPORTANT]
> A function's return type is **NOT** a differentiating factor. Because callers do not always assign or use the return value (e.g. calling `getRandomValue();` as a standalone statement), the compiler has no way to determine which overload was intended based on return type alone.

### 📁 Code Examples for Section 11.1
- [`11_1_Function_overload_differentiation/1_no_of_params.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_1_Function_overload_differentiation/1_no_of_params.cpp): Demonstrates differentiating overloaded functions based on the number of parameters.
- [`11_1_Function_overload_differentiation/2_type_of_params.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_1_Function_overload_differentiation/2_type_of_params.cpp): Demonstrates differentiating overloaded functions based on parameter types.
- [`11_1_Function_overload_differentiation/3_non_differentiators.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_1_Function_overload_differentiation/3_non_differentiators.cpp): Demonstrates non-differentiating factors including return types, typedefs/type aliases, and top-level const on value parameters.

---

## 11.2 — Function Overload Resolution and Ambiguous Matches

When a call is made to an overloaded function, the compiler must determine which overloaded function is the best match. This process is called **overload resolution**.

### The Overload Resolution Procedure
When an overloaded function is called, the compiler steps through a structured sequence of rules. At each step, it applies specific type conversions to the arguments.
For each step, three outcomes are possible:
1. **No match**: The compiler moves to the next step.
2. **Single match**: The matching function is selected as the best match; resolution terminates successfully.
3. **Multiple matches**: The compiler halts immediately with an **ambiguous match** compile error.

```
[Step 1: Exact Match & Trivial Conversions]
   └── lvalue-to-rvalue, qualification (non-const -> const), reference conversions

[Step 2: Numeric Promotions]
   └── char/bool/short -> int, float -> double

[Step 3: Numeric Conversions]
   └── int -> double, double -> int, long -> int, etc.

[Step 4: User-Defined Conversions]
   └── Converting constructors, operator T() conversion functions

[Step 5: Ellipsis Match]
   └── Functions taking (...)

[Step 6: Resolution Failure]
   └── Compile Error: No matching function found
```

### The 6-Step Resolution Sequence

#### Step 1: Exact Match (and Trivial Conversions)
The compiler searches for an exact match. If no identical match exists, it tests **trivial conversions**:
- **lvalue-to-rvalue conversion**: An lvalue expression is converted to an rvalue (e.g. passing a variable to a parameter taken by value).
- **Qualification conversion**: A non-const argument is converted to a `const` reference parameter (e.g. `int` to `const int&`).
- **Non-reference to reference conversion**: Passing an argument by reference.

#### Step 2: Numeric Promotions
If no exact match is found, narrow integral and floating-point types are widened via numeric promotions:
- `char`, `bool`, `short` $\rightarrow$ `int` (or `unsigned int`).
- `float` $\rightarrow$ `double`.

#### Step 3: Numeric Conversions
If no promotion match is found, standard numeric conversions are attempted (e.g. converting `int` to `double`, `double` to `int`, `long` to `int`).

> [!NOTE]
> **Resolution Precedence Order**:
> $$\text{Exact Match} > \text{Promotion} > \text{Conversion}$$
> An overload requiring numeric promotion is always preferred over an overload requiring a numeric conversion.

#### Step 4: User-Defined Conversions
If standard numeric conversions fail, the compiler evaluates user-defined conversions:
- Converting constructors (e.g. `UD(int)`).
- User-defined conversion operators (e.g. `operator int()`).

#### Step 5: Ellipsis
If no user-defined conversion matches, the compiler checks for functions using ellipsis parameters (`...`).

#### Step 6: Compile Error
If no matches are found after all steps, a compilation error is generated.

### Ambiguous Matches
An **ambiguous match** occurs when the compiler finds two or more functions that match equally well at the same resolution stage:
```cpp
void print(int);
void print(double);

// print(1L); // COMPILE ERROR: long converts to both int and double via numeric conversion (equal rank)
```

> [!TIP]
> **Resolving Ambiguity**: Use `static_cast` or explicit literal suffixes (e.g. `0u`, `3.14f`) to guide the compiler directly to the desired overload:
> ```cpp
> print(static_cast<int>(1L)); // Unambiguously calls print(int)
> ```

### 📁 Code Examples for Section 11.2
- [`11_2_Function_overload_resolution_and_ambiguous_matches/1_trivial-conversion.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_2_Function_overload_resolution_and_ambiguous_matches/1_trivial-conversion.cpp): Demonstrates Step 1 of overload resolution: exact match and trivial conversions (non-const to const, lvalue to rvalue).
- [`11_2_Function_overload_resolution_and_ambiguous_matches/2_numeric_promotions.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_2_Function_overload_resolution_and_ambiguous_matches/2_numeric_promotions.cpp): Demonstrates Step 2 of overload resolution: numeric promotions of narrower types.
- [`11_2_Function_overload_resolution_and_ambiguous_matches/3_numeric_conversions.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_2_Function_overload_resolution_and_ambiguous_matches/3_numeric_conversions.cpp): Demonstrates Step 3 of overload resolution: numeric conversions applied when promotions fail.
- [`11_2_Function_overload_resolution_and_ambiguous_matches/4_precedence.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_2_Function_overload_resolution_and_ambiguous_matches/4_precedence.cpp): Demonstrates resolution precedence: Exact Match > Promotion > Conversion.
- [`11_2_Function_overload_resolution_and_ambiguous_matches/5_user-defined-conversion.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_2_Function_overload_resolution_and_ambiguous_matches/5_user-defined-conversion.cpp): Demonstrates Step 4 of overload resolution: matching functions via user-defined conversion operators.
- [`11_2_Function_overload_resolution_and_ambiguous_matches/6_ambiguity.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_2_Function_overload_resolution_and_ambiguous_matches/6_ambiguity.cpp): Demonstrates ambiguous match errors arising from equal-rank numeric conversions (`long` to `int` and `double`) and resolving via explicit casts.
- [`11_2_Function_overload_resolution_and_ambiguous_matches/7_ambiguity.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_2_Function_overload_resolution_and_ambiguous_matches/7_ambiguity.cpp): Demonstrates ambiguous matches between `unsigned int` and `float` overloads and disambiguation via literals/casts.

---

## 11.3 — Default Arguments

A **default argument** is an argument provided in a function declaration that is automatically passed by the compiler if the caller omits that argument.

```cpp
void print(int x, int y = 4); // y has default argument 4
print(6);    // Equivalent to print(6, 4)
print(6, 8); // Explicitly overrides default argument
```

### Key Rules of Default Arguments

#### 1. Multiple Default Arguments (The Right-to-Left Rule)
If a parameter has a default argument, all parameters to its right must also have default arguments:
```cpp
void print(int x = 10, int y = 20, int z = 30); // OK
// void print(int x = 10, int y, int z = 30);   // COMPILE ERROR: parameter without default cannot follow one with a default
```

#### 2. Declaration and Redeclaration Rules
- Default arguments cannot be redeclared in the same scope.
- They must be declared in either the forward declaration or the function definition, but **NOT** both:
  ```cpp
  void print(int x, int y = 4); // In header or forward declaration

  // void print(int x, int y = 4) { ... } // COMPILE ERROR: redefinition of default argument
  void print(int x, int y) { ... }       // OK: definition omits default argument
  ```

> [!TIP]
> **Best Practice**: Declare default arguments in the function's forward declaration in the header file (`.h`). This ensures that any file `#include`-ing the header sees the default arguments at the call site.

#### 3. Default Arguments and Overload Ambiguity
Default arguments can cause ambiguous overload resolution errors if omitted arguments allow multiple overloads to match:
```cpp
void print(int x = 100);
void print(double d = 123.45);

// print(); // COMPILE ERROR: ambiguous call! Both overloads match with 0 arguments.
```

> [!WARNING]
> Default arguments do **NOT** work for functions called through function pointers. Function pointers only encapsulate the address and signature of a function; default argument insertion occurs exclusively at compile-time call sites.

### 📁 Code Examples for Section 11.3
- [`11_3_Default_arguments/1_multiple_default_args.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_3_Default_arguments/1_multiple_default_args.cpp): Demonstrates the right-to-left rule for multiple default arguments.
- [`11_3_Default_arguments/2_multiple_default_args.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_3_Default_arguments/2_multiple_default_args.cpp): Demonstrates calling functions with omitted vs provided arguments when default values exist.
- [`11_3_Default_arguments/3_default_args_redeclaration.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_3_Default_arguments/3_default_args_redeclaration.cpp): Demonstrates the rule prohibiting redeclaration of default arguments in function definitions.
- [`11_3_Default_arguments/4_default_args_redeclaration.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_3_Default_arguments/4_default_args_redeclaration.cpp): Demonstrates that default arguments must be declared before the point of invocation in the translation unit.
- [`11_3_Default_arguments/5_default_args_redeclaration.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_3_Default_arguments/5_default_args_redeclaration.cpp): Demonstrates adding default arguments incrementally across multiple declarations in the same scope.
- [`11_3_Default_arguments/6_default_args_redeclaration_headers.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_3_Default_arguments/6_default_args_redeclaration_headers.cpp): Demonstrates best practice of placing default arguments in header declarations.
- [`11_3_Default_arguments/foo.h`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_3_Default_arguments/foo.h): Header declaring function `print(int x, int y = 4)` with default argument.
- [`11_3_Default_arguments/7_default_args_function_overloading.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_3_Default_arguments/7_default_args_function_overloading.cpp): Demonstrates combining function overloading with default arguments.
- [`11_3_Default_arguments/8_default_args_ambiguous_matches.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_3_Default_arguments/8_default_args_ambiguous_matches.cpp): Demonstrates ambiguous calls resulting from multiple overloaded functions having all-default parameters.
- [`11_3_Default_arguments/9_default_args_ambiguous_matches.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_3_Default_arguments/9_default_args_ambiguous_matches.cpp): Demonstrates ambiguity when an overload with default arguments overlaps with a single-parameter overload.

---

## 11.4 — Function Templates

A **function template** is a function-like blueprint that enables the compiler to automatically generate overloaded functions for specific types on demand.

```cpp
template <typename T>
T max(T x, T y)
{
    return (x < y) ? y : x;
}
```

### Terminology and Mechanics
- **Primary template**: The general template definition (`template <typename T> ...`).
- **Instantiated function (Specialization)**: The concrete function code generated by the compiler when the template is called with specific types (e.g. `max<int>(int, int)`).
- **Type template parameter**: Placeholder type (`typename T` or `class T`).

### Kinds of Template Parameters
1. **Type template parameters**: Represents any concrete type (`typename T`).
2. **Non-type template parameters (NTTP)**: Represents a compile-time constant expression (`int N`).
3. **Template template parameters**: Represents a template taking other templates.

### Function Template Instantiation
- **Implicit instantiation**: The compiler instantiates a specialization only when a call site requires it.
- **Single instantiation per type**: Within a single translation unit, a template is instantiated only once for any distinct type argument. Subsequent calls reuse the already-compiled specialization.
- **Implicitly inline**: Function template specializations are implicitly `inline`, meaning multiple translation units can instantiate the same function without triggering One Definition Rule (ODR) link errors.

### Template Argument Deduction
The compiler deduces the template type `T` from the types of arguments supplied:
```cpp
max(5, 6);     // Deduces T = int
max(2.5, 3.7); // Deduces T = double
```
> [!NOTE]
> If a non-template function and an instantiated template function are equally viable matches, standard overload resolution **prefers the non-template function**.

### Caveats and Edge Cases
1. **Instantiations may not compile**:
   ```cpp
   template <typename T>
   T addOne(T x) { return x + 1; }

   std::string s { "hello" };
   // addOne(s); // COMPILE ERROR: std::string + 1 is not defined!
   ```
2. **Semantics may be flawed**: Calling a template with raw pointers may compile (e.g. `max("apple", "banana")`), but compare memory addresses rather than string contents.
3. **Static local variables are NOT shared**:
   ```cpp
   template <typename T>
   void counter()
   {
       static int count { 0 };
       std::cout << ++count << '\n';
   }
   ```
   Each distinct specialization (`counter<int>()`, `counter<double>()`) has its own independent `count` variable!

### Deleted Function Specializations (`= delete`)
To forbid calling a function template with a specific type, specialize the template and mark it as deleted:
```cpp
template <>
const char* addOne(const char*) = delete;

// addOne("Hello"); // COMPILE ERROR: use of deleted function specialization
```

### 📁 Code Examples for Section 11.4
- [`11_4_Function_templates/1_function_template.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_4_Function_templates/1_function_template.cpp): Demonstrates defining and calling a basic function template with `typename T`.
- [`11_4_Function_templates/2_function_template_equivalent_insight.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_4_Function_templates/2_function_template_equivalent_insight.cpp): Demonstrates compiler code generation for instantiated template specializations.
- [`11_4_Function_templates/3_function_instatiated_only_once.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_4_Function_templates/3_function_instatiated_only_once.cpp): Demonstrates that each distinct type generates only one function specialization per translation unit.
- [`11_4_Function_templates/4_function_instatiated_only_once_equivalent_insight.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_4_Function_templates/4_function_instatiated_only_once_equivalent_insight.cpp): Demonstrates function pointer equality proving single instantiation per type.
- [`11_4_Function_templates/5_template_argument_deduction.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_4_Function_templates/5_template_argument_deduction.cpp): Demonstrates template argument deduction and the priority of non-template functions over templates.
- [`11_4_Function_templates/6_function_templates_with_non-template_parameters.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_4_Function_templates/6_function_templates_with_non-template_parameters.cpp): Demonstrates mixing template type parameters with non-template parameters.
- [`11_4_Function_templates/7_instantiated_functions_may_not_always_compile.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_4_Function_templates/7_instantiated_functions_may_not_always_compile.cpp): Demonstrates compile errors when templates are instantiated with types lacking required operators (e.g. `std::string` with `+ 1`).
- [`11_4_Function_templates/8_Instantiated_functions_may_not_always_make_sense_semantically.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_4_Function_templates/8_Instantiated_functions_may_not_always_make_sense_semantically.cpp): Demonstrates semantically incorrect instantiations (e.g. pointer addition).
- [`11_4_Function_templates/9_template_specialization_delete_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_4_Function_templates/9_template_specialization_delete_function.cpp): Demonstrates deleting specific template specializations using `= delete` to forbid unwanted types.
- [`11_4_Function_templates/10_function_templates_n_default_arguments_for_non-template_parameters.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_4_Function_templates/10_function_templates_n_default_arguments_for_non-template_parameters.cpp): Demonstrates default arguments on non-template parameters within function templates.
- [`11_4_Function_templates/11_function_templates_with_modifiable_static_local_variables.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_4_Function_templates/11_function_templates_with_modifiable_static_local_variables.cpp): Demonstrates that each instantiated specialization has an independent static local variable.

---

## 11.5 — Function Templates with Multiple Template Types

Template argument deduction does **NOT** apply type conversions. If a template has a single type parameter `T`:
```cpp
template <typename T>
T max(T x, T y);

// max(5, 6.7); // COMPILE ERROR: conflicting deduced types for T ('int' and 'double')
```

### Three Solutions to Type Deduction Conflicts
1. **Explicit conversion via `static_cast`**:
   ```cpp
   max(static_cast<double>(5), 6.7); // OK: both arguments are double
   ```
2. **Explicit template type specification**:
   ```cpp
   max<double>(5, 6.7); // OK: T is double; int 5 converted to double via numeric conversion
   ```
3. **Multiple template type parameters**:
   ```cpp
   template <typename T, typename U>
   auto max(T x, U y)
   {
       return (x < y) ? y : x;
   }
   ```

### Return Types with Multiple Template Types
- In **C++14**, use `auto` return type deduction:
  ```cpp
  template <typename T, typename U>
  auto max(T x, U y) { return (x < y) ? y : x; }
  ```
- In **C++11**, use trailing return types with `std::common_type_t`:
  ```cpp
  #include <type_traits>
  template <typename T, typename U>
  auto max(T x, U y) -> std::common_type_t<T, U> { return (x < y) ? y : x; }
  ```

### Abbreviated Function Templates (C++20)
C++20 introduced the ability to use `auto` in function parameter lists to define function templates succinctly:
```cpp
void print(auto x, auto y)
{
    std::cout << x + y << '\n';
}
```
This is direct syntactic sugar for:
```cpp
template <typename T, typename U>
void print(T x, U y) { ... }
```
> [!IMPORTANT]
> Each `auto` parameter represents an **independent** template parameter. To require that both parameters share the same type, use standard template syntax (`template <typename T> void print(T x, T y)`).

### Function Template Overloading and Partial Ordering
Function templates can be overloaded with other templates or non-template functions. The compiler applies **partial ordering of function templates** to select the most specialized template for a given call.

### 📁 Code Examples for Section 11.5
- [`11_5_Function_templates_with_multiple_template_types/1_multiple_template_types_need_n_workarounds.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_5_Function_templates_with_multiple_template_types/1_multiple_template_types_need_n_workarounds.cpp): Demonstrates template deduction conflict on mismatched argument types and workarounds (`static_cast` and explicit `<T>`).
- [`11_5_Function_templates_with_multiple_template_types/2_multiple_template_type_parameters.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_5_Function_templates_with_multiple_template_types/2_multiple_template_type_parameters.cpp): Demonstrates function templates with multiple template type parameters (`template <typename T, typename U>`).
- [`11_5_Function_templates_with_multiple_template_types/3_multiple_template_type_parameters_with_auto.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_5_Function_templates_with_multiple_template_types/3_multiple_template_type_parameters_with_auto.cpp): Demonstrates auto return type deduction for multiple template type parameters.
- [`11_5_Function_templates_with_multiple_template_types/4_issue_with_auto_type_deduction.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_5_Function_templates_with_multiple_template_types/4_issue_with_auto_type_deduction.cpp): Demonstrates pitfalls of auto return type deduction and resolving with `std::common_type_t`.
- [`11_5_Function_templates_with_multiple_template_types/5_abbreviated_function_templates.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_5_Function_templates_with_multiple_template_types/5_abbreviated_function_templates.cpp): Demonstrates C++20 abbreviated function templates (`auto` parameters).
- [`11_5_Function_templates_with_multiple_template_types/6_function_templates_overload.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_5_Function_templates_with_multiple_template_types/6_function_templates_overload.cpp): Demonstrates overloading function templates and partial ordering.

---

## 11.6 — Non-type Template Parameters (NTTP)

A **non-type template parameter (NTTP)** is a template parameter with a concrete type that acts as a placeholder for a compile-time constant expression passed as a template argument.

```cpp
template <int N>
void printNTimes(const char* msg)
{
    for (int i = 0; i < N; ++i)
        std::cout << msg << '\n';
}

printNTimes<3>("Hello"); // N is compile-time constant 3
```

### Valid Types for Non-Type Template Parameters
In modern C++, an NTTP can be:
1. An integral type (`int`, `size_t`, `char`)
2. An enumeration type
3. `std::nullptr_t`
4. A floating-point type (`float`, `double` — **since C++20**)
5. A pointer or reference to an object
6. A pointer or reference to a function
7. A pointer or reference to a member function
8. A literal class type (structural type with public constexpr members — **since C++20**)

### Common Uses of Non-Type Template Parameters
- Compile-time container sizing: `std::bitset<8>` or `std::array<int, 5>`.
- Static assertions and compile-time unrolling.

### Allowed Conversions for Non-Type Arguments
Only certain constexpr conversions are allowed for NTTP arguments:
- Integral promotions (e.g. `char` $\rightarrow$ `int`).
- Integral conversions (e.g. `char` $\rightarrow$ `long`).
- User-defined conversions to integral types.
- Lvalue-to-rvalue conversions.

> [!WARNING]
> Because conversions between integral types are permitted for non-type arguments, overloaded function templates differing only by NTTP type (e.g. `template <int N>` vs `template <char C>`) will generate ambiguous call errors when passed integer or character literals (`print<5>()`).

### Type Deduction for Non-Type Template Parameters (`auto` NTTP)
In **C++17**, non-type template parameters can use `auto` to deduce their type automatically:
```cpp
template <auto N>
void printValue()
{
    std::cout << N << '\n';
}

printValue<42>();  // Deduced as int 42
printValue<'A'>(); // Deduced as char 'A'
```

### 📁 Code Examples for Section 11.6
- [`11_6_Non-type_template_parameters/1_non-type_template_parameters.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_6_Non-type_template_parameters/1_non-type_template_parameters.cpp): Demonstrates non-type template parameters and `std::bitset`.
- [`11_6_Non-type_template_parameters/2_our_own_non-type_template_parameters.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_6_Non-type_template_parameters/2_our_own_non-type_template_parameters.cpp): Demonstrates writing custom function templates taking non-type template parameters.
- [`11_6_Non-type_template_parameters/3_non-type_template_parameters_use.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_6_Non-type_template_parameters/3_non-type_template_parameters_use.cpp): Demonstrates practical use of non-type template parameters for compile-time loops.
- [`11_6_Non-type_template_parameters/4_Implicit_conversions_for_non-type_template_arguments.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_6_Non-type_template_parameters/4_Implicit_conversions_for_non-type_template_arguments.cpp): Demonstrates permitted constexpr conversions for non-type template arguments.
- [`11_6_Non-type_template_parameters/5_Implicit_conversions_non-type_template_arguments_ambiguity.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_6_Non-type_template_parameters/5_Implicit_conversions_non-type_template_arguments_ambiguity.cpp): Demonstrates ambiguity arising from convertible non-type template arguments.
- [`11_6_Non-type_template_parameters/6_type_deduction_non-type_template_parameters.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_6_Non-type_template_parameters/6_type_deduction_non-type_template_parameters.cpp): Demonstrates C++17 `template <auto N>` type deduction for non-type template parameters.

---

## 11.7 — Using Function Templates in Multiple Files

In C++, regular function definitions cannot be placed in header files without the `inline` keyword, because `#include`-ing the header in multiple translation units produces duplicate symbol link errors under the **One Definition Rule (ODR)**.

### The Template ODR Exemption
- **Template definitions are exempt from the single-definition rule**: Identical template definitions can be `#include`-ed into multiple translation units without violating ODR.
- **Instantiated functions are implicitly inline**: Specializations generated by the compiler are implicitly marked `inline`, allowing multiple object files to contain identical specializations that are merged by the linker.

```cpp
// add.h
#ifndef ADD_H
#define ADD_H

template <typename T>
T add(T x, T y)
{
    return x + y;
}

#endif
```

> [!TIP]
> **Best Practice**:
> - Always define function templates completely inside **header files (`.h`)**.
> - `#include` the template header wherever the template is invoked.
> - Do not split template declarations into `.h` and definitions into `.cpp` files unless using explicit template instantiations, as the compiler requires the full template definition at the call site to perform instantiation.

### 📁 Code Examples for Section 11.7
- [`11_7_Using_function_templates_in_multiple_files/add.h`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_7_Using_function_templates_in_multiple_files/add.h): Header defining a function template.
- [`11_7_Using_function_templates_in_multiple_files/main.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_7_Using_function_templates_in_multiple_files/main.cpp): Demonstrates including and using a function template header.
- [`11_7_Using_function_templates_in_multiple_files/implicitly_inline/max.h`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_7_Using_function_templates_in_multiple_files/implicitly_inline/max.h): Header demonstrating implicitly inline template functions across multiple translation units.
- [`11_7_Using_function_templates_in_multiple_files/implicitly_inline/foo.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_7_Using_function_templates_in_multiple_files/implicitly_inline/foo.cpp): Source file instantiating template from `max.h`.
- [`11_7_Using_function_templates_in_multiple_files/implicitly_inline/main.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/11_Function_Overloading_and_Function_Templates/11_7_Using_function_templates_in_multiple_files/implicitly_inline/main.cpp): Main file instantiating same template from `max.h` and linking without ODR violations.
