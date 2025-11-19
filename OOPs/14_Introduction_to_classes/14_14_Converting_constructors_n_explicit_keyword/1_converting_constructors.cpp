#include <iostream>

class Foo
{
    int m_x{};
    int m_y{};

public:
    Foo(int x, int y=10)    // if constructor is single non-default value parameter / all parameters have default values except first one,
                            // then, converting constructor will work
            : m_x{x}
            , m_y{y}
    {
    }

    const int& getX() const { return m_x; }
    const int& getY() const { return m_y; }
};

void printFoo(Foo f)
//void printFoo(const Foo& f)
{
    std::cout << f.getX() << "\t" << f.getY() << "\n";
}

int main()
{
    printFoo(5);
    return 0;
}