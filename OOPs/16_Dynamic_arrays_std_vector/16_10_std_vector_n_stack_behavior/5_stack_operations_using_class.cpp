/**
 * Class-based Stack implementation with fixed capacity
 * 
 * Key Concepts:
 * 1. Template class with type and size parameters
 * 2. Fixed-size array-based stack (no dynamic allocation)
 * 3. Using std::optional for safe return values
 * 4. Bounds checking with isEmpty() and isFull()
 * 5. Peek index to track top of stack
 * 
 * Advantages:
 * - Compile-time size specification
 * - No dynamic allocation overhead
 * - Type-safe with std::optional
 * - Clear API with named methods
 * 
 * Limitations:
 * - Fixed capacity (cannot grow)
 * - Must know maximum size at compile time
 */

#include <iostream>
#include <optional>  // For std::optional - represents values that may or may not exist
#include <iomanip>   // For std::boolalpha

/**
 * Fixed-capacity stack class template
 * @tparam T - Type of elements stored in the stack
 * @tparam size - Maximum capacity of the stack (compile-time constant)
 */
template <typename T, std::size_t size>
class Stack
{
    // Fixed-size array to store stack elements
    T m_stack_array[size]{};
    
    // Index of top element (-1 means empty stack)
    // Using signed type to allow -1 for empty state
    std::ptrdiff_t m_peek{-1};
    
public:
    /**
     * Get current peek index (top position)
     * @return Index of top element (-1 if empty)
     */
    const auto& getPeek() const { return m_peek; }
    
    /**
     * Get maximum capacity of the stack
     * @return Maximum number of elements the stack can hold
     */
    auto getSize() const { return size; }
    
    /**
     * Get top element without removing it
     * @return std::optional containing top element, or empty optional if stack is empty
     */
    std::optional<T> getTop() const 
    {
        if(m_peek >= 0) 
            return m_stack_array[m_peek];  // Return top element
        
        return {};  // Return empty optional if stack is empty
    }
    
    /**
     * Check if stack is empty
     * @return true if stack has no elements, false otherwise
     */
    bool isEmpty() const
    {
        if(m_peek < 0)
            return true;
        
        return false;
    }

    /**
     * Check if stack is full
     * @return true if stack has reached maximum capacity, false otherwise
     */
    bool isFull() const
    {
        if(m_peek == (size-1))
            return true;
        
        return false;
    }

    /**
     * Push element onto top of stack
     * @param obj - Element to push
     * @return true if successful, false if stack is full
     */
    bool push(const T& obj)
    {
        if(isFull())
            return false;  // Cannot push to full stack
        
        // Pre-increment peek, then assign element
        m_stack_array[++m_peek] = obj;
        return true;
    }

    /**
     * Pop element from top of stack
     * @return std::optional containing popped element, or empty if stack is empty
     */
    std::optional<T> pop()
    {
        if(isEmpty())
            return {};  // Cannot pop from empty stack
        
        // Return element at current peek, then post-decrement peek
        return m_stack_array[m_peek--];
    }
};



/**
 * Demonstration of the Stack class usage
 * Shows:
 * - Creating a fixed-size stack
 * - Pushing elements until full
 * - Checking top element with std::optional
 * - Popping elements until empty
 * - Handling overflow and underflow gracefully
 */
int main()
{
    // Create stack of integers with capacity 5
    Stack<int, 5> stack{};
    
    // Initial state - stack is empty
    std::cout << stack.getPeek() << "\n";  // Output: -1 (empty)
    auto top{stack.getTop()};
    top.has_value() ? std::cout << *top << "\n" : std::cout << "Stack is empty\n";
    
    // Variable to hold values being pushed
    int push{};
    
    // Print boolean values as "true"/"false" instead of 1/0
    std::cout << std::boolalpha;
    
    // Push 10 - successful (1/5 capacity)
    push = 10;
    std::cout << "pushed " << push << " : " << stack.push(push) << "\n";  // true
    std::cout << stack.getPeek() << "\n";  // 0
    top = stack.getTop();
    top.has_value() ? std::cout << *top << "\n" : std::cout << "Stack is empty\n";  // 10

    // Push 20 - successful (2/5 capacity)
    push = 20;
    std::cout << "pushed " << push << " : " << stack.push(push) << "\n";  // true
    std::cout << stack.getPeek() << "\n";  // 1
    top = stack.getTop();
    top.has_value() ? std::cout << *top << "\n" : std::cout << "Stack is empty\n";  // 20

    // Push 30 - successful (3/5 capacity)
    push = 30;
    std::cout << "pushed " << push << " : " << stack.push(push) << "\n";  // true
    std::cout << stack.getPeek() << "\n";  // 2
    top = stack.getTop();
    top.has_value() ? std::cout << *top << "\n" : std::cout << "Stack is empty\n";  // 30

    // Push 40 - successful (4/5 capacity)
    push = 40;
    std::cout << "pushed " << push << " : " << stack.push(push) << "\n";  // true
    std::cout << stack.getPeek() << "\n";  // 3
    top = stack.getTop();
    top.has_value() ? std::cout << *top << "\n" : std::cout << "Stack is empty\n";  // 40

    // Push 50 - successful (5/5 capacity - now FULL)
    push = 50;
    std::cout << "pushed " << push << " : " << stack.push(push) << "\n";  // true
    std::cout << stack.getPeek() << "\n";  // 4
    top = stack.getTop();
    top.has_value() ? std::cout << *top << "\n" : std::cout << "Stack is empty\n";  // 50

    // Push 60 - FAILS (stack is full, capacity = 5)
    push = 60;
    std::cout << "pushed " << push << " : " << stack.push(push) << "\n";  // false
    std::cout << stack.getPeek() << "\n";  // Still 4
    top = stack.getTop();
    top.has_value() ? std::cout << *top << "\n" : std::cout << "Stack is empty\n";  // Still 50
    
    std::cout << "=========================================\n";

    // Pop operations - removing elements from top to bottom
    top = stack.pop();  // Pop 50
    top.has_value() ? std::cout << "Popped : " << *top << "\n" : std::cout << "Stack is empty\n";
    std::cout << "Peek : " << stack.getPeek() << "\n";  // 3

    top = stack.pop();  // Pop 40
    top.has_value() ? std::cout << "Popped : " << *top << "\n" : std::cout << "Stack is empty\n";
    std::cout << "Peek : " << stack.getPeek() << "\n";  // 2

    top = stack.pop();  // Pop 30
    top.has_value() ? std::cout << "Popped : " << *top << "\n" : std::cout << "Stack is empty\n";
    std::cout << "Peek : " << stack.getPeek() << "\n";  // 1

    top = stack.pop();  // Pop 20
    top.has_value() ? std::cout << "Popped : " << *top << "\n" : std::cout << "Stack is empty\n";
    std::cout << "Peek : " << stack.getPeek() << "\n";  // 0

    top = stack.pop();  // Pop 10
    top.has_value() ? std::cout << "Popped : " << *top << "\n" : std::cout << "Stack is empty\n";
    std::cout << "Peek : " << stack.getPeek() << "\n";  // -1 (empty)

    top = stack.pop();  // Pop from empty stack - returns empty optional
    top.has_value() ? std::cout << "Popped : " << *top << "\n" : std::cout << "Stack is empty\n";
    std::cout << "Peek : " << stack.getPeek() << "\n";  // Still -1

    return 0;
}