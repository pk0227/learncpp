#include <iostream>

template <typename T> class Auto_ptr {
  T *m_ptr{};

public:
  Auto_ptr(T *ptr = nullptr) : m_ptr{ptr} {}

  Auto_ptr(const Auto_ptr<T> &); // copy constructor
  Auto_ptr(Auto_ptr<T> &&);      // move constructor

  ~Auto_ptr() { delete m_ptr; }

  T &operator*() const { return *m_ptr; }
  T *operator->() const { return m_ptr; }
};

template <typename T> Auto_ptr<T>::Auto_ptr(const Auto_ptr<T> &ref) {
  std::cout << "Copy constructor...called\n";
}

template <typename T> Auto_ptr<T>::Auto_ptr(Auto_ptr<T> &&ref) {
  std::cout << "Move constructor...called\n";
}

// Test 1: Simple return of local variable - eligible for implicit move
Auto_ptr<int> test1_SimpleReturn() {
  Auto_ptr<int> local{new int{1}};
  return local; // Implicit move (or copy elision)
}

// Test 2: Ternary operator - NOT eligible for implicit move
Auto_ptr<int> test2_TernaryReturn(bool flag) {
  Auto_ptr<int> local1{new int{1}};
  Auto_ptr<int> local2{new int{2}};
  return flag ? local1 : local2; // Copy constructor (lvalue)
}

int main() {
  std::cout << "=== Test 1: Simple return of local variable ===\n";
  Auto_ptr<int> res1 = test1_SimpleReturn();

  std::cout << "\n=== Test 2: Ternary operator return ===\n";
  Auto_ptr<int> res2 = test2_TernaryReturn(true);

  return 0;
}
