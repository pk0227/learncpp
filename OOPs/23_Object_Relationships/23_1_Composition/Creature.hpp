#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <iostream>
#include <string>
#include <string_view>
#include "Point2D.hpp"

class Creature
{
    std::string m_name{};
    Point2D m_location{};

public:
    Creature(const std::string_view name, const Point2D& ref) : m_name{name}, m_location{ref}
    { }

    void moveTo(int x, int y)
    {
        m_location.setPoints(x,y);
    }

    friend std::ostream& operator<<(std::ostream& out, const Creature& ref)
    {
        out << ref.m_name << " is at " << ref.m_location;
        return out;
    }
};

#endif