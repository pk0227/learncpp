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

    std::ostringstream os{};
    for(auto i{0Z}; i < std::ssize(ref); i++)
        os << ref.data()[i] << " ";
    
    std::cout << std::format("{:<{}}", os.str(), 15);
    std::cout << "\tCapacity : " << ref.capacity() << "\tLength : " << ref.size() << "\n";

}

int main()
{
    //std::vector<int> stack(3);

    std::vector<int> stack{};
    //stack.resize(3);
    //stack.reserve(3);

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