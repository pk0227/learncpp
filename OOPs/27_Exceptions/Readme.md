# Chapter 27: Exceptions

Error handling is a critical aspect of writing robust, maintainable, and production-grade C++ software. Traditional error handling mechanisms, such as return codes and global error flags, often convolute business logic, fail to handle errors in constructors, and place an excessive burden on callers to inspect and propagate errors manually.

**Exceptions** provide a language-level mechanism to decouple the detection and signaling of errors from their resolution. When an exceptional condition occurs, an exception is raised, unwinding the call stack, invoking destructors for all in-scope objects via RAII, and transferring control to an appropriate handler designed to resolve the failure.

---

### Table of Contents
1. [1 — The need for exceptions](#1--the-need-for-exceptions)
2. [2 — Basic exception handling](#2--basic-exception-handling)
3. [3 — Exceptions, functions, and stack unwinding](#3--exceptions-functions-and-stack-unwinding)
4. [4 — Uncaught exceptions and catch-all handlers](#4--uncaught-exceptions-and-catch-all-handlers)
5. [5 — Exceptions, classes, and inheritance](#5--exceptions-classes-and-inheritance)
6. [6 — Rethrowing exceptions](#6--rethrowing-exceptions)
7. [7 — Function try blocks](#7--function-try-blocks)
8. [8 — Exception dangers and downsides](#8--exception-dangers-and-downsides)
9. [9 — Exception specifications and noexcept](#9--exception-specifications-and-noexcept)
10. [10 — std::move_if_noexcept](#10--stdmove_if_noexcept)

---

## 1 — The need for exceptions

### When Return Codes Fail

One of the most common ways to handle potential errors in traditional procedural code is via **return codes**. A function returns a sentinel value (such as `-1`, `nullptr`, or `false`) to indicate an error has occurred.

However, relying strictly on return codes introduces a number of significant drawbacks:
1. **Cryptic return values**: If a function returns `-1`, is it trying to indicate an error, or is that actually a valid calculated result? It is often hard to tell without digging into the function's internal implementation or consulting documentation.
2. **Single-return limitation**: Functions can only return one value. What happens when you need to return both a computed function result and a possible error code?
   ```cpp
   double divide(int x, int y) {
       return static_cast<double>(x) / y; // Crashes if y is 0!
   }
   ```
   This function desperately needs error handling. However, it also needs to return the floating-point result of `x / y`. Returning an error code directly conflicts with returning the valid quotient.
3. **Pervasive, noisy checking**: In code sequences where many operations can fail (such as parsing a file or establishing a network connection), every single function call must be immediately followed by an `if` check. This obfuscates the primary "happy path" logic with repetitive error-checking boilerplate.
4. **Constructors have no return type**: What happens if you are creating an object and something inside the constructor goes catastrophically wrong (e.g. invalid arguments or resource allocation failure)? **Constructors cannot return a value**, so return codes cannot communicate failure during object initialization.
5. **Propagation burden**: When an error code is returned, the immediate caller may not be equipped to handle it. The caller must manually pass the error code up the call stack. If any intermediate function forgets to check or forward the error code, the error is silently swallowed and lost forever.

| Feature | Return Codes | Exceptions |
|---|---|---|
| **Separation of Concerns** | Error handling mixed with normal control flow | Error handling cleanly decoupled from business logic |
| **Constructor Support** | None (constructors have no return value) | Fully supported (aborts construction and unwinds stack) |
| **Propagation** | Manual check-and-return at every stack frame | Automatic propagation up the call stack |
| **Type Richness** | Limited to integers, enums, or booleans | Can carry arbitrary data, strings, and class hierarchies |
| **Ignorability** | Easily ignored accidentally | Cannot be ignored (unhandled exceptions abort the program) |

### Exceptions

**Exception handling** provides a structured mechanism to decouple the handling of errors and exceptional conditions from typical control flow. This allows developers the freedom to handle errors where it is most contextually appropriate, eliminating the messiness and fragility of return codes.

---

## 2 — Basic exception handling

Exceptions in C++ are implemented using three primary keywords that work in conjunction: **`throw`**, **`try`**, and **`catch`**.

### 1. Throwing Exceptions
A **`throw` statement** is used to signal that an exceptional error condition has occurred (also referred to as *raising an exception*).

To use a throw statement, write the `throw` keyword followed by any value or object representing the error:
```cpp
throw -1;                                              // Throw a literal int error code
throw ENUM_INVALID_INDEX;                              // Throw an enum value
throw "Cannot take square root of negative number";    // Throw a C-style string literal
throw dX;                                              // Throw a double variable
throw MyException{ "Fatal Error" };                    // Throw a custom exception object
```

### 2. Looking for Exceptions (`try` block)
In C++, the **`try` keyword** defines a block of statements (called a **try block**). The try block acts as an observer, monitoring any statements inside it for exceptions thrown directly or indirectly through nested function calls:

```cpp
try {
    // Code that might encounter an error and throw an exception
    double result = divide(10, 0);
    std::cout << "Result: " << result << '\n';
}
```

### 3. Handling Exceptions (`catch` block)
Handling exceptions is the responsibility of **`catch` blocks** (also known as *exception handlers*). The `catch` keyword defines a block of code tailored to handle an exception of a specific data type:

```cpp
catch (int errorCode) {
    std::cerr << "Caught integer error: " << errorCode << '\n';
}
catch (std::string_view errorMsg) {
    std::cerr << "Caught error message: " << errorMsg << '\n';
}
```

Key rules for catch blocks:
- A `try` block must be followed immediately by at least one `catch` block, and may be followed by multiple chained `catch` blocks.
- Once an exception is caught and routed to a matching handler, the exception is considered **handled**. Execution continues with the statement immediately following the last `catch` block in the chain.
- **Pass by value vs const reference**:
  - Fundamental types (`int`, `char`, `double`, enums) can be caught by value since they are cheap to copy.
  - Class types (`std::exception`, custom structs) **should always be caught by `const` reference** (`const MyException& e`) to avoid expensive copying and prevent **object slicing** of derived exception classes.
- If the catch parameter is not used inside the block, the variable name can be omitted (`catch (const std::exception&)`) to suppress unused-variable compiler warnings.

### Try and Catch Block Scope
Variables declared inside a `try` block are local to that `try` block's scope:
- They are automatically destroyed when the `try` block exits (either upon normal completion or when an exception unwinds the stack).
- They are **NOT accessible** inside subsequent `catch` blocks or after the `try/catch` construct.
- If a variable needs to be accessed both inside the `try` block and in or after the `catch` block, it must be declared *before* the `try` block:

```cpp
// INCORRECT:
try {
    int data{ computeData() };
}
catch (const std::exception& e) {
    // std::cout << data; // Compile error: 'data' is not declared in this scope!
}

// CORRECT:
int data{}; // Declared before try block
try {
    data = computeData();
}
catch (const std::exception& e) {
    std::cerr << "Computation failed; fallback data = " << data << '\n';
}
```

### No Implicit Type Conversions or Promotions

> [!WARNING]
> **C++ will NOT perform implicit type conversions or promotions when matching exceptions with catch blocks!**
> - A `char` exception will **not** match an `int` catch block.
> - An `int` exception will **not** match a `double` catch block.
> - A `float` exception will **not** match a `double` catch block.
> - A `const char*` exception will **not** match a `std::string` catch block.
>
> The **only** conversions allowed during exception matching are:
> 1. Binding a derived class object to a base class reference (`const Base&`).
> 2. Converting a non-const pointer to a `const` pointer (e.g. `Derived*` to `const Base*`).
> 3. Converting any pointer to `const void*`.

### What Catch Blocks Typically Do
When an exception reaches a matching catch block, it is considered handled even if the block body is empty. However, catch blocks typically perform one of four actions:
1. **Log and proceed**: Print an informative error message to the console or log file, perform compensatory actions, and allow execution to continue.
2. **Return an error code**: Convert the caught exception into an appropriate status indicator or fallback return value for the caller.
3. **Rethrow an exception**: Rethrow the current exception (or throw a different translated exception) up to the next outer enclosing try block.
4. **Clean termination**: In `main()`, catch fatal errors, output a diagnostic trace, and exit cleanly with a non-zero exit code.

### 📁 Code Examples for Section 2
- [`27_1_Basic_exception_handling/1_no_numeric_promotion.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_1_Basic_exception_handling/1_no_numeric_promotion.cpp): Demonstrates that numeric promotion is not applied during exception matching (a `short` exception bypasses an `int` catch block).
- [`27_1_Basic_exception_handling/2_no_numeric_conversion.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_1_Basic_exception_handling/2_no_numeric_conversion.cpp): Demonstrates that numeric conversions do not occur (a `double` exception does not match an `int` catch block).
- [`27_1_Basic_exception_handling/3_program_continues_after_handling_exception.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_1_Basic_exception_handling/3_program_continues_after_handling_exception.cpp): Demonstrates that once a catch block handles an exception, program execution resumes normally after the catch block chain.

---

## 3 — Exceptions, functions, and stack unwinding

### Throwing Exceptions from Called Functions
Try blocks observe not only the statements placed directly within their scope, but also **any functions called from within the try block** (and any functions called by those functions, transitively down the call stack).

### The Stack Unwinding Process
When an exception is thrown, C++ searches for an appropriate handler using **stack unwinding**:
1. The program first inspects whether the current function has an enclosing `try` block with a matching `catch` handler.
2. If no matching handler exists in the current function, the current function frame is popped off the call stack.
3. As the stack frame is popped, **all local variables constructed within that function are destroyed in reverse order of construction**. Their destructors execute cleanly, releasing their acquired resources (RAII).
4. The caller function is then checked to see if the call was wrapped in a `try` block with a matching handler.
5. This process continues up the call chain until a matching handler is located.
6. Once a handler is found, execution jumps directly to the matching `catch` block.

```
Call Stack during Unwinding:
┌─────────────────────────┐
│ functionC()             │ ──> Throws exception! No try block here.
├─────────────────────────┤     [Unwinds: destroys functionC locals]
│ functionB()             │ ──> Called functionC. No matching catch here.
├─────────────────────────┤     [Unwinds: destroys functionB locals]
│ functionA()             │ ──> Wrapped in try block!
│   try { functionB(); }  │
│   catch (const Error&)  │ ──> MATCH FOUND! Execution jumps here!
└─────────────────────────┘
```

> [!TIP]
> Stack unwinding ensures that if a function does not know how to handle an error, it does not have to. The exception automatically propagates up the call hierarchy until it reaches a layer of the system that possesses the contextual information needed to resolve it.

### 📁 Code Examples for Section 3
- [`27_2_Exceptions_functions_n_stack_unwinding/1_throwing_exception_from_called_function.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_2_Exceptions_functions_n_stack_unwinding/1_throwing_exception_from_called_function.cpp): Demonstrates an exception thrown inside a deeply nested function propagating back to a caller's `try`/`catch` block.
- [`27_2_Exceptions_functions_n_stack_unwinding/2_stack_unwinding.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_2_Exceptions_functions_n_stack_unwinding/2_stack_unwinding.cpp): Demonstrates that local objects in intermediate functions have their destructors properly executed as the stack unwinds during an exception.

---

## 4 — Uncaught exceptions and catch-all handlers

### Uncaught Exceptions
If an exception is thrown and no matching `catch` handler is found anywhere on the call stack before reaching the end of `main()`, the exception is an **uncaught exception**:
- The program terminates immediately via a call to **`std::terminate()`**.
- **The call stack may or may not be unwound!** The C++ standard explicitly leaves it implementation-defined whether the compiler unwinds the stack when an unhandled exception occurs.
- If the stack is *not* unwound, local destructors will not execute. However, preserving the stack in its exact error state is critical for **post-mortem debugging** and core dump analysis.

### Catch-All Handlers (`catch (...)`)
Because any function can throw an arbitrary type, catching every individual exception type explicitly is tedious. C++ provides a **catch-all handler** using the ellipsis operator (`...`):

```cpp
try {
    runApplication();
}
catch (const std::exception& e) {
    std::cerr << "Standard exception: " << e.what() << '\n';
}
catch (...) { // Catch-all handler
    std::cerr << "An unknown, unexpected exception occurred!\n";
}
```

Key rules for catch-all handlers:
- A catch-all handler **must be placed last** in the catch block sequence. Any catch blocks placed after `catch (...)` will trigger a compiler error because they are unreachable.
- The catch-all handler does not provide access to the exception object itself (its type and value are unknown).
- When a catch-all handler catches an exception, the stack is guaranteed to have unwound up to that `try` block.
- In `main()`, a catch-all handler prevents abrupt process termination, allowing emergency cleanup (such as flushing logs or autosaving data) before exiting.

### Debugging Unhandled Exceptions

> [!NOTE]
> Because a catch-all handler catches everything and triggers stack unwinding, it destroys the local variables and call frames leading up to the error, making post-mortem debugging difficult.
> In debug builds, you can conditionally disable the catch-all handler using preprocessor directives:
> ```cpp
> #ifndef NDEBUG
> catch (...) {
>     std::cerr << "Production catch-all: terminating safely.\n";
>     return 1;
> }
> #endif
> ```

### Custom Termination Handlers (`std::set_terminate`)
When an exception is thrown and cannot find a matching catch handler, the C++ runtime automatically invokes **`std::terminate()`**, which by default calls `std::abort()`.
You can customize termination behavior using **`std::set_terminate()`** (defined in `<exception>`):

```cpp
#include <exception>
#include <iostream>
#include <cstdlib>

void myTerminateHandler() {
    std::cerr << "Fatal unhandled exception encountered! Writing crash report...\n";
    std::abort(); // Handler must terminate execution
}

int main() {
    std::set_terminate(myTerminateHandler);
    // ...
}
```

### Checking In-Flight Exceptions (`std::uncaught_exceptions`)
C++17 introduced **`int std::uncaught_exceptions() noexcept`** (in `<exception>`):
- Returns the count of currently active exceptions undergoing stack unwinding.
- Allows RAII classes and scope guards to safely determine if their destructor is executing during normal stack cleanup or due to an in-flight exception.

> [!NOTE]
> C++98 originally provided `bool std::uncaught_exception()` (singular), but it was flawed because it could not distinguish between one active exception and a nested exception during stack unwinding. Consequently, it was deprecated in C++17 and completely removed in C++20 in favor of `std::uncaught_exceptions()` (plural).

### 📁 Code Examples for Section 4
- [`27_3_Uncaught_exceptions_n_catch-all_handlers/1_debugging_unhandled_exceptions.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_3_Uncaught_exceptions_n_catch-all_handlers/1_debugging_unhandled_exceptions.cpp): Demonstrates using `catch (...)` to intercept unknown exception types and using `#ifndef NDEBUG` to preserve stack traces during development.

---

## 5 — Exceptions, classes, and inheritance

### When Constructors Fail

Constructors cannot return error codes. If constructor arguments are invalid or resource allocation fails, throwing an exception is the standard, idiomatically correct way to abort construction:

```cpp
class Fraction {
    int m_numerator{ 0 };
    int m_denominator{ 1 };
public:
    Fraction(int num, int den) : m_numerator{ num }, m_denominator{ den } {
        if (den == 0)
            throw std::runtime_error{ "Denominator cannot be zero!" };
    }
};
```

> [!WARNING]
> **If a constructor throws an exception, the object's own destructor is NEVER called!**
> - An object is only considered complete once its constructor body finishes executing.
> - If an exception throws from within the constructor, the object was never fully constructed.
> - Therefore, C++ will **not** invoke the class's destructor.
> - However, any member variables or base class subobjects that were **already fully constructed prior to the exception will be destructed normally**.

### Resource Management in Constructors (RAII)
Because the class destructor will not execute if a constructor throws halfway through, allocating raw memory or resources inside the constructor body risks leaks:
- **Flawed approach**: Allocating with raw `new` in the constructor body. If a subsequent statement throws, `delete` in the destructor will never run!
- **RAII solution**: Store resources in **member objects that implement RAII** (e.g. `std::unique_ptr`, `std::vector`, `std::string`, `std::fstream`). Even if the constructor throws, those member objects are fully constructed and will have their destructors called automatically, preventing any resource leakage.

### Exception Classes
Throwing primitive types (like `int` or `const char*`) is inherently vague and error-prone. Defining **custom exception classes** allows rich diagnostic state to be bundled with the error:

```cpp
class ArrayException {
    std::string m_error{};
public:
    ArrayException(std::string error) : m_error{ std::move(error) } {}
    const std::string& getError() const { return m_error; }
};
```

### Exceptions and Inheritance
When throwing classes within an inheritance hierarchy:
- A `catch` block for a base class will catch exceptions of that base class **and all classes publicly derived from it**.
- **Derived catch blocks MUST precede base catch blocks!**
  If a base catch block is placed before a derived catch block, the base handler will intercept the derived exception first (via polymorphism), rendering the derived handler completely unreachable:

```cpp
class BaseException {};
class DerivedException : public BaseException {};

try {
    throw DerivedException{};
}
catch (const DerivedException& e) { // MUST be listed FIRST!
    std::cerr << "Caught DerivedException\n";
}
catch (const BaseException& e) {    // Base handler catches all remaining base/derived exceptions
    std::cerr << "Caught BaseException\n";
}
```

### The `std::exception` Hierarchy
The C++ standard library provides a unified hierarchy of exception classes rooted at **`std::exception`** (defined in `<exception>`):
- All exceptions thrown by standard library components (e.g. `std::bad_alloc`, `std::out_of_range`, `std::bad_cast`) derive from `std::exception`.
- By catching `const std::exception&`, a single catch block can intercept any standard library exception.
- `std::exception` defines a virtual member function:
  ```cpp
  virtual const char* what() const noexcept;
  ```
  This returns an explanatory C-style string describing the error.

> [!NOTE]
> The text returned by `what()` is intended for human-readable diagnostic output only. Do not rely on its exact string contents for programmatic logic or comparisons, as the output is implementation-defined and differs between compilers.

```
                           std::exception
                            /          \
              std::logic_error        std::runtime_error
              /      |       \         /       |        \
    invalid_argument | out_of_range range_error| overflow_error
               length_error               system_error
```

### The Two Main Branches: `std::logic_error` vs `std::runtime_error` (in `<stdexcept>`)
The standard library divides most exceptions into two major categories derived from `std::exception`:

| Branch | Meaning & Design Intent | Common Derived Standard Classes |
|---|---|---|
| **`std::logic_error`** | Represents errors resulting from faulty logic within the program itself. These are defects in code that could in theory be detected and prevented before execution by careful programming and invariant checking. | `std::invalid_argument`, `std::out_of_range`, `std::length_error`, `std::domain_error` |
| **`std::runtime_error`** | Represents errors due to runtime events outside the scope of the program and not easily avoidable by static code analysis (e.g. hardware limits, OS resource failures, environment conditions). | `std::range_error`, `std::overflow_error`, `std::underflow_error`, `std::system_error` |

### Using the Standard Exceptions Directly
- **Never throw `std::exception` directly**: It serves as a generic polymorphic interface, not a concrete error descriptor.
- **Throw derived standard exceptions**: You should freely throw standard exception classes like `std::runtime_error` or `std::invalid_argument` if they adequately represent your needs:
  ```cpp
  #include <stdexcept>
  #include <cmath>

  double computeSquareRoot(double val) {
      if (val < 0.0)
          throw std::invalid_argument{ "computeSquareRoot: negative argument provided" };
      return std::sqrt(val);
  }
  ```
- `std::runtime_error` is the most popular choice because it accepts a custom descriptive message (`std::string` or `const char*`) and handles internal string storage automatically.

### Deriving Custom Classes from `std::exception` or `std::runtime_error`
- **Deriving from `std::exception`**: You must manually store the error message and override `const char* what() const noexcept override`.
- **Deriving from `std::runtime_error` (Recommended)**: `std::runtime_error` (from `<stdexcept>`) already manages string storage and implements `what()`. Your custom class simply forwards the error string to `std::runtime_error`'s constructor:

```cpp
#include <stdexcept>

class CustomException : public std::runtime_error {
public:
    CustomException(const std::string& msg)
        : std::runtime_error{ msg } {}
};
```

### The Lifetime of Exception Objects
- When an exception is thrown, the thrown object is copied into a dedicated **exception storage area** managed by the runtime (outside of the call stack).
- This ensures the exception object survives even as stack frames are destroyed during stack unwinding.
- **Exception objects must be copyable** (or movable in C++17+).
- **Never store pointers or references to local stack-allocated variables** inside an exception object, as those objects will be destroyed during unwinding, leaving dangling pointers.

### 📁 Code Examples for Section 5
- [`27_4_Exceptions_classes_n_inheritance/1_when_constructor_fails.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_4_Exceptions_classes_n_inheritance/1_when_constructor_fails.cpp): Demonstrates throwing an exception from a constructor and illustrates that member destructors run while the class's own destructor does not.
- [`27_4_Exceptions_classes_n_inheritance/2_custom_exception_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_4_Exceptions_classes_n_inheritance/2_custom_exception_class.cpp): Demonstrates creating a custom `ArrayException` class to report bounds-checking errors in a container.
- [`27_4_Exceptions_classes_n_inheritance/3_exceptions_inheritance.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_4_Exceptions_classes_n_inheritance/3_exceptions_inheritance.cpp): Demonstrates the anti-pattern of placing a base exception catch block before a derived catch block.
- [`27_4_Exceptions_classes_n_inheritance/4_exceptions_inheritance.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_4_Exceptions_classes_n_inheritance/4_exceptions_inheritance.cpp): Demonstrates the correct order: placing derived exception catch blocks before base catch blocks.
- [`27_4_Exceptions_classes_n_inheritance/5_std_exception.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_4_Exceptions_classes_n_inheritance/5_std_exception.cpp): Demonstrates catching standard exceptions polymorphically using `const std::exception&`.
- [`27_4_Exceptions_classes_n_inheritance/5_std_exception_improved.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_4_Exceptions_classes_n_inheritance/5_std_exception_improved.cpp): Shows catching standard exceptions with detailed diagnostic output via `what()`.
- [`27_4_Exceptions_classes_n_inheritance/6_specific_std_exception.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_4_Exceptions_classes_n_inheritance/6_specific_std_exception.cpp): Demonstrates catching specific derived standard exceptions (`std::out_of_range`, `std::length_error`).
- [`27_4_Exceptions_classes_n_inheritance/7_standard_exception_usage.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_4_Exceptions_classes_n_inheritance/7_standard_exception_usage.cpp): Demonstrates throwing and catching `std::runtime_error`.
- [`27_4_Exceptions_classes_n_inheritance/8_deriving_own_exception_class_from_std_exception.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_4_Exceptions_classes_n_inheritance/8_deriving_own_exception_class_from_std_exception.cpp): Demonstrates deriving custom exception classes directly from `std::exception` and overriding `what() noexcept`.
- [`27_4_Exceptions_classes_n_inheritance/9_deriving_own_exception_class_from_std_exception.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_4_Exceptions_classes_n_inheritance/9_deriving_own_exception_class_from_std_exception.cpp): Demonstrates the cleaner approach of deriving from `std::runtime_error` to inherit string storage and `what()`.
- [`27_4_Exceptions_classes_n_inheritance/10_exception_object_must_be_copyable.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_4_Exceptions_classes_n_inheritance/10_exception_object_must_be_copyable.cpp): Explores exception copyability requirements, temporary rvalue elision in C++17, and throwing lvalues.

---

## 6 — Rethrowing exceptions

Occasionally a catch block needs to perform partial handling of an exception (such as logging an audit entry or freeing local temporary buffers) and then pass the exception along to an outer handler.

### Throwing a New Exception
A catch block can throw a completely new exception:
```cpp
catch (const DatabaseException& e) {
    logger.log(e.what());
    throw ServiceUnavailableException{ "Database failure" }; // Throws new exception type
}
```
The newly thrown exception propagates up to the outer caller's enclosing try block.

### Rethrowing an Exception: The Wrong Way (Slicing)

```cpp
catch (const Base& e) {
    // WRONG: Slices derived exceptions into a Base object!
    throw e;
}
```
If a `Derived` exception was thrown, `throw e;` creates a copy of `e`. Because `e` is statically typed as `Base`, **object slicing occurs**, throwing a sliced `Base` exception and discarding all `Derived` data!

### Rethrowing an Exception: The Right Way (`throw;`)

To rethrow the exact same active exception without copying or slicing, use the **`throw;` keyword by itself**:

```cpp
catch (const Base& e) {
    std::cerr << "Logging error before rethrowing...\n";
    
    // RIGHT: Rethrows the exact original dynamic exception object!
    throw;
}
```

Benefits of `throw;`:
- **Preserves dynamic type**: If a `Derived` exception was caught by a `Base&`, `throw;` rethrows the original `Derived` instance.
- **Zero copy overhead**: No temporary exception copy is created.
- **No slicing**: Complete integrity of derived state and virtual functions is maintained.

### Transporting Exceptions with `std::exception_ptr` (since C++11)
In modern C++, exceptions can be captured and deferred or transferred across thread boundaries using **`std::exception_ptr`** (defined in `<exception>`):
- **`std::current_exception()`**: Called within a `catch` block to capture a shared pointer (`std::exception_ptr`) to the currently active exception.
- **`std::rethrow_exception(ptr)`**: Takes a `std::exception_ptr` and rethrows the stored exception in a different stack frame, thread, or context.

```cpp
#include <exception>
#include <iostream>

std::exception_ptr g_capturedException{ nullptr };

void workerThreadTask() {
    try {
        throw std::runtime_error{ "Worker thread computation failure" };
    }
    catch (...) {
        // Capture active exception to transport across thread boundary
        g_capturedException = std::current_exception();
    }
}

void mainThreadHandler() {
    if (g_capturedException) {
        try {
            // Rethrow captured exception in main thread
            std::rethrow_exception(g_capturedException);
        }
        catch (const std::exception& e) {
            std::cerr << "Main thread caught transported exception: " << e.what() << '\n';
        }
    }
}
```

This mechanism powers standard asynchronous concurrency tools like `std::future::get()`, allowing exceptions thrown inside worker tasks to be transparently caught and rethrown in the consumer thread.

### 📁 Code Examples for Section 6
- [`27_5_Rethrowing_exceptions/1_Rethrowing_exception_object_slicing_issue.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_5_Rethrowing_exceptions/1_Rethrowing_exception_object_slicing_issue.cpp): Demonstrates the object slicing bug caused by `throw e;` inside a catch block.
- [`27_5_Rethrowing_exceptions/2_Rethrowing_exception_object_slicing_solution.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_5_Rethrowing_exceptions/2_Rethrowing_exception_object_slicing_solution.cpp): Demonstrates resolving the slicing bug using `throw;` to rethrow the original polymorphic exception object.

---

## 7 — Function try blocks

Normally, try blocks are placed inside a function body. However, if a base class constructor or a member variable initialized in the **member initializer list** throws an exception, a standard try block inside the constructor body cannot catch it because the initializer list executes *before* the constructor body begins.

To catch exceptions thrown from member initializer lists, C++ provides **function try blocks**:

```cpp
class Derived : public Base {
public:
    Derived(int x) try : Base{ x } // Function try block encloses initializer list!
    {
        // Constructor body
    }
    catch (const std::exception& e) {
        std::cerr << "Caught exception from Base initializer: " << e.what() << '\n';
        // Exception is IMPLICITLY RETHROWN at the end of this catch block!
    }
};
```

### Limitations on Function Catch Blocks

Function catch blocks have strict, idiosyncratic behavior depending on the type of function they wrap:

| Function Type | Can Resolve Exception via `return`? | Behavior at End of Catch Block |
|---|---|---|
| **Constructor** | **No** (must throw or rethrow) | **Implicit `throw;`** |
| **Destructor** | **Yes** (`return;` allowed) | **Implicit `throw;`** |
| **Non-value returning (`void`) function** | **Yes** (`return;` allowed) | **Resolves exception** (returns normally) |
| **Value-returning function** | **Yes** (`return value;` allowed) | **Undefined Behavior (UB)!** |

> [!CAUTION]
> Reaching the end of a function-level catch block in a **value-returning function results in undefined behavior (UB)**! Always explicitly `return` a value, `throw`, or `rethrow` before reaching the closing brace.

### Don't Use Function Try to Clean Up Class Members
- If a constructor fails, the object is considered unconstructed and its destructor is never called.
- By the time the function catch block executes, **all subobjects and members have already been destroyed**.
- Attempting to access or delete member variables inside a constructor function catch block results in **undefined behavior**.
- Therefore, function try blocks are primarily useful for **logging errors** or **translating exception types**, not for resource cleanup. Use RAII members for cleanup instead.

### 📁 Code Examples for Section 7
- [`27_6_Function_try_blocks/1_function_try_catch_block.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_6_Function_try_blocks/1_function_try_catch_block.cpp): Basic demonstration of function try blocks intercepting exceptions in constructor member initializer lists.
- [`27_6_Function_try_blocks/2_function_try_catch_block.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_6_Function_try_blocks/2_function_try_catch_block.cpp): Demonstrates catching base class initialization exceptions via function try blocks.
- [`27_6_Function_try_blocks/3_function_try_catch_block.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_6_Function_try_blocks/3_function_try_catch_block.cpp): Demonstrates implicit rethrowing behavior at the end of a constructor function-level catch block.

---

## 8 — Exception dangers and downsides

### 1. Cleaning Up Resources (RAII)
If raw dynamic allocations (`new`) or raw file handles (`fopen`) are used, an unexpected exception can skip the corresponding cleanup statements:
```cpp
void badFunction() {
    int* ptr = new int[100];
    doRiskyOperation(); // If this throws, delete[] is skipped -> MEMORY LEAK!
    delete[] ptr;
}
```
**The Remedy**: Always bind resources to stack-allocated **RAII wrappers** (`std::unique_ptr`, `std::vector`, `std::fstream`, `std::lock_guard`). When an exception unwinds the stack, RAII destructors are guaranteed to execute.

### 2. Exceptions and Destructors

> [!WARNING]
> **Exceptions must NEVER be allowed to escape from a destructor!**
> 
> Two distinct language mechanics strictly enforce this rule:
> 1. **Stack Unwinding Collision**: If an exception is currently in-flight during stack unwinding and another exception escapes from a destructor being cleaned up, two simultaneous active exceptions exist. The C++ runtime cannot handle multiple active exceptions and immediately calls **`std::terminate()`**, aborting the entire program.
> 2. **Implicit `noexcept(true)` Destructors (since C++11)**: All destructors are implicitly marked `noexcept(true)` by default. Therefore, even when stack unwinding is *not* in progress, allowing an unhandled exception to exit a destructor violates its `noexcept` specification, triggering an immediate call to **`std::terminate()`**!
> 
> **Best Practice**: Never allow exceptions to escape from a destructor. If an operation inside a destructor might throw, catch it internally with a local `try/catch` block, write an error message to a log file, and ensure no exception exits the destructor.

### 3. Performance Concerns
1. **Executable Size**: Exception handling tables increase binary size by roughly 10–20%.
2. **Execution Cost on Throw**: Throwing an exception is computationally expensive because it involves searching lookup tables, allocating exception storage, and unwinding call frames.
3. **Zero-Cost Exception Model**: Most modern 64-bit C++ compilers (GCC, Clang, MSVC) use table-driven zero-cost exceptions. In this model, the "happy path" incurs zero CPU overhead when no exception is thrown; the performance penalty is deferred entirely to when an exception is actually thrown.

### When Should You Use Exceptions?
Exceptions should be reserved for truly exceptional conditions. Use exceptions when all of the following criteria are met:
- The error occurs **infrequently**.
- The error is **severe** enough that execution cannot continue normally.
- The error **cannot be handled locally** at the point of failure.
- There is **no clean alternative** (such as constructors failing, or operations where all return values are valid domain results).

### 📁 Code Examples for Section 8
- [`27_7_Exception_dangers_n_downsides/1_exception_dangers_downsides.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_7_Exception_dangers_n_downsides/1_exception_dangers_downsides.cpp): Demonstrates resource leaks caused by raw pointers during exceptions and how RAII smart pointers resolve the issue.

---

## 9 — Exception specifications and noexcept

### The `noexcept` Specifier
In C++, every function is classified as either **non-throwing** or **potentially throwing**:
- A **non-throwing function** promises not to allow exceptions to escape to its caller.
- To declare a function as non-throwing, append the **`noexcept` specifier**:
  ```cpp
  void doSomething() noexcept; // Guaranteed non-throwing
  ```

> [!WARNING]
> If an unhandled exception escapes from a function marked `noexcept`, **`std::terminate()` is invoked immediately**, even if an outer catch handler exists up the call stack.

Key rules for `noexcept`:
- Functions cannot be overloaded based solely on their `noexcept` specification.
- A `noexcept` function *can* internally throw exceptions or call potentially throwing functions, provided it catches and resolves them internally before returning.
- **Parameterized `noexcept`**: The specifier accepts an optional compile-time boolean expression:
  ```cpp
  template <typename T>
  void swap(T& a, T& b) noexcept(std::is_nothrow_move_constructible_v<T>);
  ```
  `noexcept(true)` means non-throwing; `noexcept(false)` means potentially throwing.

### Which Functions Are Implicitly Non-Throwing?
- **Destructors** are non-throwing by default.
- Defaulted special member functions (default constructor, copy constructor, move constructor, copy assignment, move assignment, and comparison operators) are implicitly non-throwing **unless** any member or base subobject has a potentially throwing version.

### The `noexcept` Operator
The **`noexcept` operator** is a compile-time operator that evaluates whether an expression is non-throwing:
```cpp
void foo() { throw 1; }
void bar() noexcept {}

constexpr bool b1 = noexcept(5 + 3);  // true: primitive arithmetic is non-throwing
constexpr bool b2 = noexcept(foo());  // false: foo() is potentially throwing
constexpr bool b3 = noexcept(bar());  // true: bar() is marked noexcept
```

### The Four Exception Safety Guarantees

When designing classes and functions, C++ establishes four standardized contractual guarantees:

| Level | Guarantee | Description |
|---|---|---|
| **1. No Guarantee** | None | If an exception occurs, state is indeterminate; memory or resource leaks may occur. |
| **2. Basic Guarantee** | Invariants preserved | No memory is leaked, objects remain in valid (destructible) states, but values may be modified. |
| **3. Strong Guarantee** | Commit-or-rollback | If an operation fails, state is rolled back to exactly what it was prior to the call ("all-or-nothing"). |
| **4. No-Throw / No-Fail** | Always succeeds / Never throws | The function is guaranteed to complete successfully without throwing (`noexcept`). |

### When to Mark Functions with `noexcept`
Always mark the following as `noexcept`:
1. **Move constructors** and **move assignment operators**.
2. **Swap functions**.
3. **Destructors and memory deallocation functions**.

> [!TIP]
> Standard library containers like `std::vector` inspect `noexcept` when resizing. If your class's move constructor is not marked `noexcept`, `std::vector` will fall back to **expensive copying** during reallocation to preserve the Strong Exception Guarantee!

### `noexcept` in the Type System (since C++17)
Since C++17, `noexcept` is formally part of a function's type signature:
- A pointer to a non-throwing function (`void (*)() noexcept`) cannot be assigned the address of a potentially throwing function.
- A pointer to a potentially throwing function (`void (*)()`) can legally point to a non-throwing function (safe implicit subtype conversion):

```cpp
void nonThrowing() noexcept {}
void potentiallyThrowing() {}

void (*p1)() noexcept = nonThrowing;         // OK
// void (*p2)() noexcept = potentiallyThrowing; // COMPILE ERROR: cannot convert throwing to noexcept pointer

void (*p3)() = nonThrowing;                  // OK: implicit conversion from non-throwing to potentially throwing pointer
```

### 📁 Code Examples for Section 9
- [`27_8_Exception_specifications_n_noexcept/1_noexcept_example.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_8_Exception_specifications_n_noexcept/1_noexcept_example.cpp): Demonstrates the `noexcept` specifier, compile-time evaluation via the `noexcept()` operator, and the termination behavior when an exception exits a `noexcept` function.

---

## 10 — std::move_if_noexcept

### The Move Constructor Exception Problem
When a container like `std::vector` grows and reallocates memory, it must transfer elements from the old buffer to the new buffer:
- If elements are **copied**, a failure during copying does not modify the source objects. The original vector remains intact, preserving the **Strong Exception Guarantee**.
- If elements are **moved**, moving transfers ownership and modifies the source object. If the 5th element's move constructor throws an exception:
  - The first 4 elements are already hollowed out in the old buffer.
  - The new buffer is incomplete.
  - The vector cannot roll back to its original state, causing **data loss and violating the Strong Exception Guarantee**!

### How `std::move_if_noexcept` Solves the Problem
To maintain exception safety, C++ provides **`std::move_if_noexcept`** (defined in `<utility>`):
```cpp
// Returns an rvalue (std::move) if T's move constructor is noexcept;
// otherwise returns a const lvalue reference (forces copying).
auto item = std::move_if_noexcept(source);
```

Behavior rules:
1. If the type's move constructor is marked `noexcept` &rarr; `std::move_if_noexcept` produces an **rvalue reference** (executes fast move).
2. If the type's move constructor is potentially throwing &rarr; `std::move_if_noexcept` produces an **lvalue reference** (falls back to safe copy).
3. If the type is **move-only** (copy constructor deleted) &rarr; it still moves, even if potentially throwing, because no copy alternative exists.

```cpp
#include <utility>
#include <vector>

class Person {
public:
    // Marking noexcept enables std::vector reallocation to use move instead of copy!
    Person(Person&& other) noexcept { /* ... */ }
};
```

### 📁 Code Examples for Section 10
- [`27_9_std_move_if_noexcept/1_std_move_if_noexcept.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/27_Exceptions/27_9_std_move_if_noexcept/1_std_move_if_noexcept.cpp): Demonstrates the difference between unconditional `std::move` and exception-safe conditional `std::move_if_noexcept` when an exception is encountered during object transfer.
