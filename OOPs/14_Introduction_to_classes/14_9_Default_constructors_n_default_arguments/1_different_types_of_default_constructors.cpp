#include <iostream>

class Foo
{
    int m_x{};
    int m_y{};

public:

//class "Foo" having more than one default constructor will cause overloaded 'Foo()' is ambiguous
// compile error: ambiguous constructor function call

    //Foo() = default;

    /*
    Foo() 
    { 
        std::cout << "Empty default constructor\n";
    }
*/

    Foo(int x=1, int y=2) : m_x{x}, m_y{y}          // default constructor with default args
    {
        std::cout << "default constructor with default args\n";
    }

};

int main()
{
    Foo f1{};   // value initialization, calls Foo() default constructor
    Foo f2;     // default initialization, calls Foo() default constructor
    return 0;
}