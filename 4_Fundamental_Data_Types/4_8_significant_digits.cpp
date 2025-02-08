#include <iostream>
#include <iomanip>
#include <format>

int main()
{
  
  double d{123411.5678101112131415};
  double f{123411.5678101112131415f};

  //std::cout << std::setprecision(25);

  //This prints both double and float values with default precision (i.e precision 6).
  //double and float values have their max significant digits as 17 and 13 respectively.
  //We can set precision more than 17. But, beyond 17 for double and 13 for float, we will not get accurate digits.
  std::cout << "double : " << std::left << std::setw(30) << d << "\t" << "float : " << f << "\n";

  //This prints both double and float values with respect to their max significant digits(i.e precision 17)
  std::cout << std::format("double : {:<30}\tfloat : {}", d, f) << "\n";
  return 0;
}
