#include <iostream>

class Base
{
public:
    Base() { }
};

class Derived : public Base
{
public:
    Derived() { }
};

int main()
{
    try
    {
        throw Derived{};
    }
    catch(const Derived& e)        // Place Derived catch block first to catch Derived exceptions specifically. Now this block will be reached.
    {
        std::cerr << "Caught by Derived\n";
    }
    catch(const Base& e)
    {
        std::cerr << "Caught by Base\n";
    }

    return 0;
}