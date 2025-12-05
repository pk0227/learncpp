#include <iostream>
#include <array>


int main()
{
    std::array arr{100, 200, 300, 400, 500};  // automatically deduce to std::array<int, 5> arr{100, 200, 300, 400, 500}
    //std::array<int, 5>::iterator it{arr.begin()};
    auto it{arr.begin()};
      
    //for(it = arr.begin(); it < arr.end(); it++)   // Not recommended.
                                                    // Because some iterator types are not relationally comparable. 
                                                    // operator!= works with all iterator types.
    for(it; it != arr.end(); it++)
        std::cout << *it << " ";
    
    std::cout << "\n";


    return 0;
}