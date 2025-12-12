#include <iostream>
#include <vector>

class Foo
{
    std::vector<int> v1{10};
public:
    void getElements()
    {
        for(const auto& i : v1)
            std::cout << i << " ";

        std::cout << "\n";
    }
};

class Cls
{
    //std::vector<int> v1(20);
    std::vector<int> v1{std::vector<int>(20)};
public:
    void getElements()
    {
        for(const auto& i : v1)
            std::cout << i << " ";

        std::cout << "\n";
    }
};

int main()
{
    std::vector<int> v1{10};
    std::vector<int> v2(10);

    std::vector<Foo> v3{12};
    
    const std::vector<int> v4{10, 20, 30, 40, 50};
    //std::vector<const int> v4{10, 20, 30, 40, 50};

    std::cout << "v1.size() : " << v1.size() << "\n";
    std::cout << "v2.size() : " << v2.size() << "\n";
    std::cout << "v3.size() : " << v3.size() << "\n";

    v3[0].getElements();

    return 0;
}