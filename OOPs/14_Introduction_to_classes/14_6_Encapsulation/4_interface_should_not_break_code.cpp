#include <iostream>

class Best3
{
    int m_val1{};
    int m_val2{};
    int m_val3{};

public:
    void setValue(int value, int position)
    {
        switch(position)
        {
            case 0:
                m_val1 = value;
                break;
            case 1:
                m_val2 = value;
                break;
            case 2:
                m_val3 = value;
                break;
            default:
                    ;
        }
    }

    void print() const
    {
        std::cout << "val1 : " << m_val1 << "\nval2 : " << m_val2 << "\nval3 : " << m_val3 << "\n";
    }
};

int main()
{
    Best3 b{};
    b.setValue(10, 2);
    b.setValue(20, 1);
    b.setValue(12, 0);

    b.print();

    return 0;
}