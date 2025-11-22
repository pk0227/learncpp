#ifndef CENTS_HPP
#define CENTS_HPP

class Cents
{
    int m_cents{};
public:
    explicit Cents(int cents) : m_cents{cents}
    {
    }

    const int& getCents() const { return m_cents; }
};

Cents operator+(const Cents&, const Cents&);
Cents operator-(const Cents&, const Cents&);

#endif