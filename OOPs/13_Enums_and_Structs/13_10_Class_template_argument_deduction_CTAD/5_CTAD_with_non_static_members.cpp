#include <iostream>
#include <utility> // for std::pair

struct Foo
{
    std::pair<int, int> p1{ 1, 2 }; // ok, template arguments explicitly specified
    // std::pair p2{ 1, 2 };        // COMPILE ERROR: CTAD cannot be used for non-static member initialization in class definitions
};

int main()
{
    std::pair p3{ 1, 2 };           // ok, CTAD can be used here
    return 0;
}