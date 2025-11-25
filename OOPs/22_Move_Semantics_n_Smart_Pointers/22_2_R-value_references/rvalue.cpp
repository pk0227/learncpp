
#include <iostream>

int getValue()
{
    int i{10};
    //return 5;
    return i;
}

void receieve(const int& lref)
{
    std::cout << "lvalue-ref : " << lref << "\n";
}

void receieve(int &&rref)
{
    std::cout << "rref : " << rref << "\n";
}

int main()
{
    //int i{65};
    int&& i{65};
    
    //const int& ptr{getValue()};
    //int&& ptr{getValue()};
    //ptr = i;
    //char&& ptr{i};
    //ptr='C';
    //std::cout << "iss : " << ptr << "\n";
    
    
    //receieve(getValue());   // always calls rvalue-refernce overloaded function even if getValue returns 5 or i;
    
    //receieve(i);                // always calls lvalue-reference overloaded function irrespective of i is int or int&& type. Because i is always lvalue and independent of its type int or int&&.
    //receieve(std::move(i));     // always calls rvalue-reference overloaded function because std::move create a rvalue so that rvalue-reference can bind to rvalue.
    
    std::cout << "i is : " << i << "\n";
    
    return 0;
}