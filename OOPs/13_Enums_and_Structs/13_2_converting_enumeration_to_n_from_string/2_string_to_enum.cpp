#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <functional>

namespace AvailablePets
{
    enum Pet
    {
        Cow,
        Horse,
        Dog,
        Goat,
        Cat
    };

    std::vector<std::string> currentPets{"Cow", "Horse", "Dog", "Goat", "Cat"};
}


bool charEqualIgnoreCase(char a, char b)
{
    return std::tolower(static_cast<unsigned char>(a)) ==
           std::tolower(static_cast<unsigned char>(b));
}

bool iequals(const std::string& a, const std::string& b)
{
    if (a.size() != b.size())
        return false;

    return std::equal(a.begin(), a.end(), b.begin(), charEqualIgnoreCase);
}

std::optional<std::reference_wrapper<const std::string>> getPetMatch(const std::string& input)
//std::optional<std::string> getPetMatch(const std::string& input)
{
    for (const auto& pet : AvailablePets::currentPets)
    {
        if(iequals(pet,input))
            return pet;
    }
    
    return {};
}

int main()
{
    std::string input{};
    AvailablePets::Pet pet{};  // Pet pet{Cow} as Cow is 0.
    std::cout << "Enter pet type : ";
    //std::cin >> pet;            // Extraction operator >> doesn't know how to handle Pet Enum type. 
    std::cin >> input;

    
    //const std::optional<std::string>& e{getPetMatch(input)};
    auto e{getPetMatch(input)};     // auto deduces to std::optional<std::reference_wrapper<const std::string>>
    
    if(e)
    {
        //std::cout << "You have selected " << *e << "\n";
        std::cout << "You have selected " << e->get() << "\n";
    }
    else
        std::cout << "Sorry! The pet type is unavailable\n";

    return 0;
}