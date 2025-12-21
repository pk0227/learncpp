1 — The need for exceptions
    When return codes fail
    -- One of the most common ways to handle potential errors is via return codes.
    
    However, using return codes has a number of drawbacks :
    -- First, return values can be cryptic -- if a function returns -1, is it trying to indicate an error, or is that 
       actually a valid return value? It’s often hard to tell without digging into the guts of the function or consulting 
       documentation.
    -- Second, functions can only return one value, so what happens when you need to return both a function result and a 
       possible error code? Consider the following function:

        double divide(int x, int y)
        {
            return static_cast<double>(x)/y;
        }
       This function is in desperate need of some error handling, because it will crash if the user passes in 0 for parameter y. 
       However, it also needs to return the result of x/y. How can it do both?
    -- Third, in sequences of code where many things can go wrong, error codes have to be checked constantly. 
    -- Fourth, return codes do not mix with constructors very well. What happens if you’re creating an object and something inside 
       the constructor goes catastrophically wrong? Constructors have no return type to pass back a status indicator.
    -- Finally, when an error code is returned to the caller, the caller may not always be equipped to handle the error. 
       If the caller doesn’t want to handle the error, it either has to ignore it (in which case it will be lost forever), or 
       return the error up the stack to the function that called it.
    
    Exceptions
    Exception handling provides a mechanism to decouple handling of errors or other exceptional circumstances from the typical 
    control flow of your code. This allows more freedom to handle errors when and how ever is most useful for a given situation, 
    alleviating most (if not all) of the messiness that return codes cause.

