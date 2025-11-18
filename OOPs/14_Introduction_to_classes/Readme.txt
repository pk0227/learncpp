OOP brings a number of other useful concepts to the table: 
    -- encapsulation, 
    -- abstraction,
    -- inheritance,
    -- polymorphism.

1. Introduction to classes
    -- structs are aggregates. As useful as structs are, structs have a number of deficiencies.

    The class invariant problem for class types which include structs, classes, and unions.
        -- Perhaps the biggest difficulty with structs is that they do not provide an effective way to document and enforce class invariants.
        -- A class invariant is a condition that must be true throughout the lifetime of an object in order for the object to remain in a valid state. An object that has a violated class invariant is said to be in an invalid state, and unexpected or undefined behavior may result from further use of that object.

2. Member functions
    -- Functions that belong to a class type are called member functions.
    -- Functions that are not member functions are called non-member functions (or occasionally free functions).
    -- As a reminder, a definition is also a declaration, so if we define a member function inside the class, that counts as a declaration.
    -- Member functions defined inside the class type definition are implicitly inline, so they will not cause violations of the one-definition rule if the class type definition is included into multiple code files.
    -- All (non-static) member functions must be called using an object of that class type.
    -- The object that a member function is called on is implicitly passed to the member function. For this reason, the object that a member function is called on is often called the implicit object.
    -- With non-member functions, we have to explicitly pass an object to the function to work with, and members are explicitly accessed through that object.
    -- Member variables and functions can be defined in any order.

    Warning
    -- Data members are initialized in order of declaration. If the initialization of a data member accesses another data member that isn’t declared until later (and thus hasn’t been initialized yet), the initialization will result in undefined behavior.
    -- For this reason, it’s generally a good idea to avoid using other members inside default member initializers.

    -- Member functions can be overloaded.
    -- Member functions can be used with both structs and classes.
    -- However, structs should avoid defining constructor member functions, as doing so makes them a non-aggregate.
    -- A class type with a constructor is no longer an aggregate, and we want our structs to remain aggregates.

    -- If your class type has no data members, prefer using a namespace.
    