#include <iostream>

class Cents
{
    int m_cents{};
public:
    explicit Cents(int cents) : m_cents{cents}
    {
    }
	
    // add Cents + Cents using a friend function
    // This function is not considered a member of the class, even though the definition is inside the class
    friend Cents operator+(const Cents& left, const Cents& right)
    {
		// use the Cents constructor and operator+(int, int)
		// we can access m_cents directly because this is a friend function

        //return left.m_cents+right.m_cents;      // returning int but return type is Cents. But, no issues, with a non-explicit Cents(int) the return of an int would use the converting constructor to form a temporary
        return Cents{left.m_cents+right.m_cents};  
    }

    friend Cents operator-(const Cents& left, const Cents& right)
    {
        //return left.m_cents-right.m_cents;      // returning int but return type is Cents. But, no issues, with a non-explicit Cents(int) the return of an int would use the converting constructor to form a temporary
        return Cents{left.m_cents-right.m_cents};  
    }

    const int& getCents() const { return m_cents; }
};


int main()
{
    Cents c1{6};
    Cents c2{10};

    Cents sum{c1 + c2};
    Cents sub{c2-c1};

    std::cout << "Total Cents : " << sum.getCents() << "\n";
    std::cout << "Total Cents : " << sub.getCents() << "\n";

    return 0;
}