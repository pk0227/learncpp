#include <iostream>

class Point
{
    double m_x{};
    double m_y{};
    double m_z{};

public:
    Point(double x=0.0, double y=0.0, double z=0.0) : m_x{x}, m_y{y}, m_z{z}
    { }

    friend std::istream& operator>>(std::istream&, Point&);
    friend std::ostream& operator<<(std::ostream&, const Point&);
};

std::istream& operator>>(std::istream& in, Point& point)
{
    double x{}, y{}, z{};
    in >> x >> y >> z;  // incase of semantically invalid data like 1.25abc. it reads 1.25 and assigns to x,
                        // but when it encounters character b for next double type(y), cin gets failed and sets failbit. 
    if(x < 0.0 || y < 0.0 || z < 0.0)
    {
        in.setstate(std::ios_base::failbit);
    }

    point = in ? Point{x, y, z} : Point{};

    return in;
}

std::ostream& operator<<(std::ostream& out, const Point& point)
{
    out << "Point : (" << point.m_x << ", " << point.m_y << ", " << point.m_z << ")\n";
    return out;
}

int main()
{
    Point p1{};
    std::cout << "Enter Point values : ";
    std::cin >> p1;

    std::cout << p1 << "\n";
    return 0;
}