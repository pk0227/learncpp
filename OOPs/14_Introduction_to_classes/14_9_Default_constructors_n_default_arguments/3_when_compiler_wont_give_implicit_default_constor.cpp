#include <iostream>
#include <string>

class Person
{
    std::string m_name{};
    int m_age{};

public:
    Person(const std::string_view name, const int age) : m_name{name}, m_age{age}
    {
    }

    void print()
    {
        std::cout << "name : " << m_name << "\nage : " << m_age << "\n";
    }
};

int main()
{
    Person p1{"Rama", 30};
    p1.print();

    Person p2{};        // compiler error : no matching constructor.
    p2.print();

    return 0;
}