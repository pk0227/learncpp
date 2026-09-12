================================================================================
Chapter 12: References and Pointers
================================================================================

12.0 -- Introduction to Compound Data Types
--------------------------------------------------------------------------------
Every data type in C++ is classified as either a fundamental type (built-in primitive) or a compound type.

Compound Data Types Supported in C++:
    1. Functions
    2. C-style Arrays
    3. Pointer types:
       -- Pointer to object
       -- Pointer to function
    4. Pointer-to-member types:
       -- Pointer to data member
       -- Pointer to member function
    5. Reference types:
       -- Lvalue references (T&)
       -- Rvalue references (T&&, introduced in C++11 for move semantics)
    6. Enumerated types:
       -- Unscoped enumerations (enum)
       -- Scoped enumerations (enum class, introduced in C++11)
    7. Class types:
       -- Structs
       -- Classes
       -- Unions


12.1 -- Value Categories (Lvalues and Rvalues)
--------------------------------------------------------------------------------
Every expression in C++ is characterized by two independent properties: its type (e.g. int, double) and its value category.

Evolution of Value Categories:
    -- Prior to C++11, there were only two value categories: lvalue and rvalue.
    -- In C++11, three additional value categories (glvalue, prvalue, and xvalue) were introduced to support move semantics.
       glvalue (generalized lvalue) = lvalue + xvalue
       rvalue = prvalue (pure rvalue) + xvalue
       xvalue (expiring value) = an identifiable object that can be moved from

Lvalue and Rvalue Expressions:
    -- lvalue (left value / locator value): An expression that evaluates to an identifiable object or function that persists beyond the expression (has a distinct memory address).
    -- Lvalues come in two subtypes:
       -- Modifiable lvalue: A non-const object whose value can be modified.
       -- Non-modifiable lvalue: A const or constexpr object whose value cannot be modified.
    -- rvalue (right value): An expression that evaluates to a temporary value, literal, or result of an operator/function returning by value.
       Rvalues are not identifiable (cannot take their address with &), and only exist within the expression scope where they are evaluated.

Value Categories and Operators:
    -- Most operators expect their operands to be rvalues (e.g. binary operator+ expects rvalues).
    -- Assignment operators require their left operand to be a modifiable lvalue expression.
    -- Prefix operator++ (++x) returns a modifiable lvalue (the incremented variable itself).
    -- Postfix operator++ (x++) returns an rvalue (a temporary holding the value prior to increment).

Lvalue-to-Rvalue Conversion:
    -- An lvalue will implicitly convert to an rvalue whenever an rvalue is expected.
    -- An rvalue, on the other hand, will NEVER implicitly convert to an lvalue.

Differentiating Lvalues and Rvalues:
    -- Lvalue expressions: Evaluate to functions or identifiable objects (including named variables) that persist beyond the expression.
    -- Rvalue expressions: Evaluate to temporary values, including numeric literals and temporary objects, that do not persist beyond the expression.
    -- C-style string literals ("hello") are LVALUES! Unlike numeric literals (which are prvalues), C-style strings are array literals that decay to pointers; decay requires an lvalue with an identifiable address. C++ inherited this for backwards compatibility with C.


12.2 -- Lvalue References
--------------------------------------------------------------------------------
An lvalue reference acts as an alias for an existing lvalue object. Modifying the reference modifies the underlying referent object.

Reference Syntax and Binding:
    -- Identified by a single ampersand (&) in the type specifier (e.g. int& ref { x };).
    -- Reference initialization: Much like constants, all references must be initialized upon creation.
    -- Reference binding: The process of binding a reference to its referent.
    -- Referent: The object or function being referenced.

Fundamental Rules of References:
    1. References cannot be uninitialized: int& ref; is a compile error.
    2. References cannot be reseated: Once initialized, a reference cannot be made to refer to another object. An assignment to a reference (ref = y) assigns the value of y to the referent x, rather than changing what ref binds to.
    3. References are not objects: A reference is not an independent object in memory (it has no independent storage, address, or sizeof). You cannot have a reference to a reference, an array of references, or a pointer to a reference.
    4. Independent lifetimes: A reference and its referent have independent lifetimes.
    5. Dangling reference: When the referent is destroyed before the reference, the reference becomes a dangling reference. Accessing a dangling reference results in Undefined Behavior (UB).
    6. Non-const lvalue references can only bind to modifiable lvalues:
       -- Cannot bind to non-modifiable (const) lvalues (would violate const-correctness).
       -- Cannot bind to rvalues (literals, temporary expressions).


