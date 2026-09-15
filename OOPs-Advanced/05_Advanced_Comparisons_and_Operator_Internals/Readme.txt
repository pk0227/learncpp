================================================================================
MODULE 05: ADVANCED COMPARISONS AND OPERATOR INTERNALS
================================================================================

This module covers the modern operator overloading mechanics, C++20 three-way comparison 
internals, arrow proxy chaining, and custom memory management operators that senior C++ 
engineers use in systems architecture.

--------------------------------------------------------------------------------
TABLE OF CONTENTS
--------------------------------------------------------------------------------
1. The C++20 Spaceship Operator (<=>) and Ordering Categories
   1.1 The Evolution of Comparisons in C++20
   1.2 The Three Comparison Categories:
       - std::strong_ordering (Total order, substitutability)
       - std::weak_ordering (Equivalence without identity)
       - std::partial_ordering (Incomparable values, NaN)
   1.3 Compiler-Synthesized Operators from defaulted <=> and ==
   1.4 Custom Spaceship Operator Implementation
2. The Arrow Operator (->) Chaining and The Proxy Idiom
   2.1 How the Compiler Recursively Chains operator->
   2.2 The Return-by-Value Arrow Proxy Pattern
   2.3 Practical Application: The Thread-Safe RAII Locking Proxy
   2.4 The Smart Reference Pattern
3. User-Defined Literals (UDLs) and Compile-Time Type Safety
   3.1 Syntax and Structure of Literal Operators (operator"")
   3.2 Cooked vs Raw and Template Literal Operators
   3.3 Eliminating Unit Mismatch Bugs (Dimensional Analysis)
   3.4 Standard Library Literals (s, sv, ms, h)
4. Function Call Operator (operator()) and Inlining Advantages
   4.1 Functors as Stateful Callables
   4.2 Why Functors Outperform Function Pointers in Standard Algorithms
   4.3 Multidimensional Subscripting: operator() vs C++23 operator[]
5. Custom Class-Specific Memory Allocation Operators
   5.1 Class-Level operator new and operator delete (Memory Pools)
   5.2 Placement new Mechanics and Manual Destructor Invocation
   5.3 Sized Deallocation (C++14): operator delete(void*, std::size_t)
   5.4 Destroying Delete (C++20): operator delete(T*, std::destroying_delete_t)


================================================================================
1. THE C++20 SPACESHIP OPERATOR (<=>) AND ORDERING CATEGORIES
================================================================================

1.1 The Evolution of Comparisons in C++20
    -- Prior to C++20, providing complete comparison capabilities for a class required writing 
       up to 6 separate boiler-plate functions: ==, !=, <, <=, >, >=.
    -- C++20 introduces the **Three-Way Comparison Operator** (`<=>`), nicknamed the **spaceship operator**.
    -- Defining `<=>` allows the compiler to automatically synthesize all relational operators (<, <=, >, >=), 
       and defining `==` synthesizes `!=`.

1.2 The Three Comparison Categories
    -- The return type of `<=>` communicates mathematical properties of the comparison:
       1. `std::strong_ordering`:
          -- Yields `less`, `equal`, `greater`.
          -- Satisfies **substitutability**: If `a == b`, then for any function `f`, `f(a) == f(b)`.
          -- Typical for integers, strings, pointers.
       2. `std::weak_ordering`:
          -- Yields `less`, `equivalent`, `greater`.
          -- Two objects can be equivalent without being identical!
          -- Example: Case-insensitive string comparison ("hello" and "HELLO" are equivalent, 
             but NOT identical/substitutable).
       3. `std::partial_ordering`:
          -- Yields `less`, `equivalent`, `greater`, `unordered`.
          -- Some values cannot be compared!
          -- Example: Floating-point IEEE 754 numbers containing `NaN` (Not a Number). 
             `NaN < 5.0` is false, `NaN > 5.0` is false, `NaN == 5.0` is false!

1.3 Compiler-Synthesized Operators from defaulted <=> and ==
    -- Writing:
       struct Point {
           int x;
           int y;
           auto operator<=>(const Point&) const = default;
       };
    -- The compiler automatically generates:
       -- Lexicographical member-wise comparison of `x`, then `y`.
       -- Synthesizes `<=>`, `==`, `!=`, `<`, `<=`, `>`, `>=`!

1.4 Custom Spaceship Operator Implementation
    -- For classes requiring custom comparison logic (e.g. comparing pointer contents or ignoring certain fields):
       std::strong_ordering operator<=>(const Employee& other) const {
           if (auto cmp = m_department <=> other.m_department; cmp != 0) return cmp;
           return m_id <=> other.m_id;
       }


================================================================================
2. THE ARROW OPERATOR (->) CHAINING AND THE PROXY IDIOM
================================================================================

2.1 How the Compiler Recursively Chains operator->
    -- In C++, `operator->` has a unique syntactic rule:
       -- If `x->m` is evaluated, the compiler calls `x.operator->()`.
       -- If the result is a raw pointer `T*`, it accesses `->m`.
       -- If the result is an OBJECT that also overloads `operator->`, the compiler 
          RECURSIVELY calls `operator->` on that object until a raw pointer is reached!

2.2 The Return-by-Value Arrow Proxy Pattern
    -- Because `operator->` can return an object by value, that temporary proxy object's lifetime 
       spans the entire expression!
    -- This enables injecting setup and teardown logic around member function access.

