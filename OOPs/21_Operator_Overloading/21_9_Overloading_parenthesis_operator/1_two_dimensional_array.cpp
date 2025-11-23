#include <iostream>
#include <cassert>

class Matrix
{
    int m_matrix[4][5]{};
public:
    const int& operator()(int r, int c) const
    {
        assert((r>=0 && static_cast<std::size_t>(r) < std::size(m_matrix)) && (c>=0 && static_cast<std::size_t>(c) < std::size(*m_matrix)));
        return m_matrix[r][c];
    }

    int& operator()(int r, int c)
    {
        assert((r>=0 && static_cast<std::size_t>(r) < std::size(m_matrix)) && (c>=0 && static_cast<std::size_t>(c) < std::size(*m_matrix)));
        return m_matrix[r][c];
    }

    void operator()()
    {
        for(int i{0}; i < std::size(m_matrix); i++)
        {
            for(int j{0}; j < std::size(*m_matrix); j++)
                m_matrix[i][j] = 0;
        }
    }
};

int main()
{
    Matrix m1{};
    m1(2,3) = 10;

    std::cout << m1(2, 3) << "\n";

    m1();

    std::cout << m1(2, 3) << "\n";

    return 0;
}