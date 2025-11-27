#include <iostream>

class Teacher
{
    std::string m_name{};
public:
    Teacher(const std::string_view name="") : m_name{name}
    { }

    const std::string& getName() const { return m_name; }
};

class Department
{
    const Teacher& m_teacher{};
public:
    Department(const Teacher& teacher) : m_teacher{teacher}
    { } 
};

int main()
{
    Teacher bob{"Bob"};
    {
        Department department{bob};
    }

    std::cout << bob.getName() << " - still exists\n";
    return 0;
}