#include <iostream>

class Accumulator
{
    int m_value{};
public:
    void add(int val) { m_value += val; }
    const int& getAcc() const { return m_value; }
};

void print(const Accumulator& ref)
{
    std::cout << ref.getAcc() << "\n";
}

int main()
{
    Accumulator acc{};
    acc.add(5);
    print(acc);

    return 0;
}