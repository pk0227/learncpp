#include <iostream>
#include "animal.hpp"

int main()
{
    //const auto& cats{std::to_array<Cat>({{"Fred"}, {"Misty"}, {"Zeke"}})};
    //const auto& dogs{std::to_array<Dog>({{"Garbo"}, {"Pooky"}, {"Truffle"}})};

    //Befor C++20
    const std::array<Dog, 3> dogs{{ {"Garbo"}, {"Pooky"}, {"Truffle"} }};
    const std::array<Cat, 3> cats{{ {"Fred"}, {"Misty"}, {"Zeke"} }};

    for(const auto& cat : cats)
    {
        std::cout << cat.getName() << " says " << cat.speak() << "\n";
    }

    for(const auto& dog : dogs)
    {
        std::cout << dog.getName() << " says " << dog.speak() << "\n";
    }

    return 0;
}