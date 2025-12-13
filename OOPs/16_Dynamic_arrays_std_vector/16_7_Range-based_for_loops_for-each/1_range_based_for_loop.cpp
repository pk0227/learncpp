#include <iostream>
#include <vector>
#include <string>
#include <ranges>

int main()
{
    std::vector<std::string> vstr{"Rama", "Seetha", "Krishna", "Rukmini"};

    //for(const std::string& str : vstr)
    for(const auto& str : vstr)
        std::cout << str << " ";
    
        std::cout << "\n";

    for(const auto& str : std::views::reverse(vstr))
        std::cout << str << " ";
    
        std::cout << "\n";

    return 0;
}