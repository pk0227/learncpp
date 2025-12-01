#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <iostream>
#include <string>
#include <utility>

class Animal
{
protected:
    std::string m_name{};
public:
    Animal(std::string name="") : m_name{std::move(name)}
    { }

    // To prevent slicing
    Animal(const Animal&) = delete;
    Animal& operator=(const Animal&) = delete;

    std::string_view getName() const { return m_name; }
    virtual std::string_view speak() const { return "???"; }   
};

class Dog : public Animal
{
public:
    Dog(std::string name="") : Animal{std::move(name)}
    { }

    std::string_view speak() const override { return "Boww..."; }  
};

class Cat : public Animal
{
public:
    Cat(std::string name="") : Animal{std::move(name)}
    { }

    std::string_view speak() const override { return "Meow..."; }   
};

#endif