// Be careful returning a std::string_view

#include <iostream>
#include <string>
#include <string_view>

std::string_view firstAlphabetical(std::string_view s1, std::string_view s2)
{
    if (s1 < s2)
        return s1;
    return s2;
}

int main()
{
    std::string a { "World" };
    std::string b { "Hello" };

    std::cout << firstAlphabetical(a, b) << '\n'; // prints "Hello". It is okay.

    using namespace std::string_literals;
    std::cout << firstAlphabetical("india"s, "Bharat"s) << '\n';  // This is also okay.
    

    // The "india"s and "Bharat"s create temporary std::string objects, and firstAlphabetical returns a std::string_view to one of these temporary objects. 
    // However, these temporary std::string objects are destroyed at the end of the full expression, leaving sv with a dangling reference.
    std::string_view sv{firstAlphabetical("india"s, "Bharat"s)};
    std::cout << sv << '\n';  // sv points to dangling reference. 

    return 0;
}