#include <iostream>
#include <string>
#include <string_view>

class Employee
{
    std::string m_name{"John"};

public:
    void print() const
    {
        std::cout << m_name << " has first initial " << getFirstInitial() << "\n";
    }

    void setName(const std::string_view name)
    {
        m_name = name;
    }

    char getFirstInitial() const
    {
        return m_name.front();
    }
};

int main()
{
    Employee e{};
    e.print();

    e.setName("Mark");      // change employee's name to "Mark"
    e.print();              // prints wrong initial

    return 0;
}