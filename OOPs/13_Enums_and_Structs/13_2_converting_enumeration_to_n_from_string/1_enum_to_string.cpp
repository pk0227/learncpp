#include <iostream>

enum Pet
{
    Cow,
    Horse,
    Dog,
    Goat,
    Cat
};

//constexpr std::string_view getPetName(const Pet& pet) 
consteval std::string_view getPetName(const Pet& pet) 
{
    switch(pet)
    {
        case Cow:   return "Cow";
        case Horse: return "Horse";
        case Dog:   return "Dog";
        case Goat:  return "Goat";
        case Cat:   return "Cat";
        default:    return "???";
    }
}

int main()
{
    // Pet pet{Cow};               // error: the value of ‘pet’ is not usable in a constant expression, especially consteval function
    constexpr Pet pet{Cow};
    // pet = Horse;            // error: assignment of read-only variable ‘pet’

    std::cout << "Your Pet is " << getPetName(pet) << "\n";

    return 0;
}