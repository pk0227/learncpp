#include <iostream>

class A
{
    int m_A{};
public:
    A(int a=0) : m_A{a}
    {
        std::cout << "A - " << m_A << "\n";
    }
};

class B : public A
{
    double m_B{};
public:
    B(int a=0, double b=0) : A{a}, m_B{b}
    {
        std::cout << "B - " << m_B << "\n";
    }
};

class C : public B
{
    char m_C{};
public:
    C(int a=0, double b=0, char c=0) : B{a, b}, m_C{c}
    {
        std::cout << "C - " << m_C << "\n";
    }
};

int main()
{
    C c{10, 12.345, 'P'};

    return 0;
}