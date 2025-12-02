#include <iostream>
#include <string>
#include <string_view>
#include <utility>

class Base
{
protected:
    int m_value{};
public:
    virtual ~Base() = default;
};

class Derived : public Base
{
protected:
    std::string m_name{};
public:
    Derived(std::string name="") : m_name{std::move(name)}
    { }
    std::string_view getName() const { return m_name; }
};

int main()
{
    Derived d{"Hello World"};
    Base b{};

    Base* bptr{&d};
    //Base* bptr{&b};


    Derived *dptr{dynamic_cast<Derived*>(bptr)};
    if(dptr)
        std::cout << "downcast success : " << dptr->getName() << "\n";
    else
        std::cout << "downcast FAIL\n";

    return 0;
}