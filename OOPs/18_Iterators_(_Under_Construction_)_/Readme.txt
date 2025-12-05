1 — Introduction to iterators
    -- An iterator is an object designed to traverse through a container (e.g. the values in an array, or the characters in a string),
       providing access to each element along the way.
    
    Types of Iterators in C++
    1. Input Iterator
        -- Input Iterator can read elements from a container sequentially (one pass).
        -- Supported in containers like istream_iterator, single-pass algorithms; supports *, ++, ==, !=.
    2. Output Iterator
        -- Output Iterator can write elements to a container sequentially (one pass).
        -- Supported in containers like ostream_iterator, single-pass algorithms; supports *, ++.
    3. Forward Iterator
        -- Can read and write, allows multiple passes through the container.
        -- Supported in containers like forward_list, unordered_set, unordered_map; supports *, ++, ==, !=.
    4. Bidirectional Iterator
        -- Can move forward and backward in a container.
        -- Supported in list, set, map, multiset, multimap; supports *, ++, --, ==, !=.
    5. Random Access Iterator
        -- Can move freely to any position using arithmetic operations.
        -- Supported in vector, deque, array; supports *, ++, --, +, -, [], <, >, <=, >=.

    -- Once the appropriate type of iterator is created, we can then use the interface provided by the iterator to traverse and 
       access elements without worrying about traversal type used how the data is stored in the container.
       Because all C++ iterators use the same basic interface (operator++ for movement and operator* for access), 
       you can iterate over many different container types in a uniform way.
    
    
    Pointers as an iterator
        -- The simplest kind of iterator is a pointer, works for data stored sequentially in memory using pointer arithmetic.

    Standard library iterators
        -- All standard library containers offer direct support for iteration. 
        -- Containers provide member functions like begin() and end() that return begin and end points respectively.

    Container Iterator Functions
        -- STL containers provide member functions that return iterators.
        -- These iterators usually point to the first and last elements of the container.
        -- Most STL containers support these functions; exceptions include containers with limited access like stack and queue.
        -- The functions have consistent names across containers for uniformity.

    Lists all methods that returns the iterator to the containers:
        begin() : Returns an iterator to the beginning of container.
        end() : Returns an iterator to the theoretical element just after the last element of the container.
        cbegin() : Returns a constant iterator to the beginning of container. A constant iterator cannot modify the value of the element it is pointing to.
        cend() : Returns a constant iterator to the theoretical element just after the last element of the container.
        rbegin() : Returns a reverse iterator to the beginning of container.
        rend() : Returns a reverse iterator to the theoretical element just after the last element of the container.
        crbegin() : Returns a constant reverse iterator to the beginning of container.
        crend() : Returns a constant reverse iterator to the theoretical element just after the last element of the container.

    -- The iterator header also contains two generic functions (std::begin and std::end) that can be used.
    -- std::begin and std::end for C-style arrays are defined in the <iterator> header.
    -- std::begin and std::end for containers that support iterators are defined in the header files for those containers (e.g. <array>, <vector>).

    operator< vs operator!= for iterators
        -- These operators are used to test whether the iterator has reached the end element.
        -- Always prefer operator!= . Because some iterator types are not relationally comparable. 
           operator!= works with all iterator types.
    
    Back to range-based for loops
        -- Behind the scenes, the range-based for-loop calls begin() and end() of the type to iterate over. 
        -- std::array has begin and end member functions, so we can use it in a range-based loop.
        -- C-style fixed arrays can be used with std::begin and std::end functions, 
           so we can loop through them with a range-based loop as well. 
        -- Dynamic C-style arrays (or decayed C-style arrays) don’t work though, because there is no std::end function 
           for them (because the type information doesn’t contain the array’s length).
    
    Iterator invalidation (dangling iterators)
        -- Much like pointers and references, iterators can be left “dangling” if the elements being iterated over change address or are destroyed. 
        -- When this happens, we say the iterator has been invalidated. Accessing an invalidated iterator produces undefined behavior.
        -- Some operations that modify containers (such as adding an element to a std::vector) can cause the elements in the container to change addresses.
           When this happens, existing iterators to those elements will be invalidated.
        -- Good C++ reference documentation should note which container operations may or will invalidate iterators. 
           As an example, see the “Iterator invalidation” section of std::vector on cppreference.
        -- Since range-based for-loops use iterators behind the scenes, we must not do anything that invalidates 
           the iterators of the container we are actively traversing.
            Example: v.push_back(num + 1); in range based loops
                     v.erase(it); // erase the element currently being iterated over

            Invalidated iterators can be revalidated by assigning a valid iterator to them (e.g. begin(), end(), 
            or the return value of some other function that returns an iterator).

            The erase() function returns an iterator to the element one past the erased element (or end() 
            if the last element was removed). Therefore, we can fix the above code like this:
                        it = v.erase(it);

             