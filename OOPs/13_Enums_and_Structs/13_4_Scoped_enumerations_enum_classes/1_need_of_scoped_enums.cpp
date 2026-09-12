#include <iostream>

namespace Unscoped
{
    enum Color
    {
        red,
        green
    };

    enum Fruit
    {
        banana,
        apple
    };
}

enum class Color_scoped
{
    red,
    green
};

enum class Fruit_scoped
{
    banana,
    apple
};


int main()
{
    Unscoped::Color c{Unscoped::red};
    Unscoped::Fruit f{Unscoped::banana};

    if(c == f)          // warning: comparison between 'enum Color' and 'enum Fruit' but compilation occurs.
        std::cout << "Both unscoped enum types are equal\n";

    Color_scoped cs{Color_scoped::red};       // note: red is not directly accessible, we have to use Color_scoped::red
    Fruit_scoped fs{Fruit_scoped::banana};    // note: banana is not directly accessible, we have to use Fruit_scoped::banana

    // if(cs == fs)        // COMPILE ERROR: no match for 'operator==' (operand types are 'Color_scoped' and 'Fruit_scoped')
    //     std::cout << "Both SCOPED enum class types are equal\n"; 

    // Scoped enums provide type-safety: different enum class types cannot be compared
    if (cs == Color_scoped::red)
        std::cout << "cs is red\n";

    return 0;
}