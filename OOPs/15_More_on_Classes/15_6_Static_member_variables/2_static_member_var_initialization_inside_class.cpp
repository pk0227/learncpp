#include <iostream>

class Something
{
private:
    //static const int s_idGenerator { 1 };      // This is recommended.
    //static constexpr int s_idGenerator { 1 };   // constexpr member variable are implicitly inline
    //static inline int s_idGenerator { 1 };      // This is recommended.
    static inline auto s_idGenerator { 1 };      // Only static members may use type deduction (auto and CTAD)
                                                 // Non-static members may not use auto or CTAD.
    int m_id {};

public:
    // grab the next value from the id generator
    Something() : m_id { s_idGenerator++ }
    {
    }

    int getID() const { return m_id; }
};

int main()
{
    Something first{};
    Something second{};
    Something third{};

    std::cout << first.getID() << '\n';
    std::cout << second.getID() << '\n';
    std::cout << third.getID() << '\n';
    return 0;
}