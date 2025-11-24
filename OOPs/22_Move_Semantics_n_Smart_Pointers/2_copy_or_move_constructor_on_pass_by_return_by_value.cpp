#include <iostream>

template <typename T>
class Auto_ptr
{
    T* m_ptr{};

public:
    Auto_ptr(T* ptr=nullptr) : m_ptr{ptr}
    { 
    }

    Auto_ptr(const Auto_ptr<T>&);       // copy constructor
    Auto_ptr(Auto_ptr<T>&&);            // move constructor

    ~Auto_ptr()
    {
        delete m_ptr;
    }

    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
};

template <typename T>
Auto_ptr<T>::Auto_ptr(const Auto_ptr<T>& ref)
{
    std::cout << "Copy constructor...called\n";
}

template <typename T>
Auto_ptr<T>::Auto_ptr(Auto_ptr<T>&& ref)
{
    std::cout << "Move constructor...called\n";
}

// On function call with parameter, which constructor is called? 
// Ans: Copy constructor.
void someFunction(Auto_ptr<int> r1)     
{
}

// On conditional return, which constructor is called? 
// Ans: Copy constructor.
Auto_ptr<int> someFunctionWithConditionalReturn(bool flag=false)     
{
    Auto_ptr<int> local1{new int{1}};
    Auto_ptr<int> local2{new int{2}};
    return flag ? local1 : local2;
}

// On returning Named local variables/Function parameters directly, which constructor is called? 
// Ans: Move constructor if copy elision is disable using -fno-elide-constructors. 
// Otherwise, no constructor is called. Due to copy elision, return object is created directly in caller's space.
Auto_ptr<int> someFunctionWithReturn()     
{
    Auto_ptr<int> local1{new int{1}};
    return local1;
}

int main()
{
    Auto_ptr<int> res1{new int{}};
    //*res1 =100;
    //std::cout << "int : " << *res1 << "\n";
    someFunction(res1);                                         // Copy constructor...called               
    Auto_ptr<int> res2{someFunctionWithConditionalReturn()};    // Copy constructor...called
    Auto_ptr<int> res3{someFunctionWithReturn()};               // Move constructor...called
    return 0;
}