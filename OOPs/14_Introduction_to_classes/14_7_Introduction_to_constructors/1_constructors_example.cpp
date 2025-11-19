#include <iostream>

class Foo
{
    int m_x{};
    int m_y{};

public:
    Foo(int x, int y) : m_x{x}, m_y{y}      // constructors must be non-const
    {
        if(m_x == x)            
            m_x = 123;                      // okay to modify members in non-const constructor
    }

    const int& getX() const
    {
        return m_x;
    }
};

int main()
{
    const Foo f1{'a', 10};      // will match Foo(int, int) constructor 
                                // const object, implicitly invokes (non-const) constructor
    std::cout << f1.getX() << "\n";
    return 0;
}