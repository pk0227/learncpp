/*
 * Demonstration: std::string_view Flexibility
 * 
 * KEY FEATURES:
 * 1. Can be initialized with multiple string types:
 *    - C-style string literals ("text")
 *    - std::string objects  
 *    - Other std::string_view objects
 * 
 * 2. Function parameters accept all string types:
 *    - No overloading needed
 *    - Single function works with any string type
 *    - Compiler automatically creates view
 * 
 * 3. Best practice for read-only function parameters:
 *    - Replaces const std::string& for read-only parameters
 *    - More flexible (accepts more types)
 *    - Better performance (no conversion overhead)
 */

#include <iostream>
#include <string>
#include <string_view>

// One function to accept ALL string types!
// Creates a lightweight view of whatever argument is passed
void printSV(std::string_view str)
{
    std::cout << str << '\n';
}

int main()
{
    // INITIALIZATION METHOD 1: From C-style string literal
    // string_view just stores pointer to "Hello, world!" and length
    std::string_view s1 { "Hello, world!" };
    std::cout << s1 << '\n';

    // Create a std::string for demonstration
    std::string s{ "Hello, world!" };
    
    // INITIALIZATION METHOD 2: From std::string
    // string_view references the data inside std::string s
    std::string_view s2 { s };
    std::cout << s2 << '\n';

    // INITIALIZATION METHOD 3: From another std::string_view
    // Both s2 and s3 now view the same underlying string data
    std::string_view s3 { s2 };
    std::cout << s3 << '\n';

    // -------------------------------------------------------------------------------------

    // FUNCTION CALL FLEXIBILITY:
    // Same function accepts different string types!
    
    // Call 1: With C-style string literal
    printSV("Hello, world!");
    // Compiler creates temporary string_view to the literal

    // Call 2: With std::string
    printSV(s);
    // Compiler creates temporary string_view to s's data

    // Call 3: With std::string_view
    printSV(s2);
    // Direct pass, no conversion needed
    
    // ADVANTAGE:
    // No need for multiple overloads like:
    // void printSV(const char*);
    // void printSV(const std::string&);
    // void printSV(std::string_view);
    // One function does it all!

    return 0;
}