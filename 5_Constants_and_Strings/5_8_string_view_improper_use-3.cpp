// Improperly using std::string_view

#include <iostream>
#include <string>
#include <string_view>

int main()
{
    using namespace std::string_literals;
    std::string_view name { "Rama"s }; // "Rama"s creates a temporary std::string
    std::cout << name << '\n'; // undefined behavior

    return 0;
}