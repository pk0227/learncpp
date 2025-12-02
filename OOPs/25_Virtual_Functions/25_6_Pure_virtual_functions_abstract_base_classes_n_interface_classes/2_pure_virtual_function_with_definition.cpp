#include <iostream>
#include <string>
#include <string_view>
#include <utility>

class Animal            // This Animal is an abstract base class
{
protected:
    std::string m_name{};
public:
    Animal(std::string name="") : m_name{std::move(name)}
    { }
    
    std::string_view getName() const { return m_name; }
    
    virtual std::string_view speak() const = 0;     // note that speak is now a pure virtual function

    virtual ~Animal() = default;
};

std::string_view Animal::speak() const      // even though it has a definition
{
    return "buzz";                          // some default implementation
}

class Dog : public Animal
{
public:
    Dog(std::string name="") : Animal{std::move(name)}
    { }

    std::string_view speak() const override { return "Boww"; }  // this class is no longer abstract because we defined this function

    virtual ~Dog() = default;
};

int main()
{
    Dog d{"Snoopy"};
    std::cout << "Dog : " << d.getName() << " says " << d.speak() << "\n";
    std::cout << "Dog : " << d.getName() << " says " << d.Animal::speak() << "\n";

    return 0;
}