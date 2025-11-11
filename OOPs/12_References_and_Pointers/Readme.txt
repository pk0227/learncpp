1. Introduction to compound data types
    -- Every data type is either a fundamental type or a compound type.
    -- C++ supports the following compound types:
        Functions
        C-style Arrays
        Pointer types:
            -- Pointer to object
            -- Pointer to function
        Pointer to member types:
            -- Pointer to data member
            -- Pointer to member function
        Reference types:
            -- L-value references
            -- R-value references
        Enumerated types:
            -- Unscoped enumerations
            -- Scoped enumerations
        Class types:
            -- Structs
            -- Classes
            -- Unions

2. Value categories
    -- Prior to C++11, there were only two possible value categories: lvalue and rvalue.
    -- In C++11, three additional value categories (glvalue, prvalue, and xvalue) were added to support a new feature called move semantics.
    
    Lvalue and rvalue expressions
    -- lvalue / ell-value / left value / locator value / l-value : is an expression that evaluates to an identifiable object or function (or bit-field).
    -- lvalues come in two subtypes: 
        -- modifiable lvalue 
        -- non-modifiable lvalue where lvalue is const or constexpr.
    -- rvalue / arr-value / right value / r-value : evaluate to a value. rvalues include literals (except C-style string literals, which are lvalues) and the return value of functions and operators that return by value.
    -- Rvalues aren’t identifiable (meaning they have to be used immediately), and only exist within the scope of the expression in which they are used.

    Value categories and operators
    -- operators expect their operands to be rvalues. For example, binary operator+ expects its operands to be rvalues.
    -- an assignment operation requires its left operand to be a modifiable lvalue expression.
    
    Lvalue-to-rvalue conversion
    -- An lvalue will implicitly convert to an rvalue. This means an lvalue can be used anywhere an rvalue is expected.
    -- An rvalue, on the other hand, will not implicitly convert to an lvalue.

    How to differentiate lvalues and rvalues
    -- Lvalue expressions are those that evaluate to functions or identifiable objects (including variables) that persist beyond the end of the expression.
    -- Rvalue expressions are those that evaluate to values, including literals and temporary objects that do not persist beyond the end of the expression.

    -- whether operator++ results in an lvalue or an rvalue depends on whether it is used as a prefix operator (which returns an lvalue) or a postfix operator (which returns an rvalue).
    -- Unlike the other literals (which are rvalues), a C-style string literal is an lvalue because C-style strings (which are C-style arrays) decay to a pointer. The decay process only works if the array is an lvalue (and thus has an address that can be stored in the pointer). 
       C++ inherited this for backwards compatibility.
    
3. Lvalue references
    -- Lvalue reference types can be identified by use of a single ampersand (&) in the type specifier.
    -- Much like constants, all references must be initialized. References are initialized using a form of initialization called reference initialization.
    
    -- The process by which such a reference is bound is called reference binding.
    -- The object (or function) being referenced is sometimes called the referent.

    -- If non-const lvalue references could be bound to non-modifiable (const) lvalues or rvalues, then you would be able to change those values through the reference, which would be a violation of their const-ness.
    -- References can’t be reseated (changed to refer to another object)
    -- References and referents have independent lifetimes
    -- Dangling reference : When an object being referenced is destroyed before a reference to it, the reference is left referencing an object that no longer exists.
    -- References aren’t objects

4. Lvalue references to const
    -- Favor lvalue references to const over lvalue references to non-const unless you need to modify the object being referenced.
    
    Initializing an lvalue reference to const with a value of a different type
    -- If you try to bind a const lvalue reference to a value of a different type, the compiler will create a temporary object of the same type as the reference, initialize it using the value, and then bind the reference to the temporary.

    Const references bound to temporary objects extend the lifetime of the temporary object
    -- Lvalue references can only bind to modifiable lvalues.
    -- Lvalue references to const can bind to modifiable lvalues, non-modifiable lvalues, and rvalues. 
    -- Lifetime extension only works when a const reference is directly bound to a temporary. Temporaries returned from a function (even ones returned by const reference) are not eligible for lifetime extension.

    Constexpr lvalue references
    -- constexpr allows the reference to be used in a constant expression. Constexpr references have a particular limitation: they can only be bound to objects with static duration (either globals or static locals). 
       This is because the compiler knows where static objects will be instantiated in memory, so it can treat that address as a compile-time constant.
    -- A constexpr reference cannot bind to a (non-static) local variable. This is because the address of local variables is not known until the function they are defined within is actually called.
    -- defining a constexpr reference to a const variable, we need to apply both constexpr (which applies to the reference) and const (which applies to the type being referenced).

5. Pass by const lvalue reference
    When to use pass by value vs pass by reference
        -- Fundamental types and enumerated types are cheap to copy, so they are typically passed by value.
        -- Class types can be expensive to copy (sometimes significantly so), so they are typically passed by const reference.

    For function parameters, prefer std::string_view over const std::string& in most cases
        -- Prefer passing strings using std::string_view (by value) instead of const std::string&, unless your function calls other functions that require C-style strings or std::string parameters.

    Why std::string_view parameters are more efficient than const std::string&
    +------------------------+------------------------------+----------------------------------------------+
    | Argument Type          | std::string_view parameter   | const std::string& parameter                 |
    +------------------------+------------------------------+----------------------------------------------+
    | std::string            | Inexpensive conversion       | Inexpensive reference binding                |
    | std::string_view       | Inexpensive copy             | Expensive explicit conversion to std::string |
    | C-style string/literal | Inexpensive conversion       | Expensive conversion                         |
    +------------------------+------------------------------+----------------------------------------------+

