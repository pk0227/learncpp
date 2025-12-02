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

class Dog : public Animal
{
public:
    Dog(std::string name="") : Animal{std::move(name)}
    { }

    std::string_view speak() const override { return "Boww"; }

    virtual ~Dog() = default;
};

int main()
{
    Dog d{"Snoopy"};
    std::cout << "Dog : " << d.getName() << " says " << d.speak() << "\n";

    return 0;
}