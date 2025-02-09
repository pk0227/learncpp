#include <iostream>
#include <bitset>
#include <print>

int main()
{

    //36 in hexadecimal, binary and octal.
    //assume 16bits
    unsigned int hex{0x0024};            //if it is in 32bits, 0x00000024
    int bin{0b00100100};                 //if it is in 32bits, 0x0000000000100100
    int oct{044};

    std::bitset<32> bits{hex};

    std::cout << hex << "\n";
    std::cout << bin << "\n";
    std::cout << oct << "\n";

    std::cout << bits << "\n";
    //std::cout << std::bitset<32>{-36} << "\n";  //bitset is only for unsigned long long int. 

    int dec{36};
    std::cout << std::format("{:x}\n{:b}\n{:o}\n", dec, dec, dec);
    std::print("----------------------------------\n");     //std::print prints without new line.
    std::println("{:x}\n{:b}\n{:o}\n", dec, dec, dec);      //std::println prints with new line.

    return 0;
}
