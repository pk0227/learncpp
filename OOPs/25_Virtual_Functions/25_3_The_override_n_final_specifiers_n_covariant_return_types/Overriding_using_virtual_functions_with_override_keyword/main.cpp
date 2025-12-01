#include <iostream>
#include "animal.hpp"

//void report(const Cat& pCat)
//void report(const Dog& pDog)  
void report(const Animal& pAnimal)      // Base reference/pointer to derived types avoids dozens of overloaded functions.
{
    std::cout << "In report : " << pAnimal.getName() << " says " << pAnimal.speak() << "\n";
}

int main()
{
    const Cat cat{"Fred"};
    std::cout << "cat is named " << cat.getName() << ", and it says " << cat.speak() << '\n';

    const Dog dog{"Garbo"};
    std::cout << "dog is named " << dog.getName() << ", and it says " << dog.speak() << '\n';

    const Animal* pAnimal{&cat};
    std::cout << "pAnimal is named " << pAnimal->getName() << ", and it says " << pAnimal->speak() << '\n';

    pAnimal = &dog;
    std::cout << "pAnimal is named " << pAnimal->getName() << ", and it says " << pAnimal->speak() << '\n';

    report(cat);
    report(dog);

    return 0;
}