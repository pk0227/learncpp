#include "Cents.hpp"

Cents operator+(const Cents& left, const Cents& right)
{
    return Cents{left.getCents() + right.getCents()};
}

Cents operator-(const Cents& left, const Cents& right)
{
    return Cents{left.getCents() - right.getCents()};
}