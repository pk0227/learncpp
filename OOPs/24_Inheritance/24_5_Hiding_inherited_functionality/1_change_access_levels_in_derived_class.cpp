#include <iostream>

class Base
{
    int m_value{};
protected:
    void print() { std::cout << "Base - print : " << m_value << "\n"; }
public:
    Base(int i=0) : m_value{i}
    { }
};

class Derived : public Base
{
public:
    using Base::print;
    Derived(int x=0) : Base{x}
    { }
};

int main()
{
    Base b{5};
    //b.print();      // error: ‘void Base::print()’ is protected within this context

    Derived d{10};
    d.print();

    return 0;
}