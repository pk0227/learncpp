1 — Function Pointers
    Pointers to functions
        // fcnPtr is a pointer to a function that takes no arguments and returns an integer
        int (*fcnPtr)();
    To make a const function pointer, the const goes after the asterisk:
        int (*const fcnPtr)();
    If we put the const before the int, then that would indicate the function being pointed to would return a const int.

    Assigning a function to a function pointer
            int (*fcnPtr)(){ &foo }; // fcnPtr points to function foo
            fcnPtr = &goo; // fcnPtr now points to function goo

        NOTEs : 
        The type (parameters and return type) of the function pointer must match the type of the function.
            double goo();
            int (*fcnPtr2)(){ &goo };    // wrong -- return types don't match!
        
        Unlike fundamental types, C++ will implicitly convert a function into a function pointer if needed 
        (so you don’t need to use the address-of operator (&) to get the function’s address). 
        However, function pointers will not convert to void pointers, or vice-versa

            // function prototypes
            int foo();

            // function initializations
            int (*fcnPtr5)() { foo }; // okay, foo implicitly converts to function pointer to foo
            void* vPtr { foo };       // not okay, though some compilers may allow
        
        Function pointers can also be initialized or assigned the value nullptr:
            int (*fcnptr)() { nullptr }; // okay
        
        Calling a function using a function pointer
            The first is via explicit dereference:
                (*fcnPtr)(5); // call function foo(5) through fcnPtr.
            The second way is via implicit dereference:
                fcnPtr(5); // call function foo(5) through fcnPtr.
    
    Default arguments don’t work for functions called through function pointers 
        -- When the compiler encounters a normal function call to a function with one or more default arguments, 
           it rewrites the function call to include the default arguments. This process happens at compile-time, 
           and thus can only be applied to functions that can be resolved at compile time.
        -- However, when a function is called through a function pointer, it is resolved at runtime. In this case, 
           there is no rewriting of the function call to include default arguments.
        -- Because It is runtime resolution (Late Binding), default arguments are not resolved 
           when a function is called through a function pointer.
            void print(int x);
            void print(int x, int y = 10);

            using vnptr = void(*)(int); // define a type alias for a function pointer to a void(int) function
            vnptr pi { print }; // initialize our function pointer with function print
            pi(1); // call the print(int) function through the function pointer

            // Concise method
            static_cast<void(*)(int)>(print)(1); // call void(int) version of print with argument 1

    Passing functions as arguments to other functions
        Callback Functions
        -- One of the most useful things to do with function pointers is pass a function as an argument to another function. 
           Functions used as arguments to another function are sometimes called callback functions.

        NOTE : If a function parameter is of a function type, it will be converted to a pointer to the function type.
            void selectionSort(int* array, int size, bool (*comparisonFcn)(int, int))
            can be equivalently written as:
            void selectionSort(int* array, int size, bool comparisonFcn(int, int))

            This only works for function parameters, and so is of somewhat limited use.
            bool (*ptr)(int, int); // definition of function pointer ptr
            bool fcn(int, int);    // forward declaration of function fcn
        
    Providing default functions
        You can even set one of these as a default parameter:

            // Default the sort to ascending sort
            void selectionSort(int* array, int size, bool (*comparisonFcn)(int, int) = ascending);

    Making function pointers prettier with type aliases
        using ValidateFunction = bool(*)(int, int);

        Now instead of doing this:
        bool validate(int x, int y, bool (*fcnPtr)(int, int)); // ugly

        You can do this:
        bool validate(int x, int y, ValidateFunction pfcn) // clean

    Using std::function
        -- An alternate method of defining and storing function pointers is to use std::function, 
           which is part of the standard library <functional> header.

            #include <functional>
            bool validate(int x, int y, std::function<bool(int, int)> fcn); 
            
        std::function method that returns a bool and takes two int parameters.

        std::function<int()> fcnPtr{ &foo };
        fcnPtr = &goo;

        NOTE : std::function only allows calling the function via implicit dereference (e.g. fcnPtr()), 
               NOT explicit dereference (e.g. (*fcnPtr)()).
               
               When defining a type alias, we must explicitly specify any template arguments. 
               We can’t use CTAD in this case since there is no initializer to deduce the template arguments from.
    
    Type inference for function pointers
        -- Much like the auto keyword can be used to infer the type of normal variables, 
           the auto keyword can also infer the type of a function pointer.

        -- The downside is, of course, that all of the details about the function’s parameters types and return type are hidden, 
           so it’s easier to make a mistake when making a call with the function, or using its return value.

