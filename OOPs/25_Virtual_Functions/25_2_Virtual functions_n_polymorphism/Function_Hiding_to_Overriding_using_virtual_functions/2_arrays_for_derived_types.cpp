#include <iostream>
#include <array>
#include "animal.hpp"

int main()
{
    const Cat fred{ "Fred" };
    const Cat misty{ "Misty" };
    const Cat zeke{ "Zeke" };

    const Dog garbo{ "Garbo" };
    const Dog pooky{ "Pooky" };
    const Dog truffle{ "Truffle" };

    //const auto& cats{std::to_array<Cat>({{"Fred"}, {"Misty"}, {"Zeke"}})};
    //const auto& dogs{std::to_array<Dog>({{"Garbo"}, {"Pooky"}, {"Truffle"}})};

    //Befor C++20
    const std::array<const Animal*, 6> animals{&fred, &misty, &zeke, &garbo, &pooky, &truffle};

    for(const auto* animal : animals)
    {
        std::cout << animal->getName() << " says " << animal->speak() << "\n";
    }

    return 0;
}