#include <iostream>
#include <vector>

class Matrix 
{
  int m_row{};
  int m_col{};
  std::vector<int> m_data{};

public:
  Matrix(int r, int c) : m_row{r}, m_col{c}, m_data(r * c)  // should nt use m_data{r*c}, it will initialize vector with r*c value, not consider as size
  { }

  auto &&operator[](this auto &&self, int r, int c) 
  {
    return self.m_data[r * self.m_col + c];
  }
};

int main() {
  Matrix m1{3, 4};
  m1[1, 2] = 100;
  m1[2, 3] = 123;

  std::cout << m1[1, 2] << "\n" << m1[2, 3] << "\n";
  return 0;
}