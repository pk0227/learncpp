#include <iostream>

class Accumulator
{
    friend void print(const Accumulator&);      // it doesn't matter if friend is declared/defined under private, protected or public.
/*
    //  friend function can also be defined in a class. Friend functions defined inside a class are non-member functions
    friend void print(const Accumulator& ref)
    {
        std::cout << ref.m_value << "\n";
    }
*/
    int m_value{};
public:
    void add(int val) { m_value += val; }

    /*
    //friend void print(const Accumulator&);

    //  friend function can also be defined in a class. Friend functions defined inside a class are non-member functions
    friend void print(const Accumulator& ref)
    {
        std::cout << ref.m_value << "\n";
    }
*/

};


void print(const Accumulator& ref)
{
    std::cout << ref.m_value << "\n";
}


int main()
{
    Accumulator acc{};
    acc.add(5);
    print(acc);

    return 0;
}