#include <iostream>

class Base
{
public:
    friend std::ostream& operator<<(std::ostream& out, const Base& ref)
    {
        out << "Base";
        return out;
    }
};

class Derived : public Base
{
public:
    friend std::ostream& operator<<(std::ostream& out, const Derived& ref)
    {
        out << "Derived";
        return out;
    }
};

int main()
{
    Base b{};
    Derived d{};

    Base& bRef{d};

    std::cout << b << "\n";
    std::cout << d << "\n";
    std::cout << bRef << "\n";      // Issue : We dont see polymorphic behavior here.

    return 0;
}