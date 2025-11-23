#include <iostream>

class Digit
{
    int m_digit{};

public:
    Digit(int digit=0) : m_digit{digit}
    { }

    Digit& operator++();         // prefix increment
    Digit operator++(int);      // postfix increment

    Digit& operator--();         // prefix decrement 
    Digit operator--(int);      // postfix decrement

    friend std::ostream& operator<<(std::ostream&, const Digit&);
};

Digit& Digit::operator++()
{
    if(m_digit == 9)
        m_digit = 0;
    else
        ++m_digit;

    return *this;
}

Digit Digit::operator++(int)
{
    Digit temp{*this};
    ++(*this);
    return temp;
}

Digit& Digit::operator--()
{
    if(m_digit == 0)
        m_digit = 9;
    else
        --m_digit;

    return *this;
}

Digit Digit::operator--(int)
{
    Digit temp{*this};
    --(*this);
    return temp;
}

std::ostream& operator<<(std::ostream& out, const Digit& obj)
{
    out << obj.m_digit;
    return out;
}

int main()
{
    Digit digit { 5 };

    std::cout << digit;
    std::cout << ++digit; // calls Digit::operator++();
    std::cout << digit++; // calls Digit::operator++(int);
    std::cout << digit;
    std::cout << --digit; // calls Digit::operator--();
    std::cout << digit--; // calls Digit::operator--(int);
    std::cout << digit;

    std::puts("");

    return 0;
}