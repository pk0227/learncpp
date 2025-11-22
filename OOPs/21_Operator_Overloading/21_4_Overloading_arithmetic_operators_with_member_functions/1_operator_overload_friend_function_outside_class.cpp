#include <iostream>

class Cents
{
    int m_cents{};
public:
    explicit Cents(int cents) : m_cents{cents}
    {
    }

    // add Cents + Cents using a member function
    Cents operator+(const Cents&) const;
    Cents operator+(int) const;

    // subtract Cents - Cents using a member function
    Cents operator-(const Cents&) const;
    Cents operator-(int) const;

    const int& getCents() const { return m_cents; }
};

// note: this function is a member function!
Cents Cents::operator+(const Cents& right) const
{
    // use the Cents constructor and operator+(int, int)
	// we can access m_cents directly because this is a member function
    
    //return left.m_cents+right.m_cents;      // returning int but return type is Cents. But, no issues, with a non-explicit Cents(int) the return of an int would use the converting constructor to form a temporary
    return Cents{m_cents+right.m_cents};  
}

Cents Cents::operator+(int value) const
{    
    return Cents{m_cents+value};  
}

// note: this function is a member function!
Cents Cents::operator-(const Cents& right) const
{
    // use the Cents constructor and operator-(int, int)
	// we can access m_cents directly because this is a member function

    //return left.m_cents-right.m_cents;      // returning int but return type is Cents. But, no issues, with a non-explicit Cents(int) the return of an int would use the converting constructor to form a temporary
    return Cents{m_cents-right.m_cents};  
}

Cents Cents::operator-(int value) const
{    
    return Cents{m_cents-value};  
}

int main()
{
    Cents c1{6};
    Cents c2{10};

    Cents sum{c1 + c2};
    Cents sum1{c1 + 3};
    Cents sub{c2-c1};
    Cents sub1{c2-5};

    std::cout << "Total Cents : " << sum.getCents() << "\n";
    std::cout << "Total Cents : " << sum1.getCents() << "\n";
    std::cout << "Total Cents : " << sub.getCents() << "\n";
    std::cout << "Total Cents : " << sub1.getCents() << "\n";

    return 0;
}