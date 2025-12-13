1 — Introduction to containers and arrays
    Containers
        -- In general programming, a container is a data type that provides storage for a collection of unnamed objects (called elements).
        -- We typically use containers when we need to work with a set of related values.
        -- The elements of a container do not have their own names, so that the container can have as many elements as we want without 
           having to give each element a unique name.
        -- Each container provides some method to access these elements, but how depends on the specific type of container.

    The length of a container
        -- In programming, the number of elements in a container is often called it’s length (or sometimes count).
        -- In C++, the term size is also commonly used for the number of elements in a container.

    Container operations
        -- Programming languages offer different container types with varying supported operations and performance.
        -- Some allow fast random access but slow insertion/removal; others allow fast insertion/removal but only sequential access.
        -- Each container has strengths and limitations.
        -- Choosing the right one greatly impacts performance and code maintainability. 
    
    Element Types
        -- Most containers are homogeneous → all elements must be the same type.
        -- Some have preset types (e.g., string → char), but usually the type is user-defined.
        -- In C++, containers are class templates, so you specify the element type when instantiating.
        -- This makes containers flexible without needing new types for each element.
        -- Heterogeneous containers (mixed types) are common in scripting languages like Python.
    
    Containers in C++
        -- In C++, the term container refers specifically to the class types in the Containers library. 
           These are the official “container classes” defined by the C++ standard.
        -- Some types behave like containers in general programming, but are not considered containers in C++ because 
           they do not meet all required container specifications (such as providing certain member functions). 
           Examples include:
            -- C-style arrays
            -- std::string
            -- std::vector<bool>
           Even though std::string and std::vector<bool> behave like containers, they are missing some required features, 
           so they are often called pseudo-containers.
    
    Introduction to arrays
        -- Arrays store a sequence of values in contiguous memory, allowing fast, direct access to any element. 
           Because they are simple and efficient, arrays are often used when working with groups of related values.

        -- C++ provides three main array types:
            -- C-style arrays
                -- Inherited from C
                -- Part of the core language
                -- Fast but unsafe and behave inconsistently by modern standards
            -- std::vector (C++03)
                -- A dynamic array container class
                -- Safest and most flexible of all array types
            -- std::array (C++11)
                -- A safer, fixed-size replacement for C-style arrays
                -- Often more efficient than std::vector for small fixed-size arrays
            
            All array container classes follow similar interfaces, so learning one (like std::vector) makes it easier to use the others.
    
