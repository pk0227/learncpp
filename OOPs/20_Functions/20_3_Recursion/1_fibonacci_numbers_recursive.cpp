#include <iostream>

int fibonacci(int count)
{
    if(count == 0)
        return 0;

    if(count == 1)
        return 1;

    return  fibonacci(count-1)+fibonacci(count-2); 
}

int main()
{
    for(int i{}; i < 15; i++)
        std::cout << fibonacci(i) << " ";
    std::cout << "\n";

    return 0;
}