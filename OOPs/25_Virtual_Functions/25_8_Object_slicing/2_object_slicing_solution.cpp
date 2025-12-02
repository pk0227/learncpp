#include <iostream>
#include <string_view>
#include <vector>
#include <functional>

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
    //std::vector<Base> v{};
    //std::vector<Base&> v{};     // compilation error : The elements of std::vector must be assignable, whereas references can’t be reassigned (only initialized).
    //std::vector<Base*> v{};
    std::vector<std::reference_wrapper<Base>> v{};

    Base b{5};
    Derived d{6};

    v.push_back(b);
    v.push_back(d); 

    for(const auto& element : v)
        std::cout << element.get().getName() << "\n";

    return 0;
}