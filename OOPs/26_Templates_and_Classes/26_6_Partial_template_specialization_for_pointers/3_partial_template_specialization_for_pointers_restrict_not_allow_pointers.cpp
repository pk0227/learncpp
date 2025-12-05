#include <iostream>
#include <type_traits>      // for std::is_pointer_v and std::is_null_pointer_v

template <typename T>
class Storage
{
    // Make sure T isn't a pointer or a std::nullptr_t
    static_assert(!std::is_pointer_v<T> && !std::is_null_pointer_v<T>, "Storage<T*> and Storage<nullptr> disallowed");
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

int main()
{
    Storage i{5};
    Storage d{6.7};

    double d1{123.456};
    double *dptr{&d1};

    //Storage s{dptr};
    //Storage s{&d1};
    Storage s{nullptr};

    i.print();
    d.print();
    s.print();

    return 0;
}