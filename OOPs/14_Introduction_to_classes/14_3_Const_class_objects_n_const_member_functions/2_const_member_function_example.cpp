#include <iostream>

struct Date;
void nonMemberFuction1();
void nonMemberFuction2(Date*);

struct Date
{
    int year{};
    int month{};
    int day{};

    void print() const;
    
    void constFunc1() const 
    {
        std::cout << "constFunc1\n";
    }

    void func2()
    {
        std::cout << "func2\n"; 
    }
};

//  the const keyword must be used on both the function declaration in the class definition if the function is defined outside the class. 
void Date::print() const  
{
    int i{10};
    i = 12;         // const member functions can modify non-members (such as local variables and function parameters).
    std::cout << year << "/" << month << "/" << day << "\n"; 
    
    constFunc1();   // const member function can call another const member function.
    
    //func2();   // const member function can't call non-const member function.
    //const_cast<Date* const>(this)->func2();     // Dont do the droping the const to the const obj using const_cast.
    
    nonMemberFuction1(); // const member function can call non-member function.
    //nonMemberFuction2(this); // const member function cant call non-member function with parameter of pointer to non-const type. 
}

void nonMemberFuction1()
{
    std::cout << "Non Member Function1\n";
}

void nonMemberFuction2(Date* date)
{
    std::cout << "Non Member Function2\n";
}

int main()
{
    const Date today{2025, 11, 18};     // Const objects must be initialized at the time of creation.
    today.print();                      // Const objects can only call const member functions.

    Date today1{2025, 11, 18};          // non-const object
    today1.print();                     // const member function can be called on non-const object.

    return 0;
}