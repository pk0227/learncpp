#include <iostream>
#include <string>
#include <string_view>

class Base
{
public:
    virtual ~Base() = default;
};

class Test : public Base 
{
    int m_val{2};
    Test() = default;
public:
    friend void print(const Test&);
    friend Base* print();
    std::string_view getType() const { return "Test"; }    
};

void print(const Test& ref)
{
    std::cout << "print : " << ref.m_val << "\n";
}

Base* print()
{
    Base* bptr{new Test{}};
    return bptr;
}

int main()
{
    //Test t{};
    
    Base* bptr{print()};
 
    Test *tptr{dynamic_cast<Test*>(bptr)};
    
    std::cout << tptr->getType() << "\n";

    
    return 0;
}