#include <iostream>
#include <vector>

template <typename T>
void printStack(const std::vector<T>& ref)
{
    if(ref.empty())
        std::cout << "Empty";

    for(auto i{0Z}; i < std::ssize(ref); i++)
        std::cout << ref.data()[i] << " ";
    
    std::cout << "\tCapacity : " << ref.capacity() << "\tLength : " << ref.size() << "\n";
}

int main()
{
    std::vector<int> stack{};
    printStack(stack);

    stack.push_back(1);
    printStack(stack);

    stack.push_back(2);
    printStack(stack);

    stack.push_back(3);
    printStack(stack);

    std::cout << "Top : " << stack.back() << "\n";

    stack.pop_back();
    printStack(stack);

    stack.pop_back();
    printStack(stack);
    
    stack.pop_back();
    printStack(stack);
    
    return 0;
}