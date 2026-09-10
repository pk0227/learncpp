#include <iostream>
#include <vector>

class Matrix 
{
  int m_row{};
  int m_col{};
  std::vector<int> m_data{};

public:
  Matrix(int r, int c) : m_row{r}, m_col{c}, m_data(static_cast<std::size_t>(r * c))
  { }

  // C++20 standard way: operator() for 2D indexing
  int& operator()(int r, int c)
  {
    return m_data[static_cast<std::size_t>(r * m_col + c)];
  }

  const int& operator()(int r, int c) const
  {
    return m_data[static_cast<std::size_t>(r * m_col + c)];
  }

  // C++23 way: multidimensional operator[] (P2128R6)
#if defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202110L
  int& operator[](int r, int c) 
  {
    return m_data[static_cast<std::size_t>(r * m_col + c)];
  }

  const int& operator[](int r, int c) const
  {
    return m_data[static_cast<std::size_t>(r * m_col + c)];
  }
#endif
};

int main() {
  Matrix m1{3, 4};

  // C++20 portable indexing using operator():
  m1(1, 2) = 100;
  m1(2, 3) = 123;
  std::cout << "m1(1, 2): " << m1(1, 2) << "\nm1(2, 3): " << m1(2, 3) << "\n";

#if defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202110L
  // C++23 multidimensional operator[]:
  m1[1, 2] = 200;
  std::cout << "m1[1, 2]: " << m1[1, 2] << "\n";
#else
  std::cout << "(Note: C++23 multidimensional operator[] [r, c] requires a C++23 compiler)\n";
#endif

  return 0;
}