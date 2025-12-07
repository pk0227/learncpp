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
       


