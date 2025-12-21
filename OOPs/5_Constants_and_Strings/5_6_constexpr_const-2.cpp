/*
 * Demonstration: constexpr Functions
 * 
 * WHAT IS A constexpr FUNCTION?
 * - A function that CAN be evaluated at compile-time
 * - If given constant expressions as arguments, evaluates at compile-time
 * - Can still be called with runtime values (evaluates at runtime then)
 * 
 * REQUIREMENTS:
 * - All variables inside must be constexpr or literal types
 * - All function calls inside must be constexpr
 * - Return value must be evaluable at compile-time (when args are constexpr)
 * 
 * BENEFITS:
 * - Compile-time computation = zero runtime cost
 * - Can be used in contexts requiring constant expressions
 * - Same function works for both compile-time and runtime
 */

#include <iostream>

// constexpr function: Can execute at compile-time OR runtime
constexpr double calcCircumference(double radius)
{
    // All variables must be constexpr or compile-time evaluable
    constexpr double pi { 3.14159265359 };
    
    // Formula: C = 2πr
    // If 'radius' is a constexpr argument, entire calculation happens at compile-time
    return 2.0 * pi * radius;
}

int main()
{
    // Calling constexpr function with literal argument (compile-time constant)
    // This entire calculation happens at COMPILE-TIME
    // The compiled code will have the final value 18.8495... directly embedded
    constexpr double circumference { calcCircumference(3.0) };

    std::cout << "Our circle has circumference " << circumference << "\n";
    // No runtime calculation needed - value was computed during compilation!

    return 0;
}