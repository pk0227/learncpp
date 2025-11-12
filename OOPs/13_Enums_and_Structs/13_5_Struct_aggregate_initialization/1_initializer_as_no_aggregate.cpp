#include <iostream>

struct Employee
{
    int id {};
    int age {};
    double wage { 76000.0 };
    double whatever;

    Employee()
    {
        std::cout << "Default constructor\n";
    }

    Employee(int x, int y)
    {
        std::cout << "Param constructor\n";
    }

    Employee(std::initializer_list<int> list) {
        std::cout << "List constructor\n";
    }
};

std::ostream& operator<<(std::ostream& out, const struct Employee& emp)
{
    out << "id : " << emp.id << "\nage : " << emp.age << "\nwage : " << emp.wage << "\nwhatever : " << emp.whatever;
    return out;
}

int main()
{
    Employee emp1 { 2, 28 }; // emp1.whatever will be garbage as no value is provided in the explicit initializer list for memeber 'whatever' and the member doesnt have default member initializer.
                             // If any constructor exists (even = default), the struct/class is no longer an aggregate — {} now calls the appropriate constructor.
                             // If {} arguments don’t match any constructor parameters, compilation fails.
                             // { 2, 28 } looks for initializer_list c'tor first, if it doesnt exist , then it will looks for parameterized c'tor.
    std::cout << emp1 << "\n-----------------------------------\n";

    Employee emp2{};         // emp1.whatever will be garbage as no value is provided in the explicit initializer list for memeber 'whatever' and the member doesnt have default member initializer.
    std::cout << emp2 << "\n-----------------------------------\n";

    return 0;
}