6. Pointers
    -- Use nullptr when you need a null pointer literal for initialization, assignment, or passing a null pointer to a function.
    -- nullify pointer using reference to the pointer.

7. Return by reference and Return by address
    -- Objects returned by reference must live beyond the scope of the function returning the reference, or a dangling reference will result. 
       Never return a (non-static) local variable or temporary by reference.
    -- Lifetime extension doesn’t work across function boundaries
        -- direct binding -> will extend lifetime
        -- indirect binding -> will bind to dangling reference an cause undefined behavior (UB)
    -- When a temporary object (a prvalue) is directly bound to a const reference (or rvalue reference), the lifetime of that temporary is extended to match the lifetime of the reference.
    -- Don’t return non-const static local variables by reference.
    -- It’s okay to return reference parameters by reference. But, reference parameter must refer to the object that has full life-time/scope in caller function.
    -- It’s okay for an rvalue passed by const reference to be returned by const reference if and only if the returned const reference is used for
       initializing lvalue variable.
    -- Prefer return by reference over return by address unless the ability to return “no object” (using nullptr) is important.
    
8. In and Out parameters
    -- “In” Parameters : Used to pass data into a function. The function reads the value, but does not modify it (or at least shouldn’t).
    -- Not used to return data.
    
    -- “Out” Parameters : Used to pass results out of a function — i.e., the function writes into them.
    -- In C++, these are usually implemented using non-const references or pointers.
    -- Their initial values are ignored.

    -- “In-Out” Parameters : Used both to read and modify the argument.
    -- Function both reads and updates the argument.

    -- Avoid out-parameters (except in the rare case where no better options exist).
    -- Prefer pass by reference for non-optional out-parameters.
    -- In modern C++, “out” parameters are less common — instead, functions usually return results directly (often using std::pair, std::tuple, or a struct).
            
            std::pair<int, int> getCoordinates() {
                return {10, 20};
            }

            auto [x, y] = getCoordinates();

9. Type deduction with pointers, references, and const
    -- Type deduction will drop const from types.
    -- Type deduction drops references.
    -- If you want a const reference, reapply the const qualifier even when it’s not strictly necessary, as it makes your intent clear and helps prevent mistakes.

    constexpr references
    -- Constexpr is not part of an expression’s type, so it is not deduced by auto
    -- When defining a const reference (e.g. const int&), the const applies to the object being referenced, not the reference itself.
    -- When defining a constexpr reference to a const variable (e.g. constexpr const int&), we need to apply both constexpr (which applies to the reference) and const (which applies to the type being referenced).

    Type deduction and pointers
    -- Unlike references, type deduction does not drop pointers.

    Type deduction and const pointers
    -- we use either auto const or const auto, we’re saying, “make the deduced pointer a const pointer.
    -- However, when we use auto*, the order of the const qualifier matters.
    -- A const on the left means “make the deduced pointer a pointer to const.
    -- A const on the right means “make the deduced pointer type a const pointer.

    Top-level vs low-level const:
    -- A top-level const applies to the object itself (e.g. const int x or int* const ptr).
    -- A low-level const applies to the object accessed through a reference or pointer (e.g. const int& ref, const int* ptr).

    What type deduction deduces:
    -- Type deduction first drops any references (unless the deduced type is defined as a reference). For a const reference, dropping the reference will cause the (low-level) const to become a top-level const.
    -- Type deduction then drops any top-level const (unless the deduced type is defined as const or constexpr).
    -- Constexpr is not part of the type system, so is never deduced. It must always be explicitly applied to the deduced type.
    -- Type deduction does not drop pointers.
    -- Always explicitly define the deduced type as a reference, const, or constexpr (as applicable), and even if these qualifiers are redundant because they would be deduced. This helps prevent errors and makes it clear what your intent is.

    Type deduction and pointers:
    -- When using auto, the deduced type will be a pointer only if the initializer is a pointer. When using auto*, the deduced type is always a pointer, even if the initializer is not a pointer.
    -- auto const and const auto both make the deduced pointer a const pointer. There is no way to explicitly specify a low-level const (pointer-to-const) using auto.
    -- auto* const also makes the deduced pointer a const pointer. const auto* makes the deduced pointer a pointer-to-const. If these are hard to remember, int* const is a const pointer (to int), so auto* const must be a const pointer. const int* is a pointer-to-const (int), so const auto* must be a pointer-to-const)
    -- Consider using auto* over auto when deducing a pointer type, as it allows you to explicitly reapply both the top-level and low-level const, and will error if a pointer type is not deduced.

10. std::optional
    -- C++17 introduces std::optional, which is a class template type that implements an optional value.
    -- a std::optional<T> can either have a value of type T, or not.
    -- Prefer std::optional for optional return types.
    -- Prefer function overloading for optional function parameters (when possible).
    -- Otherwise, use std::optional<T> for optional arguments when T would normally be passed by value. 
    -- Favor const T* when T is expensive to copy.
    -- we recommend using std::optional<T> as an optional parameter only when T would normally be passed by value. Otherwise, use const T*













