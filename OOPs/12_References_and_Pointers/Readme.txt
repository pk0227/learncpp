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

6. Pass by const lvalue reference
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

7. Pointers
    -- Use nullptr when you need a null pointer literal for initialization, assignment, or passing a null pointer to a function.
    
    










