#include <iostream>
#include <array>

int main()
{
    std::array arr{101, 202, 303, 404, 505};

    for(const auto& e : arr)
        std::cout << e << " ";
    
    std::cout << "\n";
    
    return 0;
}