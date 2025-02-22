// Improperly using std::string_view

#include <iostream>
#include <string>
#include <string_view>

std::string getName()
{
    std::string s { "Rama" };
    return s;
}

int main()
{
  std::string_view name { getName() }; // name initialized with return value of function
  std::cout << name << '\n'; // undefined behavior

  return 0;
}