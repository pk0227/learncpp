/*
 * Demonstration: Move Semantics and RVO (Return Value Optimization)
 * 
 * KEY CONCEPTS:
 * 1. RVO (Return Value Optimization): Compiler optimization that eliminates copies
 * 2. Move Semantics: Transfer ownership of resources instead of copying
 * 3. Modern C++ optimizes string returns automatically
 * 
 * WHAT HAPPENS:
 * - Old C++: Returning string would copy (expensive!)
 * - Modern C++: Either RVO eliminates the copy, or move semantics transfers ownership
 * - Result: Efficient, no performance penalty for returning strings
 * 
 * ADDRESS COMPARISON:
 * - If addresses differ: Move semantics occurred (ownership transferred)
 * - If addresses same: RVO occurred (no object created in function)
 */

#include <iostream>
#include <string>

// Function returns std::string by value
// Might seem expensive, but modern C++ optimizes this
std::string getName() {
    std::string name{};
    std::cout << "Enter Name : ";
    std::cin >> name;
    
    // Print address of local variable 'name'
    std::cout << "addr of name : " << &name << std::endl;
    
    // Returning by value:
    // Option 1: RVO - Compiler constructs directly in caller's location (same address)
    // Option 2: Move - Resources transferred (different address, but no deep copy)
    return name;  // Efficient due to RVO or move semantics
}

int main() {
    // Direct initialization from function return value
    // This is the OPTIMAL way - enables RVO (Return Value Optimization)
    // Compiler may construct the string directly in u_name's location
    std::string u_name{getName()};
    
    // LESS OPTIMAL (commented out):
    // This creates empty string first, then assigns
    // Assignment can still use move semantics, but less efficient than direct init
    //std::string u_name{};
    //u_name = getName();
    
    // Print address of u_name
    std::cout << "Your name is " << u_name 
              << "\naddr of u_name : " << &u_name << std::endl;
    
    // COMPARE ADDRESSES:
    // Same address as 'name'? → RVO occurred (optimal!)
    // Different address? → Move semantics occurred (still efficient!)
    
    return 0;
}