2 — Introduction to std::vector and list constructors
    Introduction to std::vector
        -- std::vector is one of the container classes in the C++ standard containers library that implements an array. 
        -- std::vector is defined in the <vector> header as a class template, with a template type parameter that defines the type of 
           the elements. Thus, std::vector<int> declares a std::vector whose elements are of type int.
    
    Initializing a std::vector with a list of values
        std::vector<int> primes{ 2, 3, 5, 7 };
        std::vector vowels { 'a', 'e', 'i', 'o', 'u' }; // vector containing 5 char elements with values 'a', 'e', 'i', 'o', and 'u'.  
                                                        // Uses CTAD (C++17) to deduce element type char (preferred).
    List constructors and initializer lists
        -- Containers have a special constructor called a list constructor.
        -- When we provide a container with an initializer list of values, the list constructor is called, and the container is constructed
           using that list of values! The list constructor does three things:
           -- Ensures the container has enough storage to hold all the initialization values (if needed).
           -- Sets the length of the container to the number of elements in the initializer list (if needed).
           -- Initializes the elements to the values in the initializer list (in sequential order).

    Accessing array elements using the subscript operator (operator[])
        -- The subscript operator returns a reference to the actual element, not a copy. Once we’ve accessed an array element, 
           we can use it just like a normal object (e.g. assign a value to it, output it, etc…)
        -- Indexes are actually a distance (offset) from the first element of the array.

        Subscript Out of Bounds
            -- For an array of length N, valid indices are 0 to N-1.
            -- operator[] does not check bounds → invalid indices cause undefined behavior.
            -- Easy to avoid negative indices, but remember:
                -- The last element is at N-1.
                -- Index N is one past the end → invalid access.
    
    Arrays are contiguous in memory
        -- The elements of array are always allocated contiguously in memory, are all adjacent in memory (with no gaps between them).
        -- Arrays don't have any per-element overhead. It allows the compiler to quickly calculate the address of any element in the array.
        -- Arrays are one of the few container types that support efficient random access, allowing direct access to any element 
           (unlike sequential access, which requires traversing in order).
        -- Random access to array elements is typically efficient, and makes arrays very easy to use. 
        -- This is a primary reason why arrays are often preferred over other containers.
    
    Constructing a std::vector of a specific length
        -- std::vector has an explicit constructor (explicit std::vector<T>(std::size_t)).
        -- std::size_t defines the length of the std::vector to construct.
                std::vector<int> data( 10 ); // vector containing 10 int elements, value-initialized to 0
        
        NOTE : One important thing about using this constructor: it must be called using direct initialization.
    
    Non-empty initializer lists prefer list constructors
        -- To understand why the previous constructor must be called using direct initialization, consider this definition:
                std::vector<int> data{ 10 }; // what does this do?
        
        When constructing a class type object using a initializer list:
            -- If the initializer list is empty, the default constructor is preferred over the list constructor.
            -- If the initializer list is non-empty, a matching list constructor is preferred over other matching constructors.

        // Copy init
        std::vector<int> v1 = 10;     // 10 not an initializer list, copy init won't match explicit constructor: compilation error

        // Direct init
        std::vector<int> v2(10);      // 10 not an initializer list, matches explicit single-argument constructor

        // List init
        std::vector<int> v3{ 10 };    // { 10 } interpreted as initializer list, matches list constructor

        // Copy list init
        std::vector<int> v4 = { 10 }; // { 10 } interpreted as initializer list, matches list constructor
        std::vector<int> v5({ 10 });  // { 10 } interpreted as initializer list, matches list constructor

        // Default init
        std::vector<int> v6 {};       // {} is empty initializer list, matches default constructor
        std::vector<int> v7 = {};     // {} is empty initializer list, matches default constructor

        NOTE : If a class does not have a list constructor but then one is added later, this will change which constructor is called 
               for all objects initialized using a non-empty initializer list.
        
        -- v6 and v7 are both initialized using empty initializer lists. In this case, the default constructor takes precedence.
        -- When constructing a container (or any type that has a list constructor) with initializers that are not element values, 
           use direct initialization.
        
    When a std::vector is a member of a class type,
            #include <vector>
            struct Foo
            {
                std::vector<int> v1(8); // compile error: direct initialization not allowed for member default initializers
            };

            This doesn’t work because direct (parenthesis) initialization is disallowed for member default initializers.

        When providing a default initializer for a member of a class type:
            -- We must use either copy initialization or list initialization (direct or copy).
            -- CTAD is not allowed (so we must explicitly specify the element type).

        The answer is as follows:
            struct Foo
            {
                std::vector<int> v{ std::vector<int>(8) }; // ok
            };
        This creates a std::vector with a capacity of 8, and then uses that as the initializer for v.

    Const and constexpr std::vector
        Objects of type std::vector can be made const
            const std::vector<int> prime { 2, 3, 5, 7, 11 }; // prime and its elements cannot be modified
        A const std::vector must be initialized, and then cannot be modified. The elements of such a vector are treated as if they were const.

        NOTEs : 
        -- The element type of a std::vector must not be defined as const (e.g. std::vector<const int> is disallowed).
        -- One of the biggest downsides of std::vector is that it cannot be made constexpr. If you need a constexpr array, use std::array.
    
