// Demonstrates advanced istream methods: ignore(), peek(), unget(), putback()
// Shows how to manipulate the input stream buffer and handle leftover input

#include <iostream>
#include <string>

int main()
{
    /*
    // ===== Example-1: Using ignore() to discard input =====
    std::string str{};
    std::cout << "Enter string : ";
    std::cin.ignore();  // Discards the next character from input buffer
                        // Useful for clearing leftover newlines after operator>>
    std::getline(std::cin, str);  // Read full line
    
    std::cout << "You have given : " << str << "\n";
    // Example-1  -- end
    
    // ===== Example-2: Using peek() to look ahead =====
    char peek{static_cast<char>(std::cin.peek())};  // Look at next char WITHOUT extracting it
                                                     // Returns int, cast to char for display
    std::cout << "Peek is : " << peek << "\n";    // Character remains in input buffer
    // Example-2  -- end
    */
    
    // ===== Example-3: Using unget() and putback() =====
    std::cout << "Enter String : ";
    while(true)
    {
        char ch{};
        std::cin.get(ch);  // Extract one character
        
        if(ch == '\n')
        {
            // Two ways to put a character back into the stream:
            
            //std::cin.unget();           // Put the LAST READ character back (the '\n')
                                          // Doesn't specify which char - just undoes last get()
            
            //std::cin.putback('X');      // Put a SPECIFIC character ('X') into the stream
                                          // The 'X' becomes the first character to be read next
            
            // Result:
            // - Without unget/putback: next get(buf,20) reads normally from new input
            //   Output: "buf : Okay"
            // - With unget(): next get(buf,20) encounters '\n' immediately, stops (reads 0 chars)
            //   Output: "buf : " (empty)
            // - With putback('X'): next get(buf,20) reads 'X' first, then new input
            //   Output: "buf : XOkay" ('X' is prepended to whatever user types)
            break;
        }
            
        std::cout << ch;  // Echo the character
    }
    
    char buf[20]{};
    std::cout << "\nEnter buf : ";
    std::cin.get(buf, 20);  // Reads characters until delimiter or buffer full
                            // - If unget() was used: '\n' is first char, stops immediately (empty result)
                            // - If putback('X') was used: reads 'X', then continues with user input
                            //   Result: "XOkay" if user types "Okay"
                            // - If neither: reads normally starting from current buffer position
    std::cout  << "buf : " << buf << "\n";
    // Example-3  -- end
    
    return 0;
}