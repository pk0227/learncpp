#include <iostream>
#include <vector>
#include <cmath>

template <typename T>
void printVector(const std::vector<T>& ref)
{
    for(auto i{0Z}; i < std::ssize(ref); i++)
        std::cout << ref.data()[i] << " ";
    
    std::cout << "\n";
}

int main()
{
    std::vector<int> vi{1, 21, 31, 41};     // size is 4
    printVector(vi);

    vi.resize(2*vi.size());                 // size is 8 (2*4)
    printVector(vi);

    vi.resize(std::sqrt(vi.size()));        // size is sqrt(8) ~= 2
    printVector(vi);

    return 0;
}