2 — Basic exception handling
    -- Exceptions in C++ are implemented using three keywords that work in conjunction with each other: throw, try, and catch.

    Throwing exceptions
    -- In C++, a throw statement is used to signal that an exception or error case has occurred. 
       Signaling that an exception has occurred is also commonly called raising an exception.
    -- To use a throw statement, simply use the throw keyword, followed by a value of any data type you wish to use to signal that 
       an error has occurred. Typically, this value will be an error code, a description of the problem, or a custom exception class.

        Here are some examples:
            throw -1; // throw a literal integer value
            throw ENUM_INVALID_INDEX; // throw an enum value
            throw "Can not take square root of negative number"; // throw a literal C-style (const char*) string
            throw dX; // throw a double variable that was previously defined
            throw MyException("Fatal Error"); // Throw an object of class MyException

    Looking for exceptions
    -- In C++, we use the try keyword to define a block of statements (called a try block). The try block acts as an observer, 
       looking for any exceptions that are thrown by any of the statements within the try block.
    
    Handling exceptions
    -- Actually handling exceptions is the job of the catch block(s). The catch keyword is used to define a block of code (called
       a catch block) that handles exceptions for a single data type.
    -- A try block must have at least one catch block immediately following it, but may have multiple catch blocks listed in sequence.
    -- Once an exception has been caught by the try block and routed to a matching catch block for handling, the exception is 
       considered handled. After the matching catch block executes, execution then resumes as normal, starting with the first 
       statement after the last catch block.
    -- Catch parameters work just like function parameters, with the parameter being available within the subsequent catch block.
       Exceptions of fundamental types can be caught by value, but exceptions of non-fundamental types should be caught by 
       const reference to avoid making an unnecessary copy (and, in some cases, to prevent slicing).
    -- Just like with functions, if the parameter is not going to be used in the catch block, the variable name can be omitted.
       This can help prevent compiler warnings about unused variables.
    -- No type conversion is done for exceptions (An int exception won't be converted to match a catch block with a double parameter).

    Recapping exception handling
    -- When an exception is thrown, the running program finds the nearest enclosing try block to see if any of the catch handlers
       attached to the try block can handle that type of exception. If so, execution jumps to the top of the catch block, the 
       exception is considered handled.
    -- If no appropriate catch handlers exist in the nearest enclosing try block, the program continues to look at subsequent 
       enclosing try blocks for a catch handler. If no appropriate catch handlers can be found before the end of the program, 
       the program will fail with a runtime exception error.
    NOTE : The program will not perform implicit conversions or promotions when matching exceptions with catch blocks! 
           For example, a char exception will not match with an int catch block. An int exception will not match a float catch block. 
           However, casts from a derived class to one of its parent classes will be performed.

    What catch blocks typically do
    -- If an exception is routed to a catch block, it is considered “handled” even if the catch block is empty. 
       However, typically we’ll want your catch blocks to do something useful. 
       There are four common things that catch blocks do when they catch an exception:
       -- First, catch blocks may print an error (either to the console, or a log file) and then allow the function to proceed.
       -- Second, catch blocks may return a value or error code back to the caller.
       -- Third, a catch block may throw another exception. Because the catch block is outside of the try block, the newly thrown 
          exception in this case is not handled by the preceding try block -- it’s handled by the next enclosing try block.
       -- Fourth, a catch block in main() may be used to catch fatal errors and terminate the program in a clean way.
       
3 — Exceptions, functions, and stack unwinding
   Throwing exceptions from a called function
      -- Try blocks catch exceptions not only from statements within the try block, but also from functions that are called within 
         the try block.
   Exception handling and stack unwinding
      -- When an exception is thrown, the program first looks to see if the exception can be handled immediately inside the current 
         function. If not, the program next checks whether the function’s caller can handle the exception. 
      -- In order for the function’s caller to handle the exception, the call to the current function must be inside a try block, and 
         a matching catch block must be associated. If no match is found, then the caller’s caller is checked. 
      -- Similarly, in order for the caller’s caller to handle the exception, the call to the caller must be inside a try block, and 
         a matching catch block must be associated.
      -- The process of checking each function up the call stack continues until either a handler is found, or all of the functions on 
         the call stack have been checked and no handler can be found.
      -- If a matching exception handler is found, then execution jumps from the point where the exception is thrown to the top of the 
         matching catch block. This requires unwinding the stack (removing the current function from the call stack) as many times as 
         necessary to make the function handling the exception the top function on the call stack.
      -- If no matching exception handler is found, the stack may or may not be unwound.
      -- When the current function is removed from the call stack, all local variables are destroyed as usual, but no value is returned.
         Stack unwinding destroys local variables in the functions that are unwound (It is good, because it ensures its destructors execute).
      -- Stack unwinding provides some very useful behavior -- if a function does not want to handle an exception, it doesn’t have to. 
         The exception will propagate up the stack until it finds someone who will! This allows us to decide where in the call stack is 
         the most appropriate place to handle any errors that may occur.
      
4 — Uncaught exceptions and catch-all handlers
   Uncaught exceptions
      -- The call stack may or may not be unwound if an exception is unhandled.
      -- If the stack is not unwound, local variables will not be destroyed, which may cause problems if those variables have 
         non-trivial destructors.
      -- Not unwinding the stack on an unhandled exception preserves the stack’s state, which is critical for debugging. 
         If the stack were unwound, valuable context about what led to the exception would be lost. 
         Keeping it intact helps diagnose and fix the issue.
   
   Catch-all handlers
   -- Functions can potentially throw exceptions of any data type (including program-defined data types), meaning there is an infinite 
      number of possible exception types to catch.
   -- If an exception is not caught, your program will terminate immediately (and the stack may not be unwound, so your program may not
      even clean up after itself properly).
   -- Adding explicit catch handlers for every possible type is tedious, especially for the ones that are expected to be reached only 
      in exceptional cases!

      -- Fortunately, C++ also provides us with a mechanism to catch all types of exceptions. This is known as a catch-all handler. 
         A catch-all handler works just like a normal catch block, except that instead of using a specific type to catch, it uses the 
         ellipses operator (…) as the type to catch. The catch-all handler is also sometimes called an “ellipsis catch handler”.

      -- The catch-all handler must be placed last in the catch block chain. This is to ensure that exceptions can be caught by exception 
         handlers tailored to specific data types if those handlers exist.

      -- This will catch any unanticipated exceptions, ensuring that stack unwinding occurs up to this point and preventing the program 
         from terminating, but does no specific error handling.

      -- If an exception is caught by the catch-all handler, we should assume the program is now in some indeterminate state, perform 
         cleanup immediately, and then terminate.

   Debugging unhandled exceptions
      -- if we have a catch-all handler, then all exceptions are handled, and (because the stack is unwound) we lose useful diagnostic 
         information. Therefore, in debug builds, it can be useful to disable the catch-all handler. We can do this via conditional 
         compilation directives.
   
5 — Exceptions, classes, and inheritance
   When constructors fail
      -- Constructors are another area of classes in which exceptions can be very useful. If a constructor must fail for some reason 
         (e.g. the user passed in invalid input), simply throw an exception to indicate the object failed to create. In such a case, 
         the object’s construction is aborted, and all class members (which have already been created and initialized prior to the body
         of the constructor executing) are destructed as per usual.
      -- However, the class’s destructor is never called (because the object never finished construction). Because the destructor never 
         executes, we can’t rely on said destructor to clean up any resources that have already been allocated.

      This leads to the question of what we should do if we’ve allocated resources in our constructor and then an exception occurs prior 
      to the constructor finishing. How do we ensure the resources that we’ve already allocated get cleaned up properly?

      1. One way would be to wrap any code that can fail in a try block, use a corresponding catch block to catch the exception and do 
         any necessary cleanup, and then rethrow the exception.
      2. Fortunately, there is a better way. Taking advantage of the fact that class-type members are destructed even if the constructor fails, 
         if you do the resource allocations inside the members of the class (rather than in the constructor itself), then those members 
         can clean up after themselves when they are destructed.
         
         -- Even in exceptional circumstances, classes that implement RAII are able to clean up after themselves.
         -- However, creating a custom class to manage a resource allocation isn’t efficient. Fortunately, the C++ standard library comes with 
            RAII-compliant classes to manage common resource types, such as files (std::fstream) and dynamic memory (std::unique_ptr and the 
            other smart pointers).
   
   Exception classes
      -- One of the major problems with using basic data types (such as int) as exception types is that they are inherently vague. An even 
         bigger problem is disambiguation of what an exception means when there are multiple statements or function calls within a try block.
      -- One way to solve this problem is to use exception classes. An exception class is just a normal class that is designed specifically 
         to be thrown as an exception.
      -- Exceptions of a fundamental type can be caught by value since they are cheap to copy.
      -- Exceptions of a class type should be caught by (const) reference to prevent expensive copying and slicing.
   
   Exceptions and inheritance
      -- Since it’s possible to throw classes as exceptions, and classes can be derived from other classes, we need to consider what happens 
         when we use inherited classes as exceptions. As it turns out, exception handlers will not only match classes of a specific type, 
         they’ll also match classes derived from that specific type as well.
      -- Handlers for derived exception classes should be listed before those for base classes.
      -- The ability to use a handler to catch exceptions of derived types using a handler for the base class turns out to be exceedingly 
         useful.
   
   std::exception
      -- As of C++20, there are 28 different exception classes that can be thrown, with more being added in each subsequent language standard.
      -- The good news is that all of these exception classes are derived from a single class called std::exception (defined in the 
         <exception> header). std::exception is a small interface class designed to serve as a base class to any exception thrown by the 
         C++ standard library.
      -- Thanks to std::exception, we can set up an exception handler to catch exceptions of type std::exception, and we’ll end up catching 
         std::exception and all of the derived exceptions together in one place. Easy!
      -- The one thing worth noting is that std::exception has a virtual member function named what() that returns a C-style string description
         of the exception. Most derived classes override the what() function to change the message. 
         NOTE : this string is meant to be used for descriptive text only -- do not use it for comparisons, as it is not guaranteed to be 
                the same across compilers.
      -- Sometimes we’ll want to handle a specific type of exception differently. In this case, we can add a handler for that specific type, 
         and let all the others “fall through” to the base handler. 
   
   Using the standard exceptions directly
      -- Nothing throws a std::exception directly, and neither should we. However, you should feel free to throw the other standard exception
         classes in the standard library if they adequately represent your needs. 
      -- std::runtime_error (included as part of the stdexcept header) is a popular choice, because it has a generic name, and its constructor
         takes a customizable message.
   
   Deriving your own classes from std::exception or std::runtime_error
      -- We can, of course, derive our own classes from std::exception, and override the virtual what() const member function. 
      NOTE : Virtual function what() has specifier noexcept (which means the function promises not to throw exceptions itself). 
             Therefore, our override should also have specifier noexcept.
      -- std::runtime_error can take a C-style string parameter, or a const std::string& parameter.

   The lifetime of exceptions
      -- When an exception is thrown, the object being thrown is typically a temporary or local variable that has been allocated on the 
         stack. However, the process of exception handling may unwind the function, causing all variables local to the function to be 
         destroyed. So how does the exception object being thrown survive stack unwinding?
      -- When an exception is thrown, the compiler makes a copy of the exception object to some piece of unspecified memory (outside of 
         the call stack) reserved for handling exceptions. That way, the exception object is persisted regardless of whether or how many 
         times the stack is unwound. The exception is guaranteed to exist until the exception has been handled.
      -- This means that the objects being thrown generally need to be copyable (even if the stack is not actually unwound). 
         Smart compilers may be able to perform a move instead, or elide the copy altogether in specific circumstances.

      NOTEs : 
         -- Exception objects need to be copyable.
         -- Exception objects should not keep pointers or references to stack-allocated objects. If a thrown exception results in stack 
            unwinding (causing the destruction of stack-allocated objects), these pointers or references may be left dangling.
      
6 — Rethrowing exceptions
   Throwing a new exception
      -- Although it may seem weird to throw an exception from a catch block, this is allowed. Remember, only exceptions thrown within a 
         try block are eligible to be caught. This means that an exception thrown within a catch block will not be caught by the catch 
         block it’s in. Instead, it will be propagated up the stack to the caller.
      -- The exception thrown from the catch block can be an exception of any type -- it doesn’t need to be the same type as the exception 
         that was just caught.
   
   Rethrowing an exception (the wrong way)
      -- When we throw an exception of a Derived object, ensure that the catch block is catching it with a Base reference.
      -- When a derived object is thrown but caught by value using a base-type variable, the thrown exception is copy-initialized as a Base 
         object. Since the Base copy cannot hold the derived part, the Derived portion is discarded—this is object slicing.
   
   Rethrowing an exception (the right way)
      -- Fortunately, C++ provides a way to rethrow the exact same exception as the one that was just caught. To do so, simply use the throw 
         keyword from within the catch block (with no associated variable).
      -- This throw keyword that doesn’t appear to throw anything in particular actually re-throws the exact same exception that was just 
         caught. No copies are made, meaning we don’t have to worry about performance killing copies or slicing.
      -- When rethrowing the same exception, use the throw keyword by itself
   
7 — Function try blocks
   -- In the case, where constructor may fail, we have to use a slightly modified try block called a function try block.

   Function try blocks
      -- Use function try blocks when you need a constructor to handle an exception thrown in the member initializer list.
   
   -- With a regular catch block (inside a function), we have three options: We can throw a new exception, rethrow the current exception, 
      or resolve the exception (by either a return statement, or by letting control reach the end of the catch block).
   -- But, the function-level catch block has some limitations.

   Limitations on function catch blocks
      -- A function-level catch block for a constructor must either throw a new exception or rethrow the existing exception.
            -- they are not allowed to resolve exceptions.
            -- Return statements are also not allowed.
            -- reaching the end of the catch block will implicitly rethrow.
      -- A function-level catch block for a destructor can throw, rethrow, or resolve the current exception via a return statement. 
         Reaching the end of the catch block will implicitly rethrow.
      -- A function-level catch block for other functions can throw, rethrow, or resolve the current exception via a return statement.
            -- Reaching the end of the catch block will implicitly resolve the exception for non-value (void) returning functions.
            -- produce undefined behavior for value-returning functions.
      +------------------------------+---------------------------------------------+--------------------------------+   
      | Function type                | Can resolve exceptions via return statement | Behavior at end of catch block |
      +------------------------------+---------------------------------------------+--------------------------------+
      | Constructor                  | No, must throw or rethrow                   | Implicit rethrow               |
      | Destructor                   | Yes                                         | Implicit rethrow               |
      | Non-value returning function | Yes                                         | Resolve exception              |
      | Value-returning function     | Yes                                         | Undefined behavior             |
      +------------------------------+---------------------------------------------+--------------------------------+

      -- Because such behavior at the end of the catch block varies dramatically depending on the type of function (and includes undefined 
         behavior in the case of value-returning functions), 
         Avoid letting control reach the end of a function-level catch block. Instead, explicitly throw, rethrow, or return.

   -- Function try blocks can catch both base and the current class exceptions.

   Don’t use function try to clean up resources
      -- If an object’s construction fails, its destructor is never called, and the object is already “dead” by the time a function-try-block’s 
         catch executes. Accessing its members is therefore undefined behavior, so function-try-blocks cannot be used for cleanup of partially
         constructed objects.
      -- Function try blocks are mainly useful for logging failures or converting the exception type before rethrowing.

8 — Exception dangers and downsides
   Cleaning up resources
      -- The best option (whenever possible) is to prefer to stack allocate objects that implement RAII (automatically allocate resources on 
         construction, deallocate resource on destruction). That way when the object managing the resource goes out of scope for any reason, 
         it will automatically deallocate as appropriate, so we don’t have to worry about such things.
      
   Exceptions and destructors
      -- Unlike constructors, where throwing exceptions can be a useful way to indicate that object creation did not succeed, exceptions 
         should NEVER be thrown in destructors.
      -- The problem occurs when an exception is thrown out of a destructor during the stack unwinding process. If that happens, the compiler
         is put in a situation where it doesn’t know whether to continue the stack unwinding process or handle the new exception. The end 
         result is that your program will be terminated immediately.
      -- Consequently, the best course of action is just to abstain from using exceptions in destructors altogether. Write a message to a 
         log file instead.

   Performance concerns
      -- Exceptions do come with a small performance price to pay. They increase the size of your executable, and they may also cause it to 
         run slower due to the additional checking that has to be performed.
      -- However, the main performance penalty for exceptions happens when an exception is actually thrown. In this case, the stack must be 
         unwound and an appropriate exception handler found, which is a relatively expensive operation.
      -- As a note, some modern computer architectures support an exception model called zero-cost exceptions. Zero-cost exceptions, if 
         supported, have no additional runtime cost in the non-error case (which is the case we most care about performance). However, 
         they incur an even larger penalty in the case where an exception is found.
      
   So when should I use exceptions?
   Exception handling is best used when all of the following are true:
      -- The error being handled is likely to occur only infrequently.
      -- The error is serious and execution could not continue otherwise.
      -- The error cannot be handled at the place where it occurs.
      -- There isn’t a good alternative way to return an error code back to the caller.
   
9 — Exception specifications and noexcept
   -- Exception specifications are a language mechanism that was originally designed to document what kind of exceptions a function might 
      throw as part of a function specification. While most of the exception specifications have now been deprecated or removed, one useful
      exception specification was added as a replacement.

   The noexcept specifier
      -- In C++, all functions are classified as either non-throwing or potentially throwing.
      -- A non-throwing function is one that promises not to throw exceptions that are visible to the caller.
      -- A potentially throwing function may throw exceptions that are visible to the caller.
      -- To define a function as non-throwing, we can use the noexcept specifier.
            void doSomething() noexcept; // this function is specified as non-throwing
      
      NOTE : noexcept doesn’t actually prevent the function from throwing exceptions or calling other functions that are potentially throwing. 
             This is allowed so long as the noexcept function catches and handles those exceptions internally, and those exceptions do not 
             exit the noexcept function.

      -- If an unhandled exception would exit a noexcept function, std::terminate will be called (even if there is an exception handler that 
         would otherwise handle such an exception somewhere up the stack). And if std::terminate is called from inside a noexcept function, 
         stack unwinding may or may not occur (depending on implementation and optimizations), which means your objects may or may not be 
         destructed properly prior to termination.

      NOTE : Much like functions that differ only in their return values can not be overloaded, functions differing only in their exception 
             specification can not be overloaded.
      
   The noexcept specifier with a Boolean parameter
      -- The noexcept specifier has an optional Boolean parameter. noexcept(true) is equivalent to noexcept, meaning the function is 
         non-throwing. noexcept(false) means the function is potentially throwing. These parameters are typically only used in 
         template functions, so that a template function can be dynamically created as non-throwing or potentially throwing based on 
         some parameterized value.
   
   Which functions are non-throwing and potentially-throwing
      Functions that are implicitly non-throwing:
         -- Destructors
      Functions that are non-throwing by default for implicitly-declared or defaulted functions:
         -- Constructors: default, copy, move
         -- Assignments: copy, move
         -- Comparison operators (as of C++20)
      However, if any of these functions call (explicitly or implicitly) another function which is potentially throwing, then the listed 
      function will be treated as potentially throwing as well. 
      
      For example, if a class has a data member with a potentially throwing constructor, then the class’s constructors will be treated as 
      potentially throwing as well. 
      
      Another example, if a copy assignment operator calls a potentially throwing assignment operator, then the copy assignment will be 
      potentially throwing as well.

   Functions that are potentially throwing (if not implicitly-declared or defaulted):
      -- Normal functions
      -- User-defined constructors
      -- User-defined operators
   
   The noexcept operator
      -- The noexcept operator can also be used inside expressions. It takes an expression as an argument, and returns true or false 
         if the compiler thinks it will throw an exception or not. The noexcept operator is checked statically at compile-time, and 
         doesn’t actually evaluate the input expression.

         void foo() {throw -1;}
         void boo() {};
         void goo() noexcept {};
         struct S{};

         constexpr bool b1{ noexcept(5 + 3) }; // true; ints are non-throwing
         constexpr bool b2{ noexcept(foo()) }; // false; foo() throws an exception
         constexpr bool b3{ noexcept(boo()) }; // false; boo() is implicitly noexcept(false)
         constexpr bool b4{ noexcept(goo()) }; // true; goo() is explicitly noexcept(true)
         constexpr bool b5{ noexcept(S{}) };   // true; a struct's default constructor is noexcept by default

      The noexcept operator can be used to conditionally execute code depending on whether it is potentially throwing or not. 
      This is required to fulfill certain exception safety guarantees.

   Exception safety guarantees
   An exception safety guarantee is a contractual guideline about how functions or classes will behave in the event an exception occurs. 
   
   There are four levels of exception safety guarantees:
   No guarantee 
      -- There are no guarantees about what will happen if an exception is thrown (e.g. a class may be left in an unusable state)
   Basic guarantee 
      -- If an exception is thrown, no memory will be leaked and the object is still usable, but the program may be left in a modified state.
   Strong guarantee 
      -- If an exception is thrown, no memory will be leaked and the program state will not be changed. This means the function must either
         completely succeed or have no side effects if it fails. This is easy if the failure happens before anything is modified in the 
         first place, but can also be achieved by rolling back any changes so the program is returned to the pre-failure state.
   No throw / No fail guarantee 
      -- The function will always succeed (no-fail) or fail without throwing an exception that is exposed to the caller (no-throw). 
         Exceptions may be thrown internally if not exposed. The noexcept specifier maps to this level of exception safety guarantee.

   Let’s look at the no-throw/no-fail guarantees in more detail:
      The no-throw guarantee: 
         -- if a function fails, then it won’t throw an exception. Instead, it will return an error code or ignore the problem. 
            No-throw guarantees are required during stack unwinding when an exception is already being handled.
            For example, all destructors should have a no-throw guarantee (as should any functions those destructors call). 
   
         Examples of code that should be no-throw:
            -- destructors and memory deallocation/cleanup functions
            -- functions that higher-level no-throw functions need to call

      The no-fail guarantee: 
         -- A function will always succeed in what it tries to do (and thus never has a need to throw an exception, thus, no-fail is a 
            slightly stronger form of no-throw). 
            
         Examples of code that should be no-fail:
            -- move constructors and move assignment (move semantics)
            -- swap functions
            -- clear/erase/reset functions on containers
            -- operations on std::unique_ptr (also covered in chapter 22)
            -- functions that higher-level no-fail functions need to call   

   When to use noexcept
      -- Non-throwing functions can be safely called from functions that are not exception-safe, such as destructors
      -- Functions that are noexcept can enable the compiler to perform some optimizations that would not otherwise be available. 
         Because a noexcept function cannot throw an exception outside the function, the compiler doesn’t have to worry about keeping 
         the runtime stack in an unwindable state, which can allow it to produce faster code.
      -- There are significant cases where knowing a function is noexcept allows us to produce more efficient implementations in our own code:
         the standard library containers (such as std::vector) are noexcept aware and will use the noexcept operator to determine whether to 
         use move semantics (faster) or copy semantics (slower) in some places.
      -- The standard library’s policy is to use noexcept only on functions that must not throw or fail. 
         Functions that are potentially throwing but do not actually throw exceptions (due to implementation) typically are not marked 
         as noexcept.

      For your own code, always mark the following as noexcept:
         -- Move constructors
         -- Move assignment operators
         -- Swap functions
         
      For your code, consider marking the following as noexcept:
         -- Functions for which you want to express a no-throw or no-fail guarantee (e.g. to document that they can be safely called from 
            destructors or other noexcept functions)
         -- Copy constructors and copy assignment operators that are no-throw (to take advantage of optimizations).
         -- Destructors. Destructors are implicitly noexcept so long as all members have noexcept destructors

   NOTEs:
      -- Always make move constructors, move assignment, and swap functions noexcept.
      -- Make copy constructors and copy assignment operators noexcept when you can.
      -- Use noexcept on other functions to express a no-fail or no-throw guarantee.

   Best practice
      -- If you are uncertain whether a function should have a no-fail/no-throw guarantee, err on the side of caution and do not mark it 
         with noexcept. Reversing a decision to use noexcept violates an interface commitment to the user about the behavior of the 
         function, and may break existing code. Making guarantees stronger by later adding noexcept to a function that was not originally 
         noexcept is considered safe.

   Dynamic exception specifications
      -- Before C++11, and until C++17, dynamic exception specifications were used in place of noexcept. 
         The dynamic exception specifications syntax uses the throw keyword to list which exception types a function might directly or 
         indirectly throw:

         int doSomething() throw(); // does not throw exceptions
         int doSomething() throw(std::out_of_range, int*); // may throw either std::out_of_range or a pointer to an integer
         int doSomething() throw(...); // may throw anything

      -- Due to factors such as incomplete compiler implementations, some incompatibility with template functions, common misunderstandings
         about how they worked, and the fact that the standard library mostly didn’t use them, the dynamic exception specifications were 
         deprecated in C++11 and removed from the language in C++17 and C++20.

10 — std::move_if_noexcept
   The move constructors exception problem
      -- When copying an object, a failed copy doesn’t harm the source, so the strong exception guarantee is preserved.
      -- But during a move, ownership of resources is transferred; if an exception occurs after this transfer, the source object may be 
         left in a modified or invalid state. This is fine for temporaries but dangerous for non-temporaries, since restoring the source 
         may also fail.
      -- Thus, providing the strong exception guarantee for move constructors is hard: even if the move constructor itself avoids throwing,
         it may still call other potentially throwing constructors.

      -- std::move_if_noexcept will return a movable r-value if the object has a noexcept move constructor, otherwise it will return a 
         copyable l-value. We can use the noexcept specifier in conjunction with std::move_if_noexcept to use move semantics only when a 
         strong exception guarantee exists (and use copy semantics otherwise).
      
      -- If a type has both potentially throwing move semantics and deleted copy semantics (the copy constructor and copy assignment operator
         are unavailable), std::move_if_noexcept will give up the strong exception guarantee and still use move. This conditional giving-up of 
         the strong guarantee is very common in standard library container classes, because they use std::move_if_noexcept a lot.

