#include <iostream>

class Cents
{
    int m_cents{};

public:
    Cents(int cents=0) : m_cents{cents}
    { }
    
    explicit operator int() const { return m_cents; }       // now marked as explicit
};

class Dollars
{
    int m_dollars{};

public:
    Dollars(int dollars=0) : m_dollars{dollars}
    { }

    operator Cents() const { return Cents{m_dollars*100}; }
};

void printCents(const Cents& obj)
{
//  std::cout << cents;                   // no longer works because cents won't implicit convert to an int
    std::cout << "Cents : " << static_cast<int>(obj);   // we can use an explicit cast instead
}

int main()
{
    Dollars d1{6};
    printCents(d1);     // implicit conversion from Dollars to Cents okay because its not marked as explicit

    std::puts("");
    return 0;
}