20.2 — The stack and the heap
    The memory segments:
    The code segment (also called a text segment) : 
        -- where the compiled program sits in memory. The code segment is typically read-only.
    The bss segment (also called the uninitialized data segment) : 
        -- where zero-initialized global and static variables are stored.
    The data segment (also called the initialized data segment) : 
        -- where initialized global and static variables are stored.
    The heap : 
        -- where dynamically allocated variables are allocated from.
    The call stack : 
        -- where function parameters, local variables, and other function-related information are stored.
    
    The heap segment
        The heap has advantages and disadvantages:
            -- Allocating memory on the heap is comparatively slow.
            -- Allocated memory stays allocated until it is specifically deallocated (beware memory leaks) 
               or the application ends (at which point the OS should clean it up).
            -- Dynamically allocated memory must be accessed through a pointer. Dereferencing a pointer is 
               slower than accessing a variable directly.
            -- Because the heap is a big pool of memory, large arrays, structures, or classes can be allocated here.

    The stack data structure
        A stack is a container data structure that holds multiple variables (much like an array). 
        The operations that can be performed on a stack :
            -- Look at the top item on the stack (usually done via a function called top(), but sometimes called peek())
            -- Take the top item off of the stack (done via a function called pop())
            -- Put a new item on top of the stack (done via a function called push())

        A stack is a last-in, first-out (LIFO) structure. The last item pushed onto the stack will be the first item popped off.
    
    The call stack segment
        -- When the application starts, the main() function is pushed on the call stack by the operating system. 
           Then the program begins executing.
        -- When a function call is encountered, the function is pushed onto the call stack. When the current function ends, 
           that function is popped off the call stack (this process is sometimes called unwinding the stack). 
        -- The “items” we’re pushing and popping on the stack are called stack frames. 
        -- A stack frame keeps track of all of the data associated with one function call.

        CPU has two important registers : 
            Stack Pointer(SP) 
            -- A CPU register that always points to the top of the stack. It is used for managing function calls, storing return 
               addresses, local variables, and maintaining the call stack. Every push/pop updates the stack pointer. 
                
            Program Counter (PC) — also called Instruction Pointer (IP)
            -- A CPU register that holds the address of the next instruction to execute. 
               It ensures sequential execution unless a jump, branch, or function call changes its value.
            
    The call stack in action
    Here is the sequence of steps that takes place when a function is called:
        -- The program encounters a function call.
        -- A stack frame is constructed and pushed on the stack. The stack frame consists of:
            -- The address of the instruction beyond the function call (called the return address). 
               This is how the CPU remembers where to return to after the called function exits.
            -- All function arguments.
            -- Memory for any local variables
            -- Saved copies of any registers modified by the function that need to be restored when the function returns
        -- The CPU jumps to the function’s start point.
        -- The instructions inside of the function begin executing.

    When the function terminates, the following steps happen:
        -- Registers are restored from the call stack
        -- The stack frame is popped off the stack. This frees the memory for all local variables and arguments.
        -- The return value is handled.
        -- The CPU resumes execution at the return address.

    Return values can be handled in a number of different ways, depending on the computer’s architecture. 
    Some architectures include the return value as part of the stack frame. Others use CPU registers.

    Stack overflow
    -- With g++/Clang for Unix variants, it can be as large as 8MB. If the program tries to put too much information on the stack,
       stack overflow will result. Stack overflow happens when all the memory in the stack has been allocated.
       In that case, further allocations begin overflowing into other sections of memory.
    -- On modern OS, overflowing the stack will generally cause your OS to issue an access violation and terminate the program.

    The stack has advantages and disadvantages:
        -- Allocating memory on the stack is comparatively fast.
        -- Memory allocated on the stack stays in scope as long as it is on the stack. 
           It is destroyed when it is popped off the stack.
        -- All memory allocated on the stack is known at compile time. 
           Consequently, this memory can be accessed directly through a variable.
        -- Because the stack is relatively small, it is generally not a good idea to do anything that 
           eats up lots of stack space. This includes allocating or copying large arrays or other memory-intensive structures.
    
    
