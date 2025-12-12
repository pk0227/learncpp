// Demonstrates std::stringstream for in-memory string manipulation
// stringstream allows treating strings as streams for formatted I/O operations
// Useful for conversions, parsing, and building formatted strings

#include <iostream>
#include <string>
#include <sstream>

int main()
{
    /*
    // ===== EXAMPLE 1: Appending vs Overwriting stringstream =====
    std::stringstream os{};
    os << "Hello World";                            // Write to stream buffer
    os << "\nOverwriting";                          // operator<< APPENDS if buffer has content
                                                    // Stream position moves forward after each write
    os.str("\nThis is string stream example");    // str() function COMPLETELY OVERWRITES internal buffer
                                                    // Also resets stream position to beginning
    //os.str("\nThis is");
    os << "\nOkay";                                 // operator<< writes from CURRENT stream position
                                                    // Since str() reset position to start, this overwrites from beginning

    std::cout << os.str() << "\n";                 // str() returns entire buffer contents
    std::cout << os.str() << "\n";                 // Can call str() multiple times - non-destructive read
    */
    
    /*
    // ===== EXAMPLE 2: Extracting from stringstream =====
    std::stringstream os{};
    os << "12345  67.89";                          // Write formatted data to stream
    
    std::string str1{};
    os >> str1;                                    // Extract first whitespace-delimited token: "12345"
    
    std::string str2{};
    os >> str2;                                    // Extract second token: "67.89"
    
    std::cout << "str1 : " << str1 << "\n";        // Output: 12345
    std::cout << "str2 : " << str2 << "\n";        // Output: 67.89
    std::cout << "whole : " << os.str() << "\n";  // str() returns ENTIRE buffer ("12345  67.89")
                                                    // Even though >> extracted data, buffer remains unchanged
                                                    // Only the stream POSITION has moved
    */
    
    /*
    // ===== EXAMPLE 3: Converting numbers to strings =====
    std::stringstream os{};
    int i{12345};
    double d{67.89};
    
    os << i << " " << d;                           // Insert numbers as formatted strings
                                                    // Automatic conversion: int/double → string
    
    std::string str1{}, str2{};
    os >> str1 >> str2;                            // Extract as strings: "12345" and "67.89"
    
    std::cout << str1 << " :: " << str2 << "\n";   // Both are now string objects
    */
    
    /*
    // ===== EXAMPLE 4: Converting strings to numbers =====
    std::stringstream os{};
    os << "12345  67.89";                          // Insert string containing numeric data
    
    int i{};
    double d{};
    
    os >> i >> d;                                  // Extract and parse as numbers
                                                    // Automatic conversion: string → int/double
                                                    // operator>> handles parsing based on target type
    
    std::cout << i << " :: " << d << "\n";         // Output: 12345 :: 67.89 (as numbers)
    */
    
    // ===== EXAMPLE 5: Clearing a stringstream for reuse =====
    // Important: To properly reuse a stringstream, you need BOTH operations
    std::stringstream os{};
    os << "Hello";                                 // Write initial content
    
    // Step 1: Clear the buffer content
    //os.str("");                                  // Option 1: Use empty C-style string
    os.str(std::string{});                         // Option 2: Use empty std::string object (preferred)
    
    // Step 2: Reset error flags
    os.clear();                                    // Resets error flags (failbit, eofbit, badbit)
                                                    // Returns stream to "ok" state
                                                    // Without this, stream may be in failed state
                                                    // and refuse further operations
    
    os << "World";                                 // Now safe to reuse - writes "World" to clean buffer
    
    std::cout << os.str() << "\n";                // Output: "World" (not "Hello" or "HelloWorld")
    
    return 0;
}