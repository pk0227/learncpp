/*
 * Demonstration: Different Numeral Systems in C++
 * 
 * Topics Covered:
 * 1. Integer literals in different bases (decimal, hexadecimal, binary, octal)
 * 2. std::bitset for binary representation
 * 3. Formatted output using std::format (C++20)
 * 4. std::print and std::println (C++23)
 * 
 * Number Bases:
 * - Decimal (base 10): Normal numbers (0-9)
 * - Hexadecimal (base 16): Prefix 0x, digits 0-9, A-F
 * - Binary (base 2): Prefix 0b, digits 0-1
 * - Octal (base 8): Prefix 0, digits 0-7
 */

#include <iostream>
#include <bitset>   // For binary representation
#include <format>   // C++20: For std::format
#if defined(__cpp_lib_print) || (defined(__cplusplus) && __cplusplus >= 202302L)
#include <print>    // C++23: For std::print and std::println
#endif

int main()
{

    // Representing the number 36 in different numeral systems
    
    // HEXADECIMAL (base 16): 0x prefix
    // 36 decimal = 0x24 (2*16 + 4 = 36)
    unsigned int hex{0x0024};  // For 32-bit: 0x00000024
    
    // BINARY (base 2): 0b prefix
    // 36 decimal = 0b00100100 (32 + 4 = 36)
    int bin{0b00100100};       // For 32-bit: 0b00000000000000000000000000100100
    
    // OCTAL (base 8): 0 prefix (just zero, no 'o')
    // 36 decimal = 044 (4*8 + 4 = 36)
    int oct{044};              // WARNING: Leading zero indicates octal!

    // Create a bitset to view binary representation
    // std::bitset<N> stores N bits and displays them in binary
    std::bitset<32> bits{hex};

    // All values print as decimal by default (regardless of how they were initialized)
    std::cout << hex << "\n";  // Prints: 36 (decimal)
    std::cout << bin << "\n";  // Prints: 36 (decimal)
    std::cout << oct << "\n";  // Prints: 36 (decimal)

    // bitset displays the binary representation
    std::cout << bits << "\n";  // Prints: 00000000000000000000000000100100
    
    // LIMITATION: std::bitset only accepts unsigned long long
    // Cannot use negative numbers directly
    //std::cout << std::bitset<32>{-36} << "\n";  // Compilation error! 

    // Using std::format (C++20) to output in different bases
    int dec{36};
    
    // Format specifiers:
    // {:x} = hexadecimal (lowercase)
    // {:b} = binary
    // {:o} = octal
    std::cout << std::format("{:x}\n{:b}\n{:o}\n", dec, dec, dec);
    // Output: 24\n100100\n44
    
#if defined(__cpp_lib_print) || (defined(__cplusplus) && __cplusplus >= 202302L)
    // std::print (C++23): Prints formatted output WITHOUT automatic newline
    std::print("----------------------------------\n");
    
    // std::println (C++23): Prints formatted output WITH automatic newline at the end
    std::println("{:x}\n{:b}\n{:o}\n", dec, dec, dec);
#else
    std::cout << "----------------------------------\n";
    std::cout << std::format("{:x}\n{:b}\n{:o}\n", dec, dec, dec);
#endif

    return 0;
}
