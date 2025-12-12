// Demonstrates basic file input using std::ifstream
// ifstream = input file stream, used for reading data from files
// Default mode: std::ios::in (read mode, file must exist)

#include <iostream>
#include <fstream>  // For std::ifstream
#include <string>

int main()
{
    // ===== METHOD 1: Constructor initialization (preferred) =====
    // Opens "sample.txt" for reading in input mode
    // File must already exist, otherwise opening fails
    std::ifstream fin{"./sample.txt"};     // defaults to std::ios::in mode for std::ifstream
    
    // ===== METHOD 2: Two-step approach (open separately) =====
    //std::ifstream fin{};                  // Create ifstream object without opening
    //fin.open("sample.txt");               // Open file explicitly, defaults to std::ios::in mode

    // ===== ERROR CHECKING: Verify file opened successfully =====
    // File opening fails if: file doesn't exist, no read permission, path is invalid, etc.
    if(!fin)  // Equivalent to: if(fin.fail()) or if(!fin.is_open())
    {
        std::cerr << "Error : File cant be opened for reading\n";
        return 1;  // Exit with error code
    }

    // ===== READING DATA: Two common approaches =====
    std::string str{};
    
    // Approach 1: std::getline() - reads entire line including spaces
    while(std::getline(fin,str))  // Reads until newline, returns false at EOF
        std::cout << str << "\n";  // Print each line
    
    // Approach 2: operator>> - reads whitespace-delimited tokens
    //while(fin >> str)            // Reads word by word, skips whitespace
    //    std::cout << str << "\n";  // Would print each word on separate line
    
    // ===== CLOSING FILE =====
    fin.close();  // Explicit close (optional, destructor auto-closes)
    
    return 0;
}