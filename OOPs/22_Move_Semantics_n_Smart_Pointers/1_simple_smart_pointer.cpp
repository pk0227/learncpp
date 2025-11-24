#include <iostream>

template <typename T>
class Auto_ptr
{
    T* m_ptr{};

public:
    Auto_ptr(T* ptr=nullptr) : m_ptr{ptr}
    { 
    }

    ~Auto_ptr()
    {
        delete m_ptr;
    }

    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
};

class Resource
{

public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
    void print() const { std::cout << "Hello\n"; }
};

int main()
{
    Auto_ptr<Resource> res1{new Resource{}};
    res1->print();
    return 0;
}