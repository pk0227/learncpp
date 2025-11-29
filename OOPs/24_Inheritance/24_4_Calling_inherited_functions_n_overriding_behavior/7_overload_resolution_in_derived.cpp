#include <iostream>

class Base
{
public:
    void print(int) { std::cout << "Base - print - int\n"; }
    void print(double) { std::cout << "Base - print - double\n"; }
};

class Derived : public Base
{
public:
    //void print(int a) { Base::print(a); }       // It will call Base::print. But, It is bad way. not recommended.
    
    // using-declaration in Derived to make all Base functions with a certain name visible from within Derived
    using Base::print;          // It is good way. using declaraton will let Base::print participate in overload resolution.
    
    void print(double) { std::cout << "Derived - print - double\n"; }
};

int main()
{
    Derived d{};
    d.print(5);
    
    return 0;
}