// 14_3_Const_class_objects_n_const_member_functions/6_mutable_date_misuse.cpp
// Shows how mutable works on a const object, and why using it for REAL data is a misuse.
//
// 'mutable' exempts ONE member from the const lock. It can then be modified:
//   1. directly, through a const object (if the member is public)
//   2. inside a const member function
// Everything that is not mutable stays locked.
//
// MISUSE WARNING: day is part of what a Date means. Changing it on a const Date
// breaks the promise that const makes. Use mutable only for hidden bookkeeping
// (counters, caches, mutexes). See 5_mutable_data_members.cpp for the intended use.

#include <iostream>

struct Date
{
    int year{};
    int month{};
    mutable int day{};                      // exempt from the const lock (misuse: real data)

    void incrementDay() const { day++; }    // OK: day is mutable

    void print() const
    {
        std::cout << year << "/" << month << "/" << day << "\n";
    }
};

int main()
{
    const Date today{ 2025, 11, 18 };       // const object
    today.print();                          // 2025/11/18

    today.day += 1;                         // OK: direct modification of a mutable member
    today.print();                          // 2025/11/19

    today.incrementDay();                   // OK: const member function modifies a mutable member
    today.print();                          // 2025/11/20

    // The other members are still locked by const. Uncomment to see the errors:
    // today.year = 2026;                   // ERROR: year is not mutable
    // today.month += 1;                    // ERROR: month is not mutable

    // The misuse: a "const" Date changed its own date, three times, with no error.
    // A reader of 'const Date today' expects the date to stay 2025/11/18.
    return 0;
}
