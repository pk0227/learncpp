/*
 * Demonstration: Why Range-Based For Loops Don't Work with Enumerations
 * 
 * Problem: Enums are not iterable containers - they don't have begin()/end()
 * Solution: Create a constexpr std::array containing all enum values
 * 
 * Techniques Shown:
 * 1. Traditional loop with casting (manual iteration)
 * 2. Range-based for loop with helper array (elegant iteration)
 */

#include <iostream>
#include <array>
#include <string>
#include <string_view>
#include <limits>
#include <optional>

namespace Color
{
    enum Type
    {
        red,
        green,
        blue,
        orange,
        yellow,
        max_colors
    };

    using namespace std::string_view_literals;
    
    // Array of color names (for string conversion)
    constexpr std::array colorNames{"red"sv, "green"sv, "blue"sv, "orange"sv, "yellow"sv};

    // Array of all enum values (enables range-based iteration)
    constexpr std::array colorType{red, green, blue, orange, yellow};

    constexpr const std::string_view getColorName(Type color)
    {
        return colorNames[static_cast<std::size_t>(color)];
    }

    static_assert(std::size(colorNames) == max_colors);
}

std::ostream& operator<<(std::ostream& out, Color::Type color)
{
    out << Color::getColorName(color);
    return out;
}

int main()
{
    // Approach 1: Traditional for loop - requires manual casting from int to enum
    for(int i{}; i < Color::max_colors; i++)
        std::cout << static_cast<Color::Type>(i) << "\n";

    // ILLEGAL: Cannot use range-based for directly on enum type
    //for(auto i : Color::Type)       // Compilation error: enum is not iterable
    
    // Approach 2: Range-based for loop using helper array (cleaner, type-safe)
    for(auto i : Color::colorType)
        std::cout << i << "\n";

    return 0;
}