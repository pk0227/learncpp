#include <iostream>

class Point
{
    double m_x{};
    double m_y{};
    double m_z{};

public:
    Point(double x=0.0, double y=0.0, double z=0.0) : m_x{x}, m_y{y}, m_z{z}
    { }

    const double& getX() const { return m_x; }
    const double& getY() const { return m_y; }
    const double& getZ() const { return m_z; }

    Point operator-() const;
    bool operator!() const;
};

Point Point::operator-() const
{
    return Point{-m_x, -m_y, -m_z};
}

bool Point::operator!() const
{
    return (m_x == 0.0 && m_y ==0.0 && m_z ==0.0);
}

std::ostream& operator<<(std::ostream& out, const Point& point)
{
    out << "Point : (" << point.getX() << ", " << point.getY() << ", " << point.getZ() << ")";
    return out;
}

int main()
{
    Point p1{1.2, 2.3, 3.4};
    Point p2{};

    if(!p1)
        std::cout << "p1 is set to Zeros\n";
    else
        std::cout << "p1 is NOT set to zeros\n";

    std::cout << p1 << "\n";
    std::cout << p2 << "\n";

    return 0;
}