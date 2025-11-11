#include <iostream>

enum Color  :   int  
//enum Color  
{
    Red,
    Green,
    Blue
};

int main()
{
    Color c{2};  // error: invalid conversion from 'int' to 'Color' if underlying(base) is not int. Then use static_cast<Color>(2)
    //Color c{static_cast<Color>(2)};  // use static_cast<Color>(2) if base is not int.
    std::cout << "sizeof(c) : " << sizeof(c) << "\n";
    
    return 0;
}