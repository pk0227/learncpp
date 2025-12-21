/*
 * Demonstration: Better Enum Input/Output using constexpr std::array
 * 
 * Technique: Use constexpr array to map between enum values and string names
 * Benefits:
 * 1. Type-safe enum I/O without magic numbers
 * 2. Compile-time verification with static_assert
 * 3. Bidirectional conversion: enum ↔ string
 * 4. Extensible: adding new enum values is easy
 */

#include <iostream>
#include <array>
#include <string>
#include <string_view>
#include <limits>
#include <optional>

namespace Color
{
    // Enum representing color types
    enum Type
    {
        red,
        green,
        blue,
        orange,
        yellow,
        max_colors  // Sentinel value for count
    };

    using namespace std::string_view_literals;
    
    // constexpr array mapping enum values to string names
    // Index corresponds to enum value (e.g., colorNames[red] = "red")
    constexpr std::array colorNames{"red"sv, "green"sv, "blue"sv, "orange"sv, "yellow"sv};

    // Convert enum value to string name (enum → string)
    constexpr const std::string_view getColorName(Type color)
    {
        return colorNames[static_cast<std::size_t>(color)];
    }

    // Convert string to enum value (string → enum)
    // Returns std::optional to handle invalid input
    constexpr std::optional<Type> getEnumfromString(const std::string_view input)
    {
        for(auto i{0Z}; i < std::ssize(colorNames); i++)
        {
            if(colorNames[i] == input)
                return static_cast<Type>(i);
        }

        return {};  // Return empty optional if no match found
    }

    // Compile-time check: Ensure array size matches enum count
    static_assert(std::size(colorNames) == max_colors);
}

// Overload << operator for easy enum output (prints color name)
std::ostream& operator<<(std::ostream& out, Color::Type color)
{
    out << Color::getColorName(color);
    return out;
}

// Overload >> operator for enum input (reads color name from user)
std::istream& operator>>(std::istream& in, Color::Type& color)
{
    std::string input{};
    std::getline(std::cin >> std::ws, input);  // Read full line, skip leading whitespace
    std::optional<Color::Type> ret{Color::getEnumfromString(input)};
    
    if(ret.has_value())
        color = *ret;  // Valid color found
    else
    {
        in.setstate(std::ios_base::failbit);  // Set failbit for invalid input
        color = {};  // Reset to default (red)
    }

    return in;
}

// Helper function to clear input stream after errors
void clearInputStream()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main()
{
    Color::Type color{};  // Default initialized to Color::red (value 0)
    std::cout << "Your default color is " << color << "\n";

    char user_input[3]{};
    while(true)
    {
        std::cout << "Do you want to change your shirt color [Y/N]: ";
        std::cin.getline(user_input, std::size(user_input));
    
        char ch{user_input[0]};
        
        // Validate single character input
        if(user_input[1])  // More than one character entered
        {
            clearInputStream();
            std::cout << "Please enter valid input\n";
            continue;
        }
            
        if(ch == 'Y')
        {
            std::cout << "Enter your color : ";
            std::cin >> color;  // Use overloaded >> operator
            
            if(std::cin)  // Check if input was valid
                std::cout << "You have changed your shirt color to " << color << "\n";
            else
            {
                std::cin.clear();
                std::cout << "Invalid color\n";
            }
        }
        else if(ch == 'N')
        {
            break;  // Exit loop
        }
    }

    return 0;
}