2.3 Practical Application: The Thread-Safe RAII Locking Proxy
    -- Problem: How do you provide thread-safe access to an object's member methods without 
       wrapping every single method in mutex locks?
    -- Solution:
       template <typename T>
       class ThreadSafe {
           T m_obj;
           mutable std::mutex m_mtx;
           
           struct Proxy {
               std::unique_lock<std::mutex> lock;
               T* ptr;
               T* operator->() { return ptr; }
           };
       public:
           Proxy operator->() {
               return Proxy{std::unique_lock<std::mutex>(m_mtx), &m_obj};
           }
       };
    -- Usage: `safeObj->method();`
       1. `safeObj.operator->()` locks the mutex and returns a temporary `Proxy`.
       2. `Proxy.operator->()` returns `&m_obj`.
       3. `method()` executes while holding the lock.
       4. The temporary `Proxy` is destroyed at the end of the full expression, automatically UNLOCKING the mutex!


================================================================================
3. USER-DEFINED LITERALS (UDLS) AND COMPILE-TIME TYPE SAFETY
================================================================================

3.1 Syntax and Structure of Literal Operators (operator"")
    -- C++11 introduced User-Defined Literals (UDLs) via `operator"" _suffix`.
    -- User-defined suffixes MUST begin with an underscore `_` (suffixes without underscores are 
       reserved for the C++ standard library).

3.2 Cooked vs Raw and Template Literal Operators
    -- Cooked literal: Receives pre-parsed primitive values:
       constexpr Distance operator"" _km(long double val) { return Distance(val * 1000.0); }
       constexpr Distance operator"" _m(long double val) { return Distance(val); }
    -- Raw literal: Receives const char* array containing the exact digits written:
       BigInt operator"" _bignum(const char* digits);
    -- String literals:
       MyString operator"" _s(const char* str, std::size_t len);

3.3 Eliminating Unit Mismatch Bugs (Dimensional Analysis)
    -- Writing: `auto dist = 5.0_km + 200.0_m;`
    -- Guarantees compile-time type safety: prevents accidentally adding kilograms to meters!

3.4 Standard Library Literals (s, sv, ms, h)
    -- `using namespace std::string_literals;` -> `"hello"s` produces `std::string`.
    -- `using namespace std::string_view_literals;` -> `"hello"sv` produces `std::string_view`.
    -- `using namespace std::chrono_literals;` -> `100ms`, `5s`, `2h` for time durations.


================================================================================
4. FUNCTION CALL OPERATOR (OPERATOR()) AND INLINING ADVANTAGES
================================================================================

4.1 Functors as Stateful Callables
    -- Overloading `operator()` creates a Function Object (Functor).
    -- Unlike free functions, functors can retain internal state across invocations.

4.2 Why Functors Outperform Function Pointers in Standard Algorithms
    -- SENIOR INTERVIEW QUESTION: Why is `std::sort` faster with a functor/lambda than with a function pointer?
    -- When passing a raw function pointer `bool (*cmp)(int, int)`:
       -- `std::sort` receives an address. The compiler must invoke the comparator via an indirect jump.
       -- It CANNOT inline the comparison function!
    -- When passing a functor or lambda:
       -- The comparator type is baked into the template parameter `template <class Compare>`.
       -- The compiler knows the EXACT call target at compile time!
       -- The comparison is fully INLINED into the sort loop, eliminating all call overhead and branch penalties.

4.3 Multidimensional Subscripting: operator() vs C++23 operator[]
    -- Before C++23, `operator[]` was strictly limited to taking exactly ONE argument.
    -- Multidimensional indexing was forced to use `matrix(row, col)`.
    -- C++23 relaxed this: `matrix[row, col]` is now supported natively via multidimensional `operator[]`.


================================================================================
5. CUSTOM CLASS-SPECIFIC MEMORY ALLOCATION OPERATORS
================================================================================

5.1 Class-Level operator new and operator delete (Memory Pools)
    -- When a class declares `void* operator new(size_t)` and `void operator delete(void*)`:
       -- All dynamic allocations of that class bypass the general-purpose heap allocator.
       -- Allows routing allocations to high-performance fixed-size slab allocators or memory pools.

5.2 Placement new Mechanics and Manual Destructor Invocation
    -- `void* operator new(size_t, void* ptr) { return ptr; }`
    -- Constructs an object in pre-allocated memory buffer without allocating heap memory:
       `new (buffer) Widget(args...);`
    -- RULE: Memory allocated via placement new MUST have its destructor called MANUALLY:
       `widget_ptr->~Widget();`
       (Calling `delete widget_ptr` is undefined behavior because the memory was not allocated via heap new!).

5.3 Sized Deallocation (C++14): operator delete(void*, std::size_t)
    -- C++14 introduced sized deallocation:
       `void operator delete(void* ptr, std::size_t size) noexcept;`
    -- Provides the exact byte size of the object being destroyed to the allocator.
    -- Allows memory pool allocators to immediately identify the correct size bin without storing 
       extra size metadata per allocation.

5.4 Destroying Delete (C++20): operator delete(T*, std::destroying_delete_t)
    -- In standard C++, deleting an object runs the destructor FIRST, then calls `operator delete(void*)`.
    -- C++20 introduces Destroying Delete:
       `void operator delete(Node* ptr, std::destroying_delete_t);`
    -- The class takes over responsibility for running its own destructor!
    -- Enables polymorphic destruction without virtual destructors, and tail-call optimized deallocations.
