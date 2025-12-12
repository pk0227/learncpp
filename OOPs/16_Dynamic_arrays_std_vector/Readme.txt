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
    
