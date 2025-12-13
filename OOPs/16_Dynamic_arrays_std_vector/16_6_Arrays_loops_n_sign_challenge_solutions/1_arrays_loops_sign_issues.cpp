#include <iostream>
#include <vector>

using Index = std::ptrdiff_t;

template <typename T>
void printReverse(const std::vector<T>& ref)
{
    //for(auto index{static_cast<Index>(ref.size()-1)}; index >= 0; index--)
    //for(auto index{static_cast<Index>(std::size(ref)-1)}; index >= 0; index--)
    for(auto index{std::ssize(ref)-1}; index >= 0; index--)
    {
        //std::cout << ref[static_cast<std::size_t>(index)] << " ";
        std::cout << ref.data()[index] << " ";
    }
    std::cout << "\n";
}

int main()
{
    std::vector<int> vi{10, 20, 30, 40, 50};
    printReverse(vi);
    
    return 0;
}