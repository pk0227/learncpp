// Member functions can be overloaded

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

    void print(const std::string_view prefix)
    {
        std::cout << prefix << " : " << year << "/" << month << "/" << day << "\n";
    }
};

int main()
{
    Date today{2025, 11, 18};
    today.print();
    std::cout << "-----------------------------------------\n";
    today.print("Today Date is");
    
    return 0;
}