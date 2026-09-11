/**
 * @file 2_virtual_functions_default_arguments_trap.cpp
 * @brief Demonstrates the default arguments with virtual functions trap in C++.
 *
 * Concepts covered:
 * 1. Static binding of default arguments: Default argument values are determined
 *    at COMPILE TIME based on the static type of the pointer or reference.
 * 2. Dynamic binding of virtual functions: The function body is determined
 *    at RUNTIME via the vtable based on the dynamic type.
 * 3. The mismatch trap: Calling a virtual function with a default parameter via a Base pointer
 *    invokes Derived's function body using Base's default argument!
 * 4. Best practice: Never redefine an inherited default argument.
 */

#include <iostream>

class Base
{
public:
    virtual ~Base() = default;

    // Base provides default argument 10
    virtual void printValue(int x = 10)
    {
        std::cout << "Base::printValue(int) called with x = " << x << '\n';
    }
};

class Derived : public Base
{
public:
    // Anti-pattern: Redefining default argument to 20!
    void printValue(int x = 20) override
    {
        std::cout << "Derived::printValue(int) called with x = " << x << '\n';
    }
};

int main()
{
    Derived d;
    Base* basePtr = &d;

    std::cout << "--- 1. Calling via Derived object directly ---\n";
    // Static type is Derived, so Derived's default argument (20) is used:
    d.printValue(); // Prints: Derived::printValue with x = 20

    std::cout << "\n--- 2. Calling via Base pointer (The Trap!) ---\n";
    // Static type is Base*, so Base's default argument (10) is bound at compile time!
    // But dynamic dispatch invokes Derived::printValue at runtime!
    basePtr->printValue(); // Prints: Derived::printValue with x = 10!

    std::cout << "\nNotice: Derived::printValue was executed, but received Base's default argument (10)!\n";
    std::cout << "Lesson: Default arguments are statically bound, while virtual calls are dynamically dispatched.\n";

    return 0;
}
