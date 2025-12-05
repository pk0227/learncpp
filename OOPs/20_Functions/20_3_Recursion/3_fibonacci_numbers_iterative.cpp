#include <iostream>

int fibonacci(int count)
{
    if(count<=1)
        return count;

    int a{0};
    int b{1};

    for(int i{2}; i<=count; i++)
    {
        int next=a+b;
        a=b;
        b=next;
    }

    return b;
}

int main()
{
    for(int i{}; i < 15; i++)
        std::cout << fibonacci(i) << " ";
    std::cout << "\n";
    
    return 0;
}