3 — std::vector and the unsigned length and subscript problem
    The container length sign problem
        -- Standard library container classes use unsigned values for the length (and indices) is problematic, as it makes it 
           impossible to avoid unsigned values when using these types.
        -- So, we are stuck with this choice and the unnecessary complexity it causes.
    
    A review: sign conversions are narrowing conversions, except when constexpr 
        -- Sign conversions are narrowing because signed and unsigned types can’t represent each other’s full value range. 
           When such conversions occur, compilers reject them in contexts that don’t allow narrowing (like list initialization), 
           and may warn elsewhere.
        -- But if the value is constexpr and safely fits in the destination type, the conversion is not considered narrowing. 
           The compiler can verify it at compile time.
        -- This safe, non-narrowing constexpr conversion (e.g., constexpr int → constexpr std::size_t) is used frequently.
                int s { 5 };
                [[maybe_unused]] unsigned int u { s }; // compile error: list initialization disallows narrowing conversion
                                                       // if compiled with -Werror flag
                foo(s);                                // possible warning: copy initialization allows narrowing conversion

                constexpr int s { 5 };                 // now constexpr
                [[maybe_unused]] unsigned int u { s }; // ok: s is constexpr and can be converted safely, 
                                                       // not a narrowing conversion
                foo(s);                                // ok: s is constexpr and can be converted safely, 
                                                       // not a narrowing conversion

    The length and indices of std::vector have type size_type
        -- std::size_t is a typedef for some large unsigned integral type, usually unsigned long or unsigned long long.
        -- Each of the standard library container classes defines a nested typedef member named size_type (sometimes written as 
           T::size_type), which is an alias for the type used for the length (and indices, if supported) of the container.
        -- Example : Member function of std::vector indicates that size() returns a value of size_type.
        -- size_type is almost always an alias for std::size_t, but can be overridden (in rare cases) to use a different type.
        -- All of the standard library containers except std::array use std::allocator to allocate memory. For these containers, 
           T::size_type is derived from the size_type of the allocator used. 
           std::allocator<T>::size_type is defined as std::size_t. Therefore, T::size_type defaults to std::size_t.
        -- Only if a container uses a custom allocator with a different size_type will this type differ—but that’s rare and 
           intentional.
        -- When accessing the size_type member of a container class, we must scope qualify it with the fully templated name of 
           the container class. For example, std::vector<int>::size_type.
    
    Getting the length of a std::vector using the size() member function or std::size()
        -- Container classes have member function size(), which returns the length as unsigned size_type.
                std::vector prime { 2, 3, 5, 7, 11 };
                std::cout << "length: " << prime.size() << '\n'; // returns length as type `size_type` (alias for `std::size_t`)
        -- Unlike std::string and std::string_view, which have both a length() and a size() member function to return length,
           std::vector (and most other container types in C++) only has size().
        -- From C++17, we can also use the std::size() non-member function.
                std::vector prime { 2, 3, 5, 7, 11 };
                std::cout << "length: " << std::size(prime); // returns length as type `size_type` (alias for `std::size_t`)
        -- std::size() can also be used on non-decayed C-style arrays.
        -- To use either of the above methods to store the length in a variable with a signed type, static_cast the result to the 
           desired type to avoid a signed/unsigned conversion warning or error.
                int length { static_cast<int>(prime.size()) }; // static_cast return value to int
        
    Getting the length of a std::vector using std::ssize()
        -- C++20 introduces the std::ssize() non-member function, which returns the length as a large signed integral type 
           (usually std::ptrdiff_t, which is the type normally used as the signed counterpart to std::size_t):
                std::vector prime{ 2, 3, 5, 7, 11 };
                std::cout << "length: " << std::ssize(prime); // C++20, returns length as a large signed integral type
        -- This is the only function of the three which returns the length as a signed type.
        
        If you want to use this method to store the length in a variable with a signed type, you have a couple of options.
            -- First, because the int type may be smaller than the signed type returned by std::ssize(), 
               if you are going to assign the length to an int variable, you should static_cast the result to int to make any 
               such conversion explicit (otherwise you might get a narrowing conversion warning or error):
                    int length { static_cast<int>(std::ssize(prime)) }; // static_cast return value to int
            -- Alternatively, you can use auto to have the compiler deduce the correct signed type to use for the variable:
                    auto length { std::ssize(prime) }; // use auto to deduce signed type, as returned by std::ssize()
    
    Accessing array elements using operator[] does no bounds checking
        -- operator[] does not do bounds checking. The index for operator[] can be non-const. 
    
    Accessing array elements using the at() member function does runtime bounds checking
        -- The at() member function can be used to do array access with runtime bounds checking:
                std::cout << prime.at(3); // print the value of element with index 3
                std::cout << prime.at(9); // invalid index, throws an exception of type std::out_of_range. 
                                          // If the exception is not handled, the program will be terminated. 
        -- Just like operator[], the index passed to at() can be non-const.
        -- Because it does runtime bounds checking on every call, at() is slower (but safer) than operator[].

    Indexing std::vector with a constexpr signed int
        -- Indexing a std::vector with a constexpr (signed) int, can let the compiler implicitly convert this to a std::size_t 
           without it being a narrowing conversion:
                std::cout << prime[3] << '\n';     // okay: 3 converted from int to std::size_t, not a narrowing conversion
                constexpr int index { 3 };         // constexpr
                std::cout << prime[index] << '\n'; // okay: constexpr index implicitly converted to std::size_t, not a narrowing 
                                                   // conversion
    
    Indexing std::vector with a non-constexpr value
        -- The subscripts used to index an array can be non-const:
                std::size_t index { 3 };           // non-constexpr
                std::cout << prime[index] << '\n'; // operator[] expects an index of type std::size_t, no conversion required
    
        -- When our subscript is a non-constexpr signed value, we run into problems:
                int index { 3 };                   // non-constexpr
                std::cout << prime[index] << '\n'; // possible warning: index implicitly converted to std::size_t, narrowing 
                                                   // conversion
        -- Another good alternative is instead of indexing the std::vector itself, index the result of the data() member function:
                int index { 3 };                          // non-constexpr signed value
                std::cout << prime.data()[index] << '\n'; // okay: no sign conversion warnings
            Under the hood, std::vector holds its elements in a C-style array. The data() member function returns a pointer to 
            this underlying C-style array, which we can then index. Since C-style arrays allow indexing with both signed and 
            unsigned types, we don’t run into any sign conversion issues. 
        
