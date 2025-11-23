#include <algorithm>
#include <cstring>
#include <iostream>

class MyString 
{
  char *m_string{};
  int m_length{};

public:
  MyString(const char *str = nullptr)
  {
    m_length = std::max<int>(std::strlen(str), 0);

    if (m_length) 
    {
      m_string = new char[static_cast<std::size_t>(m_length + 1)];
      std::strncpy(m_string, str, m_length);
      //std::copy_n(str, m_length+1, m_string);
    }
  }

  ~MyString() 
  { 
    delete[] m_string; 
  }

  MyString(const MyString &);
  MyString &operator=(const MyString &);
  void deepCopy(const MyString&);
  friend std::ostream &operator<<(std::ostream &, const MyString &);
};

std::ostream &operator<<(std::ostream &out, const MyString &obj) 
{
  out << obj.m_string;
  return out;
}

void MyString::deepCopy(const MyString& right)
{
    if (m_string) 
    {
      delete[] m_string;
      m_string = nullptr;
    }

    m_length = right.m_length;

    if (m_length) 
    {
      m_string = new char[static_cast<std::size_t>(m_length + 1)];
      std::strncpy(m_string, right.m_string, m_length);
      //std::copy_n(right.m_string, m_length+1, m_string);
    }
}

MyString::MyString(const MyString& right)
{
    deepCopy(right);
}

MyString &MyString::operator=(const MyString &right) 
{
  if (&right != this) 
  {
    deepCopy(right);
  }

  return *this;
}

int main() 
{
	MyString rama{"Rama"}; 
	MyString employee1{""};  // MyString employee{}; or MyString employee; will cause SIGSEGV.
	employee1 = rama; // Rama is our newest employee
	std::cout << employee1 << "\n------------------------\n";

	MyString seetha{"Seetha"}; 
	MyString employee2{seetha};  // MyString employee{}; or MyString employee; will cause SIGSEGV.
	std::cout << employee2 << "\n------------------------\n";
    
  MyString language{"C++"}; 
  language = language;         
  std::cout << language << "\n"; 
  

  return 0;
}