#include <iostream>

template <typename T>
class Storage
{
    T m_value{};
public:
    Storage(T val) : m_value{val}
    { }

    void print() const;
};

template<typename T>
void Storage<T>::print() const
{
    std::cout << m_value << "\n";
}

template<>
void Storage<double>::print() const
{
    std::cout << std::scientific << m_value << "\n";
}

template<>
void Storage<double*>::print() const
{
    if(m_value)
        std::cout << std::scientific << *m_value << "\n";
}

int main()
{
    Storage i{5};
    Storage d{6.7};

    double d1{123.456};
    double *dptr{&d1};

    Storage s{dptr};

    i.print();
    d.print();
    s.print();

    return 0;
}