4 — Passing std::vector
    -- Passing a std::vector by value, causes an expensive copy. Therefore, Passing std::vector by (const) reference is 
       recommended to avoid such copies.
    
    Passing std::vector of different element types
        -- We must not pass vectors with different element types.
        -- CTAD doesn’t (currently) work with function parameters.
        -- We can create a function template that parameterizes the element type, and then C++ will use that function template 
           to instantiate functions with actual types.
                Example : const std::vector<T>& arr
    
    Passing a std::vector using a generic template or abbreviated function template
        -- We can also create a function template that will accept any type of object.
                Example : const T& arr
        -- In C++20, we can use an abbreviated function template (via an auto parameter) to do the same thing.
                Example : const auto& arr
        -- Both of these will accept an argument of any type that will compile. This can be desirable when writing functions 
           that we might want to operate on more than just a std::vector.
        -- The potential downside of this method is that it may lead to bugs if the function is passed an object of a type that 
           compiles but doesn’t make sense semantically.
    
    Asserting on array length
        -- The compiler will happily let you pass in arrays where index is out of bounds.
        -- One option here is to assert on arr.size(), which will catch such errors when run in a debug build configuration. 
           Because std::vector::size() is a non-constexpr function, we can only do a runtime assert here.
        -- A better option is to avoid using std::vector in cases where we need to assert on array length. Using a type that 
           supports constexpr arrays (e.g. std::array) is probably a better choice, as we can static_assert on the length of 
           a constexpr array.
        -- The best option is to avoid writing functions that rely on the user passing in a vector with a minimum length in 
           the first place.
           
