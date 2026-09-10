# Chapter 21: Operator Overloading

Operator overloading is a feature of C++ that allows user-defined types (classes and structs) to work seamlessly with C++ operators (such as `+`, `-`, `*`, `<<`, `[]`, and `()`). By designing custom operator implementations, user-defined classes can behave with the same natural syntax and expressiveness as built-in primitive types.

---

### Table of Contents
1. [1 — Introduction to Operator Overloading](#1--introduction-to-operator-overloading)
2. [2 — Overloading the Arithmetic Operators Using Friend Functions](#2--overloading-the-arithmetic-operators-using-friend-functions)
3. [3 — Overloading Operators Using Normal Functions](#3--overloading-operators-using-normal-functions)
4. [4 — Overloading the I/O Operators](#4--overloading-the-io-operators)
5. [5 — Overloading Operators Using Member Functions](#5--overloading-operators-using-member-functions)
6. [6 — Overloading Unary Operators +, -, and !](#6--overloading-unary-operators---and-)
7. [7 — Overloading the Comparison Operators](#7--overloading-the-comparison-operators)
8. [8 — Overloading the Increment and Decrement Operators](#8--overloading-the-increment-and-decrement-operators)
9. [9 — Overloading the Subscript Operator](#9--overloading-the-subscript-operator)
10. [10 — Overloading the Parenthesis Operator](#10--overloading-the-parenthesis-operator)
11. [11 — Overloading Typecasts](#11--overloading-typecasts)
12. [12 — Overloading the Assignment Operator](#12--overloading-the-assignment-operator)
13. [13 — Shallow vs. Deep Copying](#13--shallow-vs-deep-copying)
14. [14 — Overloading Operators and Function Templates](#14--overloading-operators-and-function-templates)

---

## 1 — Introduction to Operator Overloading

In C++, operators are implemented under the hood as functions. When an operator expression such as `x + y` is encountered, the compiler evaluates it as a function call—either a member function (`x.operator+(y)`) or a non-member function (`operator+(x, y)`).

You can define your own versions of the operators that work with different data types (including classes that you have written). Using function overloading to overload operators is called **operator overloading**.

### Limitations on Operator Overloading

While C++ provides immense flexibility when overloading operators, several strict language rules apply:

1. **Operators that cannot be overloaded**: Almost any existing operator in C++ can be overloaded. The specific exceptions are:
   - Conditional / ternary operator (`?:`)
   - `sizeof`
   - Scope resolution operator (`::`)
   - Member selector / dot operator (`.`)
   - Pointer-to-member selector (`.*`)
   - `typeid`
   - C++ casting operators (`static_cast`, `dynamic_cast`, `const_cast`, `reinterpret_cast`)
   - `decltype`

2. **Only existing operators can be overloaded**: You cannot create new operators or rename existing ones. For example, you cannot create an `operator**` to perform exponentiation.

3. **At least one user-defined operand**: At least one of the operands in an overloaded operator must be a user-defined type (a `class`, `struct`, or `enum`).
   - This means you could overload `operator+(int, MyString)`, but you **cannot** overload `operator+(int, double)`.
   - An overloaded operator should operate on at least one program-defined type (either as a parameter of the function, or as the implicit object).

4. **Arity cannot be altered**: It is not possible to change the number of operands an operator supports. A binary operator (e.g. `+`, `-`, `*`) must take exactly two operands; a unary operator (e.g. `!`, unary `-`) must take one. (The sole exception to variable parameters is `operator()`, and starting in C++23, `operator[]`).

5. **Precedence and associativity are fixed**: All operators retain their default precedence level and associativity regardless of what they are used for. This cannot be modified.

```cpp
// Example: Why overloading operator^ for exponentiation fails in practice
// In mathematics: 4 + 3 ^ 2 resolves as 4 + (3 ^ 2) => 4 + 9 = 13.
// In C++: operator+ has higher precedence than bitwise operator^!
// Therefore: 4 + 3 ^ 2 evaluates as (4 + 3) ^ 2 => 7 ^ 2 => 49.
// Callers would have to write 4 + (3 ^ 2) manually every single time!
```

> [!WARNING]
> When overloading operators, it is best to keep the semantics of the operators as close to the natural, conventional mathematical or language intent as possible.
> If the meaning of an overloaded operator is not immediately obvious, intuitive, and unambiguous, **use a named member function instead** (e.g., `.power()`, `.dotProduct()`).

### Return Value Guidelines

- **Operators that do not modify their operands** (e.g., arithmetic operators `+`, `-`, `*`, `/`) should generally **return results by value**.
- **Operators that modify their leftmost operand** (e.g., prefix `++`, `--`, and any assignment operators like `=`, `+=`, `-=`) should generally **return the leftmost operand by reference** to allow chaining.

---

## 2 — Overloading the Arithmetic Operators Using Friend Functions

All basic arithmetic operators (`+`, `-`, `*`, `/`) are **binary operators**, meaning they take two operands—one on each side of the operator: `x + y`. All four of these arithmetic operators are overloaded using identical design patterns.

There are three different ways to overload operators in C++:
1. **The friend function approach**
2. **The normal (non-member, non-friend) function approach**
3. **The member function approach**

### Overloading Arithmetic Operators with Friend Functions

A friend function has access to the private members of the class, while remaining a non-member function.

```cpp
class Cents
{
    int m_cents{};
public:
    explicit Cents(int cents = 0) : m_cents{cents} {}

    // Friend declaration inside the class
    friend Cents operator+(const Cents& c1, const Cents& c2);
};

// Definition outside the class
Cents operator+(const Cents& c1, const Cents& c2)
{
    return Cents{c1.m_cents + c2.m_cents}; // Direct access to private m_cents
}
```

#### Defining Friend Functions Inside the Class

Even though friend functions are not member functions of the class, they can be defined directly inside the class body if desired:

```cpp
class Cents
{
    int m_cents{};
public:
    explicit Cents(int cents = 0) : m_cents{cents} {}

    // Defined inside class body, yet behaves as a non-member friend function!
    friend Cents operator+(const Cents& c1, const Cents& c2)
    {
        return Cents{c1.m_cents + c2.m_cents};
    }
};
```

### Overloading Operators for Operands of Different Types

When defining arithmetic operations between a class and a primitive type (or between two different classes), order matters. Binary operators evaluate their left operand as the first parameter and right operand as the second parameter:

```cpp
// Allows: Cents + int (e.g., c1 + 5)
Cents operator+(const Cents& c, int val)
{
    return Cents{c.m_cents + val};
}

// Allows: int + Cents (e.g., 5 + c1) - implemented using the first overload!
Cents operator+(int val, const Cents& c)
{
    return c + val; // Reuses operator+(const Cents&, int)
}
```

> [!TIP]
> **Implementing operators using other operators**: It is often possible and recommended to implement overloaded operators in terms of other overloaded operators (such as implementing `int + Cents` in terms of `Cents + int`, or `!=` in terms of `!(==)`). Do so whenever it reduces boilerplate and keeps logic centralized.

### 📁 Code Examples for Section 2
- [`21_1_Overloading_arithmetic_operators_with_friend_functions/1_operator_overload_friend_function_outside_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_1_Overloading_arithmetic_operators_with_friend_functions/1_operator_overload_friend_function_outside_class.cpp): Demonstrates declaring an arithmetic friend `operator+` inside class `Cents` and providing its definition outside the class.
- [`21_1_Overloading_arithmetic_operators_with_friend_functions/2_operator_overload_friend_function_inside_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_1_Overloading_arithmetic_operators_with_friend_functions/2_operator_overload_friend_function_inside_class.cpp): Demonstrates inline definition of friend `operator+` directly within the class declaration.
- [`21_1_Overloading_arithmetic_operators_with_friend_functions/3_minmax_example.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_1_Overloading_arithmetic_operators_with_friend_functions/3_minmax_example.cpp): Demonstrates overloading `operator+` between multiple `MinMax` objects and combining them with primitive values.

---

## 3 — Overloading Operators Using Normal Functions

In general, **a normal (non-member, non-friend) function should be preferred over a friend function** if it is possible to implement the operator using the existing public member functions (accessors/getters).

> [!NOTE]
> **Core Principle**: *The fewer functions touching your class's private internals, the better.* Non-member non-friend functions maximize encapsulation and reduce coupling.

However, **do not add additional public access functions solely to avoid making an operator a friend function**. If the required getters do not naturally belong in the class's public interface, make the operator a friend instead.

```cpp
// Cents.hpp
class Cents
{
    int m_cents{};
public:
    explicit Cents(int cents = 0) : m_cents{cents} {}
    int getCents() const { return m_cents; } // Already exists in public API
};

// Normal non-member function (no 'friend' keyword needed)
Cents operator+(const Cents& c1, const Cents& c2)
{
    return Cents{c1.getCents() + c2.getCents()};
}
```

### 📁 Code Examples for Section 3
- [`21_2_Overloading_arithmetic_operators_with_normal_functions/Cents.hpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_2_Overloading_arithmetic_operators_with_normal_functions/Cents.hpp): Header declaring class `Cents` and non-member, non-friend `operator+` and `operator-` prototypes.
- [`21_2_Overloading_arithmetic_operators_with_normal_functions/Cents.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_2_Overloading_arithmetic_operators_with_normal_functions/Cents.cpp): Definitions of `operator+` and `operator-` implemented purely using `getCents()`.
- [`21_2_Overloading_arithmetic_operators_with_normal_functions/1_operator_overload_member_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_2_Overloading_arithmetic_operators_with_normal_functions/1_operator_overload_member_function.cpp): Driver program demonstrating execution and chaining of the normal-function arithmetic operators.

---

## 4 — Overloading the I/O Operators

Input and output stream operations in C++ utilize the insertion (`<<`) and extraction (`>>`) operators.

### Overloading `operator<<` (Insertion)

Overloading `operator<<` allows custom types to be printed directly via output streams like `std::cout`:

```cpp
std::ostream& operator<<(std::ostream& out, const Point& point)
{
    out << "Point(" << point.m_x << ", " << point.m_y << ", " << point.m_z << ')';
    return out; // Must return reference to stream to allow chaining
}
```

> [!IMPORTANT]
> - `operator<<` must take `std::ostream&` as its first parameter by reference, and return `std::ostream&` by reference.
> - If you attempt to return `std::ostream` by value, you will get a **compiler error** because `std::ostream`'s copy constructor is deleted (`= delete`).
> - Returning the stream by reference allows **chaining**: `std::cout << p1 << " and " << p2 << '\n';`. Each `<<` call returns `std::cout`, which becomes the left operand of the next `<<`.

### Overloading `operator>>` (Extraction)

Overloading `operator>>` allows reading user input directly into class objects:

```cpp
std::istream& operator>>(std::istream& in, Point& point)
{
    double x{}, y{}, z{};
    in >> x >> y >> z;

    // Check for semantically invalid input
    if (x < 0.0 || y < 0.0 || z < 0.0)
    {
        in.setstate(std::ios_base::failbit); // Manually signal stream failure
    }

    // Transactional assignment: assign only if extraction succeeded
    point = in ? Point{x, y, z} : Point{};

    return in;
}
```

#### Transactional Stream Operations

If an input stream enters failure mode (`failbit`), all subsequent extractions will abort immediately. To keep object state consistent and prevent partial or corrupted object updates, I/O operations should be **transactional**:

| Strategy | Description | Typical Use Case |
|---|---|---|
| **Alter on success** | Store results in temporary variables. If all extractions succeed, update the object; otherwise leave it clean. | Small objects, simple classes, standard numeric inputs |
| **Restore on failure** | Make a backup copy of the object before modifying. If any step fails, restore from the backup copy. | Objects with complex internal state |
| **Rollback on failure** | If any sub-operation fails, reverse prior modifications using inverse operations. | Databases, massive collections where full backup copying is prohibitive |

#### Handling Semantically Invalid Input

- When `std::cin` fails syntactically (e.g., trying to read the character `'a'` into an `int`), `std::cin` automatically enters failure mode (`std::ios_base::failbit`).
- However, if the user inputs syntactically valid numbers that are **semantically invalid** (e.g., entering negative numbers where only positive numbers are valid), `std::cin` will not enter failure mode automatically.
- To handle this, explicitly check the extracted values and call `in.setstate(std::ios_base::failbit);` when semantically invalid input is detected.

### 📁 Code Examples for Section 4
- [`21_3_Overloading_I_O_operators/insertion_extraction_operator_overload.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_3_Overloading_I_O_operators/insertion_extraction_operator_overload.cpp): Demonstrates overloading `operator<<` and `operator>>` for `Point`, implementing stream chaining, transactional assignment, and `failbit` error signaling.

---

## 5 — Overloading Operators Using Member Functions

When overloading an operator as a **member function**:
- The overloaded operator is defined as a member function of the class of the **left operand**.
- The left operand becomes the implicit `*this` object.
- All other operands become explicit function parameters (a binary operator takes 1 parameter; a unary operator takes 0 parameters).

```cpp
class Cents
{
    int m_cents{};
public:
    explicit Cents(int cents = 0) : m_cents{cents} {}

    // Binary operator overloaded as member function: takes 1 parameter
    Cents operator+(const Cents& right) const
    {
        return Cents{m_cents + right.m_cents}; // m_cents is from implicit *this
    }
};
```

### When Can/Can't We Use Member vs. Friend vs. Normal Functions?

#### 1. Not Everything Can Be Overloaded as a Non-Member (Friend/Normal)
The C++ standard mandates that the following four operators **must** be overloaded as member functions:
- Assignment operator (`=`)
- Subscript operator (`[]`)
- Function call operator (`()`)
- Member selection / arrow operator (`->`)

#### 2. Not Everything Can Be Overloaded as a Member Function
- `operator<<` and `operator>>` **cannot** be overloaded as member functions of your class, because their left operand is `std::ostream` or `std::istream`, which are standard library classes you cannot alter.
- Any binary operator where the left operand is not a class you own (e.g. `int + Cents`, where `int` is a primitive type) cannot be a member function.

### Summary Rule of Thumb: Choosing Operator Form

| Operator Category | Recommended Form | Rationale |
|---|---|---|
| `=`, `[]`, `()`, `->` | **Member function** | Mandated by the C++ language standard. |
| Unary operators (`+`, `-`, `!`, `*`, `++`, `--`) | **Member function** | Operates solely on the implicit `*this` object. |
| Binary operators that **modify** left operand (e.g. `+=`, `-=`, `*=`) | **Member function** | Left operand is modified directly; naturally acts on `*this`. |
| Binary operators that **do not modify** left operand (e.g. `+`, `-`, `*`) | **Normal function** (preferred) or **Friend** | Enables symmetric implicit conversions on both left and right operands. |
| Binary operators modifying left operand where left class cannot be altered (`<<`, `>>`) | **Normal function** (preferred) or **Friend** | Left operand (`std::ostream`) cannot have new members injected. |

### 📁 Code Examples for Section 5
- [`21_4_Overloading_arithmetic_operators_with_member_functions/1_operator_overload_friend_function_outside_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_4_Overloading_arithmetic_operators_with_member_functions/1_operator_overload_friend_function_outside_class.cpp): Demonstrates binary arithmetic operators `operator+` and `operator-` implemented as class member functions using the implicit `*this` object.

---

## 6 — Overloading Unary Operators +, -, and !

The unary positive (`+`), unary negative (`-`), and logical NOT (`!`) operators take only a single operand. Because they operate solely on the object they are invoked upon, unary operator overloads are almost always implemented as **member functions**.

Because the single operand is the implicit `*this` object, unary operator member functions **take no parameters**:

```cpp
class Point
{
    double m_x{}, m_y{}, m_z{};
public:
    Point(double x = 0.0, double y = 0.0, double z = 0.0)
        : m_x{x}, m_y{y}, m_z{z} {}

    // Unary negative operator
    Point operator-() const
    {
        return Point{-m_x, -m_y, -m_z};
    }

    // Logical NOT operator: returns true if Point is at origin (0, 0, 0)
    bool operator!() const
    {
        return (m_x == 0.0 && m_y == 0.0 && m_z == 0.0);
    }
};
```

> [!TIP]
> Always mark unary operators like `operator-` and `operator!` as `const`, because evaluating `-p` or `!p` should never modify the internal state of `p`.

### 📁 Code Examples for Section 6
- [`21_5_Overloading_unary_operators_+_-_!/1_unary_operator.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_5_Overloading_unary_operators_+_-_!/1_unary_operator.cpp): Demonstrates overloading unary negation (`operator-`) and logical NOT (`operator!`) as const member functions on a 3D `Point` class.

---

## 7 — Overloading the Comparison Operators

Because comparison operators (`==`, `!=`, `<`, `>`, `<=`, `>=`) are binary operators that do not modify their left operands, they are typically implemented as **friend functions** or **normal non-member functions**.

```cpp
class Fraction
{
    int m_numerator{};
    int m_denominator{1};
public:
    Fraction(int num = 0, int den = 1) : m_numerator{num}, m_denominator{den} {}

    friend bool operator==(const Fraction& f1, const Fraction& f2)
    {
        return (f1.m_numerator == f2.m_numerator && f1.m_denominator == f2.m_denominator);
    }

    friend bool operator!=(const Fraction& f1, const Fraction& f2)
    {
        return !(f1 == f2); // Reuse operator==
    }

    friend bool operator<(const Fraction& f1, const Fraction& f2)
    {
        return (f1.m_numerator * f2.m_denominator < f2.m_numerator * f1.m_denominator);
    }

    friend bool operator>(const Fraction& f1, const Fraction& f2)  { return f2 < f1; }
    friend bool operator<=(const Fraction& f1, const Fraction& f2) { return !(f1 > f2); }
    friend bool operator>=(const Fraction& f1, const Fraction& f2) { return !(f1 < f2); }
};
```

### Modern C++20 Comparison: The Three-Way Comparison Operator (`<=>`)

In **C++20**, the three-way comparison operator (`<=>`), commonly known as the **spaceship operator**, was introduced:
- Defining or defaulting `operator<=>` (`= default;`) allows the compiler to automatically synthesize all four relational comparisons (`<`, `<=`, `>`, `>=`).
- Defining or defaulting `operator==` (`= default;`) automatically generates `operator!=`.

```cpp
#include <compare>

struct Item
{
    int id{};
    double price{};

    // In C++20: compiler automatically generates ==, !=, <, <=, >, >=
    auto operator<=>(const Item&) const = default;
};
```

### 📁 Code Examples for Section 7
- [`21_6_Overloading_comparison_operators/1_comparision_operators.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_6_Overloading_comparison_operators/1_comparision_operators.cpp): Demonstrates implementing comparison operators (`==`, `!=`, `<`, `>`, `<=`, `>=`) using friend functions and cascading operator logic.

---

## 8 — Overloading the Increment and Decrement Operators

There are two forms of the increment and decrement operators:
1. **Prefix** (e.g. `++x`, `--y`)
2. **Postfix** (e.g. `x++`, `y--`)

Because both forms are unary operators that modify their operand, they are best overloaded as **member functions**.

### Differentiating Prefix vs. Postfix via the Dummy `int` Parameter

Both prefix and postfix overloads share the exact same function name (`operator++` or `operator--`). To differentiate between them:
- **Prefix increment**: takes **no parameters** (`operator++()`).
- **Postfix increment**: takes a single **dummy `int` parameter** (`operator++(int)`).

The integer parameter in the postfix overload is purely a dummy token used by the compiler to select the postfix signature; it is given no parameter name and never used in the function implementation.

```cpp
class Digit
{
    int m_digit{};
public:
    explicit Digit(int digit = 0) : m_digit{digit} {}

    // Prefix increment: ++d
    // Increments value and returns *this by reference
    Digit& operator++()
    {
        if (m_digit == 9)
            m_digit = 0;
        else
            ++m_digit;

        return *this;
    }

    // Postfix increment: d++
    // Saves old state, increments *this, and returns the old state by value
    Digit operator++(int)
    {
        Digit temp{*this}; // Save previous state
        ++(*this);         // Reuse prefix increment operator!
        return temp;       // Return prior state by value
    }
};
```

### Key Differences Between Prefix and Postfix Overloads

| Characteristic | Prefix (`++x`) | Postfix (`x++`) |
|---|---|---|
| **Parameter list** | No parameters: `operator++()` | Dummy `int` parameter: `operator++(int)` |
| **Return type** | Reference: `Digit&` | Value: `Digit` (cannot return local temporary by reference!) |
| **Return value** | Updated object (`*this`) | Snapshot of object before incrementation |
| **Efficiency** | **More efficient**; no temporary copy created | **Less efficient**; requires creating and copying a temporary object |

> [!TIP]
> Always prefer **prefix increment** (`++it`, `++i`) over postfix increment unless you specifically require the un-incremented value in the expression, because prefix avoids creating and discarding temporary objects.

### 📁 Code Examples for Section 8
- [`21_7_Overloading_increment_n_decrement_operators/1_increment_decrement_operator.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_7_Overloading_increment_n_decrement_operators/1_increment_decrement_operator.cpp): Demonstrates implementation of prefix and postfix increment and decrement operators on a `Digit` class, showing return-by-reference chaining and temporary value returns.

---

## 9 — Overloading the Subscript Operator

The subscript operator (`operator[]`) allows custom classes (such as custom arrays or matrices) to be indexed using square bracket syntax (`list[index]`).

- `operator[]` **must be overloaded as a member function**.
- An overloaded `operator[]` function traditionally takes one parameter: the subscript placed between the square brackets.
- Although you could technically supply default arguments in the parameter list, invoking `arr[]` without an expression between brackets is invalid C++ syntax.
- `operator[]` should return a **reference** (`T&`) so that the resulting expression can be used as an lvalue on the left side of an assignment (`list[2] = 5;`).

### Overloaded `operator[]` for Const Objects

To ensure compatibility with both mutable and immutable objects, provide **both non-const and const overloads**:

```cpp
class IntList
{
    int m_list[10]{};
public:
    // Non-const overload: returns modifiable reference for non-const objects
    int& operator[](int index)
    {
        return m_list[index];
    }

    // Const overload: returns const reference (or copy) for const objects
    const int& operator[](int index) const
    {
        return m_list[index];
    }
};
```

### Pointers to Objects and `operator[]`

> [!WARNING]
> **Pointers to objects and overloaded `operator[]` do not mix cleanly!**
> If you have a pointer to an object `IntList* ptr = &list;`, writing `ptr[2]` will **not** invoke `IntList::operator[]`!
> Instead, C++ treats `ptr[2]` as pointer arithmetic: `*(ptr + 2)`, which attempts to access the third `IntList` object in memory!
> To invoke the overloaded `operator[]` through a pointer, you must dereference the pointer first: `(*ptr)[2]`.

### Non-Integral Index Types

The parameter of `operator[]` does not need to be an integer. It can be any type, such as `std::string_view` or `std::string`, which is useful when creating associative dictionaries, lookup maps, or hash tables:

```cpp
class GradeBook
{
    // ...
public:
    char& operator[](std::string_view studentName);
};
```

### Multidimensional Subscripts: C++23 Evolution

- **Prior to C++23**: `operator[]` was strictly limited to taking a single parameter. Indexing a 2D matrix directly via `matrix[row, col]` was not supported (the comma was evaluated as the comma operator). Developers had to use `operator()(row, col)` instead.
- **In C++23**: C++ added support for **multidimensional subscript operators**, allowing `matrix[row, col]` to take multiple comma-separated arguments directly.

### 📁 Code Examples for Section 9
- [`21_8_Overloading_subscript_operator/1_subscript_operator_overloading.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_8_Overloading_subscript_operator/1_subscript_operator_overloading.cpp): Basic `operator[]` implementation returning an `int&` for read/write array access.
- [`21_8_Overloading_subscript_operator/2_subscript_operator_overloading_advance.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_8_Overloading_subscript_operator/2_subscript_operator_overloading_advance.cpp): Dual non-const and const overloads of `operator[]` ensuring const-correct access.
- [`21_8_Overloading_subscript_operator/3_don't_use_pointer_to_object_subscript_overload_.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_8_Overloading_subscript_operator/3_don't_use_pointer_to_object_subscript_overload_.cpp): Demonstrates the pointer dereferencing gotcha (`(*ptr)[i]` vs `ptr[i]`).
- [`21_8_Overloading_subscript_operator/4_index_can_be_any_type.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_8_Overloading_subscript_operator/4_index_can_be_any_type.cpp): Demonstrates non-integral indexing using `std::string_view` keys.
- [`21_8_Overloading_subscript_operator/5_c++23_supports_[]_multiple_subscripts.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_8_Overloading_subscript_operator/5_c++23_supports_[]_multiple_subscripts.cpp): Demonstrates multidimensional subscripting in C++23 (`operator[](r, c)`) alongside C++20 `operator()(r, c)`.

---

## 10 — Overloading the Parenthesis Operator

The function call / parenthesis operator (`operator()`) is unique in C++ because it allows you to vary **both the type and the number of parameters** it accepts.

- `operator()` **must be implemented as a member function**.
- Can accept zero, one, or multiple arguments with default parameters.

### 1. Multidimensional Array Indexing

Because `operator[]` was historically limited to a single parameter prior to C++23, `operator()` is the standard idiom for indexing multidimensional structures:

```cpp
class Matrix
{
    double m_data[4][4]{};
public:
    // 2D indexing using operator()
    double& operator()(int row, int col)
    {
        return m_data[row][col];
    }

    const double& operator()(int row, int col) const
    {
        return m_data[row][col];
    }
};

Matrix m;
m(1, 2) = 4.5; // Intuitive 2D indexing
```

### 2. Functors (Function Objects)

An object of a class that overloads `operator()` is called a **functor** (or **function object**). Functors can be invoked with the exact same syntax as a function call:

```cpp
class Accumulator
{
    int m_counter{0};
public:
    int operator()(int i)
    {
        return (m_counter += i);
    }
};

Accumulator acc;
std::cout << acc(10) << '\n'; // 10
std::cout << acc(20) << '\n'; // 30 (remembers state!)
```

> [!TIP]
> **Advantage of Functors over Normal Functions**: Normal functions have no persistent state between calls (unless using dirty global or local static variables). Functors are full class instances; they can store internal configuration, accumulate state across calls, and provide additional member helper functions.

### 📁 Code Examples for Section 10
- [`21_9_Overloading_parenthesis_operator/1_two_dimensional_array.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_9_Overloading_parenthesis_operator/1_two_dimensional_array.cpp): Implements a 2D matrix class indexed using dual-parameter `operator()(row, col)`.
- [`21_9_Overloading_parenthesis_operator/2_functor.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_9_Overloading_parenthesis_operator/2_functor.cpp): Demonstrates a stateful accumulator functor maintaining running totals across multiple function-call invocations.

---

## 11 — Overloading Typecasts

By default, the compiler has no knowledge of how to convert an object of your custom class into fundamental types or other classes.

Overloading a **user-defined conversion operator** (typecast) defines how your object can be converted into another type:

```cpp
class Cents
{
    int m_cents{};
public:
    explicit Cents(int cents = 0) : m_cents{cents} {}

    // Overloaded typecast to int
    // Note: No return type is specified; name of target type IS the return type!
    operator int() const
    {
        return m_cents;
    }
};
```

### Syntax and Characteristics of Overloaded Typecasts

1. **Member functions only**: Overloaded typecasts must be non-static member functions.
2. **Const correctness**: They should almost always be `const` so they can be called on const objects.
3. **No parameters**: They take no explicit arguments; they operate on the implicit `*this` object.
4. **No return type**: They do not declare a return type. The name of the target type (e.g. `operator int()`, `operator double()`) serves as the return type to avoid declaration redundancy.
5. **Space between keyword and type**: There must be a space between the `operator` keyword and the target type: `operator int()`, not `operatorint()`.

### Explicit Typecasts

Just as constructors can be marked `explicit` to prevent unintended implicit conversions, typecast operators can also be marked `explicit`:

```cpp
class Cents
{
    int m_cents{};
public:
    explicit Cents(int cents = 0) : m_cents{cents} {}

    // Explicit typecast: only invoked via static_cast or direct initialization
    explicit operator int() const { return m_cents; }
};

Cents c{50};
// int n = c;                     // Compile error: implicit conversion disallowed!
int n = static_cast<int>(c);      // OK: explicit static_cast
```

### Converting Constructors vs. Overloaded Typecasts

Converting constructors and overloaded typecasts serve complementary roles:
- **Converting constructor**: Defined in class `B` to create `B` from `A`: `B(const A&)`.
- **Overloaded typecast**: Defined in class `A` to convert `A` into `B`: `operator B() const`.

| Guideline | Recommendation |
|---|---|
| **Default Choice** | **Prefer converting constructors** over overloaded typecasts whenever possible. |
| **Fundamental Types** | Use **overloaded typecasts** to convert to primitive types (`int`, `double`, `bool`), since primitive types cannot have constructors defined. |
| **External/Standard Types** | Use **overloaded typecasts** when converting to types you cannot modify (e.g. converting your class to `std::string_view` or `std::vector`). |
| **Decoupling Dependencies** | Use **overloaded typecasts** when class `A` should know about `B`, but class `B` must not depend on `A` (avoiding circular `#include` dependencies). |
| **Avoid Ambiguity** | **Never define both** a converting constructor in `B` and an overloaded typecast in `A` for the same conversion, as this causes compiler ambiguity errors! |

### 📁 Code Examples for Section 11
- [`21_10_Overloading_typecasts/1_overloading_type_cast_explicit_prefer.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_10_Overloading_typecasts/1_overloading_type_cast_explicit_prefer.cpp): Demonstrates explicit user-defined typecasts (`explicit operator int()`), conversion between custom classes (`Dollars` to `Cents`), and preventing accidental implicit conversions.

---

## 12 — Overloading the Assignment Operator

The **copy assignment operator** (`operator=`) is used to copy values from one object to another **already-existing** object.

### Copy Constructor vs. Copy Assignment Operator

| Feature | Copy Constructor (`MyClass(const MyClass&)`) | Copy Assignment Operator (`operator=`) |
|---|---|---|
| **Target Object** | A brand new object being initialized for the first time. | An already initialized, existing object. |
| **Context** | Variable initialization, passing by value, returning by value. | Assignment statements (`a = b`). |
| **Resource Cleanup** | Target object does not hold existing resources; only acquires new resources. | Target object already owns existing resources; must deallocate old resources before copying new ones! |

```cpp
Point p1{5, 6};
Point p2 = p1; // Copy CONSTRUCTOR (p2 is being created)
Point p3;
p3 = p1;       // Copy ASSIGNMENT operator (p3 already exists)
```

- `operator=` **must be overloaded as a member function**.
- Should return `*this` by reference (`MyClass&`) to support assignment chaining: `a = b = c;`.

### Detecting and Handling Self-Assignment

In classes that manage dynamically allocated heap memory, **self-assignment** (`x = x;`) can be catastrophic if not guarded:
- If `operator=` naively deallocates its current memory (`delete[] m_ptr;`) before copying from the source, and the source is itself, the source data is destroyed before it can be copied!
- Accessing the deleted memory leads to reading garbage or triggering undefined behavior.

To prevent this, always include a **self-assignment check**:

```cpp
MyString& MyString::operator=(const MyString& right)
{
    // Self-assignment guard: compare memory addresses
    if (this == &right)
        return *this;

    // 1. Deallocate old local resources
    delete[] m_data;

    // 2. Allocate new memory and copy data
    m_length = right.m_length;
    m_data = new char[m_length + 1];
    std::copy_n(right.m_data, m_length + 1, m_data);

    // 3. Return *this by reference
    return *this;
}
```

### The Implicit Copy Assignment Operator and Deleted `operator=`

If you do not provide a user-defined copy assignment operator, the compiler automatically generates an **implicit copy assignment operator** that performs memberwise assignment.

> [!CAUTION]
> If a class contains **`const` members** or **reference members**, the compiler **automatically marks the implicit `operator=` as deleted (`= delete`)**!
> Const members and references cannot be reassigned after initialization. If you want such a class to be assignable, you must explicitly write an `operator=` that assigns only the non-const, non-reference members.

### 📁 Code Examples for Section 12
- [`21_11_Overloading_assignment_operator/1_overload_copy_asignment_operator.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_11_Overloading_assignment_operator/1_overload_copy_asignment_operator.cpp): Demonstrates implementing `operator=` for a dynamic string class with self-assignment detection (`this != &right`) and deep copying.
- [`21_11_Overloading_assignment_operator/2_const_member_copy_assignment_auto_delete.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_11_Overloading_assignment_operator/2_const_member_copy_assignment_auto_delete.cpp): Demonstrates how classes containing `const` member variables cause the compiler to automatically delete the default copy assignment operator.

---

## 13 — Shallow vs. Deep Copying

### Shallow Copying (Memberwise Copy)

Because the C++ compiler does not know the ownership semantics of pointer member variables in your class, the default copy constructor and default copy assignment operator perform a **shallow copy** (also known as a **memberwise copy**).
- For fundamental types, values are copied directly.
- For pointer types, only the memory address stored in the pointer is copied—the actual dynamically allocated memory being pointed to is **not duplicated**.

```
Shallow Copy Pitfall:
Object A: m_data -------------\
                               >----> [ Heap Memory: "Hello" ]
Object B: m_data -------------/

When Object A is destroyed, its destructor deletes [ Heap Memory ].
When Object B is subsequently destroyed, its destructor attempts to delete [ Heap Memory ] AGAIN!
===> CRASH: Double-Free Bug / Dangling Pointer
```

### Deep Copying

A **deep copy** allocates brand new, distinct heap memory for the copy and then duplicates the actual underlying values from the source object.
- The source and the destination objects manage completely separate blocks of dynamic memory.
- Modifying or destroying one object has zero effect on the other.

```cpp
void MyString::deepCopy(const MyString& source)
{
    // Clean up existing memory
    delete[] m_string;

    m_length = source.m_length;
    if (source.m_string)
    {
        m_string = new char[m_length + 1];
        std::copy_n(source.m_string, m_length + 1, m_string);
    }
    else
    {
        m_string = nullptr;
    }
}
```

### The Rule of Three

> [!IMPORTANT]
> **The Rule of Three**: If a class manages dynamic resources and requires a custom implementation of any of the following three special member functions, it almost certainly requires all three:
> 1. **Destructor** (to deallocate resources)
> 2. **Copy Constructor** (to perform a deep copy upon initialization)
> 3. **Copy Assignment Operator** (to perform a deep copy upon assignment)
>
> *(Note: In modern C++ with move semantics covered in Chapter 22, this expands to the **Rule of Five** or the **Rule of Zero**).*

### 📁 Code Examples for Section 13
- [`21_11_Overloading_assignment_operator/1_overload_copy_asignment_operator.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_11_Overloading_assignment_operator/1_overload_copy_asignment_operator.cpp): Provides a complete implementation of deep copying within both the copy constructor and copy assignment operator for a heap-allocated string class.

---

## 14 — Overloading Operators and Function Templates

Function templates can be instantiated with user-defined types (`class` / `struct`) as easily as fundamental types.

When a function template executes operations such as `+`, `+=`, `<`, `<<`, or `/=` on its template type parameters, the template body establishes an **implicit interface contract**: any type instantiated with that template **must provide the required overloaded operators**.

```cpp
template <typename T, std::size_t N>
T calculateAverage(const T (&arr)[N])
{
    T sum{}; // Value-initialization invokes default constructor
    for (const T& elem : arr)
    {
        sum += elem; // Requires: T::operator+=(const T&)
    }
    sum /= N;        // Requires: T::operator/=(std::size_t)
    return sum;
}
```

If an argument type is supplied that does not implement all the operators utilized in the template function body, the compiler will emit a **template instantiation compile-time error**.

### Initializing Accumulators in Generic Templates

In non-template numeric code, accumulators are commonly initialized to zero: `int sum = 0;` or `double sum{ 0.0 };`.

However, in a generic template:
- Writing `T sum{ 0 };` or `T sum = 0;` will fail to compile if type `T` does not have a constructor accepting an integer literal `0`!
- **Best Practice**: Use **value-initialization** `T sum{};`.
  - For fundamental types (`int`, `double`), value-initialization zero-initializes the variable.
  - For class types, value-initialization invokes the default constructor, creating a valid baseline object without assuming anything about integer conversion constructors.

### 📁 Code Examples for Section 14
- [`21_12_Overloading_operators_n_function_templates/1_overloading_operators_function_templates.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/21_Operator_Overloading/21_12_Overloading_operators_n_function_templates/1_overloading_operators_function_templates.cpp): Demonstrates instantiating a generic `centsAverage` function template with user-defined `Cents` objects, verifying `operator+=` and `operator/=` requirements, and accumulator value-initialization (`T sum{}`).
