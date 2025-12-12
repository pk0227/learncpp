#include <iostream>
#include <cmath>

double customSqrt(double input)
{
    if(input < 0.0)
        throw "Can't take sqrt for Negative numbers";
    
    return sqrt(input);
}

int main()
{
    double input{};
    std::cout << "Enter a Number : ";
    std::cin >> input;

    try
    {
        double output{customSqrt(input)};
        std::cout << "sqrt of " << input << " is : " << output << "\n"; 
    }
    catch(const char* e)
    {
        std::cout << e << "\n";
    }

    std::cout << "Program continues...\n";

    return 0;
}