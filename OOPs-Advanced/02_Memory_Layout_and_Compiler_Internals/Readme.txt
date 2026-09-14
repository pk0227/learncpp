================================================================================
MODULE 02: MEMORY LAYOUT AND COMPILER INTERNALS
================================================================================

This module covers the concrete physical memory layout, pointer arithmetic adjustments, 
vtable/vptr mechanics, and hardware-level alignment optimizations that define how C++ 
compilers implement object-oriented abstractions.

--------------------------------------------------------------------------------
TABLE OF CONTENTS
--------------------------------------------------------------------------------
1. Multiple Inheritance Memory Layout & Pointer Adjustment
   1.1 Memory Layout of Multiple Base Classes
   1.2 Pointer Adjustment (The Shifting 'this' Pointer)
   1.3 static_cast vs reinterpret_cast in Inheritance Hierarchies
   1.4 Downcasting Mechanics: static_cast vs dynamic_cast
2. Virtual Inheritance and the Diamond Problem Internals
   2.1 The Classic Diamond Problem: Duplicate Base Sub-Objects
   2.2 Virtual Inheritance Mechanics: Virtual Base Table (vbtbl) and Offset Tables
   2.3 Construction Responsibility: Why the Most-Derived Class Initializes Virtual Bases
   2.4 The Performance and Memory Cost of Virtual Inheritance
3. Vtable and Vptr Mechanics & Compiler Devirtualization
   3.1 Physical Structure of Vtables and Vptrs (Itanium ABI)
   3.2 Dynamic Dispatch Overhead: Indirect Branching and Cache Misses
   3.3 Multiple Inheritance Vtables (Thunks and Secondary Vptrs)
   3.4 Devirtualization: How 'final' and Speculative Inlining Optimize Virtual Calls
4. Empty Base Class Optimization (EBCO) and [[no_unique_address]]
   4.1 The 1-Byte Rule: Why Empty Classes Cannot Have sizeof == 0
   4.2 Empty Base Optimization: 0-Byte Overhead via Inheritance
   4.3 Real-World Application: How std::unique_ptr Achieves Zero Overhead
   4.4 Modern C++20 Solution: [[no_unique_address]] Attribute
5. Struct Alignment, Padding, Cache Lines, and False Sharing
   5.1 Alignment Requirements and Compiler Padding Rules (alignof, alignas)
   5.2 Reordering Struct Members to Minimize Memory Footprint
   5.3 Cache Line Architecture (64-byte lines) and Object Alignment
   5.4 Multithreaded False Sharing and std::hardware_destructive_interference_size


================================================================================
1. MULTIPLE INHERITANCE MEMORY LAYOUT & POINTER ADJUSTMENT
================================================================================

1.1 Memory Layout of Multiple Base Classes
    -- When a class inherits from multiple non-virtual base classes:
       class Base1 { int b1; };
       class Base2 { int b2; };
       class Derived : public Base1, public Base2 { int d; };
    -- The compiler lays out the memory contiguously in declaration order:
       Offset +0: [ Base1 sub-object (int b1) ]
       Offset +4: [ Base2 sub-object (int b2) ]
       Offset +8: [ Derived members  (int d)  ]
    -- The Derived object contains full, distinct sub-objects for Base1 and Base2.

