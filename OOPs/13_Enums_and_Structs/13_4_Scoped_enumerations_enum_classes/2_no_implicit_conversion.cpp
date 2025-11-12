#include <iostream>
#include <utility>

enum class Pet
{
    cow,
    horse
};

int main()
{
    //Pet pet{Pet::cow};
    int input{};

    std::cout << "Enter Pet : ";
    std::cin >> input;

    //Pet pet{1};    // As of C++17, you can list initialize a scoped enumeration using an integral value without the static_cast (unlike an unscoped enumeration)
    Pet pet{input};    // As of C++17, you can list initialize a scoped enumeration using an integral value without the static_cast (unlike an unscoped enumeration)

    //pet = 1;    // assignment of integral wont work
    //pet = input;    // assignment of integral wont work

    if(pet == Pet::horse)  //  you can still compare enumerators from within the same scoped enumeration (since they are of the same type)
    {
        //std::cout << "pet is " << pet << "\n";  // compilation error. unlike unscoped enums, scoped enums have no implicit int conversion.
        std::cout << static_cast<int>(pet) << "\n";
        std::cout << std::to_underlying(pet) << "\n";
    }

    return 0;
}