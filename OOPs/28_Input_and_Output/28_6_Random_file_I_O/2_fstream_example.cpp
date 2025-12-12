// Demonstrates bidirectional file I/O with std::fstream and binary mode
// fstream = file stream, supports both reading AND writing simultaneously
// Key concept: std::ios::binary mode for cross-platform consistency

#include <iostream>
#include <fstream>  // For std::fstream
#include <string>

int main()
{
    // ===== OPENING FILE IN BINARY MODE =====
    // Without std::ios::binary:
    //std::fstream fio{"C:\\Users\\pk255057\\Downloads\\sample.txt", std::ios::in | std::ios::out};
    
    // ⚠️ TEXT MODE ISSUE: Different newline handling on different OS
    // - Windows: \r\n (2 bytes) - seekg calculations can be off!
    // - Linux/Unix: \n (1 byte)
    // This causes position mismatches when seeking after writing
    
    // ✅ BINARY MODE SOLUTION: Disables newline translation
    std::fstream fio{"C:\\Users\\pk255057\\Downloads\\sample.txt", std::ios::in | std::ios::out | std::ios::binary};
    // Binary mode treats file as raw bytes - consistent behavior across platforms
    // std::ios::in | std::ios::out = read AND write access
    // std::ios::binary = no character translation (what you write is what you get)
    
    if(!fio)
    {
        std::cerr << "Error : File can't be opened for writing/reading...!\n";
        return 1;
    }

    // ===== READING AND MODIFYING FILE IN-PLACE =====
    char ch{};
    while(fio.get(ch))  // Read one character at a time
    {
        // Check if character is a vowel (lowercase or uppercase)
        switch(ch)
        {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
            case 'A':
            case 'E':
            case 'I':
            case 'O':
            case 'U':
                // Found a vowel - replace it with '#'
                
                // Step 1: Move write position back 1 byte (to where vowel was read)
                fio.seekg(-1, std::ios::cur);  // seekg moves the GET (read) pointer
                
                // Step 2: Write '#' at current position (overwrites the vowel)
                fio << '#';  // operator<< writes and moves position forward
                //fio.put('#');  // Alternative: put() writes single character
                
                // Step 3: Synchronize read pointer with write pointer
                // ⚠️ CRITICAL: After writing, must sync pointers for next read!
                // tellg() gets current read position, seekg() updates it
                fio.seekg(fio.tellg(), std::ios::beg);  // Reset read position
                break;
        }
    }
    
    // ===== PREPARING TO READ MODIFIED FILE =====
    fio.flush();  // Ensure all writes are committed to disk
                  // Flushes internal buffer to physical file
    
    fio.clear();  // Clear EOF flag (set by previous read loop)
                  // Without this, stream is in failed state and can't read
    
    fio.seekg(0, std::ios::beg);  // Move read position back to start of file
                                   // Now we can read the modified content
    
    // ===== READING AND DISPLAYING MODIFIED FILE =====
    std::string str{};
    while(std::getline(fio, str))  // Read modified file line by line
        std::cout << str << "\n";  // Display result (vowels replaced with '#')
    
    fio.close();
    
    return 0;
}