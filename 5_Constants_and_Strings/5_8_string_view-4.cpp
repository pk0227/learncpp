// Assignment changes what the std::string_view is viewing
// constexpr std::string_view

#include <iostream>
#include <string>
#include <string_view>

int main()
{
    std::string name { "Rama" };
    std::string_view sv { name }; // sv is now viewing name
    std::cout << sv << '\n'; // prints Rama

    sv = "Seetha"; // sv is now viewing "John" (does not change name)
    std::cout << sv << '\n'; // prints Seetha

    std::cout << name << '\n'; // prints Rama

	constexpr std::string_view s{ "Hello, world!" }; // s is a string symbolic constant
    std::cout << s << '\n'; // s will be replaced with "Hello, world!" at compile-time

    return 0;
}