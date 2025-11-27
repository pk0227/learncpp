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
/*
    Resource* res{ new Resource() };
    std::unique_ptr<Resource> res1{ res };
    std::unique_ptr<Resource> res2{ res };
*/  
    Resource* res{ new Resource() };
    std::unique_ptr<Resource> res1{ res };
    delete res;

    return 0;
}