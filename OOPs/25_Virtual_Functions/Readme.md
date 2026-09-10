# Chapter 25: Virtual Functions

Virtual functions and polymorphism are among the most powerful and fundamental capabilities of object-oriented programming in C++. While inheritance allows derived classes to reuse and extend base class code, virtual functions allow client code to interact with entire inheritance hierarchies through a uniform base-class interface, dynamically executing the most-derived behavior at runtime without needing to know the exact concrete type at compile time.

---

### Table of Contents
1. [1 — Pointers and references to the base class of derived objects](#1--pointers-and-references-to-the-base-class-of-derived-objects)
2. [2 — Virtual functions and polymorphism](#2--virtual-functions-and-polymorphism)
3. [3 — The override and final specifiers, and covariant return types](#3--the-override-and-final-specifiers-and-covariant-return-types)
4. [4 — Virtual destructors, virtual assignment, and overriding virtualization](#4--virtual-destructors-virtual-assignment-and-overriding-virtualization)
5. [5 — Early binding and late binding](#5--early-binding-and-late-binding)
6. [6 — The virtual table](#6--the-virtual-table)
7. [7 — Pure virtual functions, abstract base classes, and interface classes](#7--pure-virtual-functions-abstract-base-classes-and-interface-classes)
8. [8 — Virtual base classes](#8--virtual-base-classes)
9. [9 — Object slicing](#9--object-slicing)
10. [10 — Dynamic casting](#10--dynamic-casting)
11. [11 — Printing inherited classes using operator<<](#11--printing-inherited-classes-using-operator)

---

## 1 — Pointers and references to the base class of derived objects

One of the most important and powerful aspects of inheritance is **virtual functions**.

Remember that inheritance implies an **is-a relationship** between two classes. Since a `Derived` class is-a `Base` class, it is completely appropriate that `Derived` contains a `Base` part. Because of this relationship, C++ permits us to set a `Base` pointer or reference to a `Derived` object:

```cpp
Derived derived{};
Base* basePtr{ &derived }; // Valid: Base pointer points to Derived object
Base& baseRef{ derived };  // Valid: Base reference binds to Derived object
```

### Function Hiding vs Function Overriding

In C++, a member function is **overridden** only if the base class function is explicitly marked with the `virtual` keyword.
- If a function is **not virtual**, a function in a derived class with the same name and signature does **not** override it — it **hides** the base version.
- In **function hiding**, the derived function is **not visible** through a base pointer or reference.
- Consequently, when a base pointer or reference points to a derived object, it can **only access the base version** of the function, because dynamic dispatch does not occur without `virtual`.

```cpp
class Base {
public:
    std::string_view getName() const { return "Base"; }
};

class Derived : public Base {
public:
    std::string_view getName() const { return "Derived"; } // Hides Base::getName()
};

Derived d{};
Base& rBase{ d };
std::cout << rBase.getName(); // Prints "Base" (not polymorphic!)
```

### Why would I set a pointer or reference to the base class?

We use base-class pointers and references to enable **polymorphism**:
- They let us write **one generic function** that works for all derived types in a hierarchy.
- They help us avoid writing dozens of overloaded functions for every newly added derived class.
- They allow storing different derived objects in a **single heterogeneous container** (such as an array or `std::vector` of base pointers or `std::reference_wrapper`).

Without base pointers, we would need separate functions and separate arrays for every derived type:

```cpp
// Without base references: requires duplicate code for every animal type
void report(const Cat& cat) { std::cout << cat.getName() << " says " << cat.speak() << '\n'; }
void report(const Dog& dog) { std::cout << dog.getName() << " says " << dog.speak() << '\n'; }

// With base references: one single function handles all derived types polymorphically!
void report(const Animal& animal) { std::cout << animal.getName() << " says " << animal.speak() << '\n'; }
```

> [!NOTE]
> A template could remove overloads, but it doesn't enforce the type hierarchy or document intent, and it accepts any arbitrary type that happens to implement matching member function names. Furthermore, templates cannot store different derived types together in a single runtime collection.
> However, a normal base pointer calls only base-class functions — which is why we make functions **virtual**, so the correct derived behavior is invoked through the base interface.

### 📁 Code Examples for Section 1
- [`25_1_Pointers_n_references_to_base_class_of_derived_objects/Function_Hiding/animal.hpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_1_Pointers_n_references_to_base_class_of_derived_objects/Function_Hiding/animal.hpp): Defines the `Animal`, `Dog`, and `Cat` classes with non-virtual member functions where copy operations are deleted to prevent slicing.
- [`25_1_Pointers_n_references_to_base_class_of_derived_objects/Function_Hiding/main.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_1_Pointers_n_references_to_base_class_of_derived_objects/Function_Hiding/main.cpp): Demonstrates binding base references and pointers to derived objects, illustrating function hiding where `rAnimal.speak()` invokes `Animal::speak()` instead of derived implementations.
- [`25_1_Pointers_n_references_to_base_class_of_derived_objects/Function_Hiding/1_arrays_for_derived_types.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_1_Pointers_n_references_to_base_class_of_derived_objects/Function_Hiding/1_arrays_for_derived_types.cpp): Demonstrates the downside of lacking base pointers: separate arrays (`std::array<Cat, 3>` and `std::array<Dog, 3>`) and separate loops are required for each derived type.
- [`25_1_Pointers_n_references_to_base_class_of_derived_objects/Function_Hiding/2_arrays_for_derived_types.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_1_Pointers_n_references_to_base_class_of_derived_objects/Function_Hiding/2_arrays_for_derived_types.cpp): Demonstrates storing base pointers (`const Animal*`) in a single array; shows that without virtual functions, iterating calls the base `Animal::speak()` for all elements.

---

## 2 — Virtual functions and polymorphism

### Virtual Functions
A **virtual function** is a special type of member function that, when called, resolves to the **most-derived version** of the function for the actual type of the object being referenced or pointed to.

To make a function virtual, simply place the `virtual` keyword before the function declaration in the base class:

```cpp
class Base {
public:
    virtual std::string_view getName() const { return "Base"; }
};

class Derived : public Base {
public:
    std::string_view getName() const override { return "Derived"; }
};
```

### Overrides
A derived function is considered a match if it has the **same signature** (function name, parameter types, and `const` qualifier) and return type as the base version of the function. Such functions are called **overrides**.

> [!NOTE]
> Some modern compilers may issue a warning or error about having virtual functions and an accessible non-virtual destructor. If this is the case, add a **virtual destructor** to the base class (`virtual ~Base() = default;`).

### How Virtual Resolution Operates
- **Through pointer or reference only**: Virtual function resolution only works when a virtual member function is called through a **pointer** or **reference** to a class type object. This works because the compiler can differentiate the static type of the pointer/reference from the dynamic type of the object being pointed to or referenced.
- **Direct object calls**: Calling a virtual member function directly on an object (not through a pointer or reference) will **always** invoke the member function belonging to the compile-time type of that object, bypassing dynamic dispatch entirely.

```cpp
Derived d{};
Base b{ d }; // Slicing occurs!
b.getName(); // Calls Base::getName() directly (no dynamic dispatch on direct objects)

Base& rBase{ d };
rBase.getName(); // Calls Derived::getName() via dynamic dispatch!
```

### Polymorphism: Compile-Time vs Runtime

In programming, **polymorphism** refers to the ability of an entity to take on multiple forms (the term literally means "many forms").

| Feature | Compile-Time Polymorphism (Static) | Runtime Polymorphism (Dynamic) |
|---|---|---|
| **Resolution Time** | Resolved at compile time | Resolved at runtime |
| **Mechanisms** | Function overloading, operator overloading, templates | Virtual functions, dynamic dispatch via vtable |
| **Flexibility** | Types must be known at compile time | Concrete types determined dynamically at runtime |
| **Overhead** | Zero runtime performance cost | Extra pointer per object (`vptr`), indirect function calls via vtable |
| **Container Storage** | Homogeneous containers | Heterogeneous hierarchies via base pointers/references |

The biggest architectural benefit of virtual functions is the ability to structure your code in such a way that **newly derived classes will automatically work with existing code without requiring modifications**!

### A Word of Warning: Signature Matching
- The signature of the derived class function must **exactly match** the signature of the base class virtual function in order for the derived class function to override it.
- If the derived class function has different parameter types, the program will likely still compile fine, but the compiler will treat it as an unrelated function that **hides** the base virtual function rather than overriding it.
- If a function is declared `virtual` in a base class, all matching overrides in derived classes are **implicitly virtual**, whether or not the `virtual` keyword is repeated.
- However, this does **not** work the other way around — a virtual override in a derived class does not retroactively make the base class function virtual.

### Return Types of Virtual Functions
Under normal circumstances (with the single exception of **covariant return types**, discussed in Section 3), the return type of a virtual function and all of its derived overrides must match exactly.

### Do Not Call Virtual Functions from Constructors or Destructors

> [!WARNING]
> **Never call virtual functions from constructors or destructors.**
> - When a `Derived` class object is constructed, the `Base` portion is constructed first. If you call a virtual function from the `Base` constructor, the `Derived` portion has not yet been initialized. Therefore, C++ cannot safely call the `Derived` override because there is no valid `Derived` object for the function to execute on. In C++, the call will resolve to the **`Base` version** instead.
> - A similar issue exists for destructors. If you call a virtual function in a `Base` class destructor, it will always resolve to the **`Base` class version**, because the `Derived` portion of the object has already been destroyed!
> - Calling a pure virtual function from a base constructor or destructor results in **undefined behavior** (and typically an immediate crash, such as `pure virtual method called`).

### The Downside of Virtual Functions
Since virtual functions are so powerful, why not make every member function virtual?
1. **Efficiency**: Resolving a virtual function call takes longer than resolving a regular direct function call because it requires navigating an extra level of indirection through the virtual table.
2. **Object Memory Overhead**: To make virtual functions work, the compiler must allocate an extra hidden pointer (`*__vptr`) for every single object of a class that has virtual functions. This adds 8 bytes (on 64-bit systems) to every instance, which is significant overhead for small lightweight objects (like a 2D `Point` with two integers).

### 📁 Code Examples for Section 2
- [`25_2_Virtual functions_n_polymorphism/1_polymorphism.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_2_Virtual%20functions_n_polymorphism/1_polymorphism.cpp): Shows basic dynamic dispatch with a `Base` class declaring `virtual std::string_view getName() const` and `Derived` providing an override.
- [`25_2_Virtual functions_n_polymorphism/Function_Hiding_to_Overriding_using_virtual_functions/animal.hpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_2_Virtual%20functions_n_polymorphism/Function_Hiding_to_Overriding_using_virtual_functions/animal.hpp): Defines the `Animal` hierarchy with `virtual std::string_view speak() const` to replace function hiding with true virtual overriding.
- [`25_2_Virtual functions_n_polymorphism/Function_Hiding_to_Overriding_using_virtual_functions/main.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_2_Virtual%20functions_n_polymorphism/Function_Hiding_to_Overriding_using_virtual_functions/main.cpp): Demonstrates invoking `rAnimal.speak()` where calls dynamically dispatch to `Dog::speak()` and `Cat::speak()` at runtime.
- [`25_2_Virtual functions_n_polymorphism/Function_Hiding_to_Overriding_using_virtual_functions/1_arrays_for_derived_types.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_2_Virtual%20functions_n_polymorphism/Function_Hiding_to_Overriding_using_virtual_functions/1_arrays_for_derived_types.cpp): Shows standard fixed array usage for derived types.
- [`25_2_Virtual functions_n_polymorphism/Function_Hiding_to_Overriding_using_virtual_functions/2_arrays_for_derived_types.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_2_Virtual%20functions_n_polymorphism/Function_Hiding_to_Overriding_using_virtual_functions/2_arrays_for_derived_types.cpp): Demonstrates iterating through an array of base pointers (`const Animal*`) and achieving polymorphic behavior via virtual function dispatch.

---

## 3 — The override and final specifiers, and covariant return types

To address common challenges with inheritance, C++ provides two inheritance-related identifiers: **`override`** and **`final`** (introduced in C++11).
The C++ standard classifies them as "identifiers with special meaning" (contextual keywords) rather than reserved keywords, meaning they only have special compiler significance when placed in member function declarations.

### The `override` Specifier

To prevent subtle bugs where functions are meant to be overrides but fail to match base signatures, the `override` specifier tells the compiler to explicitly enforce that the function is overriding a base virtual function:

```cpp
class Base {
public:
    virtual std::string_view getName1(int x) { return "Base"; }
    virtual std::string_view getName2(int x) const { return "Base"; }
};

class Derived : public Base {
public:
    // COMPILE ERROR: parameter short does not match int in Base; compiler catches the mistake!
    // std::string_view getName1(short x) override { return "Derived"; }

    std::string_view getName1(int x) override { return "Derived"; } // OK: matches exact signature!
};
```

Key rules for `override`:
- It is placed at the end of the member function declaration (in the same position as `const`).
- If the member function is `const`, the `const` qualifier **must precede `override`**: `void foo() const override`.
- If a function marked with `override` does not match an existing base class virtual function, the compiler emits a compile error.
- There is **zero performance penalty** for using `override`.
- The `override` specifier **implies `virtual`**. Therefore, you do not need to prepend the `virtual` keyword when `override` is present.

> [!TIP]
> **Best Practice Rule:**
> - Use the `virtual` keyword on virtual functions in the base class.
> - Use the `override` specifier (and omit the `virtual` keyword) on all overriding functions in derived classes. This includes virtual destructors.

### The `final` Specifier

The `final` specifier tells the compiler that a virtual member function cannot be overridden, or that a class cannot be inherited from:

```cpp
// 1. Preventing function overriding:
class A {
public:
    virtual std::string_view getName() const { return "A"; }
};

class B : public A {
public:
    std::string_view getName() const override final { return "B"; } // Overrides A, but cannot be overridden further!
};

class C : public B {
public:
    // COMPILE ERROR: cannot override final function B::getName()
    // std::string_view getName() const override { return "C"; }
};

// 2. Preventing class inheritance:
class Sealed final : public A {
    // This entire class cannot be inherited from!
};

// COMPILE ERROR: cannot derive from final class Sealed
// class SubSealed : public Sealed {};
```

### Covariant Return Types

There is one special case where an overriding virtual function in a derived class can have a different return type than the base function: **covariant return types**.

If the return type of a virtual function is a **pointer or reference to some class `Base`**, an override function in a derived class can return a **pointer or reference to a derived class `Derived`**:

```cpp
class Base {
public:
    virtual Base* getThis() { return this; }
    virtual void printType() const { std::cout << "Base\n"; }
};

class Derived : public Base {
public:
    // Covariant return type: returns Derived* instead of Base*!
    Derived* getThis() override { return this; }
    void printType() const override { std::cout << "Derived\n"; }
};
```

Important details regarding covariant return types:
- **No dynamic adjustment**: C++ cannot dynamically "compute" or "adjust" return types at runtime. Whichever virtual function is actually invoked at runtime, its own declared return type is returned.
- When calling through a `Derived` pointer/reference, the return type is statically known as `Derived*`, allowing direct access to derived-specific members without casting:
  ```cpp
  Derived d{};
  Derived* dPtr = d.getThis(); // Statically typed as Derived*, no cast required!
  
  Base* bPtr = &d;
  Base* resPtr = bPtr->getThis(); // Statically typed as Base*, pointing to Derived object
  ```
- Covariant return types can be used whenever the return type of the override function is publicly derived from the return type of the base virtual function.

### 📁 Code Examples for Section 3
- [`25_3_The_override_n_final_specifiers_n_covariant_return_types/1_override specifier_use.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_3_The_override_n_final_specifiers_n_covariant_return_types/1_override%20specifier_use.cpp): Demonstrates how the `override` specifier catches signature mismatches (such as parameter type or constness discrepancies) at compile time.
- [`25_3_The_override_n_final_specifiers_n_covariant_return_types/2_final_specifier_use_on_funtion.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_3_The_override_n_final_specifiers_n_covariant_return_types/2_final_specifier_use_on_funtion.cpp): Demonstrates preventing further overriding of a virtual member function using `override final`.
- [`25_3_The_override_n_final_specifiers_n_covariant_return_types/3_final_specifier_use_on_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_3_The_override_n_final_specifiers_n_covariant_return_types/3_final_specifier_use_on_class.cpp): Demonstrates sealing an entire class using the `final` specifier so that no derived class can inherit from it.
- [`25_3_The_override_n_final_specifiers_n_covariant_return_types/covariant_types/1_covariant_types.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_3_The_override_n_final_specifiers_n_covariant_return_types/covariant_types/1_covariant_types.cpp): Shows basic covariant return types where `Derived::getThis()` returns `Derived*` while overriding `Base* Base::getThis()`.
- [`25_3_The_override_n_final_specifiers_n_covariant_return_types/covariant_types/2_covariant_types.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_3_The_override_n_final_specifiers_n_covariant_return_types/covariant_types/2_covariant_types.cpp): Demonstrates practical benefits of covariant return types, showing that calling `getThis()` on a derived object returns a pointer typed to `Derived*`.
- [`25_3_The_override_n_final_specifiers_n_covariant_return_types/Overriding_using_virtual_functions_with_override_keyword/animal.hpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_3_The_override_n_final_specifiers_n_covariant_return_types/Overriding_using_virtual_functions_with_override_keyword/animal.hpp): Header declaring the `Animal` hierarchy using modern best practices with `virtual` in base and `override` in derived classes.
- [`25_3_The_override_n_final_specifiers_n_covariant_return_types/Overriding_using_virtual_functions_with_override_keyword/main.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_3_The_override_n_final_specifiers_n_covariant_return_types/Overriding_using_virtual_functions_with_override_keyword/main.cpp): Tests the hierarchy configured with the `override` keyword.
- [`25_3_The_override_n_final_specifiers_n_covariant_return_types/Overriding_using_virtual_functions_with_override_keyword/1_arrays_for_derived_types.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_3_The_override_n_final_specifiers_n_covariant_return_types/Overriding_using_virtual_functions_with_override_keyword/1_arrays_for_derived_types.cpp): Concrete array test using `override`-annotated classes.
- [`25_3_The_override_n_final_specifiers_n_covariant_return_types/Overriding_using_virtual_functions_with_override_keyword/2_arrays_for_derived_types.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_3_The_override_n_final_specifiers_n_covariant_return_types/Overriding_using_virtual_functions_with_override_keyword/2_arrays_for_derived_types.cpp): Base-pointer array test using `override`-annotated classes.

---

## 4 — Virtual destructors, virtual assignment, and overriding virtualization

### Virtual Destructors

> [!WARNING]
> Deleting a derived class object through a base class pointer when the base class does not have a virtual destructor results in **undefined behavior (UB)**! In practice, the base destructor executes while the derived destructor is completely skipped, causing memory leaks and unreleased resources.

```cpp
class Base {
public:
    virtual ~Base() = default; // Essential: ensures derived destructors run upon deletion!
};

class Derived : public Base {
    int* m_data{};
public:
    Derived() : m_data{ new int[100] } {}
    ~Derived() override {
        delete[] m_data; // Runs cleanly when deleting through Base*!
    }
};

Base* b = new Derived{};
delete b; // Correct: invokes Derived destructor first, then Base destructor!
```

Key rules:
- You should **always make destructors virtual** if you are dealing with inheritance and polymorphic deletion.
- If a base class destructor is virtual, all derived class destructors are implicitly virtual. It is not strictly necessary to write an empty derived destructor just to mark it virtual.
- A virtual default destructor can be easily provided using `= default`:
  ```cpp
  virtual ~Base() = default;
  ```

### Virtual Assignment
It is technically possible to make `operator=` virtual. However, unlike the destructor case where virtualization is essential, **virtualizing assignment operators opens a huge can of worms**. Because assignment operators take parameters of specific class types, virtual assignment creates severe typing and slicing complications. In general, **avoid virtual assignment operators**.

### Ignoring Virtualization (Scope Resolution)
There may be situations where you have a base pointer or reference to a derived object, but you want to deliberately call the base version of a function instead of the derived override. To bypass dynamic dispatch, use the **scope resolution operator (`::`)**:

```cpp
Derived d{};
const Base& rBase{ d };

rBase.getName();        // Virtual call: resolves to Derived::getName()
rBase.Base::getName();  // Direct call: bypasses virtual dispatch and calls Base::getName()
```

### Should we make all destructors virtual?
It is tempting to make every destructor virtual just in case a class is inherited from in the future. However, doing so adds a hidden virtual table pointer (`*__vptr`) to every instance of the class, increasing its memory footprint.

**Conventional Wisdom & Best Practices**:
1. **Public and Virtual**: If a class is designed to be a base class and allows polymorphic deletion through a base pointer, its destructor must be **`public` and `virtual`**.
2. **Protected and Non-Virtual**: If a class is designed to be inherited from, but you want to prohibit deleting derived objects through base pointers, make the destructor **`protected` and non-virtual**. The compiler will prevent client code from calling `delete basePtr;`.
3. **Sealed with `final`**: If a class is **not** explicitly designed to be a base class, make its destructor non-virtual and mark the class **`final`**. This communicates design intent and completely prevents accidental inheritance without imposing runtime overhead.

| Destructor Visibility / Type | Purpose | Permitted Use |
|---|---|---|
| `public virtual` | Base class intended for polymorphic deletion | Safe polymorphic `delete bPtr;` |
| `protected non-virtual` | Base class not intended for polymorphic deletion | Prevents `delete bPtr;` at compile time |
| `public non-virtual` (`final`) | Standalone concrete leaf class | Zero vtable overhead, cannot be inherited |

### 📁 Code Examples for Section 4
- [`25_4_Virtual_destructors_virtual_assignment_n_overriding_virtualization/1_virtual_destructor.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_4_Virtual_destructors_virtual_assignment_n_overriding_virtualization/1_virtual_destructor.cpp): Demonstrates proper resource cleanup by declaring a virtual destructor in `Base`, ensuring `delete bptr;` invokes `~Derived()` followed by `~Base()`.
- [`25_4_Virtual_destructors_virtual_assignment_n_overriding_virtualization/2_ignoring_virtualization.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_4_Virtual_destructors_virtual_assignment_n_overriding_virtualization/2_ignoring_virtualization.cpp): Demonstrates ignoring virtualization and bypassing dynamic dispatch by calling `rBase.Base::getName()` via scope resolution.

---

## 5 — Early binding and late binding

### Binding and Dispatching
- **Binding**: In general programming, binding is the process of associating names with properties (such as associating a function name with its actual definition).
- **Function Binding** (or method binding): The process that determines what function definition is associated with a specific function call.
- **Dispatching**: The process of actually invoking a bound function at execution time.

### Early Binding (Static Binding)
In C++, when a direct call is made to a non-member function or a non-virtual member function, the compiler can determine which function definition matches the call at compile time.
- This is known as **early binding** (or **static binding** / **static dispatch**).
- The compiler or linker generates direct machine language instructions telling the CPU to jump directly to the known memory address of the function (`call <address>`).
- Calls to overloaded functions and function templates are also resolved at compile time via early binding.

> [!NOTE]
> In a `switch` statement, which branch executes is not known until runtime based on user input; however, that is a runtime path-of-execution decision, not a function binding issue. The functions in each case branch are still early-bound.

### Late Binding (Dynamic Binding)
In some cases, a function call cannot be resolved until runtime:
- In C++, this is known as **late binding** (or **dynamic dispatch** when resolving virtual functions).
- The compiler cannot hardcode the target function's address at compile time. Instead, the function address is retrieved dynamically at runtime from memory before making an indirect jump.
- One classic C/C++ mechanism for late binding is **function pointers**:

```cpp
// Late binding using function pointers:
using Operation = int(*)(int, int);

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }

Operation op{ add }; // Chosen at runtime based on input
std::cout << op(5, 3); // Indirect function call (late binding)
```

### Performance Trade-off
- **Early Binding**: Direct CPU jump to function address (faster, enables compiler inlining).
- **Late Binding**: Must read the address from a pointer or table in memory, then jump to that address. This extra level of indirection incurs a slight runtime performance penalty, but provides tremendous flexibility.

### 📁 Code Examples for Section 5
- [`25_5_Early_binding_n_late_binding_virtual_table/1_early_binding.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_5_Early_binding_n_late_binding_virtual_table/1_early_binding.cpp): Demonstrates early binding where function addresses are known and linked at compile time.
- [`25_5_Early_binding_n_late_binding_virtual_table/2_dynamic_binding.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_5_Early_binding_n_late_binding_virtual_table/2_dynamic_binding.cpp): Demonstrates late binding using function pointers selected dynamically at runtime based on user input.

---

## 6 — The virtual table

The C++ standard does not mandate how compilers must implement virtual functions. However, nearly all C++ implementations use a mechanism known as the **virtual table** (**vtable**).

The virtual table is a static lookup table of function pointers used to resolve function calls dynamically at runtime. It is also referred to as the *vtable*, *virtual method table*, or *dispatch table*.

### Nomenclature Summary
- **Early binding / static dispatch**: Direct function call and compile-time overload resolution.
- **Late binding**: Indirect function call resolution via an address held in memory.
- **Dynamic dispatch**: Virtual function override resolution through the virtual table.

### How the Virtual Table Works

1. **One static vtable per class**: Every class that defines or inherits virtual functions has its own virtual table.
   - The virtual table is a static array generated by the compiler at compile time.
   - It contains one entry for each virtual function that can be called by objects of the class.
   - Each entry in the vtable is a function pointer pointing to the **most-derived implementation** of that virtual function accessible to that class.
2. **One `*__vptr` member per object**: The compiler automatically injects a hidden pointer member (conventionally called `*__vptr`) into the base class.
   - `*__vptr` is automatically initialized in the constructor to point to the virtual table for that concrete class.
   - Unlike the `this` pointer (which is an implicit parameter passed to member functions), `*__vptr` is a **real physical data member** of the object.
   - Consequently, every object of a polymorphic class is larger by the size of one pointer (8 bytes on 64-bit architectures).
   - `*__vptr` is inherited by all derived classes. When a `Derived` object is constructed, the base constructor initially sets `*__vptr` to `Base`'s vtable, and when the `Derived` constructor executes, it updates `*__vptr` to point to `Derived`'s vtable.

```
       Object in Memory                         Class Virtual Table
┌────────────────────────────┐              ┌─────────────────────────┐
│ *__vptr ───────────────────┼─────────────>│ &Derived::function1()   │
├────────────────────────────┤              ├─────────────────────────┤
│ Base data members...       │              │ &Derived::function2()   │
├────────────────────────────┤              └─────────────────────────┘
│ Derived data members...    │
└────────────────────────────┘
```

> [!NOTE]
> A detailed architectural diagram illustrating how virtual table pointers resolve function calls across class hierarchies is provided in [`25_5_Early_binding_n_late_binding_virtual_table/virtual_table.png`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_5_Early_binding_n_late_binding_virtual_table/virtual_table.png).

### The Three Steps of Calling a Virtual Function
Calling a virtual function is slower than calling a non-virtual function because the CPU must execute **3 separate operations** instead of 1:
1. Fetch the object's `*__vptr` to locate the class's virtual table.
2. Index into the virtual table at a fixed offset to retrieve the target function pointer.
3. Jump to and execute the function located at that address.

---

## 7 — Pure virtual functions, abstract base classes, and interface classes

### Pure Virtual Functions (Abstract Functions)

A **pure virtual function** (also known as an **abstract function**) is a virtual function that has no default body in the class declaration. It is designated by assigning `= 0` to the function declaration:

```cpp
class Base {
public:
    virtual void doSomething() = 0; // Pure virtual function!
};
```

Adding a pure virtual function means: *"Derived classes are required to implement this function before they can be instantiated."*

### Consequences of Pure Virtual Functions
1. **Abstract Base Class**: Any class with one or more pure virtual functions becomes an **abstract base class** (**ABC**).
   - Abstract base classes **cannot be instantiated** directly (`Base b;` is a compile error).
   - Pointers and references to abstract base classes are fully supported (`Base* ptr` or `Base& ref`).
2. **Enforced Derived Definitions**: Any derived class must provide a definition for all inherited pure virtual functions. If it fails to implement even one pure virtual function, that derived class remains an abstract base class and cannot be instantiated either.

### Pure Virtual Functions CAN Have Definitions!

> [!IMPORTANT]
> A pure virtual function **can be given a definition in C++**, but the definition **must be provided outside the class body** (out-of-line)!
> - The class containing the pure virtual function remains an abstract base class because of the `= 0` specifier.
> - Derived classes are still required to explicitly override the function.
> - However, derived overrides can explicitly invoke the base class's default implementation using base scope resolution (`Base::doSomething()`).
> - This is ideal when a base class wants to provide a shared default implementation while forcing every derived class to consciously acknowledge and opt-in to calling it.

```cpp
class Animal {
public:
    virtual std::string_view speak() const = 0; // Pure virtual declaration
};

// Out-of-line definition for pure virtual function
std::string_view Animal::speak() const {
    return "Generic animal sound";
}

class Dog : public Animal {
public:
    std::string_view speak() const override {
        // Calls the base pure virtual implementation!
        return Animal::speak();
    }
};
```

### Pure Virtual Destructors
A destructor can be declared pure virtual (`virtual ~Base() = 0;`), which is useful when you want to make a base class abstract without having any other member functions to mark pure virtual.
However, **pure virtual destructors MUST provide a definition** (out-of-line: `Base::~Base() {}`), because when any derived object is destroyed, the base class destructor is always invoked as part of normal teardown.

### Interface Classes
An **interface class** (often called a pure interface) is a class that:
- Has **no member variables**.
- Has **all pure virtual functions** (`= 0`).
- Has a **virtual destructor** (`virtual ~Interface() = default;`).

Interface classes define a contract that derived classes must fulfill, leaving all implementation details completely to the derived classes.

```cpp
class IErrorLog {
public:
    virtual ~IErrorLog() = default; // Essential virtual destructor
    virtual bool openLog(std::string_view filename) = 0;
    virtual bool closeLog() = 0;
    virtual bool writeError(std::string_view errorMessage) = 0;
};
```

> [!TIP]
> Prefixing interface class names with `I` (such as `IErrorLog`, `IShape`, `IPrintable`) is a standard naming convention that clearly communicates interface semantics to users.

### Pure Virtual Functions and the Virtual Table
For architectural consistency, abstract base classes still have virtual tables. When a pure virtual function has no definition, its virtual table entry typically holds a null pointer or points to a compiler runtime error handler (often named `__purecall`).

### 📁 Code Examples for Section 7
- [`25_6_Pure_virtual_functions_abstract_base_classes_n_interface_classes/1_animal_polymorphism_pure_virtual_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_6_Pure_virtual_functions_abstract_base_classes_n_interface_classes/1_animal_polymorphism_pure_virtual_function.cpp): Demonstrates making `Animal` an abstract base class using `virtual std::string_view speak() const = 0`, forcing `Dog` and `Cat` to implement overrides.
- [`25_6_Pure_virtual_functions_abstract_base_classes_n_interface_classes/2_pure_virtual_function_with_definition.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_6_Pure_virtual_functions_abstract_base_classes_n_interface_classes/2_pure_virtual_function_with_definition.cpp): Demonstrates defining a pure virtual function out-of-line and calling it from a derived class via `Animal::speak()`.
- [`25_6_Pure_virtual_functions_abstract_base_classes_n_interface_classes/3_interface_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_6_Pure_virtual_functions_abstract_base_classes_n_interface_classes/3_interface_class.cpp): Demonstrates creating an interface class `IErrorLog` and implementing it in concrete derived classes.

---

## 8 — Virtual base classes

When using multiple inheritance, a classic problem arises when two derived classes inherit from the same common base class, and a further derived class inherits from both. This is known as the **Diamond Problem**:

```
        PoweredDevice
         /         \
    Scanner       Printer
         \         /
           Copier
```

Without virtual base classes, `Copier` receives **two separate copies** of `PoweredDevice` (one via `Scanner`, and one via `Printer`), leading to compiler ambiguity errors when accessing `PoweredDevice` members.

### Resolving the Diamond Problem with Virtual Inheritance

To share a single common base instance, specify the `virtual` keyword in the inheritance lists of intermediate classes:

```cpp
class PoweredDevice {
public:
    PoweredDevice(int power) { std::cout << "PoweredDevice: " << power << '\n'; }
};

class Scanner : virtual public PoweredDevice {
public:
    Scanner(int power, int scanSpeed)
        : PoweredDevice{ power } {} // Ignored when Copier is constructed!
};

class Printer : virtual public PoweredDevice {
public:
    Printer(int power, int printSpeed)
        : PoweredDevice{ power } {} // Ignored when Copier is constructed!
};

class Copier : public Scanner, public Printer {
public:
    Copier(int power, int scanSpeed, int printSpeed)
        : PoweredDevice{ power }, // Most derived class directly initializes virtual base!
          Scanner{ power, scanSpeed },
          Printer{ power, printSpeed }
    {}
};
```

### Construction Rules for Virtual Base Classes
1. **Most-derived class constructs virtual base**: The most-derived class (`Copier`) is responsible for directly constructing the virtual base class (`PoweredDevice`).
2. **Intermediate constructor calls ignored**: While `Scanner` and `Printer` still call the `PoweredDevice` constructor, these calls are **ignored** when creating a `Copier`. However, if an individual `Scanner` object is created directly, its call to `PoweredDevice` executes as normal.
3. **Virtual bases constructed first**: In the constructor of the most-derived class, virtual base classes are **always constructed before any non-virtual base classes**.
4. **Single inheritance applies too**: Even in single inheritance, if class `C` derives from `B`, and `B` derives virtually from `A`, `C` is responsible for initializing `A`.
5. **Memory overhead**: Classes that inherit from a virtual base class contain an extra pointer offset in their virtual table to locate the shared base subobject in memory.

### 📁 Code Examples for Section 8
- [`25_7_Virtual_base_classes/1_diamond_problem.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_7_Virtual_base_classes/1_diamond_problem.cpp): Resolves the diamond problem using virtual inheritance (`virtual public PoweredDevice`), demonstrating that `Copier` directly initializes the single shared `PoweredDevice` instance.

---

## 9 — Object slicing

When a derived class object is assigned or copied into a base class object by value, only the base portion of the derived object is copied. The derived-specific member variables and overrides are **sliced off**. This phenomenon is called **object slicing** (or slicing for short).

```cpp
Derived d{ 5, "Hello" };
Base b = d; // Object slicing! Only Base subobject copied; "Hello" is sliced off!
```

### 1. Slicing and Functions
Passing objects by value causes slicing when a derived object is passed to a function taking a base parameter:

```cpp
// BAD: Pass-by-value causes slicing!
void printName(Base b) {
    std::cout << b.getName(); // Always calls Base::getName(), derived sliced off!
}

// GOOD: Pass-by-reference preserves dynamic polymorphism!
void printName(const Base& b) {
    std::cout << b.getName(); // Calls Derived::getName() via virtual dispatch!
}
```

### 2. Slicing in Containers (`std::vector`)
Attempting to create a polymorphic collection using `std::vector<Base>` results in object slicing because vectors store objects by value:

```cpp
std::vector<Base> v{};
v.push_back(Base{ 5 });
v.push_back(Derived{ 6 }); // Slicing occurs! Derived portion discarded!
```

How to fix polymorphic container storage:
- `std::vector<Base&>`: **Will not compile!** Vector elements must be copy-assignable, and C++ references cannot be reassigned once bound.
- `std::vector<Base*>`: Works, but requires managing pointer lifecycles, and `nullptr` becomes a valid state.
- `std::vector<std::reference_wrapper<Base>>`: **Recommended modern solution!** `std::reference_wrapper` acts as a reassignable reference, preventing slicing without raw pointer overhead.

```cpp
std::vector<std::reference_wrapper<Base>> v{};
Base b{ 5 };
Derived d{ 6 };
v.push_back(b);
v.push_back(d); // No slicing! Stored polymorphically by reference!

for (const auto& elem : v) {
    std::cout << elem.get().getName() << '\n'; // Resolves polymorphically!
}
```

### 3. The Frankenobject Problem

> [!CAUTION]
> The **Frankenobject** bug occurs when assignment happens through a base reference:

```cpp
Derived d1{ 5, "First" };
Derived d2{ 6, "Second" };
Base& b{ d2 };

b = d1; // DANGEROUS! Frankenobject created!
```

Why this happens:
- Because `b` is statically typed as `Base&`, and `operator=` is not virtual by default, `b = d1` invokes `Base::operator=(const Base&)`.
- Only the `Base` portion of `d1` is copied into `d2`. The `Derived` portion of `d2` remains completely untouched!
- As a result, `d2` becomes a hybrid "Frankenobject" composed of the `Base` state of `d1` and the `Derived` state of `d2`.

> [!TIP]
> If a base class is not intended to be instantiated on its own, slicing can be prevented by **deleting the base copy constructor and assignment operator**:
> ```cpp
> Base(const Base&) = delete;
> Base& operator=(const Base&) = delete;
> ```

### 📁 Code Examples for Section 9
- [`25_8_Object_slicing/1_object_slicing.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_8_Object_slicing/1_object_slicing.cpp): Demonstrates slicing when pushing `Derived` objects into `std::vector<Base>`.
- [`25_8_Object_slicing/2_object_slicing_solution.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_8_Object_slicing/2_object_slicing_solution.cpp): Demonstrates the standard solution using `std::vector<std::reference_wrapper<Base>>` to achieve polymorphism without slicing.
- [`25_8_Object_slicing/3_the_frankenobject.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_8_Object_slicing/3_the_frankenobject.cpp): Demonstrates the Frankenobject defect created when assigning derived objects through a base reference (`b = d1`).

---

## 10 — Dynamic casting

When working with polymorphism, you will frequently hold a base class pointer or reference, but need access to member functions or state that exist only in a specific derived class.
Converting a `Derived*` to a `Base*` is called **upcasting** and is performed implicitly by C++.
Converting a `Base*` back into a `Derived*` is called **downcasting**.

### `dynamic_cast`
C++ provides the `dynamic_cast` operator specifically for safe downcasting in polymorphic hierarchies:

```cpp
Base* bPtr = getObject(); // Returns a Base* pointing to either Base or Derived

Derived* dPtr = dynamic_cast<Derived*>(bPtr);
if (dPtr) {
    // Cast succeeded: bPtr indeed points to a Derived object!
    dPtr->derivedSpecificFunction();
} else {
    // Cast failed: bPtr points to a Base or some other derived type
}
```

### Pointer vs Reference Failure Modes
- **Pointers**: If a `dynamic_cast` on a pointer fails, it returns a **`nullptr`**. Always check the returned pointer against `nullptr` before dereferencing!
- **References**: Because C++ does not have null references, if a `dynamic_cast` on a reference fails, it throws a **`std::bad_cast`** exception (from `<typeinfo>`).

```cpp
try {
    Derived& dRef = dynamic_cast<Derived&>(bRef);
    dRef.derivedFunction();
} catch (const std::bad_cast& e) {
    std::cout << "Cast failed: " << e.what() << '\n';
}
```

### When `dynamic_cast` Fails or Cannot Be Used
`dynamic_cast` will not work in the following scenarios:
1. When classes have **no virtual functions** (they lack a vtable, meaning no runtime type information is attached).
2. With **`protected` or `private` inheritance** (the public *is-a* relationship is hidden from external code).
3. When downcasting across certain complex virtual base class boundaries.

### Downcasting: `dynamic_cast` vs `static_cast`

| Feature | `dynamic_cast` | `static_cast` |
|---|---|---|
| **Safety** | Safe: runtime type check via RTTI | Unsafe: no runtime check performed |
| **Failure Behavior (pointer)** | Returns `nullptr` | Returns invalid pointer; dereferencing is **UB** |
| **Failure Behavior (reference)** | Throws `std::bad_cast` | Binds invalid reference; access is **UB** |
| **Prerequisites** | Polymorphic base (requires virtual function) | Works on non-polymorphic types |
| **Performance** | Incurs runtime cost (traverses RTTI) | Zero runtime overhead (compile-time cast) |

> [!TIP]
> Use `static_cast` only when you are 100% mathematically or architecturally certain of the dynamic type. Otherwise, **always prefer `dynamic_cast`**.

### Downcasting vs Virtual Functions
In general, **virtual functions should always be preferred over downcasting**. Downcasting is often indicative of brittle class design.
However, downcasting is justified when:
- You cannot modify the base class to add a virtual function (e.g., third-party or standard library classes).
- You need access to derived-specific accessors that do not conceptually belong in the base interface.
- Adding a virtual function to the base class makes no sense because there is no reasonable default implementation.

### A Warning About RTTI
`dynamic_cast` relies on **Runtime Type Information (RTTI)**. Because RTTI increases binary size and memory overhead, some embedded compilers or performance-critical projects allow disabling RTTI (`-fno-rtti`). If RTTI is disabled, `dynamic_cast` will fail to compile or function correctly.

### 📁 Code Examples for Section 10
- [`25_9_Dynamic_casting/1_dynamic_cast.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_9_Dynamic_casting/1_dynamic_cast.cpp): Demonstrates downcasting base pointers to derived pointers using `dynamic_cast` and checking for `nullptr`.
- [`25_9_Dynamic_casting/2_friend_created_private_class_downcast_to_base.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_9_Dynamic_casting/2_friend_created_private_class_downcast_to_base.cpp): Demonstrates downcasting considerations with access permissions and inheritance boundaries.

---

## 11 — Printing inherited classes using operator<<

### The Challenge with `operator<<`
Stream insertion operators (`operator<<`) present a classic dilemma in polymorphic hierarchies:
- `operator<<` is typically implemented as a **non-member friend function**, not a member function. Only member functions can be virtualized in C++.
- Overload resolution occurs at compile time based on the static type of the reference passed in.
- If we pass a `Derived` object through a `Base&`, overload resolution selects `operator<<(std::ostream&, const Base&)`, printing base data and ignoring the derived type:

```cpp
class Base {
    friend std::ostream& operator<<(std::ostream& out, const Base&) {
        out << "Base";
        return out;
    }
};

class Derived : public Base {
    friend std::ostream& operator<<(std::ostream& out, const Derived&) {
        out << "Derived";
        return out;
    }
};

Derived d{};
Base& bRef{ d };
std::cout << bRef; // Prints "Base"! (No polymorphism!)
```

### Can We Make `operator<<` Virtual?
**No**, for two fundamental language reasons:
1. **Non-member status**: Only member functions can be virtualized. A friend function is a non-member function and cannot override anything.
2. **Parameter mismatch**: Even if a member version were attempted, `Base::operator<<(const Base&)` and `Derived::operator<<(const Derived&)` take different parameter types, which does not constitute a valid override signature.

### The Solution: Delegating to a Virtual Member Function

The standard C++ idiom to solve this challenge is to have friend `operator<<` **delegate to a virtual member function**:

```cpp
class Base {
public:
    virtual ~Base() = default;

    // 1. Friend operator<< defined ONLY in Base
    friend std::ostream& operator<<(std::ostream& out, const Base& b) {
        // 2. Delegates to virtual member function!
        return b.print(out);
    }

    // 3. Virtual print function can be overridden by any derived class
    virtual std::ostream& print(std::ostream& out) const {
        out << "Base";
        return out;
    }
};

class Derived : public Base {
    int m_value{ 42 };
public:
    // 4. Override print() to output derived-specific state
    std::ostream& print(std::ostream& out) const override {
        out << "Derived(" << m_value << ")";
        return out;
    }
};

int main() {
    Derived d{};
    Base& bRef{ d };
    std::cout << bRef << '\n'; // Prints "Derived(42)"! Fully polymorphic!
}
```

### Why This Works
1. When `std::cout << bRef` executes, compile-time overload resolution selects `Base::operator<<(std::ostream&, const Base&)`.
2. Inside `operator<<`, the call `b.print(out)` triggers **dynamic dispatch** because `print()` is a virtual member function.
3. The virtual table resolves the call to the most-derived override (`Derived::print()`) at runtime!
4. `Derived::print()` can also call `Base::print(out)` to format inherited base members cleanly before derived members.

### 📁 Code Examples for Section 11
- [`25_10_Printing_inherited_classes_using_insertion_operator/1_challenge_with_insertion_operator.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_10_Printing_inherited_classes_using_insertion_operator/1_challenge_with_insertion_operator.cpp): Demonstrates the problem: calling `operator<<` on a `Base&` reference fails to produce polymorphic output.
- [`25_10_Printing_inherited_classes_using_insertion_operator/2_solution_to_challenge_with_insertion_operator.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_10_Printing_inherited_classes_using_insertion_operator/2_solution_to_challenge_with_insertion_operator.cpp): Demonstrates solving the problem using a virtual `getType()` member function called by `operator<<`.
- [`25_10_Printing_inherited_classes_using_insertion_operator/3_advance_solution_to_challenge_with_insertion_operator.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/25_Virtual_Functions/25_10_Printing_inherited_classes_using_insertion_operator/3_advance_solution_to_challenge_with_insertion_operator.cpp): Demonstrates the complete architectural solution delegating `operator<<` to `virtual std::ostream& print(std::ostream& out) const`, allowing derived classes with complex members to stream formatted data polymorphically.
