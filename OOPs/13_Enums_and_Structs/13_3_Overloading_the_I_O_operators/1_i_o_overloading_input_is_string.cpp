#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <limits>

namespace AvailableColors
{
    enum Color
    {
        red,
        green,
        blue
    };

    std::vector<std::string> currentColors{"red", "green", "blue"};
}

std::string_view getColorName(const AvailableColors::Color col)
{
    switch(col)
    {
        case AvailableColors::Color::red:   return "red";
        case AvailableColors::Color::green: return "green";
        case AvailableColors::Color::blue:  return "blue";
        default:    return "???";
    }
}

std::ostream& operator<<(std::ostream& out, const AvailableColors::Color col)
{
    out << getColorName(col);
    return out;
}

bool charEqualIgnoreCase(const char& a, const char& b)
{
    return (std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b)));
}

bool isequal(const std::string& a, const std::string& b)
{
    if(a.size() != b.size())
        return false;
    
    return std::equal(a.begin(), a.end(), b.begin(), charEqualIgnoreCase);
}

std::optional<AvailableColors::Color> getColorMatch(const std::string& input)
{
    if(isequal("red", input))
        return AvailableColors::Color::red;
    else if(isequal("green", input))
        return AvailableColors::Color::green;
    else if(isequal("blue", input))
        return AvailableColors::Color::blue;
    else
        return {};
}

std::istream& operator>>(std::istream& in, AvailableColors::Color& col)
{
    std::string input{};
    in >> input;
    const std::optional<AvailableColors::Color>& match{getColorMatch(input)};

    if(match)
    {
        col = *match;    
    }
    else
    {
        in.setstate(std::ios_base::failbit);
        col = {};
    }

    return in;
}

int main()
{
    AvailableColors::Color col{};
    std::cout << "Enter color(string) : ";
    std::cin >> col;                // Extraction >> operator doesnt know how to deal with Enumeration type. Extraction overload is needed.
    
    if(std::cin)
        std::cout << "selected color is : " << col << "\n";
    else
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Sorry! The color is not available\n";
    }
    
    //std::cout << "selected color is : " << getColorName(col) << "\n";
    return 0;
}