#include <iostream>
#include <memory>

class Resource
{
public:
    Resource() { std::cout << "Resource Aquired.\n"; }
    ~Resource() { std::cout << "Resource Destroyed.\n"; }
};

int main()
{
    auto ptr1{ std::make_shared<Resource>() };
    {
        auto ptr2{ ptr1 };
        std::cout << "Going out of scope : one shared pointer\n";
    }

        std::cout << "Going out of scope : another shared pointer\n";

    return 0;
}