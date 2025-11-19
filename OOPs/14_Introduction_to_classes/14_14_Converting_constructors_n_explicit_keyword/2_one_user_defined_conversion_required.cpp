#include <iostream>
#include <string>
#include <string_view>

class Employee
{
private:
    std::string m_name{};

public:
    Employee(std::string_view name)
        : m_name{ name }
    {
    }

    const std::string& getName() const { return m_name; }
};

void printEmployee(Employee e) // has an Employee parameter
{
    std::cout << e.getName() << "\n";
}

int main()
{
    //printEmployee("Joe"); // we're supplying an string literal argument
                          // error: could not convert ‘(const char*)"Joe"’ from ‘const char*’ to ‘Employee’
    
    using namespace std::literals;
    printEmployee( "Rama"sv); // now a std::string_view literal

    printEmployee(Employee{ "Krishna" });

    printEmployee(static_cast<std::string_view>("Hanuman"));

    return 0;
}