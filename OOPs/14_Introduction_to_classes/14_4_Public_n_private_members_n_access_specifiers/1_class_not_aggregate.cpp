/*
Conditions to be an aggregate:
    1. The class type should not have any private or protected members.
    2. The class type should not have any user-declared constructors (including defaulted or deleted constructors).

A class with private members or user-declared constructors is no longer an aggregate.
Therefore, the class cannot use aggregate initialization.
In such cases, brace initialization will attempt to call a matching constructor.
If a constructor taking std::initializer_list is defined, it will take precedence over other constructors.
*/

#include <iostream>
/*
struct Date
{
    int year{};
    int month{};
    int day{};

    Date() = default;
    //Date() { };

    void print() const
    {
        std::cout << "const : " << year << "/" << month << "/" << day << "\n";  
    }
};
*/

class Date
{
    int year{};
    int month{};
    int day{};

public:         // made print publicly accessible using public access specifier.
    void print() const
    {
        std::cout << "const : " << year << "/" << month << "/" << day << "\n";  
    }
};

int main()
{
    //const Date today{2025, 11, 18};     // Error: Date is not an aggregate (has private members), needs a constructor.
    const Date today{};     
    today.print();

    //Date today1{2025, 11, 18};          // Error: Date is not an aggregate (has private members), needs a constructor.
    Date today1{};          
    today1.print();

    return 0;
}