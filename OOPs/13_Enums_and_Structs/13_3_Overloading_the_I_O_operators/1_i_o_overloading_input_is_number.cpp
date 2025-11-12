#include <iostream>

enum Color
{
    red,
    green,
    blue
};

std::string_view getColorName(const Color col)
{
    switch(col)
    {
        case red:   return "red";
        case green: return "green";
        case blue:  return "blue";
        default:    return "???";
    }
}

std::ostream& operator<<(std::ostream& out, const Color col)
{
    out << getColorName(col);
    return out;
}

std::istream& operator>>(std::istream& in, Color& col)
{
    int input{};
    in >> input;
    col = static_cast<Color>(input);
    return in;
}

int main()
{
    Color col{};
    std::cout << "Enter color(number) : ";
    std::cin >> col;                // Extraction >> operator doesnt know how to deal with Enumeration type. Extraction overload is needed.
    std::cout << "selected color is : " << col << "\n";
    
    //std::cout << "selected color is : " << getColorName(col) << "\n";
    return 0;
}