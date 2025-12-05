#include <iostream>
#include <memory>

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
    if(m_value)
        std::cout << m_value << "\n";
}

template <typename T>
class Storage<T*>
{
    std::unique_ptr<T> m_value{};       // use std::unique_ptr to automatically deallocate when Storage is destroyed
public:
    Storage(T* val) : m_value{std::make_unique<T>(val ? *val : 0)}      // or throw exception when !value
    { }

    void print() const;
};

template <typename T>
void Storage<T*>::print() const
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
    //Storage s{&d1};       // ok, copies d1 on heap
    //Storage s{nullptr};

    i.print();
    d.print();
    s.print();

    return 0;
}