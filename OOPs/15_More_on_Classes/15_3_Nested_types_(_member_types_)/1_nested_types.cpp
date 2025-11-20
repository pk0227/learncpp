#include <iostream>

class Fruit
{
public:
    // Use an enum rather than an enum class
    enum Type
    {
        apple,
        banana,
        cherry
    };

private:
    Type m_type{};
    int m_percentageEaten{};

public:
    Fruit(Type type) : m_type{type}
    {
    }

    const Type& getType() const { return m_type; }
    const int& getPercentageEaten() const { return m_percentageEaten; }

    bool isCherry() { return m_type == cherry; }    // Inside members of Fruit, we no longer need to prefix enumerators with FruitType::
};

int main()
{
    // Note: Outside the class, we access the enumerators via the Fruit:: prefix now
    Fruit f1{Fruit::apple};

    if(f1.getType() == Fruit::apple)
        std::cout << "Fruit is Apple\n";

    if(f1.isCherry())
        std::cout << "Fruit is cherry\n";
    else
        std::cout << "Fruit is NOT cherry\n";
    
    return 0;
}