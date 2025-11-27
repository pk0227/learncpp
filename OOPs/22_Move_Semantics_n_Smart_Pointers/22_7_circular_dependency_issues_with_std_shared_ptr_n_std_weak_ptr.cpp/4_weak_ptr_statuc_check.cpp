#include <iostream>
#include <memory>

class Resource
{    
public:
    Resource() { std::cout << "Resource Aquired.\n"; }
    ~Resource() { std::cout << "Resource Destroyed.\n"; }
};

Resource* getRawPointer()
{
    auto ptr{std::make_shared<Resource>()};
    return ptr.get();
}

std::weak_ptr<Resource> getWeakSmartPointer()
{
    auto ptr{std::make_shared<Resource>()};
    return std::weak_ptr<Resource>{ptr};
}

int main()
{
    auto raw{getRawPointer()};
    auto weak{getWeakSmartPointer()};

    std::cout << "Raw pointer is " << (raw ? "Non-Null" : "Null") << "\n";
    std::cout << "Weak pointer is " << (weak.expired() ? "expired" : "valid") << "\n";

    return 0;
}