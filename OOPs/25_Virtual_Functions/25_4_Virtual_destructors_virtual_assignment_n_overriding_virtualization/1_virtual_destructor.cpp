#include <iostream>

class Base
{
public:
    virtual ~Base()
    {
        std::cout << "~Base() Destructor\n";
    }
};

class Derived : public Base
{
    int *m_array{};
public:
    Derived(int *arrayPtr=nullptr) : m_array{arrayPtr}
    { }

    virtual ~Derived()
    {
        std::cout << "~Derived Destructor\n";
        delete[] m_array;
    }
};

int main()
{
    Derived *dptr{ new Derived{new int[5]} };
    Base *bptr{dptr};
    delete bptr;
    
    return 0;
}