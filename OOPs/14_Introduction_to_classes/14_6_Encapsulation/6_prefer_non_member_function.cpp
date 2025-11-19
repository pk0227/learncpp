#include <iostream>

class Yogurt
{
    std::string m_flavor{"Vanilla"};

public:
    void setFlavor(const std::string_view flavor)
    {
        m_flavor = flavor;
    }

    const std::string& getFlavor() const
    {
        return m_flavor;
    }
};

void print(const Yogurt& ref)
{
    std::cout << "The Yogurt has " << ref.getFlavor() << " flavor\n";
}

int main()
{
    Yogurt y{};
    y.setFlavor("cherry");
    print(y);
    
    return 0;
}