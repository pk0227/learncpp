#include <iostream>
#include <vector>
#include <cmath>
#include <format>

template <typename T>
void printVector(const std::vector<T>& ref)
{
    for(auto i{0Z}; i < std::ssize(ref); i++)
        std::cout << ref.data()[i] << " ";
    
    std::cout << "\n";
}

template <typename T>
void printCapLen(const std::vector<T>& ref)
{
    std::cout << std::format("{:<10} : {}\t{:<10} : {}", "Capacity", ref.capacity(), "Length", ref.size()) << "\n";
}

int main()
{
    std::vector<int> vi{1, 21, 31, 41};     // size is 4
    printCapLen(vi);
    printVector(vi);
    std::cout << "=======================================\n";

    vi.resize(2*vi.size());                 // size is 8 (2*4)
    printCapLen(vi);
    printVector(vi);
    std::cout << "=======================================\n";

    vi.resize(std::sqrt(vi.size()));        // size is sqrt(8) ~= 2
    printCapLen(vi);
    printVector(vi);
    std::cout << "=======================================\n";

    vi.resize(5);                           // size is 5
    printCapLen(vi);
    printVector(vi);
    std::cout << "=======================================\n";

    vi.shrink_to_fit();
    printCapLen(vi);
    printVector(vi);
    std::cout << "=======================================\n";

    return 0;
}