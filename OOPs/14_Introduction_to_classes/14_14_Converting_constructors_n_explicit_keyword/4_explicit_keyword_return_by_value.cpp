#include <iostream>

class Foo
{
public:
    explicit Foo() // note: explicit (just for sake of example)
    {
    }

    explicit Foo(int x) // note: explicit
    {
    }
};

Foo getFoo()
{
    // explicit Foo() cases
    // return Foo{ };   // ok

    // COMPILE ERROR: can't implicitly convert initializer list to Foo because constructor is explicit:
    // return { };

    // explicit Foo(int) cases
    // COMPILE ERROR: can't implicitly convert int to Foo because constructor is explicit:
    // return 5;

    // return Foo{ 5 }; // ok

    // COMPILE ERROR: can't implicitly convert initializer list to Foo because constructor is explicit:
    // return { 5 };

    return Foo{ 5 }; // ok: direct list-initialization syntax
}

int main()
{
    return 0;
}