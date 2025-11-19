#include <iostream>

struct Date
{
    int year{};
    int month{};
    int day{};

    void print()
    {
        std::cout << year << "/" << month << "/" << day << "\n";  
    }

    void print() const
    {
        std::cout << "const : " << year << "/" << month << "/" << day << "\n";  
    }
};

int main()
{
    const Date today{2025, 11, 18};     // Const objects must be initialized at the time of creation.
    today.print();

    Date today1{2025, 11, 18};          // non-const object
    today1.print();

    return 0;
}