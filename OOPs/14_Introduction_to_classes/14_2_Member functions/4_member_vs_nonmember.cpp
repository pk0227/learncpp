// Member function vs non-member (free) function — the same operation both ways.
//
// Points demonstrated:
//   - A member function is called ON an object (obj.fn()); the object is passed
//     IMPLICITLY and members are accessed directly (the "implicit object").
//   - A non-member function must take the object EXPLICITLY as a parameter, and
//     access its members THROUGH that parameter.
//   - Member functions work with a `class` too, not only a `struct`.

#include <iostream>

class Rectangle
{
    int m_width{};
    int m_height{};

public:
    Rectangle(int w, int h) : m_width{ w }, m_height{ h } {}

    // MEMBER function: no object parameter -- the implicit object is passed
    // automatically, and m_width/m_height refer to that implicit object's members.
    int area() const { return m_width * m_height; }

    // Getters so a non-member can read the private data.
    int width()  const { return m_width; }
    int height() const { return m_height; }
};

// NON-MEMBER (free) function: the object must be passed EXPLICITLY as a parameter,
// and its members are accessed THROUGH that parameter (r.width(), r.height()).
int areaNonMember(const Rectangle& r)
{
    return r.width() * r.height();
}

int main()
{
    Rectangle rect{ 4, 5 };

    // Member call: object on the left, implicitly passed in.
    std::cout << "area() [member]          = " << rect.area() << '\n';

    // Non-member call: object passed explicitly as an argument.
    std::cout << "areaNonMember(rect) [free] = " << areaNonMember(rect) << '\n';

    return 0;
}
