#include <iostream>
#include <iterator>

int main()
{
    int arr[]{101, 202, 303, 404, 505};

    for(const auto& e : arr)
        std::cout << e << " ";
    
    std::cout << "\n";
    
    //global iterator for C-style arrays
    for(auto it{std::begin(arr)}; it != std::end(arr); it++)
        std::cout << *it << " ";
    
    std::cout << "\n";

    return 0;
}