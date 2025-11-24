#include <iostream>

class Cents
{
    int m_cents{};

public:
    Cents(int cents=0) : m_cents{cents}
    { }

    Cents& operator+=(const Cents&);
    Cents& operator/=(int);
    const int& getCents() const { return m_cents; }
};

Cents& Cents::operator+=(const Cents& ref)
{
    m_cents+=ref.m_cents;
    return *this;
}

Cents& Cents::operator/=(int num)
{
    m_cents/=num;
    return *this;
}

template<typename T, size_t N>
T centsAverage(const T (&refArr)[N])
{
    T sum{};  // Default initialization instead of assuming T can be constructed from 0
    for(const T& i : refArr)
    {
        sum += i;
    }

    sum /= std::size(refArr);

    return sum;
}


std::ostream& operator<<(std::ostream& out, const Cents& ref)
{
    out << ref.getCents();
    return out;
}

int main()
{
    Cents cArray[]{ Cents{5}, Cents{10}, Cents{15}, Cents{20}, Cents{30} };
    std::cout << centsAverage(cArray) << "\n";
    return 0;
}