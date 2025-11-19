#include <iostream>

struct Date
{
    int year{};
    int month{};
    int day{};

    void print() const
    {
        std::cout << year << "/" << month << "/" << day << "\n";  
    }
};

void doSomething(const Date& ref)
{
    ref.print();        // compilation error if print is non-const
}

int main()
{
    const Date today{2025, 11, 18};     // Const objects must be initialized at the time of creation.
    doSomething(today);

    Date today1{2025, 11, 18};          // non-const object
    doSomething(today1);

    return 0;
}