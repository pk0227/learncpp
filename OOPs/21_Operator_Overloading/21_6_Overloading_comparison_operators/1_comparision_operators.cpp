#include <iostream>

class Cents 
{
  int m_cents{};

public:
  Cents(int cents) : m_cents{cents}
  { }

  friend bool operator==(const Cents&, const Cents&);
  friend bool operator!=(const Cents&, const Cents&);
  friend bool operator<(const Cents&, const Cents&);
  friend bool operator>(const Cents&, const Cents&);
  friend bool operator<=(const Cents&, const Cents&);
  friend bool operator>=(const Cents&, const Cents&);

};

bool operator==(const Cents& left, const Cents& right)
{
  return (left.m_cents == right.m_cents);
}

bool operator!=(const Cents& left, const Cents& right)
{
  return !operator==(left, right);
}

bool operator<(const Cents& left, const Cents& right)
{
  return (left.m_cents < right.m_cents);
}

bool operator>(const Cents& left, const Cents& right)
{
  return operator<(right, left);
}

bool operator<=(const Cents& left, const Cents& right)
{
  return !operator>(left, right);
}

bool operator>=(const Cents& left, const Cents& right)
{
  return !operator<(left, right);
}

int main() 
{ 
    Cents dime{ 10 };
    Cents nickel{ 5 };

    if (nickel > dime)
        std::cout << "a nickel is greater than a dime.\n";
    if (nickel >= dime)
        std::cout << "a nickel is greater than or equal to a dime.\n";
    if (nickel < dime)
        std::cout << "a dime is greater than a nickel.\n";
    if (nickel <= dime)
        std::cout << "a dime is greater than or equal to a nickel.\n";
    if (nickel == dime)
        std::cout << "a dime is equal to a nickel.\n";
    if (nickel != dime)
        std::cout << "a dime is not equal to a nickel.\n";

  return 0;
}