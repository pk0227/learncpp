#include <iostream>
#include <memory>               // for std::unique_ptr
#include <utility>              // for std::move

class Resource
{
public:
    Resource() { std::cout << "Resource Aquired.\n"; }
    ~Resource() { std::cout << "Resource Destroyed.\n"; }
};

std::ostream& operator<<(std::ostream& out, const Resource& ref)
{
    out << "I am Resource";
    return out;
}

// Returning std::unique_ptr from a function
std::unique_ptr<Resource> getResource()
{
    return std::make_unique<Resource>();
}

// This function takes ownership of the Resource, which isn't what we want
void takeOwnership(std::unique_ptr<Resource> res)
{
    if(res)
        std::cout << *res << "\n";
}// The Resource is destroyed here

int main()
{
    auto ptr{getResource()};

    ptr.get() ? std::cout << "Not-Null\n" : std::cout << "NULL\n";      // note: get() used here to get a pointer to the Resource

    if(ptr)
        takeOwnership(std::move(ptr));      // takeOwnership(ptr); // This doesn't work, need to use move semantics

    ptr.get() ? std::cout << "Not-Null\n" : std::cout << "NULL\n";

    std::cout << "Ending Program\n";
    
    return 0;
}