12.3 -- Lvalue References to Const
--------------------------------------------------------------------------------
An lvalue reference to const (often called a const reference) is a reference that treats its referent as non-modifiable.

Best Practice:
    Favor lvalue references to const over non-const lvalue references unless you specifically need to modify the referent object.

Binding Rules for Const References:
    Unlike non-const lvalue references, a const lvalue reference can bind to:
    1. Modifiable lvalues
    2. Non-modifiable (const) lvalues
    3. Rvalues (literals and temporaries)

Binding to Values of a Different Type:
    -- If you bind a const lvalue reference to an expression of a different type (e.g. const int& r { 3.5 };), the compiler generates a temporary object of the target type (int) initialized with the value, and binds the const reference to that temporary!

Lifetime Extension of Temporaries:
    -- When a temporary object (a prvalue) is DIRECTLY bound to a const reference (or rvalue reference), the lifetime of the temporary object is extended to match the lifetime of the reference.
    -- Lifetime extension only applies to direct binding: temporaries returned from functions or bound indirectly do NOT receive lifetime extension across scopes.

Constexpr Lvalue References:
    -- A reference marked constexpr (e.g. constexpr int& ref { g_x };) can be used in constant expressions.
    -- Constexpr references can ONLY bind to objects with static storage duration (global variables or static local variables). This is because the address of static objects is known at compile/link time.
    -- A constexpr reference CANNOT bind to local variables (their stack addresses are only determined at runtime).
    -- When defining a constexpr reference to a const object, both qualifiers are required:
       constexpr const int& ref { g_const_var }; // constexpr applies to reference; const applies to referent type.


12.4 -- Pass by Const Lvalue Reference
--------------------------------------------------------------------------------
When to Use Pass by Value vs. Pass by Reference:
    -- Fundamental types (int, double, char, bool) and enum types are cheap to copy (typically 1–8 bytes) and should be passed by value.
    -- Class types (e.g. std::string, std::vector, user-defined structs/classes) can be expensive to copy and should be passed by const lvalue reference (const T&).

Passing Strings: Prefer std::string_view over const std::string&:
    Prefer passing string parameters using std::string_view (by value) instead of const std::string&, unless the function must interact with APIs requiring null-terminated C-style strings or std::string parameters.

Efficiency Comparison: std::string_view vs. const std::string&:
+------------------------+------------------------------+----------------------------------------------+
| Argument Type          | std::string_view Parameter   | const std::string& Parameter                 |
+------------------------+------------------------------+----------------------------------------------+
| std::string            | Inexpensive conversion       | Inexpensive reference binding                |
| std::string_view       | Inexpensive copy (pointer+len)| Expensive explicit conversion to std::string |
| C-style string/literal | Inexpensive conversion       | Expensive dynamic allocation of std::string  |
+------------------------+------------------------------+----------------------------------------------+


12.5 -- Pointers and Null Pointers
--------------------------------------------------------------------------------
A pointer is a compound type that stores the memory address of another object.

Pointer Basics:
    -- Address-of operator (&): Retrieves the memory address of an lvalue object.
    -- Dereference operator (*): Accesses the object located at the stored memory address.
    -- Pointers are independent objects: unlike references, pointers occupy memory, have their own addresses, and can be reseated (reassigned to point to different objects).

Const and Pointers:
    1. Pointer to non-const: int* ptr; (both pointer and referent modifiable).
    2. Pointer to const (low-level const): const int* ptr; (referent cannot be modified through pointer; pointer can be reseated).
    3. Const pointer (top-level const): int* const ptr { &x }; (pointer address is immutable; referent can be modified).
    4. Const pointer to const: const int* const ptr { &x }; (neither pointer nor referent can be modified).

