#include <iostream>
#include <array>


int main()
{
    std::array arr{100, 200, 300, 400, 500};  // automatically deduce to std::array<int, 5> arr{100, 200, 300, 400, 500}
    //std::array<int, 5>::iterator it{};

    auto begin{&arr[0]};
    auto end{&arr[0] + std::size(arr)};

    //for(begin; begin<end; begin++)
    for(begin; begin != end; begin++)
        std::cout << *begin << " ";
    
    std::cout << "\n";


    return 0;
}