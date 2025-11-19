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
    
3. Const class objects and const member functions
    -- Const objects must be initialized at the time of creation.
    -- Modifying the data members of const objects is disallowed.
        -- changing member variables directly (if they are public).
        -- calling member functions that set the value of member variables.
    -- Const objects may not call non-const member functions.
    -- A const object can only call const member functions (and static functions).

    Const member functions
        -- A const member function is a member function that guarantees it will not modify the object or call any non-const member functions (as they may modify the object).
        -- For member functions defined outside of the class definition, the const keyword must be used on both the function declaration in the class definition, and on the function definition outside the class definition.
        -- Constructors may not be made const, as they need to initialize the members of the object, which requires modifying them. 
        -- Const member functions can 
            -- modify non-members (such as local variables and function parameters).
            -- call non-member functions per usual.
            -- call const member functions.
            The const only applies to members.
        -- Const member functions may be called on non-const objects.
        -- A member function that does not (and will not ever) modify the state of the object should be made const, so that it can be called on both const and non-const objects.
        -- Const objects via pass by const reference.
        -- Member function const and non-const overloading.

4. Public and private members and access specifiers and access functions
    -- Member access : Each member of a class type has a property called an access level that determines who can access that member.
    -- C++ has three different access levels: public, private, and protected. 
    
    -- The members of a struct are public by default. Public members can be accessed by other members of the class type, and by the public.
    -- The term “the public” means outside of the members of a given class type. This includes non-member functions too.

    -- The members of a class are private by default. Private members can be accessed by other members of the class, but can not be accessed by the public.
    -- A class with private members is no longer an aggregate, and therefore can no longer use aggregate initialization.

    -- we can explicitly set the access level of our members by using an access specifier. C++ provides three access specifiers: public:, private:, and protected:.
    
    Access level summary
    +----------------+------------------+----------------+----------------------+---------------------+
    | Access Level   | Access Specifier | Member Access  | Derived Class Access | Public Access       |
    +----------------+------------------+----------------+----------------------+---------------------+
    | Public         | public:          | yes            | yes                  | yes                 |
    +----------------+------------------+----------------+----------------------+---------------------+
    | Protected      | protected:       | yes            | yes                  | no                  |
    +----------------+------------------+----------------+----------------------+---------------------+
    | Private        | private:         | yes            | no                   | no                  |
    +----------------+------------------+----------------+----------------------+---------------------+

    Access level best practices for structs and classes
        -- Classes should generally make member variables private (or protected), and member functions public.
        -- Structs should generally avoid using access specifiers (all members will default to public) to keep structs to be aggregates.

    Access levels work on a per-class basis
        -- A member function can also directly access the private members of ANY other object of the same class type that is in scope.

    -- structs inherit from other class types publicly and classes inherit privately.

    Conditions to be an aggregate:
        1. The class type should not have any private or protected members.
        2. The class type should not have any user-declared constructors (including defaulted or deleted constructors).

        A class with private members or user-declared constructors is no longer an aggregate.
        Therefore, the class cannot use aggregate initialization.
        In such cases, brace initialization will attempt to call a matching constructor.
        If a constructor taking std::initializer_list is defined, it will take precedence over other constructors.

    Access Functions
    -- is a trivial public member function whose job is to retrieve or change the value of a private member variable.
    Getters (also sometimes called accessors)
        -- public member functions that return the value of a private member variable.
        -- Getters are usually made const.
    Setters (also sometimes called mutators)
        -- public member functions that set the value of a private member variable.
    Getters should return by value or by const lvalue reference.
        -- Getters should provide “read-only” access to data. Therefore, the best practice is that they should return by 
           either value (if making a copy of the member is inexpensive) or by const lvalue reference (if making a copy of the member is expensive).

5. Member functions returning references to data members
    -- A member function returning a reference should return a reference of the same type as the data member being returned, to avoid unnecessary conversions.

    Rvalue implicit objects and return by reference
        -- An rvalue object is destroyed at the end of the full expression in which it is created. Any references to members of the rvalue object are left dangling at that point.
        -- A reference to a member of an rvalue object can only be safely used within the full expression where the rvalue object is created.

        -- Prefer to use the return value of a member function that returns by reference immediately, to avoid issues with dangling references when the implicit object is an rvalue.
        -- Do not return non-const references to private data members.
        -- Const member functions can’t return non-const references to data members.

6. Encapsulation (The benefits of data hiding)
    Interface /  class interface / public interface
        -- defines how a user of the class type will interact with objects of the class type.
        -- Because only public members can be accessed from outside of the class type, the public members of a class type form its interface.
        -- For this reason, an interface composed of public members is sometimes called a public interface.
        -- An interface is an implicit contract between the author of a class and the user of a class.
        -- If an existing interface is ever changed, any code that uses it may break. Therefore, it is important to ensure the interfaces for our class types are well-designed and stable (don’t change much).
    
     Implementation
        -- The implementation of a class type consists of the code that actually makes the class behave as intended. This includes both the member variables that store data, and the bodies of the
           member functions that contain the program logic and manipulate the member variables.
    
    Data hiding / abstraction
        -- is a technique used to enforce the separation of interface and implementation by hiding (making inaccessible) the implementation of a program-defined data type from users.
        -- data hiding in a C++ is done by making data members private and member functions are public, so that the user can call them.
        -- Structs, on the other hand, should not use data hiding, as having non-public members prevents them from being treated as aggregates.
    
    Encapsulation
        -- The bundling of data and functions in an enclosing container where the functions operate on on instances of that data.
        -- Encapsulation is a pre-requisite for data hiding.

    -- Data hiding make classes easier to use, and reduces complexity by providing public interface to operate on the object of the class.
    -- Data hiding allows us to maintain invariants.

    Class invariants
        -- are conditions that must be true throughout the lifetime of an object in order for the object to stay in a valid state.
    
    -- Data hiding allows us to do better error detection (and handling).
    -- Data hiding makes it possible to change implementation details without breaking existing programs.

    Classes with interfaces are easier to debug
        -- If everyone is able to set the member variable directly, tracking down which piece of code actually modified the member variable to the wrong value can be difficult.
        -- This can involve breakpointing every statement that modifies the member variable -- and there can be lots of them.
        -- However, if a member can only be changed through a single member function, then you can simply breakpoint that single function and watch as each caller changes the value. 
           This can make it much easier to determine who the culprit is.

    -- Prefer a member function when the function needs access to private (or protected) data that should not be exposed.
    -- Prefer a non-member function otherwise (especially for functions that do not modify the state of the object).
    



    



        