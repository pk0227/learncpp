#include <iostream>
#include <string>
#include <string_view>
#include <utility>

class Animal            // This Animal is an interface base class
{
public:
    virtual std::string_view getName() const = 0;
    virtual std::string_view speak() const = 0; 

    virtual ~Animal() = default;
};

std::string_view Animal::getName() const      // even though it has a definition
{
    return "dummyName";                          // some default implementation
}

std::string_view Animal::speak() const      // even though it has a definition
{
    return "buzz";                          // some default implementation
}

class Dog : public Animal
{
protected:
    std::string m_name{};
public:
    Dog(std::string name="") : m_name{std::move(name)}
    { }

    std::string_view getName() const override { return m_name; }
    std::string_view speak() const override { return "Boww"; }  // this class is no longer abstract because we defined this function

    virtual ~Dog() = default;
};

int main()
{
    Dog d{"Snoopy"};
    std::cout << "Dog : " << d.getName() << " says " << d.speak() << "\n";
    std::cout << "Dog : " << d.Animal::getName() << " says " << d.Animal::speak() << "\n";

    return 0;
}