1 — Dynamic memory allocation with new and delete
    The need for dynamic memory allocation
    C++ supports three basic types of memory allocation:
    -- Static memory allocation happens for static and global variables. Memory for these types of variables is allocated 
       once when your program is run and persists throughout the life of your program.
    -- Automatic memory allocation happens for function parameters and local variables. Memory for these types of variables 
       is allocated when the relevant block is entered, and freed when the block is exited, as many times as necessary.
    -- Dynamic memory allocation is the topic of this article.

    Both static and automatic allocation have two things in common:
    -- The size of the variable / array must be known at compile time.
    -- Memory allocation and deallocation happens automatically (when the variable is instantiated / destroyed). 
    
    -- Dynamic memory allocation is a way for running programs to request memory from the OS when needed. 
       This memory does not come from the program’s limited stack memory -- instead, it is allocated from a much larger pool 
       of memory managed by the OS called the heap. On modern machines, the heap can be gigabytes in size.

    -- Scalar (non-array) form of the new operator to allocate a single variable dynamically:
        int* ptr{ new int }; // dynamically allocate an integer and assign the address to ptr.

    NOTE : Accessing heap-allocated objects is generally slower than accessing stack-allocated objects. 
           Because the compiler knows the address of stack-allocated objects, it can go directly to that address to get a value.
           Heap allocated objects are typically accessed via pointer. 
           This requires two steps: 
            -- One to get the address of the object (from the pointer).
            -- Another to get the value.
    
    Deleting a single variable
        -- Scalar (non-array) form of the delete operator to delete a single variable.
            delete ptr; // return the memory pointed to by ptr to the operating system
            ptr = nullptr; // set ptr to be a null pointer
        
        NOTE : Deleting a pointer that is not pointing to dynamically allocated memory may cause bad things to happen.
    
    Dangling pointers
        -- A pointer that is pointing to deallocated memory is called a dangling pointer. 
           Dereferencing or deleting a dangling pointer will lead to undefined behavior. 
        
        NOTE : Set deleted pointers to nullptr unless they are going out of scope immediately afterward.
    
    Operator new can fail
        -- By default, if new fails, a bad_alloc exception is thrown. 
           If this exception isn’t properly handled, the program will terminate/crash with an unhandled exception error.
        -- An alternate form of new is to tell new to return a null pointer if memory can’t be allocated. 
           It is done by adding the constant std::nothrow between the new keyword and the allocation type:
            int* value { new (std::nothrow) int };
        -- Deleting a null pointer is okay, and does nothing. There is no need to conditionalize your delete statements.

    Memory leaks
        -- It happens when you allocate memory using new, new[], malloc, etc., but never release it using 
           delete, delete[], or free.
           As a result, the memory stays “in use” even though your program can no longer access it.

2 — Dynamically allocating arrays
    -- We'll be dynamically allocating C-style arrays, which is the most common type of dynamically allocated array.
    -- If we feel the need to dynamically allocate a std::array, We should use std::vector instead.
    -- The new[] operator is called to dynamically allocate arrays.
    -- The length of dynamically allocated arrays has type std::size_t. If we are using a non-constexpr int, 
       we’ll need to static_cast to std::size_t since that is considered a narrowing conversion. 
    
    Dynamically deleting arrays
        -- The array version of delete is delete[].
        -- Using the scalar version of delete on an array will result in undefined behavior, 
           such as data corruption, memory leaks, crashes, or other problems.
        
        How does array delete know how much memory to delete?
        --  Array new[] keeps track of how much memory was allocated to a variable, so that array delete[] can delete the proper amount. 
            Unfortunately, this size/length isn’t accessible to the programmer.
        
    Dynamic arrays are almost identical to fixed arrays
        -- A fixed array can decay into a pointer that points to the first element of the array.
        -- In this decayed form, the length of the fixed array is not available (size of the array via sizeof() is NOT possible).
        -- A dynamic array functions identically to a decayed fixed array, with the exception that the programmer is responsible 
           for deallocating the dynamic array via the delete[] keyword. 
    
    Initializing dynamically allocated arrays
        auto* array{ new int[5]{ 9, 7, 5, 3, 1 } };
    
    Resizing arrays
        -- However, C++ does not provide a built-in way to resize an array that has already been allocated.
        -- Work around : Allocating a new array dynamically, copying the elements over, and deleting the old array. 
           However, this is error prone, especially when the element type is a class.
           std::vector is recommended instead.

