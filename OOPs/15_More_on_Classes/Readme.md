# 15 — More on Classes

---

## Table of Contents
1. [The Hidden `this` Pointer and Member Function Chaining](#1-the-hidden-this-pointer-and-member-function-chaining)
2. [Classes and Header Files](#2-classes-and-header-files)
3. [Nested Types (Member Types)](#3-nested-types-member-types)
4. [Introduction to Destructors](#4-introduction-to-destructors)
5. [Class Templates with Member Functions](#5-class-templates-with-member-functions)
6. [Static Member Variables](#6-static-member-variables)
7. [Static Member Functions](#7-static-member-functions)
8. [Friend Non-Member Functions](#8-friend-non-member-functions)
9. [Friend Classes and Friend Member Functions](#9-friend-classes-and-friend-member-functions)
10. [Ref Qualifiers](#10-ref-qualifiers)

---

## 1. The Hidden `this` Pointer and Member Function Chaining

### The Hidden `this` Pointer
- When a member function is called, how does C++ keep track of which object it was called on?
- Inside every member function, the keyword **`this`** is a **const pointer** that holds the **address of the current implicit object**.
- All non-static member functions have a `this` pointer that holds the address of the implicit object. **The constness of `this` depends on whether the member function itself is const or not.**

### `this` Always Points to the Object Being Operated On
- Each member function has a **single `this` pointer parameter** that points to the implicit object.
- Because `this` is just a **function parameter** (and not a member), **it does not make instances of your class larger memory-wise**.

### Explicitly Referencing `this`
- If you have a member function that has a **parameter with the same name as a data member**, you can **disambiguate** them by using `this`.

```cpp
class MyClass
{
    int m_x{};
public:
    void setX(int m_x) { this->m_x = m_x; } // disambiguate member vs parameter
};
```

### Returning `*this` for Function Chaining
- The primary reason to return `*this` is to allow member functions to be **"chained" together**, so several member functions can be called on the same object in a single expression! This is called **function chaining**.
- **Returning `*this` by value in method chaining is generally discouraged** because it may create unnecessary object copies — especially in older C++ versions or when copy elision is disabled.
- **Returning `*this` by reference** (or returning `this` as a pointer) avoids copying and is the **recommended, efficient, and idiomatic approach**.
- If returning `this` as a pointer is used: because `this` always points to the implicit object, **we don't need to check whether it is a null pointer** before dereferencing it.

```cpp
class Builder
{
    int m_x{}, m_y{};
public:
    Builder& setX(int x) { m_x = x; return *this; } // returns ref to *this
    Builder& setY(int y) { m_y = y; return *this; }
};
// Builder b; b.setX(1).setY(2); // function chaining
```

### Resetting a Class Back to Default State
- **Constructors are only for initialization of new objects, and should not be called directly.** Doing so will result in unexpected behavior.
- The best way to reset a class back to a default state is to **create a `reset()` member function**, have that function create a new object (using the default constructor), and then assign that new object to the current implicit object.

```cpp
class MyClass
{
    int m_x{}, m_y{};
public:
    MyClass() = default;
    MyClass(int x, int y) : m_x{x}, m_y{y} {}
    void reset() { *this = MyClass{}; } // assign default-constructed object
};
```

### `this` and Const Objects
- For **non-const member functions**: `this` is a `T* const` — a **const pointer to a non-const value** (the pointer cannot be re-pointed, but the object can be modified).
- For **const member functions**: `this` is a `const T* const` — a **const pointer to a const value** (neither the pointer nor the object can be modified).

### Why Is `this` a Pointer and Not a Reference?
- When `this` was added to C++, **references didn't exist yet**.
- If `this` were added to C++ today, it would **undoubtedly be a reference** instead of a pointer.

### 📁 Code Examples
- [`1_this_pointer.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_1_The_hidden_this_pointer_n_member_function_chaining/1_this_pointer.cpp) — `this` pointer usage: disambiguation, returning `*this` for function chaining, and reset pattern

---

## 2. Classes and Header Files

- C++ allows us to **separate the "declaration" portion from the "implementation" portion** by defining member functions outside of the class definition.
- The **prototypes** for these member functions still exist inside the class definition (as these functions need to be declared as part of the class type definition), but the actual implementation will be outside.
- When defining member functions **outside the class**, we must **prefix the member function names** with the name of the class type (e.g., `Date::`) using the scope resolution operator.
- Access functions are typically only one line — so defining them inside the class adds minimal clutter. **Definitions of access functions (and other trivial, one-line functions) are often left inside the class definition.**

### Putting Class Definitions in a Header File
- Unlike functions (which only need a forward declaration), **the compiler typically needs to see the full definition of a class** in order to use the type.
  - The compiler needs to understand how members are declared to ensure they are used properly.
  - It needs to calculate how large objects of that type are in order to instantiate them.
- So **header files usually contain the full definition of a class** rather than just a forward declaration.

### Naming Your Class Header and Code Files
- Classes are defined in **header files of the same name as the class**, and member functions defined outside the class are put in a **`.cpp` file of the same name as the class**.
- **Trivial member functions** (access functions, constructors with empty bodies, etc.) can be defined inside the class definition.
- **Prefer to define non-trivial member functions in a source file** with the same name as the class.

### Doesn't Defining a Class in a Header File Violate the ODR?
- Including a class definition more than once into a single translation unit **is still an ODR violation**. However, **header guards** (or `#pragma once`) will prevent this from happening.

### Inline Member Functions

**Member functions defined inside the class (implicitly inline):**
- **Member functions defined inside the class definition are implicitly inline.** Inline functions are exempt from the one-definition-per-program part of the one-definition rule.

**Member functions defined outside the class (NOT implicitly inline):**
- **Member functions defined outside the class definition are not implicitly inline** (and thus are subject to the one-definition-per-program part of the ODR).
- This is why such functions are **usually defined in a code file** (where they will only have one definition across the whole program).
- Alternatively, member functions defined outside the class can be left in the header file if they are **made inline using the `inline` keyword**.

### Inline Expansion of Member Functions
- The compiler must be able to see a **full definition of a function** in order to perform inline expansion.
- If you want to define a member function outside the class but still want it to be eligible for inline expansion, **define it as an inline function just below the class definition** (in the same header file).

### So Why Not Put Everything in a Header File?
- **First**, defining members inside the class definition **clutters up your class definition**.
- **Second**, if you change any of the code in the header, **you'll need to recompile every file that includes that header**. This can have a ripple effect, where one minor change causes the entire program to need to recompile.

### Header-Only Library Files
- In modern C++, classes or libraries are increasingly being distributed as **"header-only"** — all of the code is placed in a header file (easier to distribute: just `#include` it vs adding a `.cpp` to every project).
- If intentionally creating a header-only class or library, **all non-trivial member functions can be made `inline` and placed in the header file beneath the class definition**.
- For **template classes**, template member functions defined outside the class are **almost always defined inside the header file**, beneath the class definition. The compiler needs the full template definition in order to instantiate it.

- **Put any default arguments for member functions inside the class definition.**

### 3rd Party Libraries
- Many open source packages provide both `.h` and `.cpp` files. However, most commercial libraries provide **only `.h` files and a precompiled library file**. Reasons:
  1. **Faster** to link a precompiled library than to recompile it every time.
  2. A **single copy** of a precompiled library can be shared by many applications.
  3. **Intellectual property** reasons (protecting source code).

### 📁 Code Examples
- [`Example-1/Date.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_2_Classes_n_header_files/Example-1/Date.cpp) — Member functions defined outside the class in a `.cpp` file (with `Date::` prefix)
- [`Example-1/main.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_2_Classes_n_header_files/Example-1/main.cpp) — Using a class defined across header + source files
- [`Example-2/main.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_2_Classes_n_header_files/Example-2/main.cpp) — Alternate class/header file organization example

---

## 3. Nested Types (Member Types)

- To create a nested type, **simply define the type inside the class, under the appropriate access specifier**.
- **Define any nested types at the top of your class type.**
- Since the class itself acts as a scope region, **it's somewhat redundant to use a scoped enumerator** inside it as well.

### Nested Typedefs and Type Aliases
- Class types can also contain **nested typedefs or type aliases**.

### Nested Classes and Access to Outer Class Members
- In C++, **a nested class does not have access to the `this` pointer of the outer (containing) class**, so nested classes **cannot directly access the members of the outer class**.
- This is because a nested class **can be instantiated independently** of the outer class (and in such a case, there would be no outer class members to access).
- However, because nested classes are **members of the outer class**, they **can access any private members of the outer class that are in scope**.
- **Most common use case:** In the standard library, most iterator classes are implemented as nested classes of the container they are designed to iterate over. For example, `std::string::iterator` is implemented as a nested class of `std::string`.

### Nested Types and Forward Declarations
- A nested type **can be forward declared within the class** that encloses it. The nested type can then be defined later, either within the enclosing class or outside of it.
- **A nested type cannot be forward declared prior to the definition of the enclosing class.**
- While you can forward declare a nested type after the definition of the enclosing class, since the enclosing class will already contain a declaration for the nested type, doing so is redundant.

### 📁 Code Examples
- [`1_nested_types.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_3_Nested_types_(_member_types_)/1_nested_types.cpp) — Defining and using nested types inside a class
- [`2_nested_class_n_type_aliases.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_3_Nested_types_(_member_types_)/2_nested_class_n_type_aliases.cpp) — Nested class and nested type aliases
- [`3_nested_types_forward_declaration.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_3_Nested_types_(_member_types_)/3_nested_types_forward_declaration.cpp) — Valid forward declaration of a nested type within its enclosing class
- [`4_nested_types_forward_declaration.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_3_Nested_types_(_member_types_)/4_nested_types_forward_declaration.cpp) — ⚠️ Intentional compile error: cannot forward-declare a nested type before the outer class is defined

---

## 4. Introduction to Destructors

- Classes have another type of special member function that is **called automatically when an object of a non-aggregate class type is destroyed**. This function is called a **destructor**.
- Destructors are designed to allow a class to do **any necessary cleanup** before an object of the class is destroyed.

### Destructor Naming Rules
- The destructor must have the **same name as the class, preceded by a tilde (`~`)**.
- The destructor **cannot take arguments**.
- The destructor **has no return type**.
- **A class can only have a single destructor.**

- **Generally you should not call a destructor explicitly** (as it will be called automatically when the object is destroyed), since there are rarely cases where you'd want to clean up an object more than once.
- **Destructors may safely call other member functions** since the object isn't destroyed until after the destructor executes.
- Remember that **static variables** (including global variables and static local variables) are **constructed at program startup** and **destroyed at program shutdown**.

### An Implicit Destructor
- If a non-aggregate class type object has **no user-declared destructor**, the compiler will generate a destructor with an **empty body** — called an **implicit destructor** (effectively just a placeholder).
- If your class does not need to do any cleanup on destruction, it's **fine to not define a destructor at all**, and let the compiler generate an implicit one.


> [!WARNING]
> **`std::exit()` and Destructors**: `std::exit()` **terminates the program immediately**. Local variables are **not destroyed first**, and because of this, **no destructors will be called**. Be wary if you're relying on your destructors for necessary cleanup.
> **Unhandled exceptions** will also cause the program to terminate, and may not unwind the stack before doing so. If stack unwinding does not happen, **destructors will not be called** prior to termination.


### 📁 Code Examples
- [`1_destructor_example.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_4_Introduction_to_destructors/1_destructor_example.cpp) — Destructor naming, automatic invocation, and cleanup behavior

---

## 5. Class Templates with Member Functions

### Type Template Parameters in Member Functions
- **Type template parameters** defined as part of a class template parameter declaration can be used both as the **type of data members** and as the **type of member function parameters**.
- We don't need deduction guides for CTAD to work with non-aggregate classes. **A matching constructor provides the compiler with the information it needs to deduce the template parameters** from the initializers.
- When defining a member function for a class template **outside of the class template definition**, we need to **resupply a template parameter declaration** (`template <typename T>`) so the compiler knows what `T` is.
- When defining a member function outside the class, we need to **qualify the member function name with the fully templated name** of the class template (e.g., `Pair<T>::isEqual`, not `Pair::isEqual`).

```cpp
template <typename T>
class Pair
{
    T m_a, m_b;
public:
    bool isEqual(const Pair<T>& other) const; // declaration inside class
};

// Definition outside class — must resupply template params:
template <typename T>
bool Pair<T>::isEqual(const Pair<T>& other) const
{
    return m_a == other.m_a && m_b == other.m_b;
}
```

### Injected Class Names
- Within the scope of a class, the **unqualified name of the class** is called an **injected class name**.
- In a class template, the injected class name serves as **shorthand for the fully templated name**.
- CTAD doesn't work with function parameters (it is argument deduction, not parameter deduction). However, **using an injected class name as a function parameter is okay**, as it is shorthand for the fully templated name.
- **Any member function templates defined outside the class definition should be defined just below the class definition** (in the same file).
- **Functions implicitly instantiated from templates are implicitly inline.** This includes both non-member and member function templates. Therefore, there is **no issue including member function templates defined in header files into multiple code files**, as the functions instantiated from those templates will be implicitly inline (and the linker will de-duplicate them).

### 📁 Code Examples
- [`1_class_template_with_member_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_5_Class_templates_with_member_functions/1_class_template_with_member_function.cpp) — Class template with member functions: type template parameters, CTAD, defining functions outside the class
- [`2_class_template_with_member_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_5_Class_templates_with_member_functions/2_class_template_with_member_function.cpp) — Injected class names and member function templates

---

## 6. Static Member Variables

- **Static member variables are shared by all objects of the class.**

### Static Members Are Not Associated with Class Objects
- **Static members exist even if no objects of the class have been instantiated!** They are created at the start of the program and destroyed at the end of the program, so their **lifetime is not bound to a class object** like a normal member.
- **Static members are global variables that live inside the scope region of the class.**
- Access static members using the **class name and the scope resolution operator (`::`)**.

### Defining and Initializing Static Member Variables
- When we **declare** a static member variable inside a class type, it is a **pure declaration, NOT a definition**.
- Because static member variables are essentially global variables, **you must explicitly define the static member outside of the class, in the global scope**:
  ```cpp
  // Inside class:
  class Something { static int s_value; };

  // Outside class (in .cpp), at global scope:
  int Something::s_value{ 1 }; // define and initialize s_value to 1
  ```
- This line **instantiates** the static member variable and **initializes** it. If no initializer is provided, **static member variables are zero-initialized by default**.
- **Note:** This static member definition is **not subject to access controls** — you can define and initialize the value even if it's declared as `private` (or `protected`) in the class.

**Placement rules:**
- For **non-template classes** in a header file → static member definition goes in the **associated `.cpp` file**.
- Alternatively, the member can be defined as **`inline`** and placed below the class definition in the header (useful for header-only libraries).
- For classes defined in a **`.cpp` file** → static member definition goes **directly underneath the class**.
- **Do NOT put the static member definition in a header file** — if that header is included more than once, you'll end up with multiple definitions, causing a **linker error**.
- For **template classes** → the templated static member definition is placed **directly underneath the template class definition in the header file** (doesn't violate the ODR because such definitions are implicitly inline).
- **Static member variables in a class template are instantiated separately for each template specialization**, so each type gets its own independent copy.

### Initialization of Static Member Variables Inside the Class Definition
- A `static` member can be initialized inside the class definition when:
  - It is a **constant integral type** (which includes `char` and `bool`) or a **`const enum`**.
  - The `inline` keyword is used — **`static` variables can be initialized inside the class definition regardless of whether they are constant or not** using `inline`.
  - It is **`static constexpr`** — because `constexpr` members are **implicitly inline** (as of C++17), `static constexpr` members can be initialized inside the class definition **without explicit use of `inline`**.
- **Make your static members `inline` or `constexpr` so they can be initialized inside the class definition.**
- **Only static members may use type deduction (`auto` and CTAD).**
- **Non-static members may not use `auto` or CTAD.**

### 📁 Code Examples
- [`1_static_member_variable.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_6_Static_member_variables/1_static_member_variable.cpp) — Static member variable: shared across objects, definition outside class, zero-init default
- [`2_static_member_var_initialization_inside_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_6_Static_member_variables/2_static_member_var_initialization_inside_class.cpp) — Initializing static members inside the class using `inline`/`constexpr`

---

## 7. Static Member Functions

- Just as static member variables belong to the class rather than to objects of the class, **static member functions also belong to the class rather than to any object.**
- **If a static member variable is `public`, it can be accessed directly using the class name and the scope resolution operator.**
- But what if a static member variable is `private`? → That's where **static member functions** come in.

### Static Member Functions
- **Member functions can be made `static` as well.**
- Because static member functions are **not associated with a particular object**, they can be called directly using the **class name and the scope resolution operator** (e.g., `Something::getValue()`).
- Like static member variables, they can also be called through objects of the class type, though **this is not recommended**.

```cpp
class Something
{
    static int s_value;
public:
    static int getValue() { return s_value; } // static member function
};

// Called via class name (recommended):
int x = Something::getValue();
```

### Static Member Functions Have No `this` Pointer
- **First**, because static member functions are not attached to an object, **they have no `this` pointer**.
- **Second**, static member functions can **directly access other static members** (variables or functions), but **not non-static members**. This is because non-static members must belong to a class object, and static member functions have no class object to work with.

### Static Members Defined Outside the Class Definition
- Static member functions can also be **defined outside of the class declaration** — this works the same way as for normal member functions.
- Member functions defined inside the class definition are implicitly inline. Member functions defined outside are not implicitly inline but can be made inline using the `inline` keyword.
- **A static member function defined in a header file should be made `inline`** so as not to violate the ODR if that header is included into multiple translation units.

### Monostates (Pure Static Classes)
- A **monostate** is a class with all static members.

**Some potential downsides:**
- **First**, because all static members are instantiated only once, **there is no way to have multiple copies** of a pure static class.
- Like global variables, **pure static classes carry the same dangers** as global variables.

### Pure Static Classes vs Namespaces
- Both allow you to define variables with **static duration** and functions within their scope region.
- One significant difference: **classes have access controls while namespaces do not**.
- **In general, a static class is preferable when you have static data members and/or need access controls. Otherwise, prefer a namespace.**

- **C++ does not support static constructors.**

### 📁 Code Examples
- [`1_static_member_functions.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_7_Static_member_functions/1_static_member_functions.cpp) — Static member function: no `this` pointer, access via class name
- [`2_static_member_functions.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_7_Static_member_functions/2_static_member_functions.cpp) — Monostate class, pure static class vs namespace comparison

---

## 8. Friend Non-Member Functions

- In C++, a **friend** is a class or function (member or non-member) that has been **granted full access to the private and protected members of another class**. In this way, a class can selectively give other classes or functions full access to their members without impacting anything else.
- **The `friend` declaration is not affected by access controls**, so it does not matter where within the class body it is placed.
- Defining a friend non-member inside a class.
- There were times we might prefer to use a **non-member function over a member function**.

```cpp
class MyClass
{
    int m_value{ 5 };
    friend void printValue(const MyClass& obj); // friend declaration inside the class
};

void printValue(const MyClass& obj) // defined as non-member
{
    std::cout << obj.m_value << '\n'; // can access private m_value
}
```

### Multiple Friends
- **A function can be a friend of more than one class at the same time.**

### Doesn't Friendship Violate the Principle of Data Hiding?
- **No.** Friendship is **granted by the class doing the data hiding** with the expectation that the friend will access its private members.
- Used properly, friendship can make a program **more maintainable** by allowing functionality to be separated when it makes sense from a design perspective.
- However, because friends have **direct access to the implementation** of a class, **changes to the implementation will typically necessitate changes to the friends as well**. If a class has many friends, this can lead to a **ripple effect**.
- **When implementing a friend function, prefer to use the public interface over direct access to members whenever possible.** This will help insulate your friend function from future implementation changes.
- **A friend function should prefer to use the class interface over direct access whenever possible.**
- **Prefer non-friend functions to friend functions.**

### 📁 Code Examples
- [`1_friend_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_8_Friend_non-member_functions/1_friend_function.cpp) — Basic `friend` non-member function: granting access to private members
- [`2_multiple_friends.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_8_Friend_non-member_functions/2_multiple_friends.cpp) — A function that is a friend of multiple classes simultaneously
- [`3_prefer_non_friend_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_8_Friend_non-member_functions/3_prefer_non_friend_function.cpp) — Demonstrating why non-friend functions are preferred over friend functions

---

## 9. Friend Classes and Friend Member Functions

- A **friend class** is a class that can access the private and protected members of another class.
- **Friendship is not reciprocal.** If A is a friend of B, B is not automatically a friend of A.
- **Class friendship is not transitive.** If class A is a friend of B, and B is a friend of C, that does **not** mean A is a friend of C.
- **Friendship is not inherited.** If class A makes B a friend, classes derived from B are **not** friends of A.
- **The `friend` declaration is placed inside the class that is GRANTING friendship** (not inside the friend class/function). The class grants access to its own private members.

### 📁 Code Examples
- [`1_friend_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_9_Friend_classes_n_friend_member_functions/1_friend_class.cpp) — Friend class: full access to private members; demonstrating non-reciprocal, non-transitive, non-inherited friendship
- [`2_friend_class_member.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_9_Friend_classes_n_friend_member_functions/2_friend_class_member.cpp) — Granting friendship to a specific member function (not the entire class)

---

## 10. Ref Qualifiers

- We know how calling access functions that return **references to data members** can be **dangerous when the implicit object is an rvalue**.
- The challenge: we only want **one function** to service two different cases — one where our implicit object is an **lvalue**, and one where it is an **rvalue**.
- **C++11 introduced ref-qualifiers** — a feature that allows us to **overload a member function based on whether it is being called on an lvalue or an rvalue implicit object**.
- If the implicit object is a non-const rvalue, **use `std::move` to try to move** `m_name`.

### Ref-Qualifier Syntax
- Use **`&`** for **lvalue-qualified** overloads and **`&&`** for **rvalue-qualified** overloads:
  ```cpp
  std::string& getName() &  { return m_name; }           // called on lvalue objects
  std::string  getName() && { return std::move(m_name); } // called on rvalue objects
  ```

### Notes About Ref-Qualified Member Functions
- **First**, for a given function, **non-ref-qualified overloads and ref-qualified overloads cannot coexist**. Use one or the other.
- **Second**, similar to how a `const` lvalue reference can bind to an rvalue, if only a **const lvalue-qualified function** exists, it will **accept either lvalue or rvalue implicit objects**.
- **Third**, either qualified overload can be **explicitly deleted** (using `= delete`), which prevents calls to that function. For example, deleting the rvalue-qualified version prevents use of the function with rvalue implicit objects.

### So Why Don't We Recommend Using Ref-Qualifiers?
- While ref-qualifiers are neat, there are some **downsides**:
  - Adding rvalue overloads to every getter that returns a reference **adds clutter** to the class, to mitigate against a case that isn't that common and is easily avoidable with good habits.
  - Having an rvalue overload return by value means **paying for the cost of a copy (or move)** even in cases where a reference could have been used safely.
  - **Most C++ developers are not aware of this feature** (which can lead to errors or inefficiencies in use).
  - **The standard library typically does not make use of this feature.**

- **It is not recommended to use ref-qualifiers as a best practice.** Instead, it is always recommended to **use the result of an access function immediately** and not save returned references for use later.

### 📁 Code Examples
- [`1_issue_with_rvalue.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_10_Ref_qualifiers/1_issue_with_rvalue.cpp) — The dangling reference problem when using access functions on rvalue implicit objects
- [`2_ref_qualifiers.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/15_More_on_Classes/15_10_Ref_qualifiers/2_ref_qualifiers.cpp) — ⚠️ Intentional compile error: demonstrates `= delete` on rvalue-qualified overload preventing unsafe access
