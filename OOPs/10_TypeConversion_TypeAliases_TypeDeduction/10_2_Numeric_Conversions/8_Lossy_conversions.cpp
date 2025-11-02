#include <iostream>

int main()
{
    int i = 3.0; // okay: will be converted to int value 3 (value preserved)
    int j = 3.5; // data lost: will be converted to int value 3 (fractional value 0.5 lost)

    float f = 1.2;        // okay: will be converted to float value 1.2 (value preserved)
    float g = 1.23456789; // data lost: will be converted to float 1.23457 (precision lost)

    double d { static_cast<double>(static_cast<int>(3.5)) }; // convert double 3.5 to int and back
    std::cout << d << '\n'; // prints 3

    double d2 { static_cast<double>(static_cast<float>(1.23456789)) }; // convert double 1.23456789 to float and back
    std::cout << d2 << '\n'; // prints 1.23457
    
    return 0;
}