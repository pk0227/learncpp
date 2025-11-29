#include <iostream>
#include <string>
#include <utility>
#include <iomanip>


class Person
{
    std::string m_name{};
    int m_age{};

public:
    Person(std::string name="", int age=0) : m_name{std::move(name)}, m_age{std::move(age)}
    { 
        std::cout << "Person Constructor\n";
    }

    const std::string& getName() const { return m_name; }
    int getAge() const { return m_age; }
};

class Employee
{
    std::string m_employer{};
    int m_salary{};

public:
    Employee(std::string employer="", int salary=0) : m_employer{std::move(employer)}, m_salary{std::move(salary)}
    { 
        std::cout << "Employee Constructor\n";
    }

    const std::string& getEmployer() const { return m_employer; }
    int getSalary() const { return m_salary; }
};

class Teacher : public Person, public Employee
{
    int m_teacherGrade{};
public:
    Teacher(std::string name="", int age=0, std::string employer="", int salary=0, int grade=0)
            : Person{std::move(name), std::move(age)}
            , Employee{std::move(employer), std::move(salary)}
            , m_teacherGrade{grade}
            {
                std::cout << "Teacher Constructor\n";
            }
    
    int getGrade() const { return m_teacherGrade; }
};

int main()
{
    Teacher t1{"Melissa", 26, "IIT", 1500000, 1};
    
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);

    std::cout << std::left;
    std::cout << std::setw(10) << "Name" << " : " << t1.getName() << "\n";
    std::cout << std::setw(10) << "Age" << " : " << t1.getAge() << "\n";
    std::cout << std::setw(10) << "Employer" << " : " << t1.getEmployer() << "\n";
    std::cout << std::setw(10) << "Salary" << " : " << t1.getSalary() << "\n";
    std::cout << std::setw(10) << "Grade" << " : " << t1.getGrade() << "\n";

    std::cout.copyfmt(oldState);

    return 0;
}