#include <iostream>
#include <optional>
#include <iomanip>

template <typename T, std::size_t size>
class Stack
{
    T m_stack_array[size]{};
    std::ptrdiff_t m_peek{-1};
public:
    const auto& getPeek() const { return m_peek; }
    auto getSize() const { return size; }
    std::optional<T> getTop() const 
    {
        if(m_peek >= 0) 
            return m_stack_array[m_peek];
        
        return {};
    }
    
    bool isEmpty() const
    {
        if(m_peek < 0)
            return true;
        
        return false;
    }

    bool isFull() const
    {
        if(m_peek == (size-1))
            return true;
        
        return false;
    }

    bool push(const T& obj)
    {
        if(isFull())
            return false;
        
        m_stack_array[++m_peek] = obj;
        return true;
    }

    std::optional<T> pop()
    {
        if(isEmpty())
            return {};
        
        return m_stack_array[m_peek--];
    }
};



int main()
{
    Stack<int, 5> stack{};
    std::cout << stack.getPeek() << "\n";
    auto top{stack.getTop()};
    top.has_value() ? std::cout << *top << "\n" : std::cout << "Stack is empty\n";
    
    int push{};
    std::cout << std::boolalpha;
    push = 10;
    std::cout << "pushed " << push << " : " << stack.push(push) << "\n";
    std::cout << stack.getPeek() << "\n";
    top = stack.getTop();
    top.has_value() ? std::cout << *top << "\n" : std::cout << "Stack is empty\n";

    push = 20;
    std::cout << "pushed " << push << " : " << stack.push(push) << "\n";
    std::cout << stack.getPeek() << "\n";
    top = stack.getTop();
    top.has_value() ? std::cout << *top << "\n" : std::cout << "Stack is empty\n";

    push = 30;
    std::cout << "pushed " << push << " : " << stack.push(push) << "\n";
    std::cout << stack.getPeek() << "\n";
    top = stack.getTop();
    top.has_value() ? std::cout << *top << "\n" : std::cout << "Stack is empty\n";    

    push = 40;
    std::cout << "pushed " << push << " : " << stack.push(push) << "\n";
    std::cout << stack.getPeek() << "\n";
    top = stack.getTop();
    top.has_value() ? std::cout << *top << "\n" : std::cout << "Stack is empty\n";

    push = 50;
    std::cout << "pushed " << push << " : " << stack.push(push) << "\n";
    std::cout << stack.getPeek() << "\n";
    top = stack.getTop();
    top.has_value() ? std::cout << *top << "\n" : std::cout << "Stack is empty\n";    

    push = 60;
    std::cout << "pushed " << push << " : " << stack.push(push) << "\n";
    std::cout << stack.getPeek() << "\n";
    top = stack.getTop();
    top.has_value() ? std::cout << *top << "\n" : std::cout << "Stack is empty\n";
    
    std::cout << "=========================================\n";

    top = stack.pop();
    top.has_value() ? std::cout << "Popped : " << *top << "\n" : std::cout << "Stack is empty\n";
    std::cout << "Peek : " << stack.getPeek() << "\n";

    top = stack.pop();
    top.has_value() ? std::cout << "Popped : " << *top << "\n" : std::cout << "Stack is empty\n";
    std::cout << "Peek : " << stack.getPeek() << "\n";

    top = stack.pop();
    top.has_value() ? std::cout << "Popped : " << *top << "\n" : std::cout << "Stack is empty\n";
    std::cout << "Peek : " << stack.getPeek() << "\n";

    top = stack.pop();
    top.has_value() ? std::cout << "Popped : " << *top << "\n" : std::cout << "Stack is empty\n";
    std::cout << "Peek : " << stack.getPeek() << "\n";

    top = stack.pop();
    top.has_value() ? std::cout << "Popped : " << *top << "\n" : std::cout << "Stack is empty\n";
    std::cout << "Peek : " << stack.getPeek() << "\n";

    top = stack.pop();
    top.has_value() ? std::cout << "Popped : " << *top << "\n" : std::cout << "Stack is empty\n";
    std::cout << "Peek : " << stack.getPeek() << "\n";

    return 0;
}