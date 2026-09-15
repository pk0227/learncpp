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

    // COMPILE ERROR: changing member variables directly (if they are public) NOT allowed on const object.
    // today.day += 1;

    // COMPILE ERROR: calling non-const member functions that set the value of member variables NOT allowed on const object.
    // today.incrementDay();

    // COMPILE ERROR: Const objects may not call non-const member functions.
    // today.print();

    return 0;
}