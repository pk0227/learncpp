// Demonstrates file output in APPEND mode using std::ios::app
// Append mode: writes to END of file without erasing existing content
// Comparison: std::ios::out truncates file, std::ios::app preserves and adds to end

#include <iostream>
#include <string>
#include <fstream>  // For std::ofstream

int main()
{
    // ===== APPEND MODE: std::ios::app =====
    // Key difference from default std::ios::out:
    // - std::ios::out: TRUNCATES file (erases all existing content)
    // - std::ios::app: APPENDS to file (preserves existing content, writes at end)
    
    // METHOD 1: Constructor with mode flag
    //std::ofstream fout{"sample.txt", std::ios::app};
    
    // METHOD 2: Two-step approach with mode flag
    std::ofstream fout{};  // Create ofstream object
    fout.open("./sample.txt", std::ios::app);  // Open in APPEND mode
    
    // ===== ERROR CHECKING =====
    if(!fout)
    {
        std::cerr << "Error : File can't be opened for appending\n";
        return 1;
    }

    // ===== WRITING DATA: Appended to end of existing file =====
    std::string str{};
    fout << "This is line 3 in append mode\n";  // Writes after existing lines 1 & 2
    fout << "This is line 4 in append mode\n";  // Writes after line 3
    // Existing file content remains intact, new data added at end

    fout.close();
    
    return 0;
}