// Demonstrates robust numeric input validation using extraction operator (>>)
// Handles three types of invalid input:
// 1. Non-numeric input (e.g., "abc25") - extraction fails immediately
// 2. Mixed input (e.g., "25abc") - extraction succeeds but leaves garbage in buffer
// 3. Invalid range (e.g., negative or zero age)

#include <iostream>
#include <limits>  // For std::numeric_limits

int main()
{
    int Age{};

    // Validation loop: repeats until valid input received
    while(true)
    {
        std::cout << "Enter Age : ";
        std::cin >> Age;  // Attempt to extract integer
                          // Succeeds: reads digits until non-digit found
                          // Fails: if first non-whitespace char is not a digit

        // ===== CHECK 1: Handle extraction failure =====
        // fail() returns true if extraction failed (failbit is set)
        // Examples that trigger this: "abc25", "xyz", "@#$", "" (empty)
        if(std::cin.fail())
        {
            std::cerr << "std::cin has failed due to invalid input\n";
            std::cin.clear();  // Clear error flags (failbit, eofbit)
                               // Without this, cin remains in failed state and ignores all operations
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard bad input line
            continue;  // Retry input
        }

        // ===== CHECK 2: Detect mixed numeric/non-numeric input =====
        // At this point, extraction succeeded (Age has a valid number)
        // But there might be garbage remaining in the buffer
        // Example: "25abc" - Age=25, but "abc\n" still in buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Try to ignore until newline
        if(std::cin.gcount() > 1)  // gcount() returns number of chars extracted by last ignore()
        {                          // If > 1, means there were extra characters before '\n'
                                   // (The '\n' itself counts as 1 character)
            std::cerr << "Only numbers are allowed\n";
            continue;  // Reject input, retry
        }
        
        // ===== CHECK 3: Validate value is in acceptable range =====
        // Age was successfully extracted and buffer is clean
        // Now check if the value makes logical sense
        if(Age <= 0)
        {
            std::cerr << "Invalid Age\n";  // Reject negative or zero ages
            continue;  // Retry input
        }
        
        // All checks passed - input is valid!
        break;  // Exit validation loop
    }

    // At this point, Age contains a valid positive integer
    std::cout << "You have entered age : " << Age << "\n";
    
    return 0;
}