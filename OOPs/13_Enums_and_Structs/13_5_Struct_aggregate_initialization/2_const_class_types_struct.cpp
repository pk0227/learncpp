#include <iostream>

struct Ellipsoid
{
    double length{100};
    double width{};
    double height;
};

struct Cuboid
{
    double length{20};
    double width{};
    double height;

    Cuboid()
    {
        std::cout << "Default constructor\n";
    }

    Cuboid(int x, int y)
    {
        std::cout << "Param constructor\n";
    }

    Cuboid(std::initializer_list<int> list) {
        std::cout << "List constructor\n";
    }
};

std::ostream& operator<<(std::ostream& out, const struct Cuboid& c)
{
    out << "length : " << c.length << "\nwidth : " << c.width << "\nheigth : " << c.height;
    return out;
}

std::ostream& operator<<(std::ostream& out, const struct Ellipsoid& c)
{
    out << "length : " << c.length << "\nwidth : " << c.width << "\nheigth : " << c.height;
    return out;
}

int main()
{
    const Cuboid r1 { 2, 28 };  // r1.heigth will be garbage as no value is provided in the explicit initializer list for memeber 'heigth' and the member doesnt have default member initializer.
                             // If any constructor exists (even = default), the struct/class is no longer an aggregate — {} now calls the appropriate constructor.
                             // If {} arguments don’t match any constructor parameters, compilation fails.
                             // { 2, 28 } looks for initializer_list c'tor first, if it doesnt exist , then it will looks for parameterized c'tor.
    std::cout << r1 << "\n-----------------------------------\n";

    Cuboid r2{};         // emp1.whatever will be garbage as no value is provided in the explicit initializer list for memeber 'whatever' and the member doesnt have default member initializer.
    std::cout << r2 << "\n-----------------------------------\n";

    const Ellipsoid e1{1, 2, 3};
    std::cout << e1 << "\n-----------------------------------\n";

    const Ellipsoid e2{};       // value-initialize all members
    std::cout << e2 << "\n-----------------------------------\n";

    r2.height = 2.34;
    //r1.height = 1.23;   // error: assignment of member 'Cuboid::height' in read-only object
    
    return 0;
}