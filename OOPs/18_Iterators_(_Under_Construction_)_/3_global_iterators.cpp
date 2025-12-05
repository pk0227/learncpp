#include <iostream>
#include <array>  // includes #include <iterator>

int main()
{
    std::array arr{100, 200, 300, 400, 500};  // automatically deduce to std::array<int, 5> arr{100, 200, 300, 400, 500}
    //std::array<int, 5>::iterator it{std::begin(arr)};
    auto it{std::begin(arr)};
      
    //for(it = std::begin(arr); it < std::end(arr); it++)   // Not recommended.
                                                    // Because some iterator types are not relationally comparable. 
                                                    // operator!= works with all iterator types.
    for(it; it != std::end(arr); it++)
        std::cout << *it << " ";
    
    std::cout << "\n";


    return 0;
}