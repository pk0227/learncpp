#include <iostream>

class Accumlator
{   
    int m_count{};
public:
    Accumlator& operator()(int i)
    {
        //return (m_count += i);
        m_count += i;
        return *this;
    }

    void reset() { m_count = 0; }

    friend std::ostream& operator<<(std::ostream& out, const Accumlator& obj)
    {
        out << obj.m_count;
        return out;
    }
};

int main()
{
    Accumlator acc{};
    /*
    std::cout << acc(1) << "\n";
    std::cout << acc(2) << "\n";
    std::cout << acc(3) << "\n";
    std::cout << acc(4) << "\n";
    */
    std::cout << acc(10)(20)(30) << "\n";
    return 0;
}