5 — Returning std::vector, and an introduction to move semantics
    -- When we need to pass a std::vector to a function, we pass it by (const) reference so that we do not make an expensive copy 
       of the array data.
    -- But, Surprisingly It is okay to return a std::vector by value.

    Copy semantics
        -- The term copy semantics refers to the rules that determine how copies of objects are made. 
        -- When we say a type supports copy semantics, we mean that objects of that type are copyable, because the rules for 
           making such copies have been defined.
        -- Copy semantics are typically implemented via the copy constructor (and copy assignment operator).
    
    Introduction to move semantics
        -- When ownership of data is transferred from one object to another, we say that data has been moved. 
           The cost of such a move is typically trivial.
        -- when the temporary was then destroyed at the end of the expression, it would no longer have any data to destroy, 
           so we wouldn’t have to pay that cost either.
        -- In move semantics, any data member that can be moved is moved, and any data member that can’t be moved is copied. 
        
    How move semantics is invoked
        -- Normally, when an object is being initialized with (or assigned) an object of the same type, 
           copy semantics will be used (assuming the copy isn’t elided).
        -- However, when all of the following are true, move semantics will be invoked instead:
            -- The type of the object supports move semantics.
            -- The object is being initialized with (or assigned) an rvalue (temporary) object of the same type.
            -- The move isn’t elided.
        -- Many types don't support move semantics. However, std::vector and std::string both do!
    
    We can return move-capable types like std::vector by value
        -- We can return move-capable types (like std::vector and std::string) by value. Such types will inexpensively move their
           values instead of making an expensive copy.
        -- Such types should still be passed by const reference.
    
    NOTE : Expensive-to-copy types shouldn’t be passed by value, but if they are move-capable they can be returned by value.

    One of the most common things we do in C++ are,
        1.  Construct the value to be passed.
        2.  Actually pass the value to the function.
        3.  Construct the value to be returned.
        4.  Actually pass the return value back to the caller.

            #include <iostream>
            #include <vector>

            std::vector<int> doSomething(std::vector<int> v2)
            {
                std::vector v3 { v2[0] + v2[0] }; // 3 -- construct value to be returned to caller
                return v3; // 4 -- actually return value
            }

            int main()
            {
                std::vector v1 { 5 }; // 1 -- construct value to be passed to function
                std::cout << doSomething(v1)[0] << '\n'; // 2 -- actually pass value

                std::cout << v1[0] << '\n';

                return 0;
            }

        We can’t optimize copies 1 and 3 at all. We need a std::vector to pass to the function, and we need a std::vector to 
        return -- these objects have to be constructed. std::vector is an owner of its data, so it necessarily makes a copy 
        of its initializer.
        
        Copy 2 is made because we’re passing by value from the caller to the called function. What other options do we have?
            -- Can we pass by reference or address? Yes. 
            -- Can this copy be elided? No. 
                -- Elision only works when we’re making a redundant copy or move. There’s no redundant copy or move here.    
            -- Can we use an out parameter here? No. 
                -- We’re passing a value to the function, not getting a value back.
            -- Can we use move semantics here? No. 
                -- The argument is an lvalue. If we moved data from v1 to v2, v1 would become an empty vector, and subsequently 
                   printing v1[0] would lead to undefined behavior.
        
        -- Clearly pass by const reference is our best option here, as it avoids the copy, and works with both lvalue and rvalue.
    
        Copy 4 is made because we’re passing by value from the called function back to the caller. What other options do we have?
            -- Can we return by reference or address? No. 
                -- Reference to a local variable will become dangling after the variable is destroyed when the function returns. 
            -- Can this copy be elided? Yes, 
                -- possibly. By rewriting the code (under the as-if rule) so that v3 is constructed in the scope of the caller 
                   instead, we can avoid the copy that would otherwise be made when returning. 
                   However, we are reliant upon the compiler realizing it can do this, so it is not guaranteed.
            -- Can we use an out parameter here? Yes. Instead of constructing v3 as a local variable, we can construct an empty 
               std::vector object in the scope of the caller, and pass it to the function by non-const reference. The function 
               can then fill this parameter with data. When the function returns, this object will still exist. This avoids the 
               copy, but also has some significant downsides and constraints: 
                -- ugly calling semantics, doesn’t work with objects that don’t support assignment, 
                -- it is challenging to write such functions that can work with both lvalue and rvalue arguments.
            -- Can we use move semantics here? Yes. v3 is going to be destroyed when the function returns, so instead of copying 
               v3 back to the caller, we can use move semantics to move its data to the caller, avoiding the copy.
        
        -- Elision is the best option here, but whether it happens is out of our control. The next best option for move-capable 
           types is move semantics, which can be used in cases where the compiler doesn’t elide the copy. And for move-capable 
           types, move semantics is invoked automatically when returning by value.

        To summarize, for move-capable types, we prefer to pass by const reference, and return by value.

