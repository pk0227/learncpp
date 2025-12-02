#include <iostream>
#include <string_view>

class Base
{
public:
    friend std::ostream& operator<<(std::ostream& out, const Base& ref)
    {
        out << ref.getType();
        return out;
    }
    virtual std::string_view getType() const { return "Base"; } 
};

class Derived : public Base
{
public:
    std::string_view getType() const override { return "Derived"; }
};

int main()
{
    Base b{};
    Derived d{};

    Base& bRef{d};

    std::cout << b << "\n";
    std::cout << d << "\n";
    std::cout << bRef << "\n"; 

    return 0;
}