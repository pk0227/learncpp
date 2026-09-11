/**
 * @file 3_inheriting_constructors.cpp
 * @brief Demonstrates inheriting base class constructors using 'using Base::Base;' in C++11.
 *
 * Concepts covered:
 * 1. Traditional boilerplate forwarding constructors in derived classes.
 * 2. C++11 'using Base::Base;' syntax to inherit all base constructors.
 * 3. Default member initialization of derived class members when using inherited constructors.
 */

#include <iostream>
#include <string>
#include <string_view>

class Base
{
    int m_id{};
    std::string m_name{};

public:
    Base() : m_id{0}, m_name{"Default"} 
    {
        std::cout << "Base default constructor: id=" << m_id << ", name=" << m_name << '\n';
    }

    Base(int id) : m_id{id}, m_name{"Named"}
    {
        std::cout << "Base(int) constructor: id=" << m_id << ", name=" << m_name << '\n';
    }

    Base(int id, std::string_view name) : m_id{id}, m_name{name}
    {
        std::cout << "Base(int, string_view) constructor: id=" << m_id << ", name=" << m_name << '\n';
    }
};

class Derived : public Base
{
    // Derived member with a default member initializer
    double m_cost{99.99};

public:
    // C++11 Inheriting constructors:
    // Inherits Base(int) and Base(int, string_view) without boilerplate forwarding!
    using Base::Base;

    // Derived can still define its own constructors if desired
    Derived(int id, double cost) : Base{id}, m_cost{cost}
    {
        std::cout << "Derived custom constructor: cost=" << m_cost << '\n';
    }

    void printCost() const
    {
        std::cout << "Derived cost: " << m_cost << '\n';
    }
};

int main()
{
    std::cout << "--- 1. Calling inherited Base(int) constructor ---\n";
    Derived d1{42};
    d1.printCost(); // m_cost receives its default member initializer (99.99)

    std::cout << "\n--- 2. Calling inherited Base(int, string_view) constructor ---\n";
    Derived d2{100, "Alpha"};
    d2.printCost();

    std::cout << "\n--- 3. Calling Derived's own custom constructor ---\n";
    Derived d3{200, 149.50};
    d3.printCost();

    return 0;
}