1.2 Pointer Adjustment (The Shifting 'this' Pointer)
    -- SENIOR INTERVIEW QUESTION: Does `static_cast<Base2*>(derived_ptr)` change the numerical address?
    -- YES! 
       -- A pointer to Base1 points to offset +0.
       -- A pointer to Base2 MUST point to offset +4 (where Base2's members begin).
       -- Therefore, when you write:
          Derived* d = new Derived();
          Base2* b2 = d; // Implicit or static_cast
       -- The compiler injects pointer arithmetic: `b2 = (Base2*)((char*)d + sizeof(Base1));`
       -- When calling `b2->someMethod()`, the hidden `this` pointer passed to `someMethod` is 
          adjusted to point directly at the Base2 sub-object!

1.3 static_cast vs reinterpret_cast in Inheritance Hierarchies
    -- `static_cast<Base2*>(d)` performs compile-time pointer adjustment based on the class layout.
    -- `reinterpret_cast<Base2*>(d)` simply reinterprets the raw bits of the pointer without adjusting 
       the address!
    -- DANGER: With `reinterpret_cast`, `b2` still points to offset +0 (Base1)! 
       Calling Base2 member functions or accessing Base2 members will read Base1 memory, causing 
       silent data corruption or crashes!

1.4 Downcasting Mechanics: static_cast vs dynamic_cast
    -- Downcasting from `Base2*` back to `Derived*`:
       -- `static_cast<Derived*>(b2)`: Compiler subtracts the offset at compile time. Fast, zero runtime cost.
          WARNING: Unsafe if `b2` does not actually point to a `Derived` instance!
       -- `dynamic_cast<Derived*>(b2)`: Requires at least one virtual function (polymorphic type).
          Inspects RTTI (Run-Time Type Information) stored before the vtable to verify the actual dynamic type.
          If valid, adjusts pointer and returns `Derived*`; if invalid, returns `nullptr`.


================================================================================
2. VIRTUAL INHERITANCE AND THE DIAMOND PROBLEM INTERNALS
================================================================================

2.1 The Classic Diamond Problem: Duplicate Base Sub-Objects
    -- In a non-virtual diamond hierarchy:
          Animal
          /    \
        Mammal  Bird
          \    /
           Bat
    -- A `Bat` object contains TWO copies of `Animal`:
       [ Mammal sub-object containing Animal ] + [ Bird sub-object containing Animal ]
    -- Flaws:
       1. Ambiguity: Calling `bat.eat()` is ambiguous because the compiler doesn't know whether 
          to call `Mammal::Animal::eat` or `Bird::Animal::eat`.
       2. Wasted memory: Two sets of Animal member variables.

2.2 Virtual Inheritance Mechanics: Virtual Base Table (vbtbl) and Offset Tables
    -- Declaring `class Mammal : public virtual Animal` and `class Bird : public virtual Animal`:
       -- Guarantees exactly ONE shared `Animal` sub-object inside `Bat`.
    -- HOW DOES THE COMPILER ACHIEVE THIS INTERNALLY?
       -- The shared virtual base `Animal` is placed at the END of the `Bat` object layout.
       -- Neither `Mammal` nor `Bird` knows at compile-time where `Animal` will be located relative 
          to their sub-objects, because `Mammal` could be instantiated alone or as part of a larger derived class!
       -- Solution: Compilers inject a **Virtual Base Pointer (`vbptr`)** or store negative offsets 
          in the vtable.
       -- To access `Animal` members from `Mammal`, code must read the offset from the vtable/vbtbl 
          and add it dynamically to `this`. Every access to a virtual base incurs an extra pointer indirection!

2.3 Construction Responsibility: Why the Most-Derived Class Initializes Virtual Bases
    -- Under virtual inheritance, who initializes the shared `Animal` base?
    -- If `Mammal` initialized it with `Animal("Mammal")` and `Bird` initialized it with `Animal("Bird")`, 
       a contradiction occurs!
    -- THE C++ RULE: The **MOST-DERIVED class (`Bat`)** is directly responsible for constructing the 
       virtual base class `Animal`.
    -- The initializers for `Animal` specified by intermediate classes `Mammal` and `Bird` are 
       COMPLETELY IGNORED when creating a `Bat`.

2.4 The Performance and Memory Cost of Virtual Inheritance
    -- 1. Extra indirection on every member access to the virtual base.
    -- 2. Increased object size due to `vbptr` or enlarged vtable pointers.
    -- 3. Complex pointer adjustments when casting between branches of the diamond.


================================================================================
3. VTABLE AND VPTR MECHANICS & COMPILER DEVIRTUALIZATION
================================================================================

3.1 Physical Structure of Vtables and Vptrs (Itanium ABI)
    -- For any class containing at least one virtual function:
       1. The compiler creates a static table of function pointers called the **vtable** (`vftbl`).
       2. The compiler injects an invisible pointer member, the **vptr**, into the object layout 
          (typically at offset 0).
    -- Layout of the vtable in memory (Itanium ABI):
       [-2]: Offset-to-top (offset from vptr to the start of the complete object).
       [-1]: Pointer to `std::type_info` (used for RTTI and dynamic_cast).
       [ 0]: Pointer to virtual function 1.
       [ 1]: Pointer to virtual function 2.

3.2 Dynamic Dispatch Overhead: Indirect Branching and Cache Misses
    -- When calling `ptr->virtual_func()`:
       1. Dereference `ptr` to read the `vptr`: `vptr = *(ptr->vptr_address)`
       2. Index into vtable to find function pointer: `func_ptr = vptr[slot_index]`
       3. Perform indirect call: `call func_ptr`
    -- Performance costs:
       -- Two pointer dereferences (memory latency).
       -- Indirect branch CPU instruction (defeats branch predictor if target varies).
       -- Prevents compiler from inlining the function body!

3.3 Multiple Inheritance Vtables (Thunks and Secondary Vptrs)
    -- In multiple inheritance, a derived class has MULTIPLE vptrs!
       -- Primary vptr: for Base1 and Derived virtual functions.
       -- Secondary vptr: located inside the Base2 sub-object.
    -- What happens when `Base2* b2 = new Derived(); b2->virtual_func();` is called?
       -- The function expects a `Derived*` as `this`, but `b2` points to the Base2 sub-object (+offset)!
       -- The compiler uses a **Thunk**: a small assembly trampoline that adjusts `this` 
          (subtracts offset) before jumping to `Derived::virtual_func()`.

3.4 Devirtualization: How 'final' and Speculative Inlining Optimize Virtual Calls
    -- If the compiler can deduce the exact concrete type at compile time:
       Derived d;
       d.virtual_func(); // Direct call! No vtable lookup needed!
    -- The `final` specifier (C++11):
       -- Declaring a class or method `final` tells the compiler that no further overrides can exist.
       -- This allows the compiler to turn indirect virtual calls into DIRECT function calls and INLINE them!


================================================================================
4. EMPTY BASE CLASS OPTIMIZATION (EBCO) AND [[no_unique_address]]
================================================================================

4.1 The 1-Byte Rule: Why Empty Classes Cannot Have sizeof == 0
    -- In C++, every distinct object must have a unique memory address.
    -- If an empty class `struct Empty {};` had `sizeof(Empty) == 0`, then in an array `Empty arr[10]`, 
       every element would share the exact same address (`&arr[0] == &arr[1]`), violating pointer uniqueness!
    -- Therefore, the ISO C++ standard mandates: `sizeof(Empty) >= 1` (typically 1 byte).

4.2 Empty Base Optimization: 0-Byte Overhead via Inheritance
    -- When an empty class is used as a BASE CLASS, the requirement for a distinct address is relaxed 
       as long as it does not share the same type as the first non-static member!
    -- Example:
       struct Empty {};
       struct Foo : public Empty { int x; };
       -- `sizeof(Foo)` is 4 bytes (NOT 4 + 1 + padding = 8 bytes!).
       -- The `Empty` base class takes **0 bytes**!

4.3 Real-World Application: How std::unique_ptr Achieves Zero Overhead
    -- `std::unique_ptr<T, Deleter>` needs to store both a pointer `T*` and a deleter instance `Deleter`.
    -- The default deleter is `std::default_delete<T>`, which is an empty class!
    -- By inheriting from the deleter (or using `std::tuple`), `std::unique_ptr` leverages EBCO.
    -- Result: `sizeof(std::unique_ptr<T>) == sizeof(T*)` (8 bytes on 64-bit platforms). Zero overhead!

4.4 Modern C++20 Solution: [[no_unique_address]] Attribute
    -- Prior to C++20, developers were forced to use inheritance (EBCO) just to avoid wasting space for 
       empty member objects (stateless allocators, deleters, comparators).
    -- C++20 introduces `[[no_unique_address]]`:
       struct Container {
           int* data;
           [[no_unique_address]] EmptyDeleter deleter; // Takes 0 bytes!
       };
       -- `sizeof(Container)` is 8 bytes! No weird inheritance required.


================================================================================
5. STRUCT ALIGNMENT, PADDING, CACHE LINES, AND FALSE SHARING
================================================================================

5.1 Alignment Requirements and Compiler Padding Rules (alignof, alignas)
    -- Modern CPUs read memory in chunks (word sizes: 4 bytes, 8 bytes). Unaligned memory access 
       incurs heavy CPU penalties or hardware bus faults.
    -- Alignment rules:
       1. Every primitive type `T` has an alignment requirement `alignof(T)` (e.g., char=1, short=2, int=4, double=8).
       2. A member of type `T` must be located at an offset that is a multiple of `alignof(T)`.
       3. The total size of a struct must be a multiple of the largest member alignment in that struct.
    -- Compilers inject invisible **padding bytes** between members and at the end of structs to satisfy 
       these rules.

5.2 Reordering Struct Members to Minimize Memory Footprint
    -- Naive order:
       struct Bad {
           char c1;    // 1 byte  (+3 bytes padding)
           int i;      // 4 bytes
           char c2;    // 1 byte  (+7 bytes padding)
           double d;   // 8 bytes
       }; // Total: 24 bytes! (12 bytes of wasted padding!)
    -- Optimized order (largest to smallest):
       struct Good {
           double d;   // 8 bytes
           int i;      // 4 bytes
           char c1;    // 1 byte
           char c2;    // 1 byte  (+2 bytes padding)
       }; // Total: 16 bytes! (Saved 33% memory footprint).

5.3 Cache Line Architecture (64-byte lines) and Object Alignment
    -- Modern CPU caches load memory in 64-byte chunks called **cache lines**.
    -- If a critical data structure crosses a 64-byte cache line boundary, a single read requires TWO 
       cache line fetches instead of one!
    -- `alignas(64)` can align performance-critical structures to cache line boundaries.

5.4 Multithreaded False Sharing and std::hardware_destructive_interference_size
    -- False Sharing occurs when two threads running on different CPU cores modify independent variables 
       that happen to reside on the **SAME 64-byte cache line**.
    -- Even though the variables are completely independent, CPU cache coherence protocols (MESI) 
       continuously invalidate the entire cache line across cores, tanking multicore performance!
    -- C++17 Solution:
       #include <new>
       struct alignas(std::hardware_destructive_interference_size) ThreadData {
           uint64_t counter;
       };
       -- Guarantees that each thread's data resides on its own private cache line, eliminating false sharing.
