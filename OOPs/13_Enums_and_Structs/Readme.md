# Chapter 13: Enums and Structs

Program-defined types—specifically enumerations and structures—allow C++ developers to encapsulate complex concepts and bundle related data into cohesive, type-safe abstractions. This chapter explores unscoped and scoped enumerations, aggregate initialization of structs, member padding, class templates, and Class Template Argument Deduction (CTAD).

---

## Table of Contents
1. [13.0 — Introduction to Program-Defined (User-Defined) Types](#130----introduction-to-program-defined-user-defined-types)
2. [13.1 — Unscoped Enumerations](#131----unscoped-enumerations)
3. [13.2 — Converting Enumerations to and from Strings](#132----converting-enumerations-to-and-from-strings)
4. [13.3 — Overloading the I/O Operators for Enumerations](#133----overloading-the-io-operators-for-enumerations)
5. [13.4 — Scoped Enumerations (Enum Classes)](#134----scoped-enumerations-enum-classes)
6. [13.5 — Structs and Aggregate Initialization](#135----structs-and-aggregate-initialization)
7. [13.6 — Default Member Initialization](#136----default-member-initialization)
8. [13.7 — Passing and Returning Structs](#137----passing-and-returning-structs)
9. [13.8 — Struct Miscellany and Member Selection](#138----struct-miscellany-and-member-selection)
10. [13.9 — Class Templates (Struct Templates)](#139----class-templates-struct-templates)
11. [13.10 — Class Template Argument Deduction (CTAD) and Deduction Guides](#1310----class-template-argument-deduction-ctad-and-deduction-guides)

---

## 13.0 — Introduction to Program-Defined (User-Defined) Types

Built-in primitive types (`int`, `double`, `char`, `bool`) represent single atomic quantities. To model real-world concepts, C++ provides two primary categories of compound types that allow programmers to create **program-defined types**:
1. **Enumerated types** (unscoped and scoped enumerations)
2. **Class types** (`struct`, `class`, and `union`)

### Core Rules for Program-Defined Types
- **Always terminate type definitions with a semicolon**:
  ```cpp
  struct Point { int x; int y; }; // Semicolon is mandatory!
  enum Color { red, green, blue }; // Semicolon is mandatory!
  ```
- **Header File Convention**: A program-defined type used across multiple translation units should be defined in a header file with the same name as the type (e.g. `Fraction.h`) and `#include`-ed where needed.
- **One Definition Rule (ODR) Exemption**: Type definitions are partially exempt from the single-definition ODR requirement: identical type definitions can appear in multiple source files without causing linker errors.

---

## 13.1 — Unscoped Enumerations

An **enumeration** (or **enum**) is a compound type whose values are restricted to a fixed set of named symbolic constants called **enumerators**.

```cpp
enum Color
{
    red,   // Assigned 0 by default
    green, // Assigned 1
    blue   // Assigned 2
};
```

### Key Properties of Unscoped Enumerations
- **Full definition required**: As program-defined types, enums must be fully defined before use.
- **Implicitly `constexpr`**: All enumerator values are compile-time constants.
- **Distinct types**: Each enum is a distinct type in the type system.
- **Pass by value**: Enums are small (integral-sized) and inexpensive to copy; they should be passed and returned by value.
- **Bit flag usage**: Often used to name bit positions when working with `std::bitset`.

### Scope and Namespace Pollution
In unscoped enumerations, the enumerators are injected directly into the **enclosing scope** where the enum is defined:
```cpp
enum Color { red, green, blue };
enum Feeling { happy, tired, blue }; // COMPILE ERROR: 'blue' collides with Color::blue!
```

> [!TIP]
> **Mitigating Collisions with Unscoped Enums**:
> 1. Enclose the unscoped enum inside a dedicated `namespace`:
>    ```cpp
>    namespace Colors { enum Color { red, green, blue }; }
>    ```
> 2. Define the enum inside the class that uses it:
>    ```cpp
>    struct Monster { enum Type { orc, goblin, troll }; };
>    ```

### Underlying Type and Conversions
- **Underlying type**: The integer type the compiler uses to represent the enumerators (defaults to `int`).
- **Explicit underlying type (C++11)**:
  ```cpp
  #include <cstdint>
  enum Color : std::int8_t { red, green, blue }; // Saves memory
  ```
- **Conversions**:
  - Unscoped enum $\rightarrow$ Integer: Occurs **implicitly**.
  - Integer $\rightarrow$ Unscoped enum: Disallowed implicitly; requires explicit `static_cast`:
    ```cpp
    Color c { static_cast<Color>(1) }; // OK: explicit cast
    ```

### 📁 Code Examples for Section 13.1
- [`13_1_Unscoped_enumerations/1_unscoped_enumerations.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_1_Unscoped_enumerations/1_unscoped_enumerations.cpp): Demonstrates defining and using unscoped enumerations and accessing enumerator values.
- [`13_1_Unscoped_enumerations/2_enums_as_parameters.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_1_Unscoped_enumerations/2_enums_as_parameters.cpp): Demonstrates passing and returning enumerations by value in functions.
- [`13_1_Unscoped_enumerations/3_bit_flags_enums.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_1_Unscoped_enumerations/3_bit_flags_enums.cpp): Demonstrates using enumerators to define symbolic bit positions for `std::bitset`.
- [`13_1_Unscoped_enumerations/4_scope_and_collisions.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_1_Unscoped_enumerations/4_scope_and_collisions.cpp): Demonstrates namespace pollution and naming collisions caused by unscoped enumerators in the enclosing scope.
- [`13_1_Unscoped_enumerations/5_enum_namespace_avoid_collisions.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_1_Unscoped_enumerations/5_enum_namespace_avoid_collisions.cpp): Demonstrates preventing naming collisions by enclosing unscoped enums in dedicated namespaces.
- [`13_1_Unscoped_enumerations/6_enum_implicit_conversion.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_1_Unscoped_enumerations/6_enum_implicit_conversion.cpp): Demonstrates implicit conversion of unscoped enums to integral types.
- [`13_1_Unscoped_enumerations/7_underlying_type_base.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_1_Unscoped_enumerations/7_underlying_type_base.cpp): Demonstrates specifying an explicit underlying base type (`std::int8_t`) for enumerations.
- [`13_1_Unscoped_enumerations/8_underlying_type_base_implicit_conversion.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_1_Unscoped_enumerations/8_underlying_type_base_implicit_conversion.cpp): Demonstrates requiring `static_cast` to convert an integer to an unscoped enumeration.

---

## 13.2 — Converting Enumerations to and from Strings

Because enumerators print as integer numbers by default, converting them to and from human-readable strings is essential for I/O.

### Enum to String (`getColorName`)
```cpp
#include <string_view>

std::string_view getColorName(Color c)
{
    switch (c)
    {
        case Color::red:   return "red";
        case Color::green: return "green";
        case Color::blue:  return "blue";
        default:           return "unknown";
    }
}
```

### String to Enum (`getColorMatch`)
Using `std::optional` to model potential parsing failure:
```cpp
#include <optional>
#include <string>

std::optional<Color> getColorFromString(const std::string& input)
{
    if (input == "red")   return Color::red;
    if (input == "green") return Color::green;
    if (input == "blue")  return Color::blue;
    return std::nullopt; // No match
}
```

### 📁 Code Examples for Section 13.2
- [`13_2_converting_enumeration_to_n_from_string/1_enum_to_string.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_2_converting_enumeration_to_n_from_string/1_enum_to_string.cpp): Demonstrates converting enumeration values to string representations using a switch statement.
- [`13_2_converting_enumeration_to_n_from_string/2_string_to_enum.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_2_converting_enumeration_to_n_from_string/2_string_to_enum.cpp): Demonstrates parsing string input to matching enumeration values using case-insensitive matching and `std::optional`.

---

## 13.3 — Overloading the I/O Operators for Enumerations

By overloading `operator<<` and `operator>>`, custom enumerations can be streamed natively with `std::cout` and `std::cin`.

```cpp
#include <iostream>

std::ostream& operator<<(std::ostream& out, Color c)
{
    out << getColorName(c);
    return out;
}

std::istream& operator>>(std::istream& in, Color& c)
{
    std::string s;
    in >> s;
    auto match { getColorFromString(s) };
    if (match)
        c = *match;
    else
        in.setstate(std::ios_base::failbit); // Set stream failure state
    return in;
}
```

### 📁 Code Examples for Section 13.3
- [`13_3_Overloading_the_I_O_operators/1_i_o_overloading_input_is_number.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_3_Overloading_the_I_O_operators/1_i_o_overloading_input_is_number.cpp): Demonstrates overloading `operator<<` and `operator>>` when enum input is provided as a number.
- [`13_3_Overloading_the_I_O_operators/1_i_o_overloading_input_is_string.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_3_Overloading_the_I_O_operators/1_i_o_overloading_input_is_string.cpp): Demonstrates overloading `operator<<` and `operator>>` when enum input is provided as a color string name with stream failure handling.

---

## 13.4 — Scoped Enumerations (Enum Classes)

Although unscoped enumerations are distinct types, they lack type safety because their implicit conversion to integer allows cross-type comparison (e.g. `Color::red == Fruit::banana`).

### The Scoped Enum Solution (`enum class`)
```cpp
enum class Color
{
    red,
    green,
    blue
};

enum class Fruit
{
    banana,
    apple
};
```

### Distinct Advantages of Scoped Enumerations
1. **Strong Type Safety**: No implicit conversion to integer or boolean:
   ```cpp
   Color c { Color::red };
   // int n = c;           // COMPILE ERROR: no implicit conversion
   // if (c == Fruit::banana) // COMPILE ERROR: cannot compare different enum types!
   ```
2. **Strict Scoping**: Enumerators are contained within the enum's scope and must be qualified: `Color::red`.
3. **Explicit Integer Conversion**:
   ```cpp
   int val { static_cast<int>(c) }; // Explicit cast required
   ```
4. **Direct-List-Initialization from Integer (C++17)**:
   ```cpp
   Color c { 1 }; // Valid in C++17 without static_cast
   ```
5. **`using enum` Statement (C++20)**:
   ```cpp
   using enum Color; // Injects red, green, blue into current block
   Color c { red };  // Unqualified access within block
   ```

> [!TIP]
> **Best Practice**: Always favor **scoped enumerations (`enum class`)** over unscoped enumerations unless interfacing with legacy C APIs.

### 📁 Code Examples for Section 13.4
- [`13_4_Scoped_enumerations_enum_classes/1_need_of_scoped_enums.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_4_Scoped_enumerations_enum_classes/1_need_of_scoped_enums.cpp): Demonstrates why scoped enums (`enum class`) are needed to prevent cross-type comparisons and namespace collisions.
- [`13_4_Scoped_enumerations_enum_classes/2_no_implicit_conversion.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_4_Scoped_enumerations_enum_classes/2_no_implicit_conversion.cpp): Demonstrates that scoped enums do not implicitly convert to integers and shows explicit conversion via `static_cast`.
- [`13_4_Scoped_enumerations_enum_classes/3_using_enum.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_4_Scoped_enumerations_enum_classes/3_using_enum.cpp): Demonstrates C++20 `using enum` statement importing enumerators into local scope.

---

## 13.5 — Structs and Aggregate Initialization

A **struct** is a compound type that bundles multiple member variables into a single unit.

### Aggregate Definition
A struct is an **aggregate** if it has:
- No user-declared constructors.
- No private or protected non-static data members.
- No virtual functions or virtual base classes.

### Aggregate Initialization
Aggregates are initialized using braced lists `{}`:
```cpp
struct Fraction
{
    int numerator { 0 };
    int denominator { 1 };
};

Fraction f1 { 3, 4 }; // numerator = 3, denominator = 4
Fraction f2 { 5 };    // numerator = 5, denominator uses default (1)
Fraction f3 {};       // Value-initialization: uses defaults (0 and 1)
```

### Designated Initializers (C++20)
In C++20, designated initializers allow specifying members explicitly by name:
```cpp
struct Point3D
{
    int x { 0 };
    int y { 0 };
    int z { 0 };
};

Point3D p { .x = 10, .z = 30 }; // y is value-initialized to 0
```

> [!WARNING]
> **Declaration Order Rule**: Designated initializers **MUST** match the declaration order of the struct members. Initializing out of order (`Point3D p { .z = 30, .x = 10 };`) produces a compilation error in C++!

### 📁 Code Examples for Section 13.5
- [`13_5_Struct_aggregate_initialization/1_initializer_as_aggregate.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_5_Struct_aggregate_initialization/1_initializer_as_aggregate.cpp): Demonstrates aggregate initialization of structs using braced initializer lists.
- [`13_5_Struct_aggregate_initialization/1_initializer_as_no_aggregate.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_5_Struct_aggregate_initialization/1_initializer_as_no_aggregate.cpp): Contrasts aggregate structs with non-aggregate classes having user-declared constructors.
- [`13_5_Struct_aggregate_initialization/2_const_class_types_struct.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_5_Struct_aggregate_initialization/2_const_class_types_struct.cpp): Demonstrates const struct variables and initialization requirements.
- [`13_5_Struct_aggregate_initialization/3_designated_initializer.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_5_Struct_aggregate_initialization/3_designated_initializer.cpp): Demonstrates C++20 designated initializers mapping values to named members.
- [`13_5_Struct_aggregate_initialization/4_assignment_with_initializer_list.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_5_Struct_aggregate_initialization/4_assignment_with_initializer_list.cpp): Demonstrates assigning new values to structs using braced initializer lists.
- [`13_5_Struct_aggregate_initialization/5_assignment_with_designated_initializers.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_5_Struct_aggregate_initialization/5_assignment_with_designated_initializers.cpp): Demonstrates assignment with designated initializers in C++20.
- [`13_5_Struct_aggregate_initialization/6_initializing_struct_with_another_struct_of_same_type.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_5_Struct_aggregate_initialization/6_initializing_struct_with_another_struct_of_same_type.cpp): Demonstrates copy-initialization of a struct from another struct of the same type.

---

## 13.6 — Default Member Initialization

Non-static members can be initialized with default values in their struct declaration:
```cpp
struct Employee
{
    int id { 0 };
    int age { 20 };
    double wage { 15.0 };
};
```

### Precedence Hierarchy of Member Initialization
```
[Evaluation Precedence]
   ├── 1. Explicit value provided in {}   --> Explicit value is used
   ├── 2. Omitted in {} but default exists --> Default member initializer used
   ├── 3. Omitted in {} and no default    --> Value-initialization (zeroed)
   └── 4. No braces at all (Employee e;)  --> Members without defaults remain UNINITIALIZED!
```

> [!TIP]
> Always provide default member initializers for all members, and prefer value-initialization (`Employee e{};`) over default initialization (`Employee e;`).

### 📁 Code Examples for Section 13.6
- [`13_6_Default_member_initialization/1_initialization_possibilities.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_6_Default_member_initialization/1_initialization_possibilities.cpp): Demonstrates all permutations of member initialization: explicit values, default initializers, value-initialization, and uninitialized members.

---

## 13.7 — Passing and Returning Structs

### Parameter Passing
- Structs can contain multiple members; passing them by value creates full copies of each member.
- **Rule of Thumb**: Pass structs by **`const T&`** to eliminate copying costs, unless the function requires a local, modifiable copy.

```cpp
void printEmployee(const Employee& e); // Efficient pass by const reference
```

### Passing Temporary Structs
Temporary structs can be constructed at the call site:
```cpp
printEmployee(Employee{ 1, 30, 45.0 }); // Direct temporary
printEmployee({ 1, 30, 45.0 });         // Implicit temporary from braced list
```

### Returning Structs and Structured Bindings (C++17)
Functions can return structs cleanly:
```cpp
Point3D getOrigin()
{
    return { 0, 0, 0 };
}

auto [x, y, z] = getOrigin(); // Unpacked via C++17 structured binding
```

### 📁 Code Examples for Section 13.7
- [`13_7_Passing_and_returning_structs/1_passing_temp_structs.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_7_Passing_and_returning_structs/1_passing_temp_structs.cpp): Demonstrates passing temporary struct instances directly into functions.
- [`13_7_Passing_and_returning_structs/2_returning_temp_struct.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_7_Passing_and_returning_structs/2_returning_temp_struct.cpp): Demonstrates returning temporary structs by value from functions and return type deduction.

---

## 13.8 — Struct Miscellany and Member Selection

### Member Selection: `.` vs. `->`
- Object / Reference: Use member selection operator (`.`): `emp.id`.
- Pointer: Use arrow operator (`->`): `empPtr->id` (shorthand for `(*empPtr).id`).

### Memory Alignment and Padding
The size of a struct is **NOT** simply the sum of its member sizes:
```cpp
struct Foo
{
    short a; // 2 bytes
             // 2 bytes padding
    int b;   // 4 bytes
    double c;// 8 bytes
};           // sizeof(Foo) == 16 bytes!
```
Compilers align data members to word boundaries matching their natural size.
> [!TIP]
> Declare struct members in **descending order of size** (largest first) to minimize padding bytes and cache footprint.

### 📁 Code Examples for Section 13.8
- [`13_8_Struct_miscellany/1_miscellany.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_8_Struct_miscellany/1_miscellany.cpp): Demonstrates member selection through pointers using operator `->`.
- [`13_8_Struct_miscellany/2_miscellany.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_8_Struct_miscellany/2_miscellany.cpp): Demonstrates struct size, alignment requirements, and compiler padding bytes.
- [`13_8_Struct_miscellany/3_miscellany.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_8_Struct_miscellany/3_miscellany.cpp): Demonstrates nested struct definitions and accessing nested members.

---

## 13.9 — Class Templates (Struct Templates)

Class templates allow defining generic structs and classes parameterized by types.

```cpp
template <typename T, typename U>
struct Pair
{
    T first;
    U second;
};

Pair<int, double> p1 { 1, 2.5 };
```

### Standard Library `std::pair`
The standard library provides `std::pair` in `<utility>`, which models a generic two-element class template.

### 📁 Code Examples for Section 13.9
- [`13_9_Class_templates/1_aggregate_types_need_templates.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_9_Class_templates/1_aggregate_types_need_templates.cpp): Demonstrates why aggregate types need class templates to avoid code duplication across types.
- [`13_9_Class_templates/2_templates_on_aggregate.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_9_Class_templates/2_templates_on_aggregate.cpp): Demonstrates defining and instantiating generic struct templates with `template <typename T>`.
- [`13_9_Class_templates/3_templates_on_aggregate.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_9_Class_templates/3_templates_on_aggregate.cpp): Demonstrates struct templates mixing template parameters with non-template member variables.
- [`13_9_Class_templates/4_template_on_std_pair.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_9_Class_templates/4_template_on_std_pair.cpp): Demonstrates using `std::pair` from `<utility>` as a standard class template.
- [`13_9_Class_templates/4_templates_on_more_than_one_aggregate.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_9_Class_templates/4_templates_on_more_than_one_aggregate.cpp): Demonstrates writing function templates that operate on multiple distinct class template types.

---

## 13.10 — Class Template Argument Deduction (CTAD) and Deduction Guides

Starting in **C++17**, the compiler can deduce class template arguments from the types of initializer arguments:
```cpp
std::pair p { 1, 2.5 }; // Deduced as std::pair<int, double>
```

### Deduction Guides
In C++17, aggregate class templates required explicit user-defined **deduction guides**:
```cpp
template <typename T, typename U>
struct Pair { T first; U second; };

// C++17 Deduction Guide for aggregate Pair:
template <typename T, typename U>
Pair(T, U) -> Pair<T, U>;
```
In **C++20**, the compiler generates deduction guides for aggregates automatically!

### CTAD Restrictions
- **Cannot be used for non-static members**:
  ```cpp
  struct Foo
  {
      // std::pair p { 1, 2 }; // COMPILE ERROR: CTAD not allowed for non-static members
      std::pair<int, int> p { 1, 2 }; // OK
  };
  ```
- **Cannot be used in function parameters**:
  ```cpp
  // void print(std::pair p); // COMPILE ERROR: must use auto or explicit template types
  ```

### 📁 Code Examples for Section 13.10
- [`13_10_Class_template_argument_deduction_CTAD/1_CTAD_with_suffix.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_10_Class_template_argument_deduction_CTAD/1_CTAD_with_suffix.cpp): Demonstrates Class Template Argument Deduction (CTAD) with literal suffixes in C++17.
- [`13_10_Class_template_argument_deduction_CTAD/2_CTAD_deduction_guide.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_10_Class_template_argument_deduction_CTAD/2_CTAD_deduction_guide.cpp): Demonstrates authoring user-defined deduction guides for aggregate class templates in C++17.
- [`13_10_Class_template_argument_deduction_CTAD/3_CTAD_deduction_guide.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_10_Class_template_argument_deduction_CTAD/3_CTAD_deduction_guide.cpp): More examples of deduction guides with multiple template type parameters.
- [`13_10_Class_template_argument_deduction_CTAD/4_CTAD_template_default_parameters.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_10_Class_template_argument_deduction_CTAD/4_CTAD_template_default_parameters.cpp): Demonstrates combining default template type parameters with CTAD.
- [`13_10_Class_template_argument_deduction_CTAD/5_CTAD_with_non_static_members.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_10_Class_template_argument_deduction_CTAD/5_CTAD_with_non_static_members.cpp): Demonstrates the restriction disallowing CTAD for non-static member initialization in class definitions.
- [`13_10_Class_template_argument_deduction_CTAD/6_CTAD_with_function_parameters.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/13_Enums_and_Structs/13_10_Class_template_argument_deduction_CTAD/6_CTAD_with_function_parameters.cpp): Demonstrates the rule prohibiting CTAD in function parameter declarations.
