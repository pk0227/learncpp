// std::string_view can be initialized with many different types of strings
// std::string_view parameters will accept many different types of string arguments

#include <iostream>
#include <string>
#include <string_view>

void printSV(std::string_view str)
{
    std::cout << str << '\n';
}

int main()
{
    std::string_view s1 { "Hello, world!" }; // initialize with C-style string literal
    std::cout << s1 << '\n';

    std::string s{ "Hello, world!" };
    std::string_view s2 { s };  // initialize with std::string
    std::cout << s2 << '\n';

    std::string_view s3 { s2 }; // initialize with std::string_view
    std::cout << s3 << '\n';

    // -------------------------------------------------------------------------------------

    printSV("Hello, world!"); // call with C-style string literal

    printSV(s); // call with std::string

    printSV(s2); // call with std::string_view


    return 0;
}