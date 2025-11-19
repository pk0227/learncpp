#include <iostream>

class Dollars
{
    int m_dollars{};

public:
    explicit Dollars(int);

    const int& getDollars() const { return m_dollars; }
};

// The explicit keyword is used only on the declaration.
Dollars::Dollars(int d) : m_dollars{d}
{
}

void printDollars(const Dollars& d)
{
    std::cout << "$" << d.getDollars() << "\n";
}

int main()
{
    // Explicit constructors can be used for direct and direct list initialization
    Dollars d1{20};
    printDollars(d1);
    Dollars d2(10);
    printDollars(d2);

    //printDollars(5);    // compilation error because Dollars(int) is explicit
    printDollars(Dollars{5});
    printDollars(static_cast<Dollars>(5));
    
    return 0;
}