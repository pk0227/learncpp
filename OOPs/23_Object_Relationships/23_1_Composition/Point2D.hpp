#ifndef POINT2D_HPP
#define POINT2D_HPP

#include <iostream>
#include <string>

class Point2D
{
    int m_x{};
    int m_y{};

public:
    Point2D(int x=0, int y=0) : m_x{x}, m_y{y}
    { }

    void setPoints(int x, int y)
    {
        m_x = x;
        m_y = y;
    }

    friend std::ostream& operator<<(std::ostream& out, const Point2D& ref)
    {
        out << "Point2D( " << ref.m_x << ", " << ref.m_y << " )";
        return out;
    }
};

#endif