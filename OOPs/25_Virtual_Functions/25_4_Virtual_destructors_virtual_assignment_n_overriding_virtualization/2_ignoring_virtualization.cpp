#include <iostream>
#include <string_view>

// Demonstrates ignoring virtualization using the scope resolution operator Base::
class Base
{
public:
    virtual ~Base() = default;
    virtual std::string_view getName() const { return "Base"; }
};

class Derived : public Base
{
public:
    std::string_view getName() const override { return "Derived"; }
};

int main()
{
    Derived d{};
    const Base& rBase{ d };

    // Standard virtual call: resolves dynamically to Derived::getName()
    std::cout << "Normal virtual call:   " << rBase.getName() << '\n';

    // Ignoring virtualization: scope resolution bypasses dynamic dispatch and calls Base::getName() directly
    std::cout << "Bypassing virtual call: " << rBase.Base::getName() << '\n';

    return 0;
}
