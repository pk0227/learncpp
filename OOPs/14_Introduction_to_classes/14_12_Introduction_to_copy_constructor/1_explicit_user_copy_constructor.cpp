#include <iostream>

class Fraction
{
    int m_numerator{0};
    int m_denominator{1};

public:
    // Default constructor
    Fraction(int num = 0, int den = 1) 
            : m_numerator{num}
            , m_denominator{den}
    {
        std::cout << "Parameterized con'tor\n";
    }
/*
    // Copy constructor
    Fraction(const Fraction& ref)
            : m_numerator{ref.m_numerator}
            , m_denominator{ref.m_denominator}
    {
        std::cout << "Copy con'tor called\n";
    }
*/
    // Explicitly request default copy constructor
    //Fraction(const Fraction& ref) = default;

    // Delete the copy constructor so no copies can be made
    Fraction(const Fraction& fraction) = delete;

    void print() const
    {
        std::cout << "Fraction(" << m_numerator << ", " << m_denominator << ")\n";
    }

};

void printFunction(Fraction f)      // f is pass by value
{
    f.print();
}

Fraction generateFraction(int n, int d)
{
    return Fraction{n, d};
}

int main()
{
    //Fraction f1{10, 2};     // Calls Fraction(int, int) constructor
    //f1.print();
    //Fraction f2{f1};        // Calls Fraction(const Fraction&) copy constructor
    //f2.print();

    Fraction f3{generateFraction(20, 4)};   // Fraction is returned using copy constructor
    printFunction(f3);          // f is copied into the function parameter using copy constructor

    return 0;
}