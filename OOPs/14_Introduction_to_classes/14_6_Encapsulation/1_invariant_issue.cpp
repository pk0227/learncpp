#include <iostream>
#include <string>

struct Employee
{
    std::string m_name{"John"};
    char m_firstInitial{'J'};

    void print() const
    {
        std::cout << m_name << " has first initial " << m_firstInitial << "\n";
    }
};

int main()
{
    Employee e{};
    e.print();

    e.m_name = "Mark";      // change employee's name to "Mark"
    e.print();              // prints wrong initial

    return 0;
}