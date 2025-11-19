#include <iostream>

class Person
{
    std::string m_name{};
public:
    void kisses(const Person& ref) const
    {
        std::cout << m_name << " kisses " << ref.m_name << "\n"; 
    }

    void setName(std::string_view name)
    {
        m_name = name;
    }
    const std::string& getName() const
    {
        return m_name;
    }
};

int main()
{
    Person joe;
    joe.setName("Joe");

    Person kate;
    kate.setName("Kate");

    joe.kisses(kate);

    std::cout << joe.getName() << "\n";
    std::cout << kate.getName() << "\n";

    return 0;
}