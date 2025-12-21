#include <iostream>

std::size_t printFibonacci(std::size_t input)
{
    std::size_t a{0}, b{1};
    if(input == 0) return a;
    if(input <= 2) return b;
    
    for(std::size_t i{1}; i < input; i++)
    {
        std::size_t temp = a+b;
        a = b;
        b = temp;
    }
    
    return b;
}

std::size_t recursivePrintFibonacci(std::size_t input)
{
    if(input < 2) return input;
    
    return recursivePrintFibonacci(input-1) + recursivePrintFibonacci(input-2);
}

int main()
{
    int input{};
    std::cout << "Enter number of fibonacci : ";
    std::cin >> input;
    
    for(std::size_t i{}; i < input; i++)
    {
        std::cout << printFibonacci(i) << " ";
    }
    
    std::cout << "\n";
    
    for(std::size_t i{}; i < input; i++)
    {
        std::cout << recursivePrintFibonacci(i) << " ";
    }
    
    return 0;
}