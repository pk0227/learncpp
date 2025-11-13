#include <iostream>
#include <string>
#include <string_view>

struct Owner
{
    std::string name{}; // std::string is an owner
};

struct Viewer
{
    std::string_view sv{};  // std::string_view is a viewer
};

// getName() returns the user-entered string as a temporary std::string
// This temporary std::string will be destroyed at the end of the full expression
// containing the function call.
std::string getName()
{
    std::string str{};              
    std::cout << "Enter Name : ";
    std::cin >> str;
    return str;
}

int main()
{
    Owner o{getName()};                         // The return value of getName() is destroyed just after initialization
    std::cout << "Owner is " << o.name << "\n";

    Viewer v{getName()};                        // The return value of getName() is destroyed just after initialization
    std::cout << "Viewer is " << v.sv << "\n";
    
    return 0;
}