#include <iostream>
#include <memory>

class Member
{
public:
    Member()
    {
        std::cout << "Member allocated some resources\n";
    }

    ~Member()
    {
        std::cout << "Member cleaned up\n";
    }
};

class A
{
    int m_A{};
    Member m_member{};  // member object of class Member. 
                        // Resources allocated in its constructor will be cleaned up in its destructor on failure of A's constructor.
    std::unique_ptr<int> m_ptr{}; // smart pointer member to demonstrate automatic cleanup
public:
    A(int x=0) : m_A{x}, m_ptr{std::make_unique<int>(42)} // allocate resource
    {
        if(x <= 0)
            throw -1;
    }

    ~A()
    {
        std::cerr << "~A() : cleaning up...\n";
    }
};

int main()
{
    try
    {
        A a{0};
    }
    catch(int)
    {
        std::cerr << "Oops : Constructor failed\n";
    }

    return 0;
}