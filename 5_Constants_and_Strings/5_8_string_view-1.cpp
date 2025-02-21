#include <iostream>
#include <string>
#include <string_view> // C++17

// str provides read-only access to whatever argument is passed in
void printSV(std::string_view str) // now a std::string_view
{
    std::cout << str << '\n';
}

void printString(std::string str) // str makes a copy of its initializer
{
    std::cout << str << '\n';
}

int main()
{
    std::string s{ "Hello, world!" }; // s makes a copy of its initializer
    printString(s);

    // The above example makes two copies of the C-style string “Hello, world!”: 
    // one when we initialize s in main(), and another when we initialize parameter str in printString(). 
    // That’s a lot of needless copying just to print a string!

    // -----------------------------------------------------------------------------------
    
    std::string_view sv{ "Hello, world!" }; 
    printSV(sv);

    // std::string_view provides read-only access to an existing string (a C-style string, a std::string, or another std::string_view) without making a copy. 
    // Read-only means that we can access and use the value being viewed, but we can not modify it.
    
    return 0;
}