6 — Arrays and loops
    Arrays and loops
        -- Accessing each element of a container in some order is called traversal, or traversing the container. Traversal is 
           often called iteration, or iterating over or iterating through the container.
    Templates, arrays, and loops unlock scalability
        -- Arrays provide a way to store multiple objects without having to name each element.
        -- Loops provide a way to traverse an array without having to explicitly list each element.
        -- Templates provide a way to parameterize the element type.
        -- Together, templates, arrays, and loops allow us to write code that can operate on a container of elements, 
           regardless of the element type or number of elements in the container!
    What we can do with arrays and loops
        -- Calculate a new value based on the value of existing elements (e.g. average value, sum of values).
        -- Search for an existing element (e.g. has exact match, count number of matches, find highest value).
        -- Operate on each element (e.g. output each element, multiply all elements by 2).
        -- Reorder the elements (e.g. sort the elements in ascending order).
        -- Reordering the elements of a container is quite a bit more tricky, as doing so typically involves using a loop inside 
           another loop. While we can do this manually, it’s better to use an existing algorithm from the standard libraries.
    Arrays and off-by-one errors
        -- When iterating arrays with indices, use index < length (not <= length) to avoid off‑by‑one errors and out‑of‑bounds 
           access.

7 — Arrays, loops, and sign challenge solutions
    -- std::vector (and other container classes) uses unsigned integral type std::size_t for length and indices.
       This causes issues. Example : printing an array in reverse order like this,
            for(std::size_t index{arr.size()}; index >= 0; index--)     // index is unsigned

        -- Since index is unsigned, the loop never terminates. It exhibits undefined behavior. It might print garbage values, 
           or crash the application.
    
    Leave signed/unsigned conversion warnings off
        -- Signed/unsigned conversion warnings are often disabled by default because indexing standard containers with signed 
           types would generate many warnings, cluttering build logs and hiding real issues.
        -- While disabling these warnings is the easiest workaround, it’s not recommended, as it also suppresses legitimate 
           sign-conversion warnings that can lead to bugs.
    
    Unsigned loop variables
        -- Containers define size_type (usually std::size_t) for lengths/indices.
        -- Using size_type is consistent but verbose (std::vector<int>::size_type).
        -- In templates, dependent names require typename (e.g.,  :   typename std::vector<T>::size_type).
        -- With decltype(arr)::size_type, the compiler deduces the type.
            If arr is a reference, you must strip the reference first:
                for (typename std::remove_reference_t<decltype(arr)>::size_type i{0}; i < arr.size(); ++i)
        -- Since size_type is almost always std::size_t, most developers just use std::size_t directly for loops.

        Rule of thumb: Prefer std::size_t for indexing unless you’re working with custom allocators. But know how to handle 
        decltype + remove_reference_t when writing generic templates.
    
    Using a signed loop variable
        -- Using a signed loop variable aligns with the general best practice of favoring signed types for quantities, improving 
           consistency across code even though standard containers use unsigned indices.
        When using signed loop variables, you must handle three things:
            -- Choose an appropriate signed type
            -- Obtain the container length as a signed value
            -- Safely convert the signed index to an unsigned type when indexing
    
    What signed type should we use?
    Good options for signed loop indices:
        -- int: Fine for most cases and small/medium arrays.
        -- std::ptrdiff_t: Preferred for large arrays; signed counterpart to std::size_t.
        -- Type alias: Improves readability and future-proofing:
                using Index = std::ptrdiff_t;
        -- auto: Let the compiler deduce the signed type when possible.
        -- C++23 Z suffix: Creates a signed counterpart to size_t literals:
                for (auto i{0Z}; i < static_cast<std::ptrdiff_t>(arr.size()); ++i)

    Getting the length of an array as a signed value
        -- Pre-C++20, the best way is to static_cast the return value of the size() member function or std::size() to a 
           signed type.     Example : for (auto index{ static_cast<std::ptrdiff_t>(arr.size())-1 }; index >= 0; --index)
        -- In C++20, use std::ssize():
            for (auto index{ std::ssize(arr)-1 }; index >= 0; --index) // std::ssize introduced in C++20
    
    Index the underlying C-style array instead
        -- We believe that this method is the best of the indexing options:
            -- We can use signed loop variables and indices.
            -- We don’t have to define any custom types or type aliases.
            -- The hit to readability from using data() isn’t very big.
            -- There should be no performance hit in optimized code.
    
    The sane choice: avoid indexing
        -- Indexing has many pitfalls (signed/unsigned issues, off-by-one errors). A better approach is to avoid 
           integral indices altogether.
        -- Prefer range-based for loops or iterators to traverse containers. If the index is only used for traversal, 
           don’t use it.
        Rule of thumb : avoid array indexing with integral values whenever possible.
    
