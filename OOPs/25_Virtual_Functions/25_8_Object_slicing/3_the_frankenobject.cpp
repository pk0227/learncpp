#include <iostream>
#include <string>

// Demonstrates the Frankenobject problem when assigning through a base reference
class Base
{
protected:
    int m_baseVal{};
public:
    Base(int baseVal = 0) : m_baseVal{ baseVal } {}
    virtual ~Base() = default;

    virtual void print() const
    {
        std::cout << "Base val: " << m_baseVal << '\n';
    }
};

class Derived : public Base
{
    std::string m_derivedName{};
public:
    Derived(int baseVal, std::string derivedName)
        : Base{ baseVal }, m_derivedName{ std::move(derivedName) }
    {}

    void print() const override
    {
        std::cout << "Base val: " << m_baseVal << ", Derived name: " << m_derivedName << '\n';
    }
};

int main()
{
    Derived d1{ 10, "First" };
    Derived d2{ 20, "Second" };

    std::cout << "Initial state:\n";
    std::cout << "d1: "; d1.print();
    std::cout << "d2: "; d2.print();

    // Base reference bound to d2
    Base& b{ d2 };

    // Problematic assignment:
    // Because operator= in Base is NOT virtual, this calls Base::operator=(const Base&).
    // Only the Base subobject of d1 (m_baseVal: 10) is copied into d2.
    // The Derived portion of d2 (m_derivedName: "Second") remains unchanged!
    b = d1;

    std::cout << "\nAfter b = d1 (Frankenobject created):\n";
    std::cout << "d2: "; d2.print(); // Output: Base val: 10, Derived name: Second

    return 0;
}
