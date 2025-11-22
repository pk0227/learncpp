#include <iostream>

class MinMax
{
    int m_min{};
    int m_max{};

public:
    MinMax(int x, int y) : m_min{x}, m_max{y}
    { }

    const int& getMin() const { return m_min; }
    const int& getMax() const { return m_max; }

    friend MinMax operator+(const MinMax&, const MinMax&);
    friend MinMax operator+(const MinMax&, int);
    friend MinMax operator+(int, const MinMax&);
};

MinMax operator+(const MinMax& left, const MinMax& right)
{
    // Get the minimum value seen in m1 and m2
    int min{left.m_min < right.m_min ? left.m_min : right.m_min};

    // Get the maximum value seen in m1 and m2
    int max{left.m_max > right.m_max ? left.m_max : right.m_max};

    return MinMax{min, max};
}

MinMax operator+(const MinMax& left, int value)
{
    // Get the minimum value seen in m and value
    int min{left.m_min < value ? left.m_min : value};

    // Get the maximum value seen in m and value
    int max{left.m_max > value ? left.m_max : value};

    return MinMax{min, max};
}

MinMax operator+(int value, const MinMax& right)
{
    // calls operator+(MinMax, int)
    return right+value;
}

int main()
{
    MinMax m1{10, 15};
    MinMax m2{8, 11};
    MinMax m3{3, 12};

    MinMax mFinal{m1+m2+5+8+m3+16};

    std::cout << "Result: (" << mFinal.getMin() << ", " << mFinal.getMax() << ")\n";

    return 0;
}