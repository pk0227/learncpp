#include <iostream>
#include <cstdint>

enum Color : std::int8_t
{
    Red,
    Green,
    Blue
};

int main()
{
    Color c{Red};
    std::cout << "sizeof(c) : " << sizeof(c) << "\n";
    
    return 0;
}