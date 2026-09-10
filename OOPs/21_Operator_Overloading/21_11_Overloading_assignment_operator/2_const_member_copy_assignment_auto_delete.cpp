#include <iostream>
#include <string>

class Points
{
    int m_x{};
    const int m_y{};        // error: non-static const member ‘const int Points::m_y’, cannot use default assignment operator
                            // because const members can’t be assigned
                            // It is a deleted function.  Points& operator=(const Points&) = delete;
    int m_z{};

public:
    Points(int x=0, int y=0, int z=0) : m_x{x}, m_y{y}, m_z{z}
    {
    }

    //Points(const Points&) = default;
    //Points& operator=(const Points&) = default;
    //Points& operator=(const Points&){ return *this; }
    friend std::ostream& operator<<(std::ostream&, const Points&);

};

std::ostream& operator<<(std::ostream& out, const Points& obj)
{
    out << "Points(x, y, z) : (" << obj.m_x << ", " << obj.m_y << ", " << obj.m_z << ") : ";
    return out;
}

int main()
{
    Points p1{20, 30, 50};
    Points p2{};
    //Points p2{p1};

    std::cout << p2 << "\n";
    // p2=p1;                      // compile error: non-static const member ‘const int Points::m_y’, cannot use default assignment operator
                                   // because const members can’t be assigned
                                   // It is an implicitly deleted function: Points& operator=(const Points&) = delete;
    std::cout << p2 << "\n";

    return 0;
}