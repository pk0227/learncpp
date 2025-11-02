#include <iostream>

int main()
{
   // int i = {3.5}; // won't compile as {} disallows narrowing conversions
   // static_cast<int> converts double to int, initializes i with int result
   // int i { static_cast<int>(3.5) };

   constexpr  double d{3.0};
   //int i{d};    // won't compile as {} disallows narrowing conversions
   int i{static_cast<int>(d)};

    std::cout << i << std::endl;
    return 0;
}