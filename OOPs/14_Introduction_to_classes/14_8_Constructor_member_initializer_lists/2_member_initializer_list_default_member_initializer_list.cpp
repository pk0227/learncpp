#include <iostream>

class Foo
{
private:
    int m_x {};    // default member initializer (will be ignored)
    int m_y { 2 }; // default member initializer (will be used)
    int m_z;      // no initializer

public:
    Foo(int x)
        : m_x { x } // member initializer list
    {
        std::cout << "Foo constructed\n";
    }

    //Foo() = default;

    Foo() { };

    void print() const
    {
        std::cout << "Foo(" << m_x << ", " << m_y << ", " << m_z << ")\n";
    }
};

int main()
{
    Foo foo { 6 };
    foo.print();
    std::cout << "--------------------------------\n";
    Foo foo1{};                 // check it with compiler provided default constructor and user provided default constructor having no member initializer list.
    foo1.print();
    std::cout << "--------------------------------\n";
    Foo foo2;                    // check it with compiler provided default constructor and user provided default constructor having no member initializer list.
    foo2.print();

    return 0;
}