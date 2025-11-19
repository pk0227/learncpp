// A const object can only call const member functions (and static functions).

#include <iostream>

struct Date
{
    int year{};
    int month{};
    int day{};

    void incrementDay() 
    {
        day++;
    }

//    void print() const
    void print()
    {
        std::cout << year << "/" << month << "/" << day << "\n"; 
    }
};

int main()
{
    const Date today{2025, 11, 18};     // Const objects must be initialized at the time of creation.

    today.day += 1;                     // changing member variables directly (if they are public) NOT allowed.

    today.incrementDay();               // calling member functions that set the value of member variables NOT allowed

    today.print();                      // Const objects may not call non-const member functions.

    return 0;
}