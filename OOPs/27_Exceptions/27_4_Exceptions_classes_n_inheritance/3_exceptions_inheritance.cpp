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
    catch(const Base& e)
    {
        std::cerr << "Caught by Base\n";
    }
    catch(const Derived& e)                 // This block will never be reached because Derived is-a Base and will be caught by the above catch block
                                            // due to exception object slicing. 
                                            // To catch Derived exceptions specifically, place this catch block before the Base catch block.
    {
        std::cerr << "Caught by Derived\n";
    }

    return 0;
}