3 — Destructors
    RAII (Resource Acquisition Is Initialization) 
    -- is a programming technique whereby resource use is tied to the lifetime of objects with automatic duration (e.g. non-dynamically allocated objects). 
       In C++, RAII is implemented via classes with constructors and destructors. 
    -- A resource (such as memory, a file or database handle, etc…) is typically acquired in the object’s constructor 
       (though it can be acquired after the object is created if that makes sense). That resource can then be used 
       while the object is alive. The resource is released in the destructor, when the object is destroyed. 
    -- The primary advantage of RAII is that it helps prevent resource leaks (e.g. memory not being deallocated) as all 
       resource-holding objects are cleaned up automatically.
    
    A warning about the std::exit() function
    NOTE : If we use the std::exit() function, the program will terminate and no destructors will be called. 
           Be wary if we’re relying on your destructors to do necessary cleanup work 
           (e.g. write something to a log file or database before exiting).
    
4 — Pointers to pointers and dynamic multidimensional arrays
    Arrays of pointers
        int** array { new int*[10] }; // allocate an array of 10 int pointers
    
    Two-dimensional dynamically allocated arrays
    There are two possible solutions here. 
        int x { 7 }; // non-constant
        int (*array)[5] { new int[x][5] }; // rightmost dimension must be constexpr
    
        int x { 7 }; // non-constant
        auto array { new int[x][5] }; // so much simpler!

        NOTE : The rightmost array dimension must be constexpr.
        Otherwise, we can use little complex method.

        int** array { new int*[10] }; // allocate an array of 10 int pointers — these are our rows
        for (int count { 0 }; count < 10; ++count)
            array[count] = new int[5]; // these are our columns

        With this method, because each array column is dynamically allocated independently, 
        it’s possible to make dynamically allocated two dimensional arrays that are not rectangular. 
        For example, we can make a triangle-shaped array:

        int** array { new int*[10] }; // allocate an array of 10 int pointers — these are our rows
        for (int count { 0 }; count < 10; ++count)
            array[count] = new int[count+1]; // these are our columns

    Deallocating a dynamically allocated two-dimensional array using this method requires a loop as well:
        for (int count { 0 }; count < 10; ++count)
            delete[] array[count];
        delete[] array; // this needs to be done last
    
    NOTEs :
        We delete the array in the opposite order that we created it (elements first, then the array itself). 
        If we delete array before the array columns, then we’d have to access deallocated memory to delete 
        the array columns. And that would result in undefined behavior.
    
    Because allocating and deallocating two-dimensional arrays is complex and easy to mess up, 
    it’s often easier to “flatten” a two-dimensional array (of size x by y) into a one-dimensional array of size x * y:

        // Instead of this:
        int** array { new int*[10] }; // allocate an array of 10 int pointers — these are our rows
        for (int count { 0 }; count < 10; ++count)
            array[count] = new int[5]; // these are our columns

        // Do this
        int *array { new int[50] }; // a 10x5 array flattened into a single array

    Simple math can then be used to convert a row and column index for a rectangular two-dimensional array into a single index for a one-dimensional array:

        int getSingleIndex(int row, int col, int numberOfColumnsInArray)
        {
            return (row * numberOfColumnsInArray) + col;
        }

        // set array[9,4] to 3 using our flattened array
        array[getSingleIndex(9, 4, 5)] = 3;

5 — Void pointers
    Void pointer miscellany
        void* ptr{ nullptr }; // ptr is a void pointer that is currently a null pointer
    
    -- As a void pointer does not know what type of object it points to, deleting the pointer will result in undefined behavior.
       If you need to delete a void pointer, static_cast it back to the appropriate type first.
    -- It is not possible to do pointer arithmetic on a void pointer.
       Because pointer arithmetic requires the pointer to know what size object it is pointing to,

    NOTE : A void reference doesn't exist. Because, It would not know what type of value it referenced.
    






