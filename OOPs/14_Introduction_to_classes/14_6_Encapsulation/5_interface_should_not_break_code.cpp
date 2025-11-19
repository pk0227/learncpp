#include <iostream>
#include <sstream>

class Best3
{
    int m_val[3]{};

public:
    void setValue(int value, int position)
    {
        if(position >= 0 && position < 3)
        {
            m_val[position] = value;
        }
    }
/*
    void print() const
    {
        std::string output{};
        for(int i{0}; i<3; i++)
        {
            switch(i)
            {
                case 0:
                    output += "val1 : ";
                    output += std::to_string(m_val[i]);
                    break;
                case 1:
                    output += "\nval2 : ";
                    output += std::to_string(m_val[i]);
                    break;
                case 2:
                    output += "\nval3 : ";
                    output += std::to_string(m_val[i]);
                    break;
                default:
                    ;
            }
        }
        std::cout << output << "\n";
    }
*/
    void print() const
    {
        std::ostringstream output{};
        for(int i{0}; i<3; i++)
        {
            switch(i)
            {
                case 0:
                    output << "val1 : " << m_val[i] << "\n";
                    break;
                case 1:
                    output << "val2 : " << m_val[i] << "\n";
                    break;
                case 2:
                    output << "val3 : " << m_val[i] << "\n";
                    break;
                default:
                    ;
            }
        }
        std::cout << output.str();
    }
};

int main()
{
    Best3 b{};
    b.setValue(1000, 2);
    b.setValue(100, 1);
    b.setValue(10, 0);

    b.print();

    return 0;
}