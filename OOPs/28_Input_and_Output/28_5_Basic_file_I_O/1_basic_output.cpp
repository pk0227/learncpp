// Demonstrates basic file output using std::ofstream
// ofstream = output file stream, used for writing data to files
// Default mode: std::ios::out (write mode, creates file if doesn't exist, truncates if exists)

#include <iostream>
#include <fstream>  // For std::ofstream

int main()
{
    // ===== METHOD 1: Constructor initialization (preferred) =====
    // Opens "sample.txt" for writing in output mode
    // If file exists: truncates (erases) existing content
    // If file doesn't exist: creates new file
    std::ofstream fout{"./sample.txt"};       // defaults to std::ios::out mode for std::ofstream
    
    // ===== METHOD 2: Two-step approach (open separately) =====
    //std::ofstream fout{};                  // Create ofstream object without opening
    //fout.open("sample.txt");               // Open file explicitly, defaults to std::ios::out mode
    
    // ===== ERROR CHECKING: Verify file opened successfully =====
    // File opening can fail due to: permission issues, disk full, invalid path, etc.
    if(!fout)  // Equivalent to: if(fout.fail()) or if(!fout.is_open())
    {
        std::cerr << "Error : File cant be opened for writing\n";
        return 1;  // Exit with error code
    }

    // ===== WRITING DATA: Use insertion operator (<<) just like std::cout =====
    fout << "This is line 1\n";  // Write first line to file
    fout << "This is line 2\n";  // Write second line to file
    // Note: std::endl could be used instead of "\n", but "\n" is more efficient
    //       (std::endl flushes buffer, which is slower)

    // ===== CLOSING FILE: Explicitly close when done =====
    fout.close();  // Flushes buffer and closes file handle
                   // Note: File closes automatically when fout goes out of scope (destructor)
                   // Explicit close() is optional but good practice for early resource release

    return 0;  // File will be closed here anyway if not already closed
}