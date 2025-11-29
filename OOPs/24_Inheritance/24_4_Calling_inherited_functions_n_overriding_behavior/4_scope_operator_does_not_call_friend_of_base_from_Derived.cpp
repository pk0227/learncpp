#include <iostream>

class Base
{
public:
    friend std::ostream& operator<<(std::ostream& out, const Base& ref)
    {
        out << "In Base\n";
        return out;
    }
};

class Derived : public Base
{
public:
    friend std::ostream& operator<<(std::ostream& out, const Derived& ref)
    {
        out << "In Derived\n";
        out << static_cast<const Base&>(ref);
        return out;
    }
};

int main()
{
    Derived d{};
    std::cout << d;
    return 0;
}