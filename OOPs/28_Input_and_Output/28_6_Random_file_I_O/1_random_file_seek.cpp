// Demonstrates random file access using seekg() for input streams
// seekg = "seek get" - moves the read position pointer to a specific location
// Allows non-sequential reading (jumping around the file instead of reading start-to-end)

#include <iostream>
#include <string>
#include <fstream>  // For std::ifstream

int main()
{
    std::ifstream fin{"./sample.txt"};
    if(!fin)
    {
        std::cerr << "Error : File can't be opened for reading\n";
        return 1;
    }

    std::string str{};
    
    // ===== SEEK METHOD 1: Absolute position from beginning (std::ios::beg) =====
    fin.seekg(5);  // Move to byte 5 from start (skip first 5 characters)
                   // Default direction is std::ios::beg (beginning)
                   // Equivalent to: fin.seekg(5, std::ios::beg)
    std::getline(fin, str);  // Read from position 5 to next newline
    std::cout << str << "\n";

    // ===== SEEK METHOD 2: Relative position from current (std::ios::cur) =====
    fin.seekg(8, std::ios::cur);  // Move 8 bytes forward from CURRENT position
                                  // Positive offset = move forward
                                  // Negative offset = move backward
    std::getline(fin, str);  // Read from new position to next newline
    std::cout << str << "\n";

    // ===== SEEK METHOD 3: Relative position from end (std::ios::end) =====
    fin.seekg(-18, std::ios::end);  // Move to 18 bytes BEFORE end of file
                                     // Negative offset required (can't go past EOF)
    std::getline(fin, str);  // Read from position to next newline
    std::cout << str << "\n";

    fin.close();

    return 0;
}