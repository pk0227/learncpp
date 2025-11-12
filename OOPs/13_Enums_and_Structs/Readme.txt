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
        example : Pet pet {3};  // compilation error
        
        work around : use static_cast to convert the integer to appropriate enumerated type
        example : Pet pet{ static_cast<Pet>(3) };

        Extraction >> operator doesnt know how to deal with Enumeration type.
        Example :     Pet pet { pig };
                      std::cin >> pet; // compile error: std::cin doesn't know how to input a Pet

                        int input{};
                        std::cin >> input; // input an integer
                        Pet pet{ static_cast<Pet>(input) }; // static_cast our integer to a Pet

3. Introduction to overloading the I/O operators
    -- Operators can also be overloaded as member functions of the left-most operand.
    -- Overloading operator<< to print an enumerator.
    -- Overloading operator>> to input an enumerator.

4. Scoped enumerations (enum classes)
    -- Although unscoped enumerations are distinct types in C++, they are not type-safe.
       For example, two distinct unscoped enum types can be compared if their enumerators have the same underlying integer value, because unscoped enums implicitly convert to integers.

    -- That solution is the scoped enumeration (enum class): 
        -- They won’t implicitly convert to integers. 
            -- Example : comparision between distinct scoped numeration types , red+5 expressions wont work.
        -- The enumerators are only placed into the scope region of the enumeration (not into the scope region where the enumeration is defined).
            -- Comparison of enumeration type with scoped enumerators will work.
        -- To make a scoped enumeration, we use the keywords enum class.
        -- enum struct also works in this context, and behaves identically to enum class. However, use of enum struct is non-idiomatic, so avoid its use.

    -- scoped enumerations act like a namespace for their enumerators. This built-in namespacing helps reduce global namespace pollution.
       the potential for name conflicts when scoped enumerations are used in the global scope.        

    -- Since scoped enumerations offer their own implicit namespacing for enumerators, there’s no need to put scoped enumerations inside another scope region (such as a namespace), 
       unless there’s some other compelling reason to do so, as it would be redundant.
    -- using enum statement imports all of the enumerators from an enum into the current scope.
    -- Favor scoped enumerations over unscoped enumerations unless there’s a compelling reason to do otherwise.

5. Introduction to structs, members, and member selection
    Struct aggregate initialization
    -- aggregate is any type that can contain multiple data members. 
    -- Some types of aggregates allow members to have different types (e.g. structs), 
       while others require that all members must be of a single type (e.g. arrays).
    -- Aggregates use a form of initialization called aggregate initialization, which allows us to directly initialize the members of aggregates.
       To do this, we provide an initializer list as an initializer, which is just a braced list of comma-separated values.
       There are 2 primary forms of aggregate initialization:
        -- copy-list initialization using braced list
        -- list initialization using braced list (preferred)
    
    Missing initializers in an initializer list
    -- If an aggregate is initialized but the number of initialization values is fewer than the number of members, then each member without an explicit initializer is initialized as follows:
        -- If the member has a default member initializer, that is used.
        -- Otherwise, the member is copy-initialized from an empty initializer list. In most cases, this will perform value-initialization on those members (on class types, this will invoke the default constructor even if a list constructor exist).

    Aggregate initialization
    -- If a class/struct has no user-declared constructors, {} performs aggregate initialization — directly initializing members.
    -- {} without values (Point p{}) still does aggregate initialization, but members get value-initialized (zeroed) if members have no default member initializer.
    -- If any constructor exists (even = default), the class is no longer an aggregate — {} now calls the appropriate constructor.
    -- If {} arguments don’t match any constructor parameters, compilation fails.

    -- Variables of a struct type can be const (or constexpr), and just like all const variables, they must be initialized.
    
    Designated initializers
    -- When initializing a struct from a list of values, the initializers are applied to the members in order of declaration.
    -- Now consider what would happen if you were to update this struct definition to add a new member that is not the last member.
    -- To help avoid this, C++20 adds a new way to initialize struct members called designated initializers. 
       Designated initializers allow you to explicitly define which initialization values map to which members. 
       The members can be initialized using list or copy initialization, and must be initialized in the same order in which they are declared in the struct, otherwise a warning or error will result. 
       Members not designated an initializer will be value initialized.

     -- When adding a new member to an aggregate, it’s safest to add it to the bottom of the definition list so the initializers for other members don’t shift.
     -- Assignment with an initializer list.
     -- Assignment with designated initializers.
     -- Initializing a struct with another struct of the same type.

    Default member initialization
    -- Recapping the initialization possibilities
        -- If an aggregate is defined with an initialization list:
            -- If an explicit initialization value exists, that explicit value is used.
            -- If an initializer is missing and a default member initializer exists, the default is used.
            -- If an initializer is missing and no default member initializer exists, value initialization occurs.

        -- If an aggregate is defined with no initialization list:
            -- If a default member initializer exists, the default is used.
            -- If no default member initializer exists, the member remains uninitialized.

    -- Members are always initialized in the order of declaration.
    -- Provide a default value for all members. This ensures that your members will be initialized even if the variable definition doesn’t include an initializer list.
    -- For aggregates, prefer value initialization (with an empty braces initializer) to default initialization (with no braces).

6. Passing and returning structs
    -- Passing temporary structs, A few more things about temporary objects:
        -- They are created and initialized at the point of definition, and are destroyed at the end of the full expression in which they are created.
        -- Evaluation of a temporary object is an rvalue expression, it can only be used in places where rvalues are accepted.
        -- When a temporary object is used as a function argument, it will only bind to parameters that accept rvalues. 
        -- This includes pass by value and pass by const reference, and excludes pass by non-const reference and pass by address.

    -- Returning structs, Deducing the return type.
    












