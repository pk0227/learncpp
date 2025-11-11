1. Introduction to program-defined (user-defined) types
    -- C++ has two different categories of compound types that can be used to create program-defined types:
        -- Enumerated types (including unscoped and scoped enumerations)
        -- Class types (including structs, classes, and unions).
    -- Don’t forget to end your type definitions with a semicolon.
    -- A program-defined type used in multiple code files should be defined in a header file with the same name as the program-defined type and then #included into each code file as needed.
    -- Type definitions are partially exempt from the one-definition rule (ODR)

2. Unscoped enumerations
    Enumerations / enumerated type / enum 
    -- is a compound data type whose values are restricted to a set of named symbolic constants (called enumerators).
    
    Enumerator
    -- is a specific named value belonging to the enumeration.
    -- C++ supports two kinds of enumerations: 
        -- unscoped enumerations.
        -- scoped enumerations.
    -- Since enumerations are user-defined types, each enumeration needs to be fully defined before we can use it (a forward declaration is not sufficient).
    -- Enumerators are implicitly constexpr.
    -- Enumerated types are distinct types.
    -- Because enumerations are small and inexpensive to copy, it is fine to pass (and return) them by value.
    -- Enums can also be used to define a collection of related bit flag positions for use with std::bitset.
    -- Unscoped enumerations also provide a named scope region for their enumerators.
    -- This means we can access the enumerators of an unscoped enumeration with enumeration name.
        Color::red where Color is enumeration and red is enumerator.  
    
    Scope of unscoped enumerations
    -- Scope of enumerator names into the same scope as the enumeration definition itself.
    -- This significantly raises the chance of naming collisions.
    
    Avoiding enumerator naming collisions using namespace, class.
    -- Prefer putting your enumerations inside a named scope region (such as a namespace or class) so the enumerators don’t pollute the global namespace.
    -- Classes also provide a scope region, and it’s common to put enumerated types related to a class inside the scope region of the class.

    -- Avoid assigning explicit values to your enumerators unless you have a compelling reason to do so.
    
    Value-initializing an enumeration
    -- If an enumeration is zero-initialized (which happens when we use value-initialization), the enumeration will be given value 0, even if there is no corresponding enumerator with that value.

    -- Unscoped enumerations will implicitly convert to integral values. Because enumerators are compile-time constants, this is a constexpr conversion.

    Enumeration size and underlying type (base)
    -- The specific integral type used to represent the value of enumerators is called the enumeration’s underlying type (or base).
    -- Most compilers will use int as the underlying type.

    -- No implicitly conversion of an integer to an unscoped enumeration if underlying(base) is not int.







