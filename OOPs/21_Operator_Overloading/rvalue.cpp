
#include <iostream>

int getValue()
{
    int i{10};
    //return 5;
    return i;
}

void receieve(int &&rref)
{
    std::cout << "rref : " << rref << "\n";
}

int main()
{
    
    int i{65};
    /*
    //const int &ptr{getValue()};
    int &&ptr{getValue()};
    //char &&ptr{i};
    std::cout << "iss : " << ptr << "\n";
    */
    
    receieve(getValue());
    
    return 0;
}