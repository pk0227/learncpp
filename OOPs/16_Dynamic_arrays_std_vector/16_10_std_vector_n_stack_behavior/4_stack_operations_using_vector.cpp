#include <iostream>
#include <vector>
#include <iomanip>
#include <format>

template <typename T>
void printStack(const std::vector<T>& ref, bool pop_empty=false)
{
    auto oldFlags{std::cout.flags()};
    if(!ref.size())
    {
        if(!pop_empty)
            std::cout << std::setw(7) << "";
        std::cout << "(Stack: empty)" << "\n";
        return;
    }
    
    std::cout << "(Stack: ";
    for(auto i{0Z}; i<std::ssize(ref); i++)
        std::cout << ref.data()[i] << " ";
    
    std::cout << ")\n";
    std::cout.flags(oldFlags);
}

template <typename T>
void pushStack(std::vector<T>& ref, T obj)
{
    auto oldFlags{std::cout.flags()};
    ref.push_back(obj);
    std::cout << std::setw(7) << std::left << std::format("{} {}", "push", obj);
    std::cout.flags(oldFlags);
}

template <typename T>
bool popStack(std::vector<T>& ref)
{
    auto oldFlags{std::cout.flags()};
    ref.pop_back();
    std::cout << std::setw(7) << std::left << "Pop";
    std::cout.flags(oldFlags);
    
    if(ref.size())
        return false;
    else
        return true;
}

int main()
{
    std::vector<int> stack{};

    printStack(stack);
    
    pushStack(stack, 1);
    printStack(stack);

    pushStack(stack, 2);
    printStack(stack);

    pushStack(stack, 3);
    printStack(stack);

    printStack(stack, popStack(stack));

    pushStack(stack, 4);
    printStack(stack);
    
    printStack(stack, popStack(stack));
    printStack(stack, popStack(stack));
    printStack(stack, popStack(stack));

    return 0;
}