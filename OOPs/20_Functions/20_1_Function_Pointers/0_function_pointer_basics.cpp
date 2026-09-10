/**
 * @file 0_function_pointer_basics.cpp
 * @brief Demonstrates basic function pointer syntax, assignment, invocation, and default arguments
 * 
 * Key Concepts:
 * 1. Function pointer declaration syntax: ReturnType (*ptrName)(ParamTypes).
 * 2. Functions implicitly convert to function pointers (address-of operator & is optional).
 * 3. Calling via explicit dereference (*fcnPtr)(args) vs implicit dereference fcnPtr(args).
 * 4. Function pointers can be initialized or assigned nullptr.
 * 5. Default arguments do NOT work through function pointers because default arguments are 
 *    resolved at compile-time, whereas function pointers use runtime late binding.
 */

#include <iostream>

int add(int x, int y)
{
    return x + y;
}

int subtract(int x, int y)
{
    return x - y;
}

void greet(int times, const char* message = "Hello")
{
    for (int i{ 0 }; i < times; ++i)
    {
        std::cout << message << ' ';
    }
    std::cout << '\n';
}

int main()
{
    // 1. Defining and assigning a function pointer
    int (*fcnPtr)(int, int){ &add }; // Points to add
    std::cout << "Calling add via fcnPtr (implicit dereference): " << fcnPtr(5, 3) << '\n';
    std::cout << "Calling add via fcnPtr (explicit dereference): " << (*fcnPtr)(5, 3) << '\n';

    // 2. Reassigning to another function with matching signature
    fcnPtr = subtract; // Implicit conversion from function to function pointer (& is optional)
    std::cout << "Calling subtract via fcnPtr: " << fcnPtr(5, 3) << '\n';

    // 3. Nullptr safety
    int (*nullPtr)(int, int){ nullptr };
    if (nullPtr)
    {
        nullPtr(1, 2);
    }
    else
    {
        std::cout << "nullPtr is currently null, skipping call.\n";
    }

    // 4. Default arguments limitation:
    // Normal call resolves default argument at compile-time:
    greet(2); // Prints "Hello Hello "

    // Call via function pointer requires ALL arguments explicitly because
    // the call is resolved at runtime (late binding):
    void (*greetPtr)(int, const char*){ greet };
    // greetPtr(2); // COMPILE ERROR: too few arguments for call!
    greetPtr(2, "Welcome"); // Works when explicitly supplied

    return 0;
}
