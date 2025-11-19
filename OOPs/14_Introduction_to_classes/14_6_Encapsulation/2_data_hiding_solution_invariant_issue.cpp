#include <iostream>
#include <string>
#include <string_view>

class Employee
{
    std::string m_name{"John"};
    char m_firstInitial{'J'};

public:
    void print() const
    {
        std::cout << m_name << " has first initial " << m_firstInitial << "\n";
    }

    void setName(const std::string_view name)
    {
        m_name = name;
        m_firstInitial = name.front();
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