Null Pointers:
    -- A null pointer points to nothing.
    -- In modern C++ (since C++11), ALWAYS use nullptr (type std::nullptr_t).
    -- Avoid legacy 0 and NULL macro: NULL is often defined as integer 0, causing dangerous overload resolution errors where NULL unexpectedly matches integer overloads instead of pointer overloads. nullptr has distinct type std::nullptr_t and matches only pointer overloads.
    -- Dereferencing a null pointer causes undefined behavior (usually immediate segmentation fault). Always check if (ptr) before dereferencing if nullability is possible.

Nullifying a Pointer Using a Reference to Pointer:
    -- To allow a function to modify the caller's pointer variable (such as setting it to nullptr), pass the pointer by reference: void clearPtr(int*& ptr) { ptr = nullptr; }.


12.6 -- Return by Reference and Return by Address
--------------------------------------------------------------------------------
Returning by reference (or address) avoids copying the returned value and allows direct access to the referenced object.

Rules and Pitfalls of Returning by Reference:
    1. Lifetime requirement: Objects returned by reference must outlive the function scope.
       CRITICAL WARNING: Never return a local non-static variable by reference or address! The local variable is destroyed when the function returns, leaving a dangling reference.
    2. Lifetime extension does NOT cross function boundaries:
       If a function creates a temporary and returns it by const reference, binding the result in the caller will NOT extend the temporary's lifetime. It creates a dangling reference and causes UB.
    3. Returning reference parameters: It is completely safe to return a reference parameter by reference, provided the parameter refers to an object that outlives the call.
    4. Returning rvalues passed by const reference: If an rvalue argument is passed to a const reference parameter and returned by const reference, it can only be safely used to initialize a value object (which copies/moves it), NOT to bind another reference.
    5. Don't return non-const static local variables by reference: Multiple callers or re-entrant invocations can overwrite or conflict with each other.

Prefer Return by Reference over Return by Address:
    Use return by reference as the primary mechanism. Reserve return by address for scenarios where returning "no object" (via nullptr) is a valid, expected outcome.


12.7 -- In, Out, and In-Out Parameters
--------------------------------------------------------------------------------
Classification of Function Parameters:
    1. "In" Parameters: Pass data into a function. The function reads the data but does not modify it.
       -- Fundamental types: pass by value.
       -- Class types: pass by const lvalue reference.
    2. "Out" Parameters: Used to return results back to the caller by writing into variables passed by non-const reference or pointer.
    3. "In-Out" Parameters: The function reads the initial value and modifies the object in place (passed by non-const reference).

Why Out-Parameters Should Be Avoided:
    -- Awkward call syntax: Caller must declare uninitialized dummy variables before calling the function.
    -- Unclear call semantics: Looking at a function call f(x), it is not obvious whether x is read or overwritten.
    -- Error-prone: Callers may pass variables whose existing values are unintentionally overwritten.

Modern C++ Alternatives to Out-Parameters:
    Instead of out-parameters, modern C++ functions should return values directly:
    -- For single values: return by value (benefiting from Return Value Optimization / Move Semantics).
    -- For multiple return values: return a std::pair, std::tuple, or a named struct:
       struct Point { int x; int y; };
       Point getCoordinates() { return { 10, 20 }; }
       auto [x, y] = getCoordinates(); // Structured binding (C++17)


12.8 -- Type Deduction with Pointers, References, and Const
--------------------------------------------------------------------------------
When using type deduction (auto), the compiler follows a strict set of rules regarding modifiers (const, &, *).

Rules of Type Deduction:
    1. Type deduction DROPS references:
       int x { 5 };
       int& ref { x };
       auto a { ref }; // a is int, NOT int&! (Copies the value).
       To deduce a reference, explicitly supply &: auto& b { ref }; // b is int&.
    2. Type deduction DROPS top-level const:
       const int c { 5 };
       auto d { c }; // d is int (top-level const dropped).
       To retain const: const auto e { c }; // e is const int.
    3. Dropping a reference converts low-level const to top-level const, which is then dropped:
       const int& cref { x };
       auto f { cref }; // Reference dropped -> becomes const int -> top-level const dropped -> int!
       const auto& g { cref }; // g is const int&.
    4. Type deduction does NOT drop pointers:
       int* ptr { &x };
       auto p { ptr }; // p is int*.
    5. Low-level const on pointers is NOT dropped:
       const int* cptr { &x };
       auto cp { cptr }; // cp is const int* (pointer to const; low-level const preserved).
    6. auto vs. auto*:
       -- auto can deduce both pointer and non-pointer types.
       -- auto* forces the deduced type to be a pointer (fails to compile if initializer is not a pointer):
          auto* p1 { ptr }; // OK: int*
          // auto* p2 { x }; // COMPILE ERROR: x is not a pointer!
    7. Const placement with auto*:
       -- const auto* ptr : Pointer to const (low-level const).
       -- auto* const ptr : Const pointer (top-level const).
       -- const auto* const ptr : Const pointer to const.


