#include <iostream>
#include <string>
#include <string_view>
#include <format>
#include <utility>

class Base
{
public:
    friend std::ostream& operator<<(std::ostream& out, const Base& ref)
    {
        return ref.print(out);
    }
    virtual std::string_view getType() const { return "Base"; }
    virtual std::ostream& print(std::ostream& out) const
    {
        out << getType();
        return out;
    }
};

struct Employee
{
    std::string m_name{};
    int m_id{};
    friend std::ostream& operator<<(std::ostream& out, const Employee& ref)
    {
        std::string s_out{std::move(std::format("( {}, {} )", ref.m_name, ref.m_id))};
        out << s_out;
        return out;
    }
};

class Derived : public Base
{
    Employee m_employee{};
public:
    Derived(Employee e={}) : m_employee{e}
    { }
    std::string_view getType() const override { return "Derived"; }
    std::ostream& print(std::ostream& out) const override
    {
        out << getType();
        out << m_employee;
        return out;
    }
};

int main()
{
    Base b{};
    //Derived d{};
    Derived d{Employee{"Rama", 1024}};

    Base& bRef{d};

    std::cout << b << "\n";
    std::cout << d << "\n";
    std::cout << bRef << "\n"; 

    return 0;
}