8 — Range-based for loops (for-each)
        -- C++ provides range-based for loops to traverse containers without explicit indexing. They are simpler, safer, and work
           with common array types such as std::vector, std::array, and C-style arrays.
        
    Range-based for loops
    The range-based for statement has a syntax that looks like this:
        for (element_declaration : array_object)
            statement;
    
    -- Favor range-based for loops over regular for-loops when traversing containers.
    
    Range-based for loops and empty containers
        -- If the container being traversed has no elements, the body of the range-based for-loop will simply not execute.
    
    Range-based for loops and type deduction using the auto keyword
        -- Use type deduction (auto) with range-based for loops to have the compiler deduce the type of the array element. 
           This avoids redundant typing and prevents accidental type mismatches.
            for (auto elem : arr) { /* ... */ }
        -- Another benefit to using auto is that if the element type of the array is ever updated (e.g. from int to long), 
           auto will automatically deduce the updated element type, ensuring they stay in sync and preventing type conversion
           from occurring.
        
    Avoid element copies using references
        -- Copying std::string in a loop is expensive; prefer referencing elements (e.g., const auto& word) to avoid unnecessary 
           copies.
                for (const auto& word : words)
                    std::cout << word << '\n';
    
    When to use auto vs auto& vs const auto&
        -- For range-based for loops, prefer to define the element type as:
            -- auto when you want to modify copies of the elements.
            -- auto& when you want to modify the original elements.
            -- const auto& otherwise (when you just need to view the original elements).

    Range-based for loops and other standard container types
        -- Range-based for loops won’t work with decayed C-style arrays. This is because a range-based for-loop needs to know 
           the length of the array to know when traversal is complete, and decayed C-style arrays do not contain this information.
        -- Range-based for loops also won’t work with enumerations.
    
    Getting the index of the current element
        -- Range-based for loops don’t provide indices because some iterable types (e.g., std::list) don’t support them. 
           We can maintain a manual counter since iteration is sequential, but if we need the index, a traditional for loop 
           may be clearer.
    
    Range-based for loops in reverse 
        -- Range-based for loops iterate only forwards. Before C++20, reverse traversal required normal for loops. 
           Since C++20, std::views::reverse (Ranges) allows clean reverse iteration with range-based for loops.
    
