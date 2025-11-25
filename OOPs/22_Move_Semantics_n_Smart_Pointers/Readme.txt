1 — Introduction to smart pointers and move semantics
    -- The problem with raw-pointers is that Even if we remember to delete ptr at the end of the function, there are a myriad of ways that ptr may not be deleted if the function exits early.
    -- The raw pointers have no inherent mechanism to clean up after themselves.

    Smart pointer classes to the rescue
        -- classes contain destructors that automatically get executed when an object of the class goes out of scope. So if we allocate (or acquire) memory in your constructor, we can deallocate it in our destructor,
           and be guaranteed that the memory will be deallocated when the class object is destroyed.
        -- Consider a class whose sole job was to hold and “own” a pointer passed to it, and then deallocate that pointer when the class object went out of scope.
        -- A Smart pointer is a composition class that is designed to manage dynamically allocated memory and ensure that memory gets deleted when the smart pointer object goes out of scope.
    
    Why smart pointers use move semantics instead of copy semantics
    -- Smart pointers avoid shallow copy problems and the impracticality of deep copying.
       A shallow copy of a raw pointer copies only the pointer value, not the resource it points to. 
       If multiple smart pointers shallow-copied the same resource, they would all try to delete it when destroyed → double-free crash.

    -- To fix that, one might think of performing a deep copy (duplicate the resource), but deep copying:   
        -- May be impossible (e.g., file handles, sockets, mutexes)
        -- May be extremely expensive (large containers, images, database connections)
        -- May break resource semantics (unique ownership should remain unique)

    Because of these limitations, smart pointers choose move semantics, which transfers ownership safely rather than copying it.

2 — R-value references
    L-value references : L-value references can only be initialized with modifiable l-values.
    +--------------------------+-----------------------------+--------------+
    |     L-value reference    |     Can be initialized with |   Can modify |
    +--------------------------+-----------------------------+--------------+
    | Modifiable l-values      |             Yes             |      Yes     |
    | Non-modifiable l-values  |             No              |      No      |
    | R-values                 |             No              |      No      |
    +--------------------------+-----------------------------+--------------+

    L-value references to const objects can be initialized with modifiable and non-modifiable l-values and r-values alike. However, those values can’t be modified.
    +------------------------------+-----------------------------+--------------+
    |   L-value reference to const |   Can be initialized with   |  Can modify  |
    +------------------------------+-----------------------------+--------------+
    | Modifiable l-values          |             Yes             |      No      |
    | Non-modifiable l-values      |             Yes             |      No      |
    | R-values                     |             Yes             |      No      |
    +------------------------------+-----------------------------+--------------+

    Note : L-value references to const objects are particularly useful because they allow us to pass any type of argument (l-value or r-value) into a function without making a copy of the argument.


    R-value references
    -- An r-value reference is a reference that is designed to be initialized with an r-value (only). While an l-value reference is created using a single ampersand, an r-value reference is created using a double ampersand.
    -- R-values references cannot be initialized with l-values.
    +------------------------+---------------------------+--------------+
    |   R-value reference    | Can be initialized with   |  Can modify  |
    +------------------------+---------------------------+--------------+
    | Modifiable l-values    |            No             |      No      |
    | Non-modifiable l-values|            No             |      No      |
    | R-values               |            Yes            |      Yes     |
    +------------------------+---------------------------+--------------+

    +------------------------------+---------------------------+--------------+
    |   R-value reference to const | Can be initialized with   |  Can modify  |
    +------------------------------+---------------------------+--------------+
    | Modifiable l-values          |            No             |      No      |
    | Non-modifiable l-values      |            No             |      No      |
    | R-values                     |            Yes            |      No      |
    +------------------------------+---------------------------+--------------+

    R-value references have two properties that are useful.
        -- First, r-value references extend the lifespan of the object they are initialized with to the lifespan of the r-value reference (l-value references to const objects can do this too). 
        -- Second, non-const r-value references allow you to modify the r-value.
    
    R-value references as function parameters
        -- R-value references are more often used as function parameters. This is most useful for function overloads when you want to have different behavior for l-value and r-value arguments.
        -- when passed an l-value, the overloaded function resolved to the version with the l-value reference. When passed an r-value, the overloaded function resolved to the version with the r-value reference (this is considered a better match than an l-value reference to const).
    
    Rvalue reference variables are lvalues
        -- Consider the following snippet:

        int&& ref{ 5 };
        fun(ref);

        Which version of fun would you expect the above to call: fun(const int&) or fun(int&&)?
        The answer might surprise you. This calls fun(const int&).

        Although variable ref has type int&&, when used in an expression it is an lvalue (as are all named variables). The type of an object and its value category are independent.
        You already know that literal 5 is an rvalue of type int, and int x is an lvalue of type int. Similarly, int&& ref is an lvalue of type int&&.

        So not only does fun(ref) call fun(const int&), it does not even match fun(int&&), as rvalue references can’t bind to lvalues.
    
    Returning an r-value reference
        You should almost never return an r-value reference, for the same reason you should almost never return an l-value reference. 
        In most cases, you’ll end up returning a hanging reference when the referenced object goes out of scope at the end of the function.
    
3 — Move constructors and move assignment
    -- By default, C++ will provide a copy constructor and copy assignment operator if one is not explicitly provided. These compiler-provided functions do shallow copies, which may cause problems for classes that allocate dynamic memory.
       So classes that deal with dynamic memory should override these functions to do deep copies.
    -- C++11 defines two new functions in service of move semantics: a move constructor, and a move assignment operator. 
       The goal of the move constructor and move assignment is to move ownership of the resources from one object to another (which is typically much less expensive than making a copy).
    -- The move semantic functions use non-const rvalue reference parameters (which only bind to rvalues).
    -- Instead of deep copying the source object into the destination object (the implicit object), we simply move (steal) the source object’s resources. 
       This involves shallow copying the source pointer into the implicit object, then setting the source pointer to null.
    -- Move constructors and move assignment should be marked as noexcept. This tells the compiler that these functions will not throw exceptions.
    
    When are the move constructor and move assignment called?
        -- When the argument for construction or assignment is an rvalue. Most typically, this rvalue will be a literal or temporary value.
    
    When are the copy constructor and copy assignment called?
        -- When the argument for construction or assignment is an lvalue.
        -- When the argument is an rvalue and the move constructor or move assignment functions aren’t defined.

    Implicit move constructor and move assignment operator
        -- The compiler will create an implicit move constructor and move assignment operator if all of the following are true:
            -- There are no user-declared copy constructors or copy assignment operators.
            -- There are no user-declared move constructors or move assignment operators.
            -- There is no user-declared destructor.

        -- These functions do a memberwise move, which behaves as follows:
            -- If member has a move constructor or move assignment (as appropriate), it will be invoked.
            -- Otherwise, the member will be copied.

        Notably, this means that pointers will be copied, not moved!
        If you want to move a pointer member, you will need to define the move constructor and move assignment yourself.

    The key insight behind move semantics        
        -- If we construct an object or do an assignment where the argument is an l-value, the only thing we can reasonably do is copy the l-value. 
           We can’t assume it’s safe to alter the l-value, because it may be used again later in the program.
        -- However, if we construct an object or do an assignment where the argument is an r-value, then we know that r-value is just a temporary object of some kind. 
           Instead of copying it (which can be expensive), we can simply transfer its resources (which is cheap) to the object we’re constructing or assigning. 
           This is safe to do because the temporary will be destroyed at the end of the expression anyway, so we know it will never be used again!
        -- Move semantics is an optimization opportunity.
    
    Move functions should always leave both objects in a valid state
        -- When implementing move semantics, it is important to ensure the moved-from object is left in a valid state, so that it will destruct properly (without creating undefined behavior).
    
    Automatic l-values returned by value may be moved instead of copied
        -- The C++ has a special rule that automatic objects returned from a function by value can be moved even if they are l-values. 
           This makes sense, since res was going to be destroyed at the end of the function anyway! We might as well steal its resources instead of making an expensive and unnecessary copy.
        -- Although the compiler can move l-value return values, in some cases it may be able to do even better by simply eliding the copy altogether
           (which avoids the need to make a copy or do a move at all). In such a case, neither the copy constructor nor move constructor would be called.

    Deleting the move constructor and move assignment
        -- We can delete the move constructor and move assignment using the = delete syntax in the exact same way you can delete the copy constructor and copy assignment.
        -- If you delete the copy constructor, the compiler will not generate an implicit move constructor (making your objects neither copyable nor movable).
        -- Therefore, when deleting the copy constructor, it is useful to be explicit about what behavior you want from your move constructors.
        -- Either explicitly delete them (making it clear this is the desired behavior), or default them (making the class move-only).

        Rule of Five
            -- if the copy constructor, copy assignment, move constructor, move assignment, or destructor are defined or deleted, then each of those functions should be defined or deleted.
         


-- Since C++17, when you initialize an object from a prvalue, the compiler must directly construct the object in place - this is called mandatory copy elision or guaranteed copy elision. This is NOT an optimization that can be disabled - it's part of the C++ language specification.
