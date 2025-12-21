/**
 * Advanced stack operations with formatted output and helper functions
 * 
 * Key Concepts:
 * 1. Encapsulating stack operations in helper functions
 * 2. Using std::setw for formatted output alignment
 * 3. Tracking stack state after each operation
 * 4. Returning boolean from popStack to indicate empty state
 * 
 * This example demonstrates a more polished stack implementation
 * with better user feedback and formatting.
 */

#include <iostream>
#include <vector>
#include <iomanip>  // for std::setw, std::left
#include <format>

/**
 * Print current stack contents with formatted output
 * @param ref - Stack (vector) to print
 * @param pop_empty - If true and stack is empty, print without operation label
 */
template <typename T>
void printStack(const std::vector<T>& ref, bool pop_empty=false)
{
    // Save current format flags to restore later
    auto oldFlags{std::cout.flags()};
    
    if(!ref.size())
    {
        // If not displaying after a pop operation, add spacing
        if(!pop_empty)
            std::cout << std::setw(7) << "";
        std::cout << "(Stack: empty)" << "\n";
        return;
    }
    
    // Print stack contents from bottom to top
    std::cout << "(Stack: ";
    for(auto i{0Z}; i<std::ssize(ref); i++)
        std::cout << ref.data()[i] << " ";
    
    std::cout << ")\n";
    std::cout.flags(oldFlags);  // Restore original format flags
}

/**
 * Push an element onto the stack and display the operation
 * @param ref - Stack (vector) to push onto
 * @param obj - Object to push
 */
template <typename T>
void pushStack(std::vector<T>& ref, T obj)
{
    auto oldFlags{std::cout.flags()};
    ref.push_back(obj);
    
    // Display "push N" left-aligned in 7-character field
    std::cout << std::setw(7) << std::left << std::format("{} {}", "push", obj);
    std::cout.flags(oldFlags);
}

/**
 * Pop an element from the stack and display the operation
 * @param ref - Stack (vector) to pop from
 * @return true if stack becomes empty, false otherwise
 */
template <typename T>
bool popStack(std::vector<T>& ref)
{
    auto oldFlags{std::cout.flags()};
    ref.pop_back();
    
    // Display "Pop" left-aligned in 7-character field
    std::cout << std::setw(7) << std::left << "Pop";
    std::cout.flags(oldFlags);
    
    // Return true if stack is now empty
    if(ref.size())
        return false;
    else
        return true;
}

int main()
{
    std::vector<int> stack{};

    // Initial state
    printStack(stack);  // (Stack: empty)
    
    // Push operations with formatted output
    pushStack(stack, 1);  // "push 1 "
    printStack(stack);    // (Stack: 1)

    pushStack(stack, 2);  // "push 2 "
    printStack(stack);    // (Stack: 1 2)

    pushStack(stack, 3);  // "push 3 "
    printStack(stack);    // (Stack: 1 2 3)

    // Pop operation - pass result to printStack for conditional formatting
    printStack(stack, popStack(stack));  // "Pop    (Stack: 1 2)"

    pushStack(stack, 4);  // "push 4 "
    printStack(stack);    // (Stack: 1 2 4)
    
    // Multiple pop operations
    printStack(stack, popStack(stack));  // "Pop    (Stack: 1 2)"
    printStack(stack, popStack(stack));  // "Pop    (Stack: 1)"
    printStack(stack, popStack(stack));  // "Pop    (Stack: empty)"

    return 0;
}