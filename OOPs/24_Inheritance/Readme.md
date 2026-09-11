# Chapter 24: Inheritance

In object-oriented programming, **inheritance** allows a class to acquire the properties, member variables, and behaviors of an existing class, and then extend, specialize, or refine them. Inheritance models an **"is-a"** relationship between classes (e.g. a `Car` *is-a* `Vehicle`), enabling powerful code reuse, modular class hierarchies, and simplified system maintenance.

---

### Table of Contents
1. [1 — Introduction to Inheritance](#1--introduction-to-inheritance)
2. [2 — Basic Inheritance in C++](#2--basic-inheritance-in-c)
3. [3 — Order of Construction of Derived Classes](#3--order-of-construction-of-derived-classes)
4. [4 — Constructors and Initialization of Derived Classes](#4--constructors-and-initialization-of-derived-classes)
5. [5 — Inheritance and Access Specifiers](#5--inheritance-and-access-specifiers)
6. [6 — Adding New Functionality to a Derived Class](#6--adding-new-functionality-to-a-derived-class)
7. [7 — Calling Inherited Functions and Overriding Behavior](#7--calling-inherited-functions-and-overriding-behavior)
8. [8 — Hiding Inherited Functionality](#8--hiding-inherited-functionality)
9. [9 — Multiple Inheritance](#9--multiple-inheritance)

---

## 1 — Introduction to Inheritance

Inheritance involves creating new objects by directly acquiring the attributes and behaviors of existing objects, and then extending or specializing them.

Rather than re-implementing shared logic across disparate classes, inheritance establishes a hierarchical taxonomy where common state and functionality reside in high-level base abstractions, while derived types introduce domain-specific specializations.

---

## 2 — Basic Inheritance in C++

Inheritance in C++ takes place between classes. In an inheritance **("is-a")** relationship:
- The class being inherited from is called the **parent class**, **base class**, or **superclass**.
- The class doing the inheriting is called the **child class**, **derived class**, or **subclass**.

To inherit from a base class, use a colon (`:`), the inheritance access specifier (`public`, `protected`, or `private`), and the name of the base class:

```cpp
class Person
{
public:
    std::string m_name{};
    int m_age{};
};

// Employee is-a Person: publicly inherits all members of Person
class Employee : public Person
{
public:
    double m_hourlySalary{};
    long m_employeeID{};
};
```

### Inheritance Chains

It is possible to inherit from a class that is itself derived from another class, forming an **inheritance chain**:

```cpp
class Supervisor : public Employee
{
public:
    long m_overseesIDs[5]{};
};
```

In this hierarchy, `Supervisor` inherits all members from both `Employee` and `Person`. General properties (`name`, `age`) reside at the top of the tree, while specific specializations (`hourlySalary`, `overseesIDs`) are added progressively at lower levels.

### Why is Inheritance Useful?

1. **Avoids redundant code**: Common fields and member functions do not need to be duplicated across multiple classes.
2. **Automatic propagation of bug fixes and enhancements**: When a base class member function is updated, refined, or patched, all classes derived from it automatically receive those updates.
3. **Natural real-world modeling**: Accurately reflects real-world domain taxonomies and classifications.

### 📁 Code Examples for Section 2
- [`24_2_Constructors_n_initialization_of_derived_classes/1_initialization_of_classes_in_inheritance.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_2_Constructors_n_initialization_of_derived_classes/1_initialization_of_classes_in_inheritance.cpp): Basic inheritance setup demonstrating how a derived class receives base class members.
- [`24_2_Constructors_n_initialization_of_derived_classes/2_inheritance_chain.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_2_Constructors_n_initialization_of_derived_classes/2_inheritance_chain.cpp): Demonstrates a multi-tier inheritance chain (`A` -> `B` -> `C`) passing data up the hierarchy.

---

## 3 — Order of Construction of Derived Classes

Because a derived class inherits functions and variables from its base class, one might assume that the members of `Base` are copied into `Derived`. However, this is not the case. Instead, an instance of `Derived` is internally structured as a **two-part object**:
1. One part **`Base`**
2. One part **`Derived`**

```
┌──────────────────────────────────────┐
│        Derived Class Instance        │
│  ┌────────────────────────────────┐  │
│  │   Base Subobject Portion       │  │  <--- Constructed FIRST
│  └────────────────────────────────┘  │
│  ┌────────────────────────────────┐  │
│  │   Derived Subobject Portion    │  │  <--- Constructed SECOND
│  └────────────────────────────────┘  │
└──────────────────────────────────────┘
```

### Construction Happens in Phases (Top-Down)

When C++ constructs a derived object, it does so in phases:
1. The **most-base class** (at the very top of the inheritance tree) is constructed first.
2. Each child class is constructed in order down the tree.
3. The **most-derived class** (at the bottom of the inheritance tree) is constructed last.

```cpp
class Base
{
public:
    Base() { std::cout << "Base constructed\n"; }
};

class Derived : public Base
{
public:
    Derived() { std::cout << "Derived constructed\n"; }
};

Derived d;
// Output:
// Base constructed
// Derived constructed
```

### Why Does C++ Construct from the Base Down?

The derived class often references, manipulates, and relies upon variables and member functions inherited from the base class, but the base class knows nothing about the derived class. Instantiating the base class first ensures all base members and invariants are **fully initialized and ready for use** by the time the derived constructor body executes.

### 📁 Code Examples for Section 3
- [`24_1_Order_of_construction_of_derived_classes/1_order_of_constructors.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_1_Order_of_construction_of_derived_classes/1_order_of_constructors.cpp): Demonstrates the step-by-step phased execution order of base and derived constructors.

---

## 4 — Constructors and Initialization of Derived Classes

Before a derived class constructor body can execute, the base class constructor is called first. The base constructor sets up the base portion of the object, control returns to the derived constructor, and the derived constructor initializes its own members.

### Why Derived Member Initializer Lists Cannot Directly Initialize Base Members

C++ **strictly prevents** derived classes from directly initializing inherited base member variables in the derived constructor's member initializer list:

```cpp
class Base
{
public:
    int m_id{};
};

class Derived : public Base
{
public:
    // COMPILE ERROR: m_id does not belong directly to Derived!
    // Derived(int id) : m_id{id} {} 
};
```

#### Why Does C++ Enforce This Rule?
The answer relates to **`const` and reference members**:
- If a base class member is `const`, it must be initialized at the exact moment of creation inside the base constructor.
- If derived classes were permitted to initialize inherited members in their own member initializer lists, the base constructor would initialize the `const` member, and then the derived member initializer list would re-initialize it, altering a constant value!
- By restricting initialization of member variables exclusively to the constructor of the class where they are declared, C++ guarantees that **all variables are initialized exactly once**.

### Explicitly Selecting a Base Class Constructor

To initialize base class members with specific values, **explicitly call the desired base class constructor** in the derived class's member initializer list:

```cpp
class Base
{
    int m_id{};
public:
    Base(int id = 0) : m_id{id} {}
};

class Derived : public Base
{
    double m_cost{};
public:
    // Explicitly delegates base initialization to Base(int):
    Derived(double cost = 0.0, int id = 0)
        : Base{id}       // Call Base constructor
        , m_cost{cost}   // Initialize Derived member
    {
    }
};
```

> [!NOTE]
> It does not matter where in the derived member initializer list the base constructor call appears—the base constructor **will always execute first** before any derived member initializers.

### Constructors in Inheritance Chains

Constructors can only directly invoke constructors of their **immediate parent class**. For instance, in an inheritance chain `A` -> `B` -> `C`:
- The `C` constructor cannot call `A` directly.
- `C` calls the `B` constructor, which in turn calls the `A` constructor.

### Destruction Order (Bottom-Up)

When a derived object is destroyed, destructors execute in the **exact reverse order of construction**:
1. Most-derived destructor runs first (`C`).
2. Immediate parent destructor runs next (`B`).
3. Most-base destructor runs last (`A`).

```
Construction:  Base  ───>  Derived
Destruction:   Base  <───  Derived
```

> [!WARNING]
> If a base class is intended to be used polymorphically (has virtual member functions), **its destructor must also be declared `virtual`**! Otherwise, deleting a derived object via a base class pointer causes **undefined behavior** because the derived destructor will never execute.

### Inheriting Constructors (C++11)

When a derived class has no member variables of its own (or when all its member variables have default member initializers), writing boilerplate constructors just to forward arguments to base constructors is tedious:

```cpp
class Derived : public Base
{
public:
    // C++11 Inheriting constructors:
    using Base::Base; // Makes all Base constructors available to construct Derived!
};
```

- Inherited constructors initialize base subobjects via the chosen base constructor.
- Any derived class member variables receive their **default member initializers**.
- Default, copy, and move constructors are not inherited; they are generated or handled according to standard class rules.
- If Derived defines a custom constructor with the exact same signature as a Base constructor, the Derived version takes precedence.

### 📁 Code Examples for Section 4
- [`24_2_Constructors_n_initialization_of_derived_classes/1_initialization_of_classes_in_inheritance.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_2_Constructors_n_initialization_of_derived_classes/1_initialization_of_classes_in_inheritance.cpp): Demonstrates initializing base class members by explicitly invoking base constructors in the derived member initializer list.
- [`24_2_Constructors_n_initialization_of_derived_classes/2_inheritance_chain.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_2_Constructors_n_initialization_of_derived_classes/2_inheritance_chain.cpp): Demonstrates multi-level constructor cascading across an inheritance chain and verifies destruction order.
- [`24_2_Constructors_n_initialization_of_derived_classes/3_inheriting_constructors.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_2_Constructors_n_initialization_of_derived_classes/3_inheriting_constructors.cpp): Demonstrates C++11 inheriting constructors using `using Base::Base;`, eliminating boilerplate forwarding constructors.

---

## 5 — Inheritance and Access Specifiers

Access specifiers determine how members are accessed from within the class hierarchy and by external callers.

### The `protected` Access Specifier

The `protected` access specifier provides an intermediate access tier:
- Accessible by the **class's own member functions**.
- Accessible by the class's **friends**.
- Accessible by **derived classes**.
- **Inaccessible** to external callers / public scope.

> [!TIP]
> **Favor private members over protected members**. Private members ensure strict encapsulation. When members are protected, any derived class can directly mutate base state, increasing coupling and making future modifications to base class internals much harder to maintain.

### Public, Protected, and Private Inheritance Modes

When inheriting from a base class, the inheritance access specifier determines the visibility of inherited members in the derived class:

```cpp
class Derived1 : public Base    { /* ... */ };
class Derived2 : protected Base { /* ... */ };
class Derived3 : private Base   { /* ... */ };
```

- If no inheritance specifier is provided, **C++ defaults to private inheritance** for classes (`class Derived : Base`), and **public inheritance** for structs.

#### Access Level Transformation Matrix

| Access in Base Class | Inherited Publicly (`public Base`) | Inherited Privately (`private Base`) | Inherited Protectedly (`protected Base`) |
|---|:---:|:---:|:---:|
| **Public** | **Public** | **Private** | **Protected** |
| **Protected** | **Protected** | **Private** | **Protected** |
| **Private** | *Inaccessible* | *Inaccessible* | *Inaccessible* |

### Summary of Inheritance Types
- **Public Inheritance**: Used in 99% of object-oriented designs. Represents an authentic "is-a" relationship. Inherited public members remain public.
- **Private Inheritance**: Used for internal implementation details ("implemented-in-terms-of"). All inherited members become private to the derived class.
- **Protected Inheritance**: Extremely rare; makes inherited public and protected members protected in the derived class.

### 📁 Code Examples for Section 5
- [`24_3_Inheritance_n_access_specifiers/1_public_inheritance.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_3_Inheritance_n_access_specifiers/1_public_inheritance.cpp): Demonstrates access levels under public inheritance.
- [`24_3_Inheritance_n_access_specifiers/2_protected_inheritance.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_3_Inheritance_n_access_specifiers/2_protected_inheritance.cpp): Demonstrates access levels under protected inheritance.
- [`24_3_Inheritance_n_access_specifiers/3_private_inheritance.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_3_Inheritance_n_access_specifiers/3_private_inheritance.cpp): Demonstrates access levels under private inheritance.
- [`24_3_Inheritance_n_access_specifiers/4_inheritance_example.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_3_Inheritance_n_access_specifiers/4_inheritance_example.cpp): Comprehensive comparative example contrasting member accessibility across various inheritance configurations.

---

## 6 — Adding New Functionality to a Derived Class

Derived classes can define new member variables and member functions beyond what is inherited from the base class:

```cpp
class Base
{
public:
    void print() { std::cout << "Base\n"; }
};

class Derived : public Base
{
public:
    void display() { std::cout << "Derived extension\n"; }
};
```

Because `Derived` *is-a* `Base`, an instance of `Derived` has access to both `print()` and `display()`. However, `Base` has **no access to anything in `Derived`**. Inheritance is strictly **unidirectional**: base knows nothing about derived.

---

## 7 — Calling Inherited Functions and Overriding Behavior

When a member function is called on a derived object:
1. The compiler first searches for that function name in the **derived class**.
2. If at least one function with that name exists in the derived class, **name lookup stops immediately** in the derived class! The compiler then performs overload resolution among the functions found in the derived class.
3. If no function with that name is found, the compiler walks up the inheritance chain to the base class and repeats the search.

```cpp
class Base
{
public:
    void identify() { std::cout << "I am a Base\n"; }
};

class Derived : public Base
{
public:
    void identify() { std::cout << "I am a Derived\n"; } // Redefines identify()
};
```

### Calling Base Functions from Derived Class Using Scope Resolution (`::`)

If a derived function needs to execute the base class implementation of a redefined function, use the **scope resolution operator** (`Base::`):

```cpp
void Derived::identify()
{
    Base::identify(); // Calls base class implementation
    std::cout << "I am also a Derived\n";
}
```

> [!CAUTION]
> If you omit the `Base::` qualifier and simply write `identify();` inside `Derived::identify()`, the function will recursively call itself, resulting in **infinite recursion and a stack overflow crash**!

### Calling Friend Functions of Base Classes from Derived Classes

Friend functions are **not member functions** of the class, so they are not inherited, and they cannot be called using `Base::operator<<`.

To call a base class friend function from a derived class friend function:
- **Solution**: Upcast the derived object reference to a base class reference using **`static_cast<const Base&>(derived_object)`**:

```cpp
class Base
{
public:
    friend std::ostream& operator<<(std::ostream& out, const Base& b)
    {
        out << "In Base";
        return out;
    }
};

class Derived : public Base
{
public:
    friend std::ostream& operator<<(std::ostream& out, const Derived& d)
    {
        out << "In Derived: ";
        // Upcast d to const Base& so Base::operator<< is selected!
        out << static_cast<const Base&>(d);
        return out;
    }
};
```

### Overload Resolution Across Inheritance Boundaries

> [!WARNING]
> In C++, **name lookup occurs before overload resolution**.
> If a derived class defines any function with the same name as a base class function, it **hides all overloads** of that function in the base class, even if the base overloads have different parameter lists!

```cpp
class Base
{
public:
    void print(int x)    { std::cout << "Base: " << x << '\n'; }
    void print(double d) { std::cout << "Base: " << d << '\n'; }
};

class Derived : public Base
{
public:
    void print(double d) { std::cout << "Derived: " << d << '\n'; }
};

Derived d;
// d.print(5); // Calls Derived::print(double)! The int overload in Base is HIDDEN!
```

To make all base class overloads visible in the derived class, use a **`using` declaration**:
```cpp
class Derived : public Base
{
public:
    using Base::print; // Brings all Base::print overloads into Derived scope!
    void print(double d) { std::cout << "Derived: " << d << '\n'; }
};
```

### 📁 Code Examples for Section 7
- [`24_4_Calling_inherited_functions_n_overriding_behavior/1_calling_base_class_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_4_Calling_inherited_functions_n_overriding_behavior/1_calling_base_class_function.cpp): Demonstrates directly invoking inherited base member functions on derived objects.
- [`24_4_Calling_inherited_functions_n_overriding_behavior/2_calling_base_class_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_4_Calling_inherited_functions_n_overriding_behavior/2_calling_base_class_function.cpp): Demonstrates redefining a member function in a derived class.
- [`24_4_Calling_inherited_functions_n_overriding_behavior/3_calling_base_class_function_from_Derived_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_4_Calling_inherited_functions_n_overriding_behavior/3_calling_base_class_function_from_Derived_function.cpp): Demonstrates using `Base::function()` to prevent recursive self-calling.
- [`24_4_Calling_inherited_functions_n_overriding_behavior/4_scope_operator_does_not_call_friend_of_base_from_Derived.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_4_Calling_inherited_functions_n_overriding_behavior/4_scope_operator_does_not_call_friend_of_base_from_Derived.cpp): Demonstrates invoking base class friend `operator<<` by upcasting derived objects with `static_cast<const Base&>(ref)`.
- [`24_4_Calling_inherited_functions_n_overriding_behavior/5_overload_resolution_in_derived.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_4_Calling_inherited_functions_n_overriding_behavior/5_overload_resolution_in_derived.cpp): Illustrates how derived class functions hide base class overloads during name lookup.
- [`24_4_Calling_inherited_functions_n_overriding_behavior/6_overload_resolution_in_derived.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_4_Calling_inherited_functions_n_overriding_behavior/6_overload_resolution_in_derived.cpp): Explores compiler resolution choices when base and derived share function names.
- [`24_4_Calling_inherited_functions_n_overriding_behavior/7_overload_resolution_in_derived.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_4_Calling_inherited_functions_n_overriding_behavior/7_overload_resolution_in_derived.cpp): Demonstrates bringing hidden base overloads back into scope using `using Base::function;`.

---

## 8 — Hiding Inherited Functionality

### Changing an Inherited Member's Access Level

You can alter the access specifier of an inherited member in the derived class using a **`using` declaration**:

```cpp
class Base
{
protected:
    int m_value{};
};

class Derived : public Base
{
public:
    // Exposes m_value publicly in Derived!
    using Base::m_value; 
};
```

> [!IMPORTANT]
> You can **only change the access specifier of members the derived class would normally be able to access**. You cannot change a `private` base member into a `protected` or `public` member, because derived classes cannot access private base members in the first place.

### Hiding Functionality in Derived Classes

Base functionality can be hidden in derived classes by placing a `using` declaration in the `private:` section of the derived class:

```cpp
class Base
{
public:
    void print() { std::cout << "Base\n"; }
};

class Derived : public Base
{
private:
    using Base::print; // Hide print() through Derived!
};

Derived d;
// d.print(); // Compile ERROR: print() is private in Derived!
```

#### Subverting Hidden Functionality via Upcasting

> [!CAUTION]
> Making a public base member private in a derived class **does not truly secure the member**!
> The member is still public in the base class. Anyone can easily bypass the derived class's private encapsulation simply by casting to a base class reference:
> ```cpp
> static_cast<Base&>(d).print(); // Bypasses Derived access control completely!
> ```

### Deleting Functions in Derived Classes

You can also explicitly mark inherited member functions as deleted using **`= delete`**:

```cpp
class Derived : public Base
{
public:
    void print() = delete; // Cannot be called on Derived
};

Derived d;
// d.print(); // Compile error: use of deleted function
static_cast<Base&>(d).print(); // Still callable via Base!
```

### 📁 Code Examples for Section 8
- [`24_5_Hiding_inherited_functionality/1_change_access_levels_in_derived_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_5_Hiding_inherited_functionality/1_change_access_levels_in_derived_class.cpp): Demonstrates promoting a protected base member to public access in a derived class via `using Base::function;`.
- [`24_5_Hiding_inherited_functionality/2_hide_functionality_in_derived_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_5_Hiding_inherited_functionality/2_hide_functionality_in_derived_class.cpp): Demonstrates hiding public base members by pulling them into the `private:` section of the derived class.
- [`24_5_Hiding_inherited_functionality/3_hide_functionality_in_derived_class_subvert.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_5_Hiding_inherited_functionality/3_hide_functionality_in_derived_class_subvert.cpp): Demonstrates subverting derived class hiding by casting to `Base&`.
- [`24_5_Hiding_inherited_functionality/4_using_applies_to_all_overloaded_functions_in_derived_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_5_Hiding_inherited_functionality/4_using_applies_to_all_overloaded_functions_in_derived_class.cpp): Shows that `using` declarations apply across all overloads sharing the same identifier.
- [`24_5_Hiding_inherited_functionality/5_delete_function_in_derived_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_5_Hiding_inherited_functionality/5_delete_function_in_derived_class.cpp): Demonstrates marking inherited base functions as `= delete` in the derived class.

---

## 9 — Multiple Inheritance

**Multiple inheritance** allows a derived class to inherit members from more than one base class:

```cpp
class Teacher { /* ... */ };
class Researcher { /* ... */ };

// Professor inherits from both Teacher AND Researcher:
class Professor : public Teacher, public Researcher
{
    // ...
};
```

### Mixins and CRTP

A **mixin** is a small, specialized class designed to be mixed into other classes via inheritance to add specific behaviors without being instantiated on its own. Mixins typically do not use virtual functions.

Mixins frequently employ the **Curiously Recurring Template Pattern (CRTP)**, where a derived class inherits from a templated base class instantiated with the derived class itself:

```cpp
template <typename Derived>
class Cloneable
{
public:
    Derived clone() const
    {
        // Upcasts 'this' to the concrete Derived type at compile time:
        return static_cast<const Derived&>(*this);
    }
};

class MyClass : public Cloneable<MyClass>
{
    // ...
};
```

### Problems with Multiple Inheritance

1. **Ambiguity**: If multiple base classes declare a function with the same name, calling that function on the derived object produces a compiler ambiguity error:
   ```cpp
   class USBDevice { public: long getID(); };
   class WirelessDevice { public: long getID(); };
   class WirelessDongle : public USBDevice, public WirelessDevice {};

   WirelessDongle d;
   // d.getID(); // Compile ERROR: Ambiguous! USBDevice::getID() or WirelessDevice::getID()?
   d.USBDevice::getID(); // OK: Explicitly disambiguated
   ```

2. **The Diamond Problem**: Occurs when two classes inherit from a common base class, and a derived class inherits from both of them:

```
        ┌──────────┐
        │  Device  │
        └──────────┘
         /        \
┌───────────┐  ┌────────────────┐
│ USBDevice │  │ WirelessDevice │
└───────────┘  └────────────────┘
         \        /
       ┌────────────┐
       │ Copier/Hub │  <--- Contains TWO separate copies of Device!
       └────────────┘
```

In a standard diamond hierarchy, the bottom class receives **two separate copies** of the topmost `Device` subobject, causing memory waste and ambiguity.

> [!NOTE]
> C++ solves the diamond problem through **virtual base classes** (virtual inheritance: `class USBDevice : virtual public Device`), which guarantees that only a single, shared instance of the topmost base subobject exists in the final derived object.

### Guideline: When to Use Multiple Inheritance

> [!TIP]
> **Avoid multiple inheritance unless alternatives lead to greater complexity.** Most designs can be modeled cleanly using single inheritance combined with composition. However, multiple inheritance is safely and effectively utilized in:
> - Standard library implementations (such as `std::iostream`, which multiply inherits from `std::istream` and `std::ostream`).
> - Lightweight mixin utility classes.

### 📁 Code Examples for Section 9
- [`24_6_Multiple_inheritance/1_multiple_inheritance.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_6_Multiple_inheritance/1_multiple_inheritance.cpp): Demonstrates basic syntax for multiple inheritance and constructor invocation order.
- [`24_6_Multiple_inheritance/2_multiple_inheritance_ambiguity.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/24_Inheritance/24_6_Multiple_inheritance/2_multiple_inheritance_ambiguity.cpp): Demonstrates resolving member function naming collisions between multiple base classes using explicit scope resolution.
