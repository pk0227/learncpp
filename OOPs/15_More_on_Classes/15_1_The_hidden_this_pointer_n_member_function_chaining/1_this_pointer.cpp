#include <iostream>

class Calc
{
    int m_value{};

public:
    // returning this as a pointer can also be used instead.
    Calc& add(int val)        
    {
        this->m_value += val;       // this always points to the object being operated on
        return *this;               // Returning *this
    }

    Calc& sub(int val)
    {
        this->m_value -= val;       // Explicitly referencing this
        return *this;
    }

    Calc& mul(int val)
    {
        this->m_value *= val;
        return *this;
    }

    const int& getValue() const { return this->m_value; }      // this and const objects : With const member functions, this is a const pointer to a const value (meaning the pointer cannot be pointed at something else, nor may the object being pointed to be modified).

    void reset() { *this = {}; }            // Resetting a class back to default state
};

int main()
{
    Calc c1{};
    c1.add(100).sub(50).mul(5);
    std::cout << "value : " << c1.getValue() << "\n";

    c1.reset();
    std::cout << "value : " << c1.getValue() << "\n";
    return 0;
}