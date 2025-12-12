#include <iostream>
#include <cmath>

int main()
{
    double input{};
    std::cout << "Enter a Number : ";
    std::cin >> input;

    try
    {
        if(input < 0.0)
            throw "Can't take sqrt for Negative numbers";
        std::cout << "sqrt of " << input << " is : " << sqrt(input) << "\n"; 
    }
    catch(const char* e)
    {
        std::cout << e << "\n";
    }

    std::cout << "Program continues...\n";

    return 0;
}