/*
The value of an object is stored as a sequence of bits, and the data type of the object 
tells the compiler how to interpret those bits into meaningful values. 
Different data types may represent the “same” value differently. 

For example, the integer value 3 might be stored as binary 0000 0000 0000 0000 0000 0000 0000 0011, 
whereas floating point value 3.0 might be stored as binary 0100 0000 0100 0000 0000 0000 0000 0000.
*/

#include <iostream>
#include <cstring>

int main()
{
    int n{3};       // here's int value 3
    float f1{};     // here's our float variable
    float f2{3};

    std::memcpy(&f1, &n, sizeof(f1));   // copy the bits from n into f1

    std::cout << "f1 : " << f1 << std::endl;    // print f1 (containing the bits from n)
    std::cout << "f2 : " << f2 << std::endl;

    return 0;
}