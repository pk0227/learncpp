#include <iostream>

int main()
{
    //float f{3.1456789123};  // not narrowing, even though loss of precision results

 /* // not narrowing, even though loss of precision results, still the value is representable in float -- within range.
    constexpr double d{3.1456789123};
    std::cout << "d : " << d << std::endl;
    float f{d}; // {} checks for constexpr and the value is representable though precision is lost.
*/

/*
  // narrowing and error because even though loss of precision results, the value isn't representable in float -- out of range.
    constexpr double d{1e40};
    std::cout << "d : " << d << std::endl;
    float f{d}; // {} checks for constexpr and the value is representable though precision is lost.
*/
   
    // narrowing and loss of precision results due to d is not constexpr
    double d{3.1456789123};
    //double d{1e40};
    std::cout << "d : " << d << std::endl;
    float f{d};

    std::cout << f << std::endl;
    return 0;
}