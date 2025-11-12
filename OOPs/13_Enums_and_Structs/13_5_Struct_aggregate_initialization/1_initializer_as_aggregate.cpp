#include <iostream>

struct Employee
{
    int id {};
    int age {};
    double wage { 76000.0 };
    double whatever;
};

std::ostream& operator<<(std::ostream& out, const struct Employee& emp)
{
    out << "id : " << emp.id << "\nage : " << emp.age << "\nwage : " << emp.wage << "\nwhatever : " << emp.whatever;
    return out;
}

int main()
{
    Employee emp1 { 2, 28 }; // emp1.whatever will be value-initialized to 0.0
                             // Since class/struct has no user-declared constructors, {} performs aggregate initialization — directly initializing members.
    Employee emp2{};         // {} without values still does aggregate initialization, but members get value-initialized (zeroed) if members have no default member initializer.

    std::cout << emp1 << "\n-----------------------------------\n";
    std::cout << emp2 << "\n-----------------------------------\n";
    return 0;
}