12.9 -- std::optional (C++17)
--------------------------------------------------------------------------------
Introduced in C++17, std::optional<T> (defined in <optional>) is a vocabulary type that represents a value that may or may not exist.

Key Features of std::optional:
    -- Contains either a valid value of type T, or an empty state represented by std::nullopt.
    -- Does not perform dynamic memory allocation: stores the value inline alongside a boolean flag.

Basic Usage:
    std::optional<int> findUser(int id)
    {
        if (id == 42) return 100;
        return std::nullopt; // Return empty optional
    }

Accessing Values Safely:
    -- Check existence: if (opt) or if (opt.has_value()).
    -- Dereference operator (*opt): Accesses the value without checking (Undefined Behavior if empty!).
    -- Member access operator (opt->member): Accesses members of T.
    -- value(): Returns the value, or throws std::bad_optional_access if empty.
    -- value_or(fallback): Returns the value if present, or fallback value if empty:
       int val = opt.value_or(0);

Best Practices for std::optional:
    -- Prefer std::optional for optional return types.
    -- For optional function parameters:
       -- Prefer function overloading when possible.
       -- Use std::optional<T> for optional parameters only when T is cheap to pass by value.
       -- When T is expensive to copy, use const T* (defaulted to nullptr) instead of std::optional<T>.
    -- Optional references: std::optional<T&> is ill-formed in standard C++. To store an optional reference, use std::optional<std::reference_wrapper<T>>.


================================================================================
Directory Structure and Code Examples
================================================================================
Chapter folder: 12_References_and_Pointers/

12_1_Value categories/
    1_lvalue_n_rvalue.cpp
    2_identify_lvalue_rvalue.cpp

12_2_Lvalue references/
    1_reference_initialization.cpp
    2_non_const_n_const_ref.cpp
    3_ref_type_and_object_type_matching.cpp
    4_misc.cpp

12_3_Lvalue_references_to_const/
    1_const_lvalue reference_with_different_type.cpp
    2_Const_reference_extend_lifetime_of_temporary_object.cpp
    3_constexpr_lvalue_references.cpp

12_4_Pass_by_const_lvalue_reference/
    1_pass_by_const_lvalue_ref.cpp
    2_str_strings_string_views_conversions.cpp

12_5_Pointers/
    1_pointers_cast.cpp
    2_nullptr.cpp
    3_const_to_pointer.cpp
    4_nullify_ptr_using_ref.cpp
    5_NULL_0_nullptr.cpp
    6_NULL_0_nullptr.cpp

12_6_return_by_reference_n_return_by_address/
    1_return_by_reference_scope.cpp
    2_life_time_extension_of_temp_by_ref.cpp
    3_return_const_ref.cpp
    4_return_const_ref_parameter.cpp

12_7_In_n_Out_parameters/
    1_in_parameters.cpp
    2_out_parameters.cpp
    3_in_out_parameters.cpp
    4_out_parameter_alternative_in_modern_cpp.cpp

12_8_Type_deduction_with_pointers_references_and_const/
    1_drop_const.cpp
    2_drop_const_reapply.cpp
    3_type_deduction_drops_references.cpp
    4_drops_references_to_const_reapply.cpp
    4_type_deduction_drops_references_to_const.cpp
    5_low-level_const_not_dropped.cpp
    5_type_deduction_for_constexpr_references.cpp
    6_type_deduction_and_pointers.cpp
    7_type_deduction_and_const_pointers.cpp
    8_type_deduction_and_const_pointers.cpp

12_9_std_optional/
    1_std_optional.cpp
    1_std_optional_ref_wrap.cpp
    2_std_optional_examples.cpp
    3_std_optional_examples.cpp
    4_std_optional_examples.cpp
    5_std_optional_examples.cpp
    6_std_optional_examples.cpp
