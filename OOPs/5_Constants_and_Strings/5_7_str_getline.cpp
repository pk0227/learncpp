/*
 * Demonstration: std::getline and Input Buffer Management
 * 
 * KEY CONCEPTS:
 * 1. std::cin >> extracts until whitespace, leaves \n in buffer
 * 2. std::getline extracts entire line including \n, but discards the \n
 * 3. std::ws manipulator skips leading whitespace
 * 4. Input buffer cleanup is crucial for proper input flow
 * 
 * COMMON ISSUE:
 * When mixing >> and getline, leftover \n from >> will be consumed by getline
 * resulting in empty string input
 */

#include <iostream>
#include <string> // For std::string and std::getline

int main()
{
    std::cout << "Enter your full name : ";
    std::string name{};
    
    // std::getline() BEHAVIOR:
    // - Reads characters until it encounters \n (newline)
    // - Extracts the \n from the buffer
    // - DISCARDS the \n (doesn't store it)
    // - Stores everything else in the string
    
    // Option 1: Use std::ws to skip leading whitespace (useful after previous >>)
    //std::getline(std::cin >> std::ws, name);
    
    // Option 2: Direct getline (works if no leftover \n in buffer)
    std::getline(std::cin, name);

    std::cout << "Enter your age : ";
    int age{};
    std::cin >> age;  // INPUT BUFFER ISSUE:
                      // User types: "20" + ENTER
                      // Buffer contains: "20\n"
                      // std::cin >> age extracts: "20"
                      // Buffer still contains: "\n" (leftover!)
                      //
                      // This leftover \n will be consumed by next getline
                      // unless we clean it up!

    std::cout << "Enter your favorite color : ";
    std::string color{};
    
    // SOLUTION: Use std::ws manipulator to clean up leftover whitespace
    // std::ws (whitespace) INPUT MANIPULATOR:
    // - Extracts and DISCARDS leading whitespace (spaces, tabs, newlines)
    // - Ensures getline starts reading from actual content
    // - Critical when getline follows std::cin >>
    std::getline(std::cin >> std::ws, color);
    
    // WITHOUT std::ws (would cause problems):
    // The leftover \n from age input would be immediately consumed
    // Result: color would be an empty string
    //std::getline(std::cin, color);  // Don't do this after std::cin >>

    std::cout << "Your name is " << name 
              << ", your age is " << age 
              << " and your favorite color is " << color << '\n';

    return 0;
}