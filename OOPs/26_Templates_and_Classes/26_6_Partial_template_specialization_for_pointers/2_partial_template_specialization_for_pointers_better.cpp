#include <iostream>

template <typename T>       // This is our primary template class (same as previous)    
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

template <typename T>       // we still have a type template parameter
class Storage<T*>           // This is partially specialized for T*
{
    T* m_value{};
public:
    Storage(T* val) : m_value{val}
    { }

    void print() const;
};

template <typename T>
void Storage<T*>::print() const         // This is a non-specialized function of partially specialized class Storage<T*>
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

    Storage s{dptr};            // instantiates Storage<double*> from partially specialized class

    i.print();
    d.print();
    s.print();                  // calls Storage<double*>::print()

    return 0;
}