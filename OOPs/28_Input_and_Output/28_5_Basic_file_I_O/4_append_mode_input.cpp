// Demonstrates reading a file that was written in append mode
// Note: std::ios::app flag has NO EFFECT on ifstream (input streams)
// For reading, std::ios::app is ignored - file is always read from beginning

#include <iostream>
#include <string>
#include <fstream>  // For std::ifstream

int main()
{
    // ⚠️ IMPORTANT: std::ios::app is for OUTPUT streams only!
    // Using std::ios::app with ifstream does nothing - it's ignored
    // File is read from the beginning regardless of this flag
    
    // METHOD 1: Constructor with app flag (unnecessary for input)
    //std::ifstream fin{"sample.txt", std::ios::app};  // app flag ignored for ifstream
    
    // METHOD 2: Two-step approach with app flag (still unnecessary)
    std::ifstream fin{};  // Create ifstream object
    fin.open("./sample.txt", std::ios::app);  // app flag has no effect on reading
    
    // ===== ERROR CHECKING =====
    if(!fin)
    {
        std::cerr << "Error : File can't be opened for appending\n";  // Misleading error message
        return 1;                                                       // (it's actually for reading)
    }

    // ===== READING DATA: Reads from BEGINNING of file =====
    // Even though std::ios::app was specified, reading always starts at beginning
    // This will read ALL lines in the file (lines 1-4 if append mode output was run)
    std::string str{};
    while(std::getline(fin, str))  // Reads from start to end of file
        std::cout << str << "\n";
    
    fin.close();
    
    return 0;
}