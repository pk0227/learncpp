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
       Why noexcept is critical: Standard library containers like std::vector provide the strong exception guarantee during reallocations. 
       When resizing, std::vector uses std::move_if_noexcept(). If your class's move constructor is not marked noexcept, std::vector will silently fall back to making expensive deep copies during reallocations to avoid leaving elements in an invalid state if an exception is thrown.
    
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
    
    Issues with move semantics and std::swap
        -- As per the copy and swap idiom. Copy and swap also works for move semantics, meaning we can implement our move constructor and move assignment by swapping resources with the object that will be destroyed.
        -- This has two benefits:
            -- The persistent object now controls the resources that were previously under ownership of the dying object (which was our primary goal).
            -- The dying object now controls the resources that were previously under ownership of the persistent object. When the dying object actually dies, it can do any kind of cleanup required on those resources.

        When you think about swapping, the first thing that comes to mind is usually std::swap(). However, implementing the move constructor and move assignment using std::swap() is problematic, 
        as std::swap() calls both the move constructor and move assignment on move-capable objects. This will result in an infinite recursion issue.

        -- We can implement the move constructor and move assignment using your own swap function, as long as your swap member function does not call the move constructor or move assignment.

        -- Since C++17, when you initialize an object from a prvalue, the compiler must directly construct the object in place - this is called mandatory copy elision or guaranteed copy elision. This is NOT an optimization that can be disabled - it's part of the C++ language specification.

4 — std::move
    -- Because making copies can be inefficient, switch from copy semantics to move semantics are recommended.
    -- But, we don’t have a way to invoke the move semantics instead of copy semantics.

    std::move
        --  std::move is a standard library function that casts (using static_cast) its argument into an r-value reference, so that move semantics can be invoked. 
            Thus, we can use std::move to cast an l-value into a type that will prefer being moved over being copied. 
        -- Importantly, std::move does NOT move anything itself at runtime and generates zero machine code instructions! It is purely a compile-time cast that converts an lvalue expression into an rvalue (an xvalue), enabling overload resolution to select move constructors or move assignment operators instead of copy operations.
        -- std::move is defined in the utility header.
        -- We can also use std::move when filling elements of a container, such as std::vector, with l-values.

    Moved from objects will be in a valid, but possibly indeterminate state
        -- When we move the value from a lvalue objects, Because we can continue to access these objects after their values have been moved, it is useful to know what value they are left with.
        -- The C++ standard says, “Unless otherwise specified, moved-from objects [of types defined in the C++ standard library] shall be placed in a valid but unspecified state.”
        -- Therefore, we should avoid using the value of a moved-from object, as the results will be implementation-specific.
        -- We can set or reset the value of the moved-from object (using operator=, or any kind of clear() or reset() member function). 
           We can also test the state of the moved-from object (e.g. using empty() to see if the object has a value). 
        -- However, we should avoid functions like operator[] or front() (which returns the first element in a container), 
           because these functions depend on the container having elements, and a moved-from container may or may not have elements.
        -- It is okay to give a moved-from object a new value (e.g. using operator=) after the current value has been moved.

    Where else is std::move useful?
        -- std::move can also be useful when sorting an array of elements. Many sorting algorithms (such as selection sort and bubble sort) work by swapping pairs of elements.
           There we can use move semantics, which is more efficient.
    
    -- There is a useful variant of std::move() called std::move_if_noexcept() that returns a movable r-value if the object has a noexcept move constructor, otherwise it returns a copyable l-value.

5 — std::unique_ptr
    -- Smart pointer is that it manages a dynamically allocated resource provided by the user of the smart pointer, and ensures the dynamically allocated object is properly cleaned up at the appropriate time (usually when the smart pointer goes out of scope).
    -- Because of this, smart pointers should never be dynamically allocated themselves (otherwise, there is the risk that the smart pointer may not be properly deallocated, which means the object it owns would not be deallocated, causing a memory leak).
    -- By always allocating smart pointers on the stack (as local variables or composition members of a class), we’re guaranteed that the smart pointer will properly go out of scope when the function or object it is contained within ends, 
       ensuring the object the smart pointer owns is properly deallocated.
    
    std::unique_ptr
    -- std::unique_ptr is the C++11 replacement for std::auto_ptr (std::auto_ptr was deprecated in C++11 and completely removed in C++17 because its copy operations silently moved ownership, causing surprising bugs).
    -- It should be used to manage any dynamically allocated object that is not shared by multiple objects.
    -- So, std::unique_ptr should completely own the object it manages, not share that ownership with other classes.
    -- Unlike std::auto_ptr, std::unique_ptr properly implements move semantics.
    -- If you want to transfer the contents managed by std::unique_ptr, you must use move semantics.

    Accessing the managed object
        -- std::unique_ptr has an overloaded operator* and operator-> that can be used to return the resource being managed.
        -- Operator* returns a reference to the managed resource, and operator-> returns a pointer.
        -- Remember that std::unique_ptr may not always be managing an object 
            -- either because it was created empty (using the default constructor or passing in a nullptr as the parameter).
            -- because the resource it was managing got moved to another std::unique_ptr. 
        -- So before we use either of these operators, we should check whether the std::unique_ptr actually has a resource. 
        -- Fortunately, this is easy: std::unique_ptr has a cast to bool that returns true if the std::unique_ptr is managing a resource.

    std::unique_ptr and arrays
        -- Unlike std::auto_ptr, std::unique_ptr is smart enough to know whether to use scalar delete or array delete, so std::unique_ptr is okay to use with both scalar objects and arrays.
        -- However, Favor std::array, std::vector, or std::string over a smart pointer managing a fixed array, dynamic array, or C-style string.
    
    std::make_unique
        -- C++14 comes with an additional function named std::make_unique(). This templated function constructs an object of the template type and initializes it with the arguments passed into the function.
        -- Use of std::make_unique() is optional, but is recommended over creating std::unique_ptr yourself. 
        -- Furthermore, in C++14 it resolves an exception safety issue that can result from C++ leaving the order of evaluation for function arguments unspecified.

    The exception safety issue in more detail
        -- For those wondering what the “exception safety issue” mentioned above is, here’s a description of the issue.
            Consider an expression like this one:
                some_function(std::unique_ptr<T>(new T), function_that_can_throw_exception());
        
        -- The compiler is given a lot of flexibility in terms of how it handles this call. It could create a new T, then call function_that_can_throw_exception(), then create the std::unique_ptr that manages the dynamically allocated T. 
           If function_that_can_throw_exception() throws an exception, then the T that was allocated will not be deallocated, because the smart pointer to do the deallocation hasn’t been created yet. This leads to T being leaked.

        -- std::make_unique() doesn’t suffer from this problem because the creation of the object T and the creation of the std::unique_ptr happen inside the std::make_unique() function, where there’s no ambiguity about order of execution.
        -- This issue was fixed in C++17, as evaluation of function arguments can no longer be interleaved.
    
    Returning std::unique_ptr from a function
        -- std::unique_ptr can be safely returned from a function by value.
        -- In C++14 or earlier, move semantics will be employed to transfer the Resource from the return value to the object assigned to.
        -- In C++17 or newer, the return will be elided. This makes returning a resource by std::unique_ptr much safer than returning raw pointers.

    Passing std::unique_ptr to a function
        -- If you want the function to take ownership of the contents of the pointer, pass the std::unique_ptr by value.
        -- We will need to use std::move to actually pass the variable in, because copy semantics have been disabled.
        -- However, most of the time, you won’t want the function to take ownership of the resource.
        -- Although you can pass a std::unique_ptr by const reference (which will allow the function to use the object without assuming ownership), 
           it’s better to just pass the resource itself (by pointer or reference, depending on whether null is a valid argument).
        -- To get a raw pointer from a std::unique_ptr, you can use the get() member function.

    std::unique_ptr and classes
        -- In case, we use std::unique_ptr as a composition member of a class. The class destructor deletes the dynamic memory, as the std::unique_ptr will be automatically destroyed when the class object is destroyed.
        -- But, if the class object is not destroyed properly (e.g. it is dynamically allocated and not deallocated properly), 
           then the std::unique_ptr member will not be destroyed either, and the object being managed by the std::unique_ptr will not be deallocated.
    
    Misusing std::unique_ptr
        -- First, don’t let multiple objects manage the same resource.
        -- Second, don’t manually delete the resource out from underneath the std::unique_ptr.
           If you do, the std::unique_ptr will try to delete an already deleted resource, again leading to undefined behavior.
        -- Note that std::make_unique() prevents both of the above cases from happening inadvertently.
    
6 — std::shared_ptr
    -- std::shared_ptr is meant to solve the case where you need multiple smart pointers co-owning a resource.
    -- Internally, std::shared_ptr keeps track of how many std::shared_ptr are sharing the resource.
    -- As long as at least one std::shared_ptr is pointing to the resource, the resource will not be deallocated, even if individual std::shared_ptr are destroyed.
    -- As soon as the last std::shared_ptr managing the resource goes out of scope (or is reassigned to point at something else), the resource will be deallocated.
    -- We use the copy constructor to create a second std::shared_ptr that points to the same Resource.
    -- Note that we must create a second shared pointer from the first shared pointer. This is important. Not from resource pointer.
    -- Always make a copy of an existing std::shared_ptr if you need more than one std::shared_ptr pointing to the same resource.
    -- Just like with std::unique_ptr, std::shared_ptr can be a null pointer, so check to make sure it is valid before using it.

    std::make_shared
        -- std::make_shared() can (and should) be used to make a std::shared_ptr.
        -- The reasons for using std::make_shared() are the same as std::make_unique() 
            -- std::make_shared() is simpler and safer (there’s no way to create two independent std::shared_ptr pointing to the same resource but unaware of each other).
    
    Digging into std::shared_ptr
        -- Unlike std::unique_ptr, which uses a single pointer internally, std::shared_ptr uses two pointers internally.
            -- One pointer points at the resource being managed. 
            -- The other points at a “control block”, which is a dynamically allocated object that tracks of a bunch of stuff, including how many std::shared_ptr are pointing at the resource.
        -- When a std::shared_ptr is created via a std::shared_ptr constructor, the memory for the managed object (which is usually passed in) and control block (which the constructor creates) are allocated separately. 
           However, when using std::make_shared(), this can be optimized into a single memory allocation, which leads to better performance.
        -- So, When two independent std::shared_ptr points to the same resource, it gets us into trouble. Because, each std::shared_ptr will have one pointer pointing at the resource. 
           However, each std::shared_ptr will independently allocate its own control block, which will indicate that it is the only pointer owning that resource. 
           Thus, when that std::shared_ptr goes out of scope, it will deallocate the resource, not realizing there are other std::shared_ptr also trying to manage that resource.
        -- However, when a std::shared_ptr is cloned using copy assignment, the data in the control block can be appropriately updated to indicate that there are now additional std::shared_ptr co-managing the resource.
    
    Shared pointers can be created from unique pointers
        -- A std::unique_ptr can be converted into a std::shared_ptr via a special std::shared_ptr constructor that accepts a std::unique_ptr r-value. 
           The contents of the std::unique_ptr will be moved to the std::shared_ptr.
        -- However, std::shared_ptr can not be safely converted to a std::unique_ptr. This means that if you’re creating a function that is going to return a smart pointer, 
           you’re better off returning a std::unique_ptr and assigning it to a std::shared_ptr if and when that’s appropriate.
    
    The perils of std::shared_ptr
        The difference is in responsibility:
            -- With std::unique_ptr, there is only one owner. You only need to ensure that this single smart pointer is destroyed correctly.
            -- With std::shared_ptr, there can be multiple owners. The resource will be freed only when all std::shared_ptr instances pointing to it are destroyed. 
               If even one remains alive, the resource will continue to exist, leading to a potential memory leak. 
    
    std::shared_ptr and arrays
        -- In C++17 and earlier, std::shared_ptr does not have proper support for managing arrays, and should not be used to manage a C-style array. 
        -- As of C++20, std::shared_ptr does have support for arrays.
    
7 — Circular dependency issues with std::shared_ptr, and std::weak_ptr
    Circular references / cyclical reference / a cycle
        -- It is a series of references where each object references the next, and the last object references back to the first, causing a referential loop.
        -- The references can be pointers, unique IDs, or any other means of identifying specific objects.
        -- In the context of shared pointers, the references will be pointers.
    
    A reductive case
        -- cyclical reference issue can even happen with a single std::shared_ptr -- a std::shared_ptr referencing the object that contains it is still a cycle (just a reductive one).
    
    So what is std::weak_ptr for anyway?
        -- std::weak_ptr was designed to solve the “cyclical ownership” problem described above. 
        -- A std::weak_ptr is an observer -- it can observe and access the same object as a std::shared_ptr (or other std::weak_ptrs) but it is not considered an owner. 
           Remember, when a std::shared pointer goes out of scope, it only considers whether other std::shared_ptr are co-owning the object. std::weak_ptr does not count!
        
        Using std::weak_ptr
            -- One downside of std::weak_ptr is that std::weak_ptr are not directly usable (they have no operator->). 
            -- To use a std::weak_ptr, you must first convert it into a std::shared_ptr.
            -- To convert a std::weak_ptr into a std::shared_ptr, you can use the lock() member function.
    
    Avoiding dangling pointers with std::weak_ptr
        -- It has access to the reference count for an object, it can determine if it is pointing to a valid object or not.
            -- If the reference count is non-zero, the resource is still valid. 
            -- If the reference count is zero, then the resource has been destroyed.
        -- The easiest way to test whether a std::weak_ptr is valid is to use the expired() member function, which returns 
            -- true if the std::weak_ptr is pointing to an invalid object, and false otherwise.
        -- Note that if a std::weak_ptr is expired, then we shouldn’t call lock() on it, because the object being pointed to has already been destroyed.
           so there is no object to share. If you do call lock() on an expired std::weak_ptr, it will return a std::shared_ptr to nullptr.
        