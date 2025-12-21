/**
 * Understanding resize() vs reserve() for stack implementation
 * 
 * Key Concepts:
 * 1. resize(n): Changes size to n, creates n default-initialized elements
 * 2. reserve(n): Allocates capacity for n elements, size remains 0
 * 3. Direct initialization: vector(n) creates n elements (wrong for stack)
 * 4. Proper stack initialization: Start with empty vector
 * 
 * Common mistake:
 * - Using resize() or direct initialization creates unwanted default elements
 * - This makes the stack start with elements already in it
 * 
 * Correct approach:
 * - Start with empty vector {} or use reserve() to preallocate memory
 */

#include <iostream>
#include <vector>
#include <format>
#include <sstream>
#include <iomanip>

template <typename T>
void printStack(const std::vector<T>& ref)
{
    if(ref.empty())
        std::cout << "Empty";

    // Format output with fixed width for alignment
    std::ostringstream os{};
    for(auto i{0Z}; i < std::ssize(ref); i++)
        os << ref.data()[i] << " ";
    
    std::cout << std::format("{:<{}}", os.str(), 15);
    std::cout << "\tCapacity : " << ref.capacity() 
              << "\tLength : " << ref.size() << "\n";

}

int main()
{
    // PROBLEMATIC APPROACHES (commented out):
    
    // std::vector<int> stack(3);
    // Creates vector with 3 zero-initialized elements: [0, 0, 0]
    // Stack starts with unwanted elements!
    
    // stack.resize(3);
    // Same problem - adds 3 default elements to the stack
    
    // CORRECT APPROACH 1: Empty vector initialization
    std::vector<int> stack{};  // Size: 0, Capacity: 0 (grows as needed)
    
    // CORRECT APPROACH 2: Reserve capacity (commented out example)
    // stack.reserve(3);  // Size: 0, Capacity: 3 (preallocated, no reallocation needed)
    // This is optimal when you know the maximum stack size in advance

    printStack(stack);  // Output: Empty

    // Push operations work correctly
    stack.push_back(1);  // Stack: [1]
    printStack(stack);

    stack.push_back(2);  // Stack: [1, 2]
    printStack(stack);

    stack.push_back(3);  // Stack: [1, 2, 3]
    printStack(stack);

    std::cout << "Top : " << stack.back() << "\n";  // Top: 3

    // Pop operations
    stack.pop_back();  // Stack: [1, 2]
    printStack(stack);

    stack.pop_back();  // Stack: [1]
    printStack(stack);
    
    stack.pop_back();  // Stack: []
    printStack(stack);  // Output: Empty
    
    return 0;
}