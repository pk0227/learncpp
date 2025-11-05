#include <iostream>
#include <bitset>

int main()
{
    std::bitset<8> bits{ 0b0000'0101 }; // The <8> is a non-type template parameter
    std::cout << "bits : " << bits << '\n';
    return 0;
}