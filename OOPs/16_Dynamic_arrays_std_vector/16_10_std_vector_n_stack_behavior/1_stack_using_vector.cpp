/**
 * Implementing stack behavior using std::vector
 * 
 * Key Concepts:
 * 1. Stack data structure: Last-In-First-Out (LIFO)
 * 2. push_back(): Add element to top of stack (end of vector)
 * 3. pop_back(): Remove element from top of stack (end of vector)
 * 4. back(): Access top element without removing it
 * 5. empty(): Check if stack is empty
 * 
 * Stack operations:
 * - Push: Add element to the top
 * - Pop: Remove element from the top
 * - Top/Peek: View top element without removing
 */

#include <iostream>
#include <vector>

// Helper function to print current stack state
template <typename T>
void printStack(const std::vector<T>& ref)
{
    // Check if stack is empty
    if(ref.empty())
        std::cout << "Empty";

    // Print all elements from bottom to top
    for(auto i{0Z}; i < std::ssize(ref); i++)
        std::cout << ref.data()[i] << " ";
    
    // Display capacity and size information
    std::cout << "\tCapacity : " << ref.capacity() 
              << "\tLength : " << ref.size() << "\n";
}

int main()
{
    // Create empty stack (vector)
    std::vector<int> stack{};
    printStack(stack);  // Output: Empty  Capacity: 0  Length: 0

    // Push operations - add elements to top of stack
    stack.push_back(1);  // Stack: [1]
    printStack(stack);

    stack.push_back(2);  // Stack: [1, 2]
    printStack(stack);

    stack.push_back(3);  // Stack: [1, 2, 3]
    printStack(stack);

    // Peek at top element without removing it
    std::cout << "Top : " << stack.back() << "\n";  // Output: Top : 3

    // Pop operations - remove elements from top of stack
    stack.pop_back();    // Remove 3, Stack: [1, 2]
    printStack(stack);

    stack.pop_back();    // Remove 2, Stack: [1]
    printStack(stack);
    
    stack.pop_back();    // Remove 1, Stack: []
    printStack(stack);   // Output: Empty
    
    return 0;
}