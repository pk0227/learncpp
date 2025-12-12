// Demonstrates various istream input methods: operator>>, get(), getline(), peek(), gcount()
// Shows different approaches to reading character arrays and strings safely

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

int main()
{
    /*
    // ===== EXAMPLE 1: Using operator>> with std::setw() =====
    // std::setw() limits input to prevent buffer overflow
    char buf[6]{};
    std::cout << "Enter string : ";
    std::cin >> std::setw(6) >> buf;  // Reads max 5 chars (1 for null terminator)
    std::cout << "You have entered \"" << buf << "\"\n";

    std::cout << "================================================\n";

    // ===== EXAMPLE 2: Reading character by character with get() =====
    char ch{};
    std::cin.clear();  // Clear any error flags
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard remaining input

    // Comparison: operator>> skips whitespace, get() reads everything
    //while(std::cin >> ch)      // Skips whitespace (spaces, tabs, newlines)
    while(std::cin.get(ch))      // Reads all characters including whitespace
    {
        if(ch == '\n')
            break;
        std::cout << ch;
    }
    
    // ===== EXAMPLE 3: Using get() with buffer (does NOT extract delimiter) =====
    char buf1[10]{};
    
    do
    {
        std::cout << "Enter str : ";
        std::cin.get(buf1, 10);  // Reads up to 9 chars, STOPS at newline (doesn't extract it)

        std::cout << std::cin.gcount() << " :: " << buf1 << "\n";  // gcount() returns chars read
    }while(std::cin.peek() != '\n');  // peek() checks next char without extracting

    // ===== EXAMPLE 4: Using getline() with buffer (DOES extract delimiter) =====
    char buf2[10]{};
    while(true)
    {
        std::cout << "Enter string : ";
        std::cin.getline(buf2, 10);  // Reads up to 9 chars, extracts and discards newline
        std::cout << std::cin.gcount() << buf2 << "\n";
    }
    
    // ===== EXAMPLE 5: Using getline() in loop until EOF =====
    do
    {
        char buf2[10]{};
        std::cout << "Enter string : ";
        std::cin.getline(buf2, 10);  // Extracts delimiter (newline)
        std::cout << std::cin.gcount() << " :: " << buf2 << "\n";
    }while(std::cin.peek() != EOF);  // Check for end-of-file (Ctrl+Z on Windows, Ctrl+D on Unix)


    */

    // ===== ACTIVE EXAMPLE: Using std::getline() with std::string (RECOMMENDED) =====
    // std::string automatically handles memory management - safer than char arrays!
    std::string str{};
    while(true)
    {
        std::cout << "Enter string : ";
        std::getline(std::cin, str);  // Reads entire line including spaces, extracts newline
        if(str.length())  // Continue if non-empty
            std::cout << str.length() << " :: " << str << "\n";
        else
            break;  // Exit on empty line
    }

    return 0;
}