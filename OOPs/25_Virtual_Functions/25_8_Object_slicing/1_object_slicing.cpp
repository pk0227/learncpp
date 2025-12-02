#include <iostream>
#include <string_view>
#include <vector>

class Base
{
protected:
    int m_value{};
public:
    Base(int val=0) : m_value{val}
    { }

    int getVal() const { return m_value; }
    virtual std::string_view getName() const { return "Base"; }
};

class Derived : public Base
{
public:
    Derived(int val=0) : Base{val}
    { }
    std::string_view getName() const override { return "Derived"; }
};

int main()
{
    std::vector<Base> v{};

    Base b{5};
    Derived d{6};

    v.push_back(b);
    v.push_back(d);     //object slicing

    for(const auto& element : v)
        std::cout << element.getName() << "\n";

    return 0;
}