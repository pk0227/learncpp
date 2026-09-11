# 14 — Introduction to Classes

OOP brings a number of other useful concepts to the table:
- **Encapsulation**
- **Abstraction**
- **Inheritance**
- **Polymorphism**

---

## Table of Contents
1. [Introduction to Classes](#1-introduction-to-classes)
2. [Member Functions](#2-member-functions)
3. [Const Class Objects and Const Member Functions](#3-const-class-objects-and-const-member-functions)
4. [Public and Private Members, Access Specifiers, and Access Functions](#4-public-and-private-members-access-specifiers-and-access-functions)
5. [Member Functions Returning References to Data Members](#5-member-functions-returning-references-to-data-members)
6. [Encapsulation (The Benefits of Data Hiding)](#6-encapsulation-the-benefits-of-data-hiding)
7. [Introduction to Constructors](#7-introduction-to-constructors)
8. [Constructor Member Initializer Lists](#8-constructor-member-initializer-lists)
9. [Default Constructors and Default Arguments](#9-default-constructors-and-default-arguments)
10. [Delegating Constructors](#10-delegating-constructors)
11. [Temporary Class Objects](#11-temporary-class-objects)
12. [Introduction to the Copy Constructor](#12-introduction-to-the-copy-constructor)
13. [Class Initialization and Copy Elision](#13-class-initialization-and-copy-elision)
14. [Converting Constructors and the `explicit` Keyword](#14-converting-constructors-and-the-explicit-keyword)
15. [Constexpr Aggregates and Classes](#15-constexpr-aggregates-and-classes)

---

## 1. Introduction to Classes

- **Structs are aggregates.** As useful as structs are, structs have a number of deficiencies.

### The Class Invariant Problem
For class types which include structs, classes, and unions:
- Perhaps the biggest difficulty with structs is that **they do not provide an effective way to document and enforce class invariants.**
- A **class invariant** is a condition that **must be true throughout the lifetime of an object** in order for the object to remain in a valid state. An object that has a violated class invariant is said to be in an **invalid state**, and unexpected or **undefined behavior** may result from further use of that object.

### 📁 Code Examples
- [`classes_need.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_1_Introduction_to_classes/classes_need.cpp) — Demonstrates the class invariant problem: struct with a zero-denominator fraction causing divide-by-zero

---

## 2. Member Functions

- Functions that belong to a class type are called **member functions**.
- Functions that are not member functions are called **non-member functions** (or occasionally **free functions**).
- As a reminder, a definition is also a declaration, so if we define a member function inside the class, that counts as a declaration.
- Member functions defined inside the class type definition are **implicitly inline**, so they will not cause violations of the **one-definition rule** if the class type definition is included into multiple code files.
- **All (non-static) member functions must be called using an object of that class type.**
- The object that a member function is called on is **implicitly passed** to the member function. For this reason, the object that a member function is called on is often called the **implicit object**.
- With non-member functions, we have to explicitly pass an object to the function to work with, and members are explicitly accessed through that object.
- **Member variables and functions can be defined in any order.**

> [!WARNING]
> - Data members are initialized **in order of declaration**. If the initialization of a data member accesses another data member that isn't declared until later (and thus hasn't been initialized yet), the initialization will result in **undefined behavior**.
> - For this reason, it's generally a good idea to **avoid using other members inside default member initializers**.

- Member functions can be **overloaded**.
- Member functions can be used with both structs and classes.
- **However, structs should avoid defining constructor member functions**, as doing so makes them a non-aggregate.
- **A class type with a constructor is no longer an aggregate**, and we want our structs to remain aggregates.
- If your class type has no data members, **prefer using a namespace**.

### 📁 Code Examples
- [`1_member_function_example.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_2_Member%20functions/1_member_function_example.cpp) — Basic member function usage
- [`2_member_funtions_overload.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_2_Member%20functions/2_member_funtions_overload.cpp) — Member function overloading

---

## 3. Const Class Objects and Const Member Functions

- **Const objects must be initialized at the time of creation.**
- Modifying the data members of const objects is **disallowed**:
  - Changing member variables directly (if they are public).
  - Calling member functions that set the value of member variables.
- **Const objects may not call non-const member functions.**
- **A const object can only call const member functions (and static functions).**

### Const Member Functions
- A **const member function** is a member function that **guarantees it will not modify the object** or call any non-const member functions (as they may modify the object).
- For member functions defined outside of the class definition, the `const` keyword **must be used on both** the function declaration in the class definition, and on the function definition outside the class definition.
- **Constructors may not be made const**, as they need to initialize the members of the object, which requires modifying them.
- Const member functions **can**:
  - Modify non-members (such as local variables and function parameters).
  - Call non-member functions per usual.
  - Call const member functions.
  - The `const` **only applies to members**.
- Const member functions **may be called on non-const objects**.
- Const objects via pass by const reference.
- Member function const and non-const overloading.

### Mutable Data Members
In C++, the **`mutable` keyword** can be applied to a non-static, non-const class member variable:
- A `mutable` data member **can be modified even when the object is const**, and can be modified inside `const` member functions.
- This is intended for members that represent internal implementation details that do not affect the observable logical state of the object (**bitwise constness vs logical constness**), such as internal caches, memoization tables, access counters, or thread synchronization primitives (`std::mutex`):

```cpp
class DataViewer
{
    std::string m_data{};
    mutable int m_viewCount{ 0 }; // mutable member

public:
    DataViewer(std::string_view data) : m_data{ data } {}

    const std::string& getData() const
    {
        ++m_viewCount; // Allowed in const member function!
        return m_data;
    }

    int getViewCount() const { return m_viewCount; }
};
```

### 📁 Code Examples
- [`1_const_object_example.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_3_Const_class_objects_n_const_member_functions/1_const_object_example.cpp) — ⚠️ Intentional compile errors: demonstrates what's disallowed on const objects
- [`2_const_member_function_example.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_3_Const_class_objects_n_const_member_functions/2_const_member_function_example.cpp) — Const member functions: what they can/cannot do
- [`3_obj_via_const_ref.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_3_Const_class_objects_n_const_member_functions/3_obj_via_const_ref.cpp) — Passing const objects via const reference
- [`4_const_member_function_overloading.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_3_Const_class_objects_n_const_member_functions/4_const_member_function_overloading.cpp) — Overloading const vs non-const member functions

---

## 4. Public and Private Members, Access Specifiers, and Access Functions

- **Member access**: Each member of a class type has a property called an **access level** that determines who can access that member.
- C++ has three different access levels: `public`, `private`, and `protected`.

- The members of a `struct` are **public by default**. Public members can be accessed by other members of the class type, and by the public.
- The term "the public" means outside of the members of a given class type. This includes non-member functions too.

- The members of a `class` are **private by default**. Private members can be accessed by other members of the class, but **can not be accessed by the public**.
- **A class with private members is no longer an aggregate**, and therefore can no longer use aggregate initialization.

- We can explicitly set the access level of our members by using an **access specifier**. C++ provides three access specifiers: `public:`, `private:`, and `protected:`.

### Access Level Summary

| Access Level | Access Specifier | Member Access | Derived Class Access | Public Access |
|:------------:|:----------------:|:-------------:|:--------------------:|:-------------:|
| Public       | `public:`        | yes           | yes                  | yes           |
| Protected    | `protected:`     | yes           | yes                  | no            |
| Private      | `private:`       | yes           | no                   | no            |

### Access Level Best Practices for Structs and Classes
- **Classes should generally make member variables private (or protected), and member functions public.**
- **Structs should generally avoid using access specifiers** (all members will default to public) to keep structs to be aggregates.

### Access Levels Work on a Per-class Basis
- A member function can also **directly access the private members of ANY other object of the same class type** that is in scope.

- Structs inherit from other class types **publicly** and classes inherit **privately**.

### Conditions to be an Aggregate
1. The class type should **not** have any private or protected members.
2. The class type should **not** have any user-declared constructors (including defaulted or deleted constructors).

- A class with private members or user-declared constructors **is no longer an aggregate**.
- Therefore, the class **cannot use aggregate initialization**.
- In such cases, brace initialization will attempt to call a matching constructor.
- If a constructor taking `std::initializer_list` is defined, **it will take precedence over other constructors**.

### Access Functions
- An **access function** is a trivial public member function whose job is to retrieve or change the value of a private member variable.

**Getters** (also sometimes called accessors):
- Public member functions that return the value of a private member variable.
- Getters are **usually made `const`**.

**Setters** (also sometimes called mutators):
- Public member functions that set the value of a private member variable.

**Getters should return by value or by const lvalue reference:**
- Getters should provide **"read-only" access** to data. Therefore, the best practice is that they should return by either **value** (if making a copy of the member is inexpensive) or by **const lvalue reference** (if making a copy of the member is expensive).

### 📁 Code Examples
- [`1_class_not_aggregate.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_4_Public_n_private_members_n_access_specifiers/1_class_not_aggregate.cpp) — Class with private members loses aggregate status; brace initialization calls constructor
- [`2_access_level_on_per_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_4_Public_n_private_members_n_access_specifiers/2_access_level_on_per_class.cpp) — Access levels work per-class: member functions can access private members of other objects of the same type

---

## 5. Member Functions Returning References to Data Members

- A member function returning a reference should return **a reference of the same type** as the data member being returned, to avoid unnecessary conversions.

### Rvalue Implicit Objects and Return by Reference
- An rvalue object is **destroyed at the end of the full expression** in which it is created. Any references to members of the rvalue object are **left dangling** at that point.
- A reference to a member of an rvalue object can **only be safely used within the full expression** where the rvalue object is created.

- **Prefer to use the return value of a member function that returns by reference immediately**, to avoid issues with dangling references when the implicit object is an rvalue.
- **Do not return non-const references to private data members.**
- **Const member functions can't return non-const references to data members.**

### 📁 Code Examples
- [`1_rvalue_return.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_5_Member_functions_returning_references_to_data_members/1_rvalue_return.cpp) — Dangling reference risk when returning references from rvalue implicit objects

---

## 6. Encapsulation (The Benefits of Data Hiding)

### Interface / Class Interface / Public Interface
- Defines how a user of the class type will interact with objects of the class type.
- Because only public members can be accessed from outside of the class type, the public members of a class type form its **interface**.
- For this reason, an interface composed of public members is sometimes called a **public interface**.
- An interface is an **implicit contract** between the author of a class and the user of a class.
- **If an existing interface is ever changed, any code that uses it may break.** Therefore, it is important to ensure the interfaces for our class types are **well-designed and stable** (don't change much).

### Implementation
- The **implementation** of a class type consists of the code that actually makes the class behave as intended. This includes both the member variables that store data, and the bodies of the member functions that contain the program logic and manipulate the member variables.

### Data Hiding / Abstraction
- Is a technique used to enforce the **separation of interface and implementation** by hiding (making inaccessible) the implementation of a program-defined data type from users.
- Data hiding in C++ is done by **making data members private and member functions public**, so that the user can call them.
- **Structs, on the other hand, should not use data hiding**, as having non-public members prevents them from being treated as aggregates.

### Encapsulation
- The **bundling of data and functions** in an enclosing container where the functions operate on instances of that data.
- **Encapsulation is a pre-requisite for data hiding.**

- Data hiding makes classes easier to use, and reduces complexity by providing public interface to operate on the object of the class.
- **Data hiding allows us to maintain invariants.**

### Class Invariants
- Are conditions that **must be true throughout the lifetime of an object** in order for the object to stay in a valid state.

- **Data hiding allows us to do better error detection (and handling).**
- **Data hiding makes it possible to change implementation details without breaking existing programs.**

### Classes with Interfaces are Easier to Debug
- If everyone is able to set the member variable directly, tracking down which piece of code actually modified the member variable to the wrong value can be difficult.
- This can involve breakpointing every statement that modifies the member variable — and there can be lots of them.
- However, **if a member can only be changed through a single member function**, then you can simply breakpoint that single function and watch as each caller changes the value. This can make it much easier to determine who the culprit is.

- **Prefer a member function** when the function needs access to private (or protected) data that should not be exposed.
- **Prefer a non-member function** otherwise (especially for functions that do not modify the state of the object).

### 📁 Code Examples
- [`1_invariant_issue.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_6_Encapsulation/1_invariant_issue.cpp) — Problem: struct with no invariant enforcement
- [`2_data_hiding_solution_invariant_issue.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_6_Encapsulation/2_data_hiding_solution_invariant_issue.cpp) — Solution: data hiding enforces invariants
- [`3_data_hiding_solution_invariant_issue.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_6_Encapsulation/3_data_hiding_solution_invariant_issue.cpp) — Extended data hiding solution
- [`4_interface_should_not_break_code.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_6_Encapsulation/4_interface_should_not_break_code.cpp) — Stable interface: implementation changes without breaking callers
- [`5_interface_should_not_break_code.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_6_Encapsulation/5_interface_should_not_break_code.cpp) — Further interface stability example
- [`6_prefer_non_member_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_6_Encapsulation/6_prefer_non_member_function.cpp) — When to prefer non-member functions over member functions

---

## 7. Introduction to Constructors

### Constructors
- A **constructor** is a special member function that is **automatically called after a class type object is created**.
- Many new programmers are confused about whether constructors create the objects or not. **They do not** — the compiler sets up the memory allocation for the object prior to the constructor call. The constructor is then called on the **uninitialized object**.
- However, if a matching constructor cannot be found for a set of initializers, the compiler will error. So while constructors don't create objects, **the lack of a matching constructor will prevent creation of an object**.

Beyond determining how an object may be created, constructors generally perform two functions:
- They typically perform **initialization of any member variables** (via a member initialization list).
- They may perform other **setup functions** (via statements in the body of the constructor). This might include things such as error checking the initialization values, opening a file or database, etc.

- After the constructor finishes executing, we say that the object has been **"constructed"**, and the object should now be in a **consistent, usable state**.

### Naming Constructors
- **Constructors must have the same name as the class** (with the same capitalization). For template classes, this name excludes the template parameters.
- **Constructors have no return type** (not even `void`).
- Constructors are typically part of the interface for your class. So, they are **usually public**.

### Constructor Implicit Conversion of Arguments
- The compiler will perform **implicit conversion of arguments** in a function call (if needed) in order to match a function definition where the parameters are a different type.

### Constructors Should Not Be Const
- A constructor needs to be able to initialize the object being constructed — therefore, **a constructor must not be `const`**.

### Constructors vs Setters
- **Constructors** are designed to **initialize an entire object** at the point of instantiation.
- **Setters** are designed to **assign a value to a single member** of an existing object.

### 📁 Code Examples
- [`1_constructors_example.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_7_Introduction_to_constructors/1_constructors_example.cpp) — Basic constructor usage: initialization, naming rules, no return type

---

## 8. Constructor Member Initializer Lists

### Member Initialization via a Member Initialization List
- To have a constructor initialize members, we do so using a **member initializer list** (often called a "member initialization list").
- **Do not confuse** this with the similarly named "initializer list" that is used to initialize aggregates with a list of values.
- You **must use a direct form of initialization** here (preferably using braces, but parentheses works as well) — **using copy initialization (with an equals) does not work here**. Also note that the member initializer list **does not end in a semicolon**.

```cpp
class Fraction
{
    int m_num;
    int m_den;
public:
    Fraction(int num, int den)
        : m_num{ num }   // member initializer list
        , m_den{ den }   // comma-separated, no semicolon at end
    {}
};
```

### Member Initialization Order
- **The members in a member initializer list are always initialized in the order in which they are defined inside the class** (not in the order they are defined in the member initializer list).
- It's also a good idea to **avoid initializing members using the value of other members** (if possible). That way, even if you do make a mistake in the initialization order, it shouldn't matter because there are no dependencies between initialization values.

### Member Initializer List vs Default Member Initializers and Constructor Function Body
Members can be initialized in a few different ways:
- If a member is **listed in the member initializer list**, that initialization value is used.
- Otherwise, if the member has a **default member initializer**, that initialization value is used.
- Otherwise, the member is **default-initialized**.

This means that if a member has both a default member initializer and is listed in the member initializer list for the constructor, **the member initializer list value takes precedence**.

```cpp
class MyClass
{
    int m_x{ 5 };        // default member initializer
public:
    MyClass(int x) : m_x{ x } {}  // MIL takes precedence over default
};
// MyClass obj{ 10 }; => m_x is 10, not 5
```

- The statements in the body of the constructor execute **after** the member initializer list has executed.
- Once the member initializer list has finished executing, the object is considered **initialized**.
- Once the function body has finished executing, the object is considered **constructed**.
- **Prefer using the member initializer list to initialize your members over assigning values in the body of the constructor.**

### Detecting and Handling Invalid Arguments to Constructors
- Inside the body of the constructor, we can use statements, so we have more options for detecting and handling errors. This is a good place to `assert` or `static_assert` that the arguments passed in are semantically valid, but that **doesn't actually handle runtime errors in a production build**.

- In such constructor failure cases, **throwing an exception is a good idea**.
- If exceptions aren't possible or desired, instead of letting the user create the class directly, **provide a function that either returns an instance of the class or something that indicates failure**. A `createObject()` function returns a `std::optional<Fraction>` that optionally contains a valid Object.

```cpp
std::optional<Fraction> makeFraction(int num, int den)
{
    if (den == 0) return {};        // return empty optional on failure
    return Fraction{ num, den };    // return valid object on success
}
```

### 📁 Code Examples
- [`1_member_initialization_order.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_8_Constructor_member_initializer%20_lists/1_member_initialization_order.cpp) — ⚠️ Demonstrates that MIL initializes in declaration order (intentional `-Wreorder` warning)
- [`2_member_initializer_list_default_member_initializer_list.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_8_Constructor_member_initializer%20_lists/2_member_initializer_list_default_member_initializer_list.cpp) — MIL vs default member initializer precedence
- [`3_constructor_failure_handle.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_8_Constructor_member_initializer%20_lists/3_constructor_failure_handle.cpp) — Handling invalid arguments in constructors using exceptions / `std::optional`

---

## 9. Default Constructors and Default Arguments

- A **default constructor** is a constructor that **accepts no arguments**.

### Value Initialization vs Default Initialization for Class Types
- If a class type has a default constructor, both value initialization and default initialization will call the default constructor.
- **Prefer value initialization over default initialization for all class types.**

### Constructors with Default Arguments
- As with all functions, the **rightmost parameters** of constructors can have default arguments.
- **If all of the parameters in a constructor have default arguments, the constructor is a default constructor** (because it can be called with no arguments).

### Overloaded Constructors
- A corollary of the above is that **a class should only have one default constructor**. If more than one default constructor is provided, the compiler will be unable to disambiguate which should be used.

### An Implicit Default Constructor
- If a non-aggregate class type object has **no user-declared constructors**, the compiler will generate a **public default constructor**.
- So that the class can be value or default initialized.

### Using `= default` to Generate an Explicitly Defaulted Default Constructor
- We can instead tell the compiler to generate a default constructor for us. This constructor is called an **explicitly defaulted default constructor**. It can be generated by using the `= default` syntax.

```cpp
class MyClass
{
    int m_x{};
public:
    MyClass() = default;  // explicitly defaulted default constructor
    MyClass(int x) : m_x{ x } {}
};
```

- **If we have any user-defined constructor (other than default constructor), an implicit default constructor would not normally be generated.**
- **Prefer an explicitly defaulted default constructor (`= default`) over a default constructor with an empty body.**

### Explicitly Defaulted Default Constructor vs Empty User-defined Constructor
- When value initializing a class, if the class has a **user-defined default constructor**, the object will be **default initialized**.
- If the class has a default constructor that is **not user-provided** (that is, a default constructor that is either implicitly defined, or defined using `= default`), the object will be **zero-initialized before being default initialized**.
- **Practical implication:** with `= default` or implicit constructor + value-init (`MyClass obj{}`), members without initializers will be **zero-initialized** (e.g., `int` members = 0). With a user-provided default constructor + value-init, members without explicit initialization in the constructor body or member initializer list are **left uninitialized**.

- Only create a default constructor **when it makes sense**.

### 📁 Code Examples
- [`1_different_types_of_default_constructors.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_9_Default_constructors_n_default_arguments/1_different_types_of_default_constructors.cpp) — Implicit, explicit `= default`, and user-defined default constructors
- [`2_empty_constor_vs_default_constor.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_9_Default_constructors_n_default_arguments/2_empty_constor_vs_default_constor.cpp) — Zero-init difference: `= default` vs empty user-defined constructor
- [`3_when_compiler_wont_give_implicit_default_constor.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_9_Default_constructors_n_default_arguments/3_when_compiler_wont_give_implicit_default_constor.cpp) — ⚠️ Intentional compile error: user-defined constructor suppresses implicit default constructor

---

## 10. Delegating Constructors

- Constructors are allowed to call other functions, including other member functions of the class.
- **Constructors should not be called directly from the body of another function.** Doing so will either result in a compilation error, or will direct-initialize a temporary object.
- If you do want a temporary object, **prefer list-initialization** (which makes it clear you are intending to create an object).

### Delegating Constructors
- Constructors are allowed to **delegate** (transfer responsibility for) initialization to another constructor from the same class type. This process is sometimes called **constructor chaining** and such constructors are called **delegating constructors**.

```cpp
class MyClass
{
    int m_x{}, m_y{}, m_z{};
public:
    MyClass(int x, int y, int z) : m_x{x}, m_y{y}, m_z{z} {} // non-delegating
    MyClass(int x, int y) : MyClass{ x, y, 0 } {}             // delegates to above
    MyClass(int x)        : MyClass{ x, 0, 0 } {}             // delegates to above
    MyClass()             : MyClass{ 0, 0, 0 } {}             // delegates to non-delegating
};
```
- **First**, a constructor that delegates to another constructor **cannot have any member initializers in its member initializer list** — delegation and member initialization are **mutually exclusive in the MIL**. However, the constructor body can still contain statements. So your constructors can delegate or initialize (via MIL), **but not both**.
- **Second**, it's possible for one constructor to delegate to another constructor, which delegates back to the first constructor. This forms an **infinite loop**, and will cause your program to **run out of stack space and crash**. You can avoid this by ensuring all of your constructors resolve to a **non-delegating constructor**.
- If you have multiple constructors, consider whether you can use delegating constructors to **reduce duplicate code**.

### Reducing Constructors Using Default Arguments
- Members for which the user **must** provide initialization values should be defined **first** (and as the **leftmost** parameters of the constructor). Members for which the user can optionally provide initialization values (because the default values are acceptable) should be defined **second** (and as the **rightmost** parameters of the constructor).

### 📁 Code Examples
- [`1_delegating_constructor.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_10_Delegating_constructors/1_delegating_constructor.cpp) — Delegating constructors: chaining constructors to reduce duplicate initialization code

---

## 11. Temporary Class Objects

- A **temporary object** (sometimes called an **anonymous object** or an **unnamed object**) is an object that has **no name** and exists only for the **duration of a single expression**.
- Creating temporary objects via direct initialization.

### Temporary Objects and Return by Value
- When a function returns by value, the object that is returned is a **temporary object** (initialized using the value or object identified in the return statement).

### `static_cast` vs Explicit Instantiation of a Temporary Object
- `static_cast<int>(c)` returns a temporary `int` that is **direct-initialized** with the value of `c`. `int { c }` creates a temporary `int` that is **list-initialized** with the value of `c`. Either way, we get a temporary `int` initialized with the value of `c`, which is what we want.

- **Prefer `static_cast` when converting to a fundamental type**, and a **list-initialized temporary when converting to a class type**.

- **Prefer `static_cast` to create a temporary object** when any of the following are true:
  - We need to perform a **narrowing conversion**.
  - We want to make it really obvious that we're converting to a type that will result in some different behavior (e.g. a `char` to an `int`).
  - We want to use direct-initialization for some reason (e.g. to avoid list constructors taking precedence).

- **Prefer creating a new object (using list initialization)** to create a temporary object when any of the following are true:
  - We want to use list-initialization (e.g. for the protection against narrowing conversions, or because we need to invoke a list constructor).
  - We need to provide additional arguments to a constructor to facilitate the conversion.

### 📁 Code Examples
- [`1_temp_object.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_11_Temporary_class_objects/1_temp_object.cpp) — Creating and using temporary (anonymous) objects
- [`2_temp_object_on_return.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_11_Temporary_class_objects/2_temp_object_on_return.cpp) — Temporary objects created on function return by value
- [`3_static_cast_vs_explicit_instantiate_temp_obj.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_11_Temporary_class_objects/3_static_cast_vs_explicit_instantiate_temp_obj.cpp) — `static_cast` vs list-initialized temporary: when to use which

---

## 12. Introduction to the Copy Constructor

### The Copy Constructor
- A **copy constructor** is a constructor that is used to **initialize an object with an existing object of the same type**.
- C++ will create a public **implicit copy constructor** for you. By default, the implicit copy constructor will do **memberwise initialization**. This means each member will be initialized using the corresponding member of the class passed in as the initializer.
- We know that **access controls work on a per-class basis** (not a per-object basis). This means the member functions of a class can access the private members of any class object of the same type. We use that to our advantage in the copy constructor in order to directly access the private members of the parameter object of the same type.
- **A copy constructor should not do anything other than copy an object.** This is because the compiler may optimize the copy constructor out in certain cases.
- **Copy constructors should have no side effects beyond copying.**
- **Prefer the implicit copy constructor**, unless you have a specific reason to create your own.
- If you write your own copy constructor, **the parameter should be a `const` lvalue reference**.

### Pass by Value and the Copy Constructor
- When an object is **passed by value**, the argument is copied into the parameter. When the argument and parameter are the same class type, the copy is made by **implicitly invoking the copy constructor**.

### Return by Value and the Copy Constructor
- When the return type and the return value are the same class type, the temporary object is initialized by **implicitly invoking the copy constructor**.

### Using `= default` to Generate a Default Copy Constructor
- If a class has no copy constructor, the compiler will implicitly generate one for us. If we prefer, we can explicitly request the compiler create a default copy constructor for us using the **`= default` syntax**.

### Using `= delete` to Prevent Copies
- Occasionally we run into cases where we do not want objects of a certain class to be copyable. We can prevent this by **marking the copy constructor function as deleted**, using the **`= delete` syntax**.

> [!IMPORTANT]
> **Why must a copy constructor's parameter be a reference?**
> If a copy constructor took its parameter by value (`MyClass(MyClass other)`), passing an argument would require copying it into `other`. That copy would invoke the copy constructor, which would pass by value, invoking the copy constructor again — resulting in **infinite compile-time recursion**! Therefore, C++ strictly requires copy constructors to accept their parameter by reference (almost always `const MyClass&`).

```cpp
class NoCopy
{
public:
    NoCopy() = default;
    NoCopy(const NoCopy&) = delete;  // copy constructor deleted
};
// NoCopy a;
// NoCopy b{ a }; // ERROR: copy constructor is deleted
```

- The **rule of three** is a well known C++ principle that states that if a class requires a user-defined copy constructor, destructor, or copy assignment operator, **then it probably requires all three**. In C++11, this was expanded to the **rule of five**, which adds the move constructor and move assignment operator to the list. **Not following the rule of three/rule of five is likely to lead to malfunctioning code.** We'll revisit the rule of three and rule of five when we cover dynamic memory allocation.

### 📁 Code Examples
- [`1_explicit_user_copy_constructor.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_12_Introduction_to_copy_constructor/1_explicit_user_copy_constructor.cpp) — ⚠️ Intentional compile error: demonstrates `= delete` on copy constructor preventing pass-by-value

---

## 13. Class Initialization and Copy Elision

- In modern C++, copy initialization, direct initialization, and list initialization essentially do the same thing — they initialize an object.

There are **three key differences** between the initialization forms:
- **List initialization disallows narrowing conversions.**
- **Copy initialization only considers non-explicit constructors/conversion functions.**
- **List initialization prioritizes matching list constructors over other matching constructors.**

- In constructor member initializer lists, **we can only use direct forms of initialization** (direct initialization, direct list initialization), **not copy initialization**.

### Copy Elision
- **Copy elision** is a compiler optimization technique that allows the compiler to **remove unnecessary copying of objects**. In other words, in cases where the compiler would normally call a copy constructor, the compiler is free to rewrite the code to avoid the call to the copy constructor altogether. When the compiler optimizes away a call to the copy constructor, we say the constructor has been **elided**.
- Unlike other types of optimization, **copy elision is exempt from the "as-if" rule**.
- **Copy constructors should not have side effects other than copying** — if the compiler elides the call to the copy constructor, the side effects won't execute, and the observable behavior of the program will change.
- Copy elision has two common forms: **RVO** (Return Value Optimization) — when a temporary object is returned directly, and **NRVO** (Named Return Value Optimization) — when a named local variable is returned.

### Guaranteed (Mandatory) Copy Elision (since C++17)
- In C++17 and later, copy elision when initializing an object from a prvalue (such as returning an unmaterialized temporary object via RVO) is **MANDATORY and guaranteed by the language standard**, not merely an optional compiler optimization.
- Because no copy or move constructor is ever called, the class **does not even need to have an accessible copy or move constructor** for guaranteed elision to occur!
- In contrast, **Named Return Value Optimization (NRVO)** remains an *optional* optimization that compilers are permitted (but not required) to perform.

### 📁 Code Examples
- [`1_type_of_initializations.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_13_Class_initialization_n_copy_elision/1_type_of_initializations.cpp) — Copy, direct, and list initialization compared side-by-side
- [`2_unnecessary_copies.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_13_Class_initialization_n_copy_elision/2_unnecessary_copies.cpp) — How unnecessary copies arise
- [`3_copy_elision_pass_by_value_n_return_by_value.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_13_Class_initialization_n_copy_elision/3_copy_elision_pass_by_value_n_return_by_value.cpp) — Copy elision (RVO/NRVO) in pass-by-value and return-by-value scenarios

---

## 14. Converting Constructors and the `explicit` Keyword

### Converting Constructors
- A constructor that can be used to perform an implicit conversion is called a **converting constructor**.
- **By default, all constructors are converting constructors.**
- **Only one user-defined conversion may be applied.**
- An implicit conversion (that would otherwise use a converting constructor) can be **made explicit at the call site** by using direct initialization or direct list initialization instead of copy initialization.

```cpp
class MyString
{
public:
    MyString(int size) {}  // converting constructor: int -> MyString
};

MyString s1 = 5;    // OK: copy init triggers implicit conversion
MyString s2{ 5 };   // OK: direct-list-init triggers implicit conversion
```

### The `explicit` Keyword
- We can use the `explicit` keyword to **tell the compiler that a constructor should not be used as a converting constructor**.
- For constructors with a separate declaration (inside the class) and definition (outside the class), the `explicit` keyword is **used only on the declaration**.
- **Explicit constructors can be used for direct and direct list initialization.**

```cpp
class MyString
{
public:
    explicit MyString(int size) {}  // NOT a converting constructor
};

MyString s1 = 5;    // ERROR: implicit conversion disallowed (copy-init)
MyString s2{ 5 };   // OK: direct-list-init allowed
MyString s3(5);     // OK: direct-init allowed
```

### Return by Value and Explicit Constructors
- When we return a value from a function, if that value does not match the return type of the function, an **implicit conversion will occur**. Just like with pass by value, **such conversions cannot use explicit constructors**.

### Best Practices for Use of `explicit`
- **The modern best practice is to make any constructor that will accept a single argument `explicit` by default.** This includes constructors with multiple parameters where most or all of them have default values.
- If such a conversion is actually desired in a particular case, it is trivial to convert the implicit conversion into an explicit definition using direct list initialization.
- If an implicit conversion between types is both semantically equivalent and performant, you can consider making the constructor non-explicit.
- **Do not make copy or move constructors explicit**, as these do not perform conversions.

### Conditionally Explicit Constructors with `explicit(bool)` (since C++20)
C++20 introduced the **`explicit(bool)`** specifier, which allows a constructor to be conditionally explicit based on a compile-time boolean expression:
- `explicit(true)` behaves identically to `explicit` (non-converting constructor).
- `explicit(false)` behaves identically to omitting `explicit` (converting constructor).
- This is widely used in standard library templates (e.g. `std::pair`, `std::optional`) where a wrapper constructor is explicit if and only if the wrapped type's constructor is explicit:

```cpp
template <typename T>
class Wrapper
{
    T m_val;
public:
    // Explicit only if T is not implicitly constructible from U
    template <typename U>
    explicit(!std::is_convertible_v<U, T>)
    Wrapper(U&& val) : m_val{ std::forward<U>(val) } {}
};
```

### 📁 Code Examples
- [`1_converting_constructors.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_14_Converting_constructors_n_explicit_keyword/1_converting_constructors.cpp) — Implicit conversion via converting constructors
- [`2_one_user_defined_conversion_required.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_14_Converting_constructors_n_explicit_keyword/2_one_user_defined_conversion_required.cpp) — Only one user-defined conversion allowed per expression
- [`3_explicit_keyword.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_14_Converting_constructors_n_explicit_keyword/3_explicit_keyword.cpp) — `explicit` keyword preventing implicit conversions
- [`4_explicit_keyword_return_by_value.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_14_Converting_constructors_n_explicit_keyword/4_explicit_keyword_return_by_value.cpp) — ⚠️ Intentional compile error: `explicit` constructor blocks implicit return conversion

---

## 15. Constexpr Aggregates and Classes

### Constexpr Aggregates
- **Aggregates implicitly support `constexpr`.**

### Constexpr Class Objects and Constexpr Constructors

### Literal Type
- Is any type for which it might be possible to create an object within a **constant expression**.
- Put another way, **an object can't be `constexpr` unless the type qualifies as a literal type**.
- **Non-aggregate does not qualify to be a literal type if its constructor isn't `constexpr`.**

#### Nomenclature
- A **literal** and a **literal type** are **distinct** (but related) things.
- A **literal** is a constexpr value that is inserted into the source code.
- A **literal type** is a type that can be used as the type of a constexpr value.

- **A literal always has a literal type. However, a value or object with a literal type need not be a literal.**

#### Some Examples of Literal Types
- Scalar types (those holding a single value, such as fundamental types and pointers)
- Reference types
- Most aggregates
- Classes that have a `constexpr` constructor

- **If you want your class to be able to be evaluated at compile-time, make your member functions and constructor `constexpr`.**
- Implicitly defined constructors are `constexpr` if they can be defined as such. **Explicitly defaulted constructors must be explicitly defined as `constexpr`.**
- **`constexpr` is part of the interface of the class**, and removing it later will break callers who are calling the function in a constant context.
- **`constexpr` members may be needed with non-constexpr/non-const objects.**
- When a `constexpr` function is evaluating in a compile-time context, **only `constexpr` functions can be called**.
- `constexpr` member functions may be `const` or non-`const`.

### Constexpr Non-const Member Functions Can Change Data Members
- A non-const member function **can modify members of non-const objects**.
- A `constexpr` member function **can be called in either runtime contexts or compile-time contexts**.

### Constexpr Functions That Return Const References (or Pointers)
```cpp
constexpr const int& getX() const { return m_x; }
```

A member function that returned a const pointer to const instead might look something like this:
```cpp
constexpr const int* const getXPtr() const { return &m_x; }
```

### 📁 Code Examples
- [`1_constexpr_aggregate.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_15_Constexpr_aggregates_n_classes/1_constexpr_aggregate.cpp) — `constexpr` with aggregate types
- [`2_constexpr_non_aggregate.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_15_Constexpr_aggregates_n_classes/2_constexpr_non_aggregate.cpp) — `constexpr` constructor in non-aggregate class
- [`3_constexpr_members_needed_by_non_constexpr.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_15_Constexpr_aggregates_n_classes/3_constexpr_members_needed_by_non_constexpr.cpp) — `constexpr` members called from non-constexpr context
- [`4_constexpr_non_const_member_can_modify_data.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/14_Introduction_to_classes/14_15_Constexpr_aggregates_n_classes/4_constexpr_non_const_member_can_modify_data.cpp) — `constexpr